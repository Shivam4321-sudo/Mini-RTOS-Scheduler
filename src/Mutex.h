#ifndef MUTEX_H
#define MUTEX_H

#include <iostream>
#include <string>

using namespace std;


class Mutex
{
private:

    bool locked;

    int ownerTaskId;

    string ownerTaskName;


public:

    Mutex();


    // Try to acquire mutex
    bool lock(
        int taskId,
        string taskName
    );


    // Release mutex
    bool unlock(
        int taskId,
        string taskName
    );


    bool isLocked() const;


    int getOwnerTaskId() const;


    string getOwnerTaskName() const;
};

#endif