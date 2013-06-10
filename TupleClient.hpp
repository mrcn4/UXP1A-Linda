#ifndef TUPLECLIENT_HPP
#define TUPLECLIENT_HPP

#include "semaphore.h"
#include "fcntl.h"
#include "Semaphore.hpp"
#include "Message.hpp"
#include "Tuple.hpp"
#include <string>

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

        Tuple input(string inputTuple);

        private:
        Semaphore* m_Sem1;
        Semaphore* m_Sem2;
        int m_ReadFD;
        int m_WriteFD;
        int m_Tag;
        Message m_Msg;
        
    }; // -----  end of class CTupleClient  -----
}		// -----  end of namespace linda  -----

#endif
