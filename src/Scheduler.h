#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include <queue>
#include <windows.h>

#include "Task.h"
#include "Mutex.h"
#include "Semaphore.h"

using namespace std;

// Scheduling Algorithm
enum class SchedulingAlgorithm
{
    ROUND_ROBIN,
    PRIORITY
};

class Scheduler
{
private:

    // Task collection
    vector<Task*> tasks;

    // Ready Queue
    queue<Task*> readyQueue;

    // Blocked Queue
    queue<Task*> blockedQueue;

    // Scheduler parameters
    int timeQuantum;
    int tickCount;
    int elapsedTime;

    // Day 9
    int contextSwitchCount;

    // Algorithm
    SchedulingAlgorithm algorithm;

    // Synchronization
    Mutex resourceMutex;
    Semaphore resourceSemaphore;
    int sharedResource;

    // RR-Correction
    bool synchronizationEnabled;

    // Tasks currently holding semaphore permits
    vector<int> semaphoreOwners;


     // Priority Selection
    Task* selectHighestPriorityTask();


    // Semaphore
    bool ownsSemaphore(int taskId);

    // Wake blocked task
    void wakeBlockedTask();

    // Add arrived tasks
    void addArrivedTasks();


public:

    // Constructor
   Scheduler(int quantum,
          int semaphoreCount,
          SchedulingAlgorithm algorithm,
          bool enableSynchronization = true);

   // Task Management
    // ==================================

    void addTask(Task* task);
    void displayTasks();
    void displayReadyQueue();

    // Timer
    void timerTick(int duration);

    // Run Scheduler
    void runScheduler();

    // Day 9
    void displayPerformanceMetrics();

    // Getters
    int getTickCount() const;
    int getContextSwitchCount() const;
    int getElapsedTime() const;
};

#endif