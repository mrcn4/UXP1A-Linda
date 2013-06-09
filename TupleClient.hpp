// =====================================================================================
// 
//       Filename:  TupleClient.hpp
// 
//    Description:  Header of CTupleClient class
// 
//        Version:  1.0
//        Created:  03.06.2013 16:26:21
//       Revision:  none
//       Compiler:  g++
//         Author:  Marcin Swend (ms), M.Swend@mion.elka.pw.edu.pl
// 
// =====================================================================================

#include "semaphore.h"
#include "fcntl.h"
#include "Semaphore.hpp"

namespace linda {
    /**
     * \brief Class used for communicating with TupleServer
     */
    class TupleClient
    {
        public:
        // ====================  LIFECYCLE     =======================================
        TupleClient ();                             // constructor

        private:
        Semaphore* m_Sem1;
        Semaphore* m_Sem2;
        int m_ReadFD;
        int m_WriteFD;

        
    }; // -----  end of class CTupleClient  -----
}		// -----  end of namespace linda  -----
