#include "Scheduler.h"

#include <windows.h>
#include <iomanip>
#include <algorithm>

using namespace std;


// CONSTRUCTOR
Scheduler::Scheduler(int quantum,
                     int semaphoreCount,
                     SchedulingAlgorithm algorithm,
                     bool enableSynchronization)
    : resourceSemaphore(semaphoreCount)
{
    timeQuantum = quantum;

    tickCount = 0;

    elapsedTime = 0;

    contextSwitchCount = 0;

    this->algorithm = algorithm;

    synchronizationEnabled = enableSynchronization;

    sharedResource = 0;
}

// ADD TASK
void Scheduler::addTask(Task* task)
{
    tasks.push_back(task);
}


// ADD ARRIVED TASKS
void Scheduler::addArrivedTasks()
{
    for(size_t i = 0; i < tasks.size(); i++)
    {
        Task* task = tasks[i];

        // Task has arrived and is READY
        if(task->getState() == TaskState::READY &&
           task->getArrivalTime() <= elapsedTime)
        {
            bool alreadyInQueue = false;


            // Check whether task is already in ready queue
            queue<Task*> temp = readyQueue;


            while(!temp.empty())
            {
                if(temp.front() == task)
                {
                    alreadyInQueue = true;

                    break;
                }

                temp.pop();
            }

            // Add only once
            if(!alreadyInQueue)
            {
                task->setReadySince(elapsedTime);

                readyQueue.push(task);
            }
        }
    }
}



// DISPLAY TASKS

void Scheduler::displayTasks()
{
    cout << endl;

    cout << "========== TASK LIST =========="
         << endl;


    for(size_t i = 0; i < tasks.size(); i++)
    {
        tasks[i]->printTask();
    }
}


// DISPLAY READY QUEUE
void Scheduler::displayReadyQueue()
{
    cout << endl;

    cout << "========== READY QUEUE =========="
         << endl;


    queue<Task*> temp = readyQueue;


    while(!temp.empty())
    {
        Task* task = temp.front();

        temp.pop();


        cout << task->getName()
             << " | Priority = "
             << task->getPriority()
             << endl;
    }


    cout << "================================="
         << endl;
}


// TIMER TICK
void Scheduler::timerTick(int duration)
{
    if(duration <= 0)
    {
        return;
    }


    Sleep(duration);


    tickCount++;

    elapsedTime += duration;


    cout << "[Timer Tick "
         << tickCount
         << "] Time = "
         << elapsedTime
         << " ms"
         << endl;
}


// CHECK SEMAPHORE OWNERSHIP

bool Scheduler::ownsSemaphore(int taskId)
{
    for(size_t i = 0;
        i < semaphoreOwners.size();
        i++)
    {
        if(semaphoreOwners[i] == taskId)
        {
            return true;
        }
    }


    return false;
}


// SELECT HIGHEST PRIORITY TASK
// Smaller number = higher priority

Task* Scheduler::selectHighestPriorityTask()
{
    if(readyQueue.empty())
    {
        return nullptr;
    }


    queue<Task*> temp = readyQueue;


    Task* selectedTask = nullptr;


    int highestPriority = 999999;


    while(!temp.empty())
    {
        Task* task = temp.front();

        temp.pop();


        if(task->getState() == TaskState::READY &&
           task->getArrivalTime() <= elapsedTime)
        {
            if(task->getPriority() < highestPriority)
            {
                highestPriority =
                    task->getPriority();


                selectedTask = task;
            }
        }
    }


    if(selectedTask == nullptr)
    {
        return nullptr;
    }


    // Remove selected task from ready queue

    queue<Task*> newQueue;

    bool removed = false;

    while(!readyQueue.empty())
    {
        Task* task = readyQueue.front();

        readyQueue.pop();

        if(task == selectedTask && !removed)
        {
            removed = true;
        }
        else
        {
            newQueue.push(task);
        }
    }

    readyQueue = newQueue;

    return selectedTask;
}


// WAKE BLOCKED TASK

void Scheduler::wakeBlockedTask()
{
    if(blockedQueue.empty())
    {
        return;
    }


    Task* task = blockedQueue.front();

    blockedQueue.pop();


    task->setState(TaskState::READY);


    task->setReadySince(elapsedTime);


    readyQueue.push(task);


    cout << "[Wake Up] "
         << task->getName()
         << " : BLOCKED -> READY"
         << endl;
}


// RUN SCHEDULER

