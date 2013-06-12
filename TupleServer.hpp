#include <vector>
#include <iostream>
#include <list>

#include <semaphore.h>
#include <fcntl.h>
#include "Semaphore.hpp"
#include "Message.hpp"
#include "TupleDatabase.hpp"

using std::vector;
using std::list;

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
        void handle_input(int ClientNo);
        void handle_output(int ClientNo);
        void handle_read(int ClientNo);
        void handle_cancel(int ClientNo);
        bool sendTupleIfStillRequested(int ClientNo, Tuple& t);

        vector<int> m_InputPipes;
        vector<int> m_OutputPipes;
        vector<Semaphore*> m_Sem1;
        vector<Semaphore*> m_Sem2;
        Message m_Msg;
        TupleDatabase m_DB;
        list<ParsedClientRequest> m_WaitingRequests;

    };
}		// -----  end of namespace linda  -----
