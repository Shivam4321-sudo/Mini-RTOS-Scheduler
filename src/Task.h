#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>

using namespace std;

enum class TaskState
{
    READY,
    RUNNING,
    WAITING,
    BLOCKED,
    TERMINATED
};

struct TaskContext
{
    int programCounter;
    int stackPointer;
    int registers[4];
};

class Task
{
private:

    int id;
    string name;
    int priority;
    int executionTime;
    int remainingTime;

    TaskState state;
    TaskContext context;


    // Day 9 - Performance Metrics
    int arrivalTime;
    int completionTime;
    int actualExecutionTime;
    int waitingTime;
    int turnaroundTime;

    int readySince;

public:

      // Constructor
    Task(int id,
         string name,
         int priority,
         int arrivalTime,
         int burstTime);

    // getter
    int getId() const;
    string getName() const;
    int getPriority() const;

    int getExecutionTime() const;
    int getRemainingTime() const;
    
    TaskState getState() const;

    // Setter
    void setPriority(int priority);
    void setState(TaskState state);
    void setRemainingTime(int time);

    // Execution
    void execute();
    
    // Context switching
    void saveContext();
    void restoreContext();

    // Display
    void printTask() const;

    // Day 9 Metrics
    void setArrivalTime(int time);
    int getArrivalTime() const;

    void setCompletionTime(int time);
    int getCompletionTime() const;

    void addExecutionTime(int time);
    int getActualExecutionTime() const;

    void addWaitingTime(int time);
    int getWaitingTime() const;

    void setReadySince(int time);
    int getReadySince() const;

    void calculateTurnaroundTime();
    int getTurnaroundTime() const;

};

#endif