void Scheduler::runScheduler()
{
    cout << endl;


    cout << "======================================"
         << endl;


    if(algorithm ==
       SchedulingAlgorithm::ROUND_ROBIN)
    {
        cout << "       ROUND ROBIN SCHEDULER"
             << endl;
    }
    else
    {
        cout << "       PRIORITY SCHEDULER"
             << endl;
    }


    cout << "======================================"
         << endl;


    Task* previousTask = nullptr;


    // MAIN SCHEDULER LOOP

    while(true)
    {

        // Add tasks which have arrived

        addArrivedTasks();


        // Check whether all tasks are completed

        bool allCompleted = true;


        for(size_t i = 0;
            i < tasks.size();
            i++)
        {
            if(tasks[i]->getState() !=
               TaskState::TERMINATED)
            {
                allCompleted = false;

                break;
            }
        }


        if(allCompleted)
        {
            break;
        }


        // SELECT NEXT TASK

        Task* currentTask = nullptr;


        // ROUND ROBIN

        if(algorithm == SchedulingAlgorithm::ROUND_ROBIN)
        {
            if(!readyQueue.empty())
            {
                currentTask = readyQueue.front();


                readyQueue.pop();
            }
        }

        // PRIORITY

        else
        {
            currentTask = selectHighestPriorityTask();
        }


        // No READY task

        if(currentTask == nullptr)
        {
            // Check for blocked task
            if(!blockedQueue.empty())
            {
                wakeBlockedTask();

                continue;
            }


            // Check next arrival
            int nextArrival = -1;


            for(size_t i = 0; i < tasks.size(); i++)
            {
                Task* task = tasks[i];


                if(task->getState() == TaskState::READY && task->getArrivalTime() > elapsedTime)
                {
                    if(nextArrival == -1 || task->getArrivalTime() < nextArrival)
                    {
                        nextArrival = task->getArrivalTime();
                    }
                }
            }


            if(nextArrival != -1)
            {
                int idleTime = nextArrival - elapsedTime;


                cout << "CPU IDLE for " << idleTime << " ms" << endl;

                timerTick(idleTime);

                continue;
            }

            continue;
        }

        // CONTEXT SWITCH

        if(previousTask != nullptr && previousTask != currentTask)
        {
            contextSwitchCount++;

            cout << "[Context Switch] " << previousTask->getName() 
                 << " -> " << currentTask->getName() << endl;
        }


        // RUNNING

        currentTask->setState( TaskState::RUNNING);


        cout << endl;

        cout << "Running: "
             << currentTask->getName()<< " | Priority = " << currentTask->getPriority()
             << " | Remaining = "<< currentTask->getRemainingTime()<< " ms"<< endl;


        // SYNCHRONIZATION
        // synchronizationEnabled = false
        // Therefore Mutex/Semaphore do not interfere
        // with scheduling performance.

        if(synchronizationEnabled)
        {
            // MUTEX

            bool mutexAcquired = false;


            if(resourceMutex.isLocked() &&
               resourceMutex.getOwnerTaskId() ==
               currentTask->getId())
            {
                mutexAcquired = true;
            }
            else
            {
                mutexAcquired =
                    resourceMutex.lock(
                        currentTask->getId(),
                        currentTask->getName());
            }


            if(!mutexAcquired)
            {
                cout << "Mutex unavailable."
                     << endl;


                currentTask->setState(
                    TaskState::BLOCKED);


                blockedQueue.push(
                    currentTask);


                continue;
            }


            // SEMAPHORE
            bool semaphoreAcquired = false;

            if(ownsSemaphore( currentTask->getId()))
            {
                semaphoreAcquired = true;
            }
            else
            {
                semaphoreAcquired =
                    resourceSemaphore.wait();


                if(semaphoreAcquired)
                {
                    semaphoreOwners.push_back(
                        currentTask->getId());


                    cout << "[Semaphore ACQUIRED]"
                 << endl;


                    cout << "Available Permits : "
                         << resourceSemaphore.getCount()
                         << endl;
                }
            }


            if(!semaphoreAcquired)
            {
                cout << "Semaphore unavailable."
                     << endl;


                currentTask->setState(
                    TaskState::BLOCKED);


                blockedQueue.push(
                    currentTask);


                continue;
            }
        }



        // CALCULATE EXECUTION SLICE

        int executionSlice =
            min(
                timeQuantum,
                currentTask->getRemainingTime()
            );


        // EXECUTE

        currentTask->execute();


        currentTask->addExecutionTime(
            executionSlice);


        // TIMER

        timerTick(
            executionSlice);



        // UPDATE REMAINING TIME

        currentTask->setRemainingTime(
            currentTask->getRemainingTime()
            - executionSlice
        );


    
        // TASK COMPLETED

        if(currentTask->getRemainingTime() == 0)
        {
            currentTask->setState(
                TaskState::TERMINATED);


        
            // Completion Time

            currentTask->setCompletionTime(
                elapsedTime);

            // Turnaround Time
            // TAT = CT - AT

            currentTask->calculateTurnaroundTime();


            // Waiting Time
            //
            // WT = TAT - BT


            int waitingTime =
                currentTask->getTurnaroundTime()
                - currentTask->getExecutionTime();


            if(waitingTime > 0)
            {
                currentTask->addWaitingTime(
                    waitingTime);
            }


            cout << "Task Completed: "
                 << currentTask->getName()
                 << endl;


            // RELEASE MUTEX + SEMAPHORE
            // Only when synchronization is enable

            if(synchronizationEnabled)
            {
        
                // Mutex
                if(resourceMutex.isLocked() &&
                   resourceMutex.getOwnerTaskId() ==
                   currentTask->getId())
                {
                    resourceMutex.unlock(
                        currentTask->getId(),
                        currentTask->getName());


                    cout << "[Mutex UNLOCKED]"
                         << endl;


                    cout << "Released by : "
                         << currentTask->getName()
                         << endl;
                }


        
                // Semaphore

                for(size_t i = 0;
                    i < semaphoreOwners.size();
                    i++)
                {
                    if(semaphoreOwners[i] ==
                       currentTask->getId())
                    {
                        semaphoreOwners.erase(
                            semaphoreOwners.begin()
                            + i);


                        resourceSemaphore.signal();


                        cout << "[Semaphore RELEASED]"
                             << endl;


                        cout << "Available Permits : "
                             << resourceSemaphore.getCount()
                             << endl;


                        break;
                    }
                }


        
                // Wake blocked task


                wakeBlockedTask();
            }
        }

        // TIME QUANTUM EXPIRED


        else
        {
            currentTask->setState(
                TaskState::READY);


            // Task becomes READY again at current time
            currentTask->setReadySince(
                elapsedTime);


            
            // ROUND ROBIN:
            // Put current task at BACK of queue.
          
            // Sensor -> UART -> Logger -> Sensor...
            // -----------------------------------------

            readyQueue.push(
                currentTask);


            cout << "Time Quantum expired for "
                 << currentTask->getName()
                 << endl;


            cout << "Remaining Time = "
                 << currentTask->getRemainingTime()
                 << " ms"
                 << endl;
        }

        // SAVE CURRENT TASK

        previousTask =
            currentTask;
    }

    // It prints only ONCE after all tasks finish.

    cout << endl;


    cout << "========== SCHEDULER FINISHED =========="
         << endl;


    cout << "Timer Ticks       : "
         << tickCount
         << endl;


    cout << "Context Switches  : "
         << contextSwitchCount
         << endl;


    cout << "Total Time        : "
         << elapsedTime
         << " ms"
         << endl;
}


