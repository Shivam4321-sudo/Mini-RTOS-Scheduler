#include "Task.h"


// =====================================================
// Constructor
// =====================================================

Task::Task(int id,
           string name,
           int priority,
           int arrivalTime,
           int burstTime)
{
    this->id = id;
    this->name = name;
    this->priority = priority;

    this->arrivalTime = arrivalTime;

    this->executionTime = burstTime;
    this->remainingTime = burstTime;

    this->state = TaskState::READY;


    // ==========================================
    // Simulated CPU Context
    // ==========================================

    context.programCounter = 0;

    context.stackPointer = 1000;

    for(int i = 0; i < 4; i++)
    {
        context.registers[i] = 0;
    }


    // ==========================================
    // Day 9 Metrics
    // ==========================================

    completionTime = 0;

    actualExecutionTime = 0;

    waitingTime = 0;

    turnaroundTime = 0;

    readySince = arrivalTime;
}


// =====================================================
// Getters
// =====================================================

int Task::getId() const
{
    return id;
}


string Task::getName() const
{
    return name;
}


int Task::getPriority() const
{
    return priority;
}


int Task::getExecutionTime() const
{
    return executionTime;
}


int Task::getRemainingTime() const
{
    return remainingTime;
}


TaskState Task::getState() const
{
    return state;
}


// =====================================================
// Setters
// =====================================================

void Task::setPriority(int priority)
{
    this->priority = priority;
}


void Task::setState(TaskState state)
{
    this->state = state;
}


void Task::setRemainingTime(int time)
{
    remainingTime = time;
}


// =====================================================
// Execute
// =====================================================

void Task::execute()
{
    context.programCounter++;

    context.registers[0]++;
}


// =====================================================
// Save Context
// =====================================================

void Task::saveContext()
{
    context.programCounter++;

    context.stackPointer--;

    context.registers[0]++;
}


// =====================================================
// Restore Context
// =====================================================

void Task::restoreContext()
{
    cout << "Restoring context of Task "
         << id
         << " ("
         << name
         << ")"
         << endl;

    cout << "PC = "
         << context.programCounter
         << ", SP = "
         << context.stackPointer
         << endl;
}


// =====================================================
// Print Task
// =====================================================

void Task::printTask() const
{
    cout << "Task ID       : "
         << id
         << endl;

    cout << "Task Name     : "
         << name
         << endl;

    cout << "Priority      : "
         << priority
         << endl;

    cout << "Arrival Time  : "
         << arrivalTime
         << " ms"
         << endl;

    cout << "Burst Time    : "
         << executionTime
         << " ms"
         << endl;

    cout << "Remaining     : "
         << remainingTime
         << " ms"
         << endl;

    cout << "-----------------------------"
         << endl;
}


// =====================================================
// Day 9 - Arrival Time
// =====================================================

void Task::setArrivalTime(int time)
{
    arrivalTime = time;
}


int Task::getArrivalTime() const
{
    return arrivalTime;
}


// =====================================================
// Day 9 - Completion Time
// =====================================================

void Task::setCompletionTime(int time)
{
    completionTime = time;
}


int Task::getCompletionTime() const
{
    return completionTime;
}


// =====================================================
// Day 9 - Actual Execution Time
// =====================================================

void Task::addExecutionTime(int time)
{
    actualExecutionTime += time;
}


int Task::getActualExecutionTime() const
{
    return actualExecutionTime;
}


// =====================================================
// Day 9 - Waiting Time
// =====================================================

void Task::addWaitingTime(int time)
{
    if(time > 0)
    {
        waitingTime += time;
    }
}


int Task::getWaitingTime() const
{
    return waitingTime;
}


// =====================================================
// Day 9 - Ready Since
// =====================================================

void Task::setReadySince(int time)
{
    readySince = time;
}


int Task::getReadySince() const
{
    return readySince;
}


// =====================================================
// Day 9 - Turnaround Time
// =====================================================

void Task::calculateTurnaroundTime()
{
    turnaroundTime =
        completionTime - arrivalTime;
}


int Task::getTurnaroundTime() const
{
    return turnaroundTime;
}