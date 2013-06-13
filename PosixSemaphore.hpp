#ifndef POSIXSEMAPHORE_HPP
#define POSIXSEMAPHORE_HPP

#include <semaphore.h>
#include "Semaphore.hpp"
#include <string>

using std::string;

namespace linda
{
    /**
     * \brief Posix semaphore implementation of Semaphore interface
     */
    class PosixSemaphore : public Semaphore
    {
    public:
        PosixSemaphore(const char* Name, int Flags, mode_t Mode, int Value);
        PosixSemaphore(const char* Name, int Flags);
        virtual ~PosixSemaphore();

        //Semaphore interface implementation
        void lock();
        void unlock();
        bool isLocked();
        bool timedLock(int Miliseconds);
        bool unlink();


    private:
        PosixSemaphore(const PosixSemaphore&);
        PosixSemaphore& operator=( const PosixSemaphore& other );
        sem_t* m_sem;
        string m_name;
    };
}

#endif // POSIXSEMAPHORE_HPP
