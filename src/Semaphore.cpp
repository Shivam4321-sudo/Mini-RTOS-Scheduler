#include "Semaphore.h"

Semaphore::Semaphore(int initialCount)
{
    count = initialCount;
    maxCount = initialCount;
}

bool Semaphore::wait()
{
    if(count > 0)
    {
        count--;
        return true;
    }

    return false;
}

bool Semaphore::signal()
{
    if(count < maxCount)
    {
        count++;
        return true;
    }

    return false;
}

int Semaphore::getCount() const
{
    return count;
}

int Semaphore::getMaxCount() const
{
    return maxCount;
}