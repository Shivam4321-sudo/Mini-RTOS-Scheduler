#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <iostream>

using namespace std;


class Semaphore
{
private:

    // Current number of available permits
    int count;

    // Maximum number of permits
    int maxCount;


public:

    // Constructor
    Semaphore(int initialCount);


    // Try to acquire one permit
    bool wait();


    // Release one permit
    bool signal();


    // Get current count
    int getCount() const;


    // Get maximum count
    int getMaxCount() const;
};

#endif