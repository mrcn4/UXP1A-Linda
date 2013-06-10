#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

namespace linda
{
    class Semaphore
    {
    public:
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual bool isLocked() = 0;
        virtual bool timedLock(int Miliseconds) = 0;
        virtual ~Semaphore() {}
    };

}
#endif // SEMAPHORE_HPP