// PERFORMANCE METRICS

void Scheduler::displayPerformanceMetrics()
{
    cout << endl;


    cout << "=============================================="
         << endl;


    if(algorithm ==
       SchedulingAlgorithm::ROUND_ROBIN)
    {
        cout << "       ROUND ROBIN PERFORMANCE"
             << endl;
    }
    else
    {
        cout << "       PRIORITY PERFORMANCE"
             << endl;
    }


    cout << "=============================================="
         << endl;


    cout << endl;


    // Table Header

    cout << left
         << setw(15) << "Task"
         << setw(8)  << "AT"
         << setw(8)  << "BT"
         << setw(8)  << "CT"
         << setw(8)  << "TAT"
         << setw(8)  << "WT"
         << endl;


    cout << "------------------------------------------------"
         << endl;


    double totalTAT = 0;

    double totalWT = 0;


    // Task Metrics

    for(size_t i = 0;
        i < tasks.size();
        i++)
    {
        Task* task = tasks[i];


        cout << left
             << setw(15)
             << task->getName()

             << setw(8)
             << task->getArrivalTime()

             << setw(8)
             << task->getExecutionTime()

             << setw(8)
             << task->getCompletionTime()

             << setw(8)
             << task->getTurnaroundTime()

             << setw(8)
             << task->getWaitingTime()

             << endl;


        totalTAT +=
            task->getTurnaroundTime();


        totalWT +=
            task->getWaitingTime();
    }


    cout << "------------------------------------------------"
         << endl;



    // Average TAT / WT

    double averageTAT = 0;

    double averageWT = 0;


    if(!tasks.empty())
    {
        averageTAT =
            totalTAT /
            tasks.size();


        averageWT =
            totalWT /
            tasks.size();
    }


    cout << fixed
         << setprecision(2);


    cout << "Average TAT       : "
         << averageTAT
         << " ms"
         << endl;


    cout << "Average WT        : "
         << averageWT
         << " ms"
         << endl;


    cout << "Timer Ticks       : "
         << tickCount
         << endl;


    cout << "Context Switches  : "
         << contextSwitchCount
         << endl;


    cout << "=============================================="
         << endl;
}



// GETTERS

int Scheduler::getTickCount() const
{
    return tickCount;
}


int Scheduler::getContextSwitchCount() const
{
    return contextSwitchCount;
}


int Scheduler::getElapsedTime() const
{
    return elapsedTime;
}