#include <iostream>

#include "Task.h"
#include "Scheduler.h"

using namespace std;

// ROUND ROBIN TEST

void runRoundRobin()
{
    cout << endl;
    cout << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;

    cout << "          ROUND ROBIN TEST" << endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;


    // Quantum = 10 ms
    // Synchronization = OFF
    Scheduler scheduler(10,2,SchedulingAlgorithm::ROUND_ROBIN,false);

    // ID, Name, Priority, AT, BT
     Task* task1 = new Task(1,"Sensor",1,0,50);
    Task* task2 = new Task(2,"UART",2,0,40);
    Task* task3 = new Task(3,"Logger",3,0,60);

    scheduler.addTask(task1);
    scheduler.addTask(task2);
    scheduler.addTask(task3);



        cout << "All Tasks:" << endl;
    scheduler.displayTasks();

    cout << endl;

    cout << "Initial Ready Queue:" << endl;
    scheduler.displayReadyQueue();


    scheduler.runScheduler();

      scheduler.displayReadyQueue();
      
    scheduler.displayPerformanceMetrics();

    delete task1;
    delete task2;
    delete task3;
}


// PRIORITY TEST

void runPriority()
{
    cout << endl;
    cout << endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;

    cout << "          PRIORITY TEST"<< endl;

    cout << "~~~~~~~~~~~~~~~~~~~~~~~"<< endl;


    // Quantum is still kept as a safety time slice.
      // Quantum = 10 ms
    // Synchronization = OFF
    Scheduler scheduler(10,2,SchedulingAlgorithm::PRIORITY,false);

    Task* task1 = new Task(1,"Sensor",1,0,50);
    Task* task2 = new Task(2,"UART",2,0,40);
    Task* task3 = new Task(3,"Logger",3,0,60);

    scheduler.addTask(task1);
    scheduler.addTask(task2);
    scheduler.addTask(task3);
    scheduler.runScheduler();
    scheduler.displayPerformanceMetrics();

    delete task1;
    delete task2;
    delete task3;
}


int main()
{
  
    // Run Round Robin
    runRoundRobin();

    // Run Priority Scheduling
    runPriority();


    return 0;
}