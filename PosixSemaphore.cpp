#include "PosixSemaphore.hpp"
#include <stdexcept>
#include <ctime>
#include <errno.h>

linda::PosixSemaphore::PosixSemaphore(const char* name, int flags ,mode_t mode, int value)
{
    m_sem = sem_open(name,flags,mode, value);
    if(m_sem == SEM_FAILED)
    {
        throw std::logic_error("sem_open failed!");
    }
}

linda::PosixSemaphore::PosixSemaphore(const char* name, int flags)
{
    m_sem = sem_open(name,flags);
    if(m_sem == SEM_FAILED)
    {
        throw std::logic_error("sem_open failed!");
    }
}\

void linda::PosixSemaphore::lock()
{
    int Result = sem_wait(m_sem);
    if(Result != 0)
    {
        throw std::logic_error("sem_wait failed!");
    }
}

void linda::PosixSemaphore::unlock()
{
    int Result = sem_post(m_sem);
    if(Result != 0)
    {
        throw std::logic_error("sem_post failed!");
    }
}

bool linda::PosixSemaphore::isLocked()
{
    int SemValue;
    int Result = sem_getvalue(m_sem,&SemValue);
    if(Result != 0)
    {
        throw std::logic_error("sem_getvalue failed!");
    }
    return (SemValue == 0);
}

bool linda::PosixSemaphore::timedLock(int Miliseconds)
{
    //requires absolute time
    timespec TimeSpec;
    clock_gettime(CLOCK_REALTIME, &TimeSpec);
    int Sec=Miliseconds/1000;
    int NSec = 1000*(Miliseconds - TimeSpec.tv_sec*1000);
    TimeSpec.tv_sec += Sec;
    TimeSpec.tv_nsec += NSec;

    int Result = sem_timedwait(m_sem,&TimeSpec);
    if(Result == -1)
    {
        if(errno == ETIMEDOUT)
        {
            return false;
        }
        else
        {
            throw std::logic_error("sem_timedwait failed!");
        }
    }
    return true;
}
