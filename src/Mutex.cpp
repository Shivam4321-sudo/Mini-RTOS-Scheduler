#include "Mutex.h"


Mutex::Mutex()
{
    locked = false;

    ownerTaskId = -1;

    ownerTaskName = "";
}


// LOCK

bool Mutex::lock(
    int taskId,
    string taskName
)
{
    // Mutex is free
    if(!locked)
    {
        locked = true;

        ownerTaskId = taskId;

        ownerTaskName = taskName;


        cout << "\n[Mutex LOCKED]\n";

        cout << "Owner : "
             << taskName
             << endl;


        return true;
    }


    // Mutex already owned
    cout << "\n[Mutex BUSY]\n";

    cout << "Current Owner : "
         << ownerTaskName
         << endl;


    cout << "Task "
         << taskName
         << " cannot acquire mutex"
         << endl;


    return false;
}


// UNLOCK

bool Mutex::unlock(
    int taskId,
    string taskName
)
{
    // Only owner can unlock
    if(
        locked &&
        ownerTaskId == taskId
    )
    {
        locked = false;

        ownerTaskId = -1;

        ownerTaskName = "";


        cout << "\n[Mutex UNLOCKED]\n";

        cout << "Released by : "
             << taskName
             << endl;


        return true;
    }


    cout << "\n[Mutex ERROR]\n";

    cout << "Task "
         << taskName
         << " is not the mutex owner"
         << endl;


    return false;
}


// Is Locked

bool Mutex::isLocked() const
{
    return locked;
}

// Owner ID

int Mutex::getOwnerTaskId() const
{
    return ownerTaskId;
}


// Owner Name
string Mutex::getOwnerTaskName() const
{
    return ownerTaskName;
}