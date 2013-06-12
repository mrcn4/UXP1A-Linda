#ifndef TUPLECLIENT_HPP
#define TUPLECLIENT_HPP


#include "Message.hpp"
#include "Tuple.hpp"
#include "PosixSemaphore.hpp"
#include "Globals.hpp"
#include "HelperFunctions.hpp"


#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <sys/resource.h>
#include <random>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <cstring>


using std::string;

namespace linda {
    /**
     * \brief Class used for communicating with TupleServer
     */
    class TupleClient
    {
    public:
        // ====================  LIFECYCLE     =======================================
        TupleClient ();                             // constructor
        ~TupleClient ();
        Tuple input(string inputTuple, timeval* timeout);
        Tuple readd(string inputTuple, timeval* timeout);
        bool output(const Tuple& tuple);

    private:
        Tuple readTupleImpl(EMessageType type, string inputTuple, timeval* timeout);
        Semaphore* m_Sem1;
        Semaphore* m_Sem2;
        int m_ReadFD;
        int m_WriteFD;
        int m_Tag;
        Message m_Msg;


    }; // -----  end of class CTupleClient  -----
}		// -----  end of namespace linda  -----

#endif
