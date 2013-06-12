#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

namespace linda
{
    /**
     * \brief Simple abstract semaphore interface
     */
    class Semaphore
    {
    public:
        /**
         * \brief Lock/wait method
         */
        virtual void lock() = 0;
        /**
         * \brief Unlock/post method
         */
        virtual void unlock() = 0;
        /**
         * \brief Checks whether is locked.
         *
         * \returns Whether semaphore is locked
         */
        virtual bool isLocked() = 0;
        /**
         * \brief Lock/wait with timeout
         *
         * \param Miliseconds timeout value in miliseconds
         *
         * \returns 
         */
        virtual bool timedLock(int Miliseconds) = 0;
        /**
         * \brief Destructor.
         */
        virtual ~Semaphore() {}
    };

}
#endif // SEMAPHORE_HPP

