// =====================================================================================
// 
//       Filename:  TupleServer.hpp
// 
//    Description:  Header of class CTupleServer
// 
//        Version:  1.0
//        Created:  03.06.2013 11:24:44
//       Revision:  none
//       Compiler:  g++
//         Author:  Marcin Swend (ms), M.Swend@mion.elka.pw.edu.pl
// 
// =====================================================================================


#include <vector>
#include <iostream>

#include <semaphore.h>
#include <fcntl.h>

#include "Semaphore.hpp"

using std::vector;
using std::string;

namespace linda {

    /**
     * \brief Class  which creates clients and proceses their requests 
     */
    class TupleServer
    {
        public:
        /**
         * \brief Initializes TupleServer. Creates child processes whose names
         * are passed in parameter and enters infinite loop processing client's
         * requests
         *
         * \param ChildrenProcesses vector of processes' names
         */
        void init(vector<string> ChildrenProcesses, vector<char**> ChildrenArgs );
        
        private:
        vector<int> m_InputPipes;
        vector<int> m_OutputPipes;
        vector<Semaphore*> m_Sem1;
        vector<Semaphore*> m_Sem2;
    };
}		// -----  end of namespace linda  -----
