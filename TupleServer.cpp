#include "TupleServer.hpp"
#include "HelperFunctions.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <pthread.h>
#include "PosixSemaphore.hpp"
#include "errno.h"
#include "Globals.hpp"
#include "Tuple.hpp"

using std::string;
using std::cout;
using std::endl;
using linda::Tuple;

#define READPIPE 0
#define WRITEPIPE 1

    void
linda::TupleServer::init ( vector<string> ChildrenProcesses, vector<char**> ChildrenArgs )
{
    if(Globals::c_Debug)
    {
        cout<<"List of procs to run:\n";
        for(auto it = ChildrenProcesses.begin(); it != ChildrenProcesses.end(); ++it)
        {
            cout << *it << endl;
        }
        cout<<"List of args to run:\n";
        for(auto it = ChildrenArgs.begin(); it != ChildrenArgs.end(); ++it)
        {
            cout << *it[0] << endl;
        }
    }


    if(ChildrenProcesses.size() != ChildrenArgs.size())
    {
        if(Globals::c_Debug)
            cout << "not matching processnames/args vector sizes" << endl;
        return;
    }

    //run processes
    Semaphore* CreationSemaphore= new PosixSemaphore("linda_creationSem",O_CREAT,S_IRUSR | S_IWUSR,1);
    auto it = ChildrenProcesses.begin();
    auto it2 = ChildrenArgs.begin();
    for(;it != ChildrenProcesses.end(); ++it,++it2)
    {
        int PipeDOut[2];
        int PipeDIn[2];
        int ChildPid;

        if(pipe(PipeDOut) < 0)
        {
            if(Globals::c_Debug)
                cout << "failure creating pipe" << endl;
            return;
        }
        if(pipe(PipeDIn) < 0) 
        {
            if(Globals::c_Debug)
                cout << "failure creating pipe" << endl;
            return;
        }
        m_InputPipes.push_back(PipeDIn[READPIPE]);
        m_OutputPipes.push_back(PipeDOut[WRITEPIPE]);

        CreationSemaphore->lock();
        if((ChildPid = fork()) != 0)
        {
            //parent case
            //close child ends
            close(PipeDIn[WRITEPIPE]);
            close(PipeDOut[READPIPE]);
            
            //create semaphores
            string Sem1Name = getSemName(ChildPid,1);
            string Sem2Name = getSemName(ChildPid,2);
            try
            {
                Semaphore* Sem1 = new PosixSemaphore(Sem1Name.c_str(),O_CREAT,S_IRUSR | S_IWUSR,1); //Shared ptrs maybe?
                Semaphore* Sem2 = new PosixSemaphore(Sem2Name.c_str(),O_CREAT,S_IRUSR | S_IWUSR,1);
                m_Sem1.push_back(Sem1);
                m_Sem2.push_back(Sem2);
            }
            catch(...)
            {
                if(Globals::c_Debug)
                    cout << "Error creating semaphore!" << endl;
                CreationSemaphore->unlock();
                return;
            }
            CreationSemaphore->unlock();
        } //end parent case
        else
        {
            //child case
            //wait for parent to establish semaphores basing on pid
            CreationSemaphore->lock();
            CreationSemaphore->unlock();

            dup2(PipeDIn[WRITEPIPE],Globals::c_WriteFD);
            dup2(PipeDOut[READPIPE],Globals::c_ReadFD);
            close(PipeDIn[WRITEPIPE]);
            close(PipeDOut[READPIPE]);

            execvp(it->c_str(),*it2); //should never return
            if(Globals::c_Debug)
                cout << "Error: unknown command or wrong arguments!" << endl;
            return;
        }//end child case

    }

    CreationSemaphore->unlink();
    delete CreationSemaphore;

    //begin processing events: enter infinite loop
    bool LongLiveTheServer = true;
    while(LongLiveTheServer)
    {
        //select preparation
        fd_set set;
        int rv; //select return value
        int max_fd =-1; //dynamic for fd can change and its cost is low
        FD_ZERO(&set); // clear the set
        for(int ReadFd : m_InputPipes)
        {
            if(ReadFd>max_fd) max_fd = ReadFd;
            FD_SET(ReadFd, &set); // add file descriptor to the set
        }
        if(max_fd == -1)
        {
            if(Globals::c_Debug)
                cout<<"All clients disconnected"<<endl;
            return;
        }
        //endof select preparation

        //1st arg: "the highest-numbered file descriptor in any of the three sets, plus 1."
        //2nd arg: read set; 3rd arg: write set (0 FD); 4th arg: alarm set (0 FD)
        //5th arg: timeout (no timeout - till infinity)
        rv = select(max_fd + 1, &set, NULL, NULL, NULL);
        if(Globals::c_Debug)
            cout<<"Server select finish\n";
        if (rv <= 0)
        {
            cout<<"Server select unexpected error\n";
        }
        int clientCount = 0;
        for(int ReadFd : m_InputPipes)
        {
            if(FD_ISSET(ReadFd, &set))
            {
                int rv;
                if((rv = read(ReadFd,&m_Msg,sizeof(MessageHeader))) == sizeof(MessageHeader))
                {
                    //decide of program flow based on incomming message type
                    switch(m_Msg.id)
                    {
                        case EMessageType::INPUT:
                            handle_input_read(clientCount,true);
                        break;
                        case EMessageType::OUTPUT:
                            handle_output(clientCount);
                        break;
                        case EMessageType::CANCEL_REQUEST:
                            handle_cancel(clientCount);
                        break;
                        case EMessageType::READ:
                            handle_input_read(clientCount,false);
                        break;
                        default:
                            if(Globals::c_Debug)
                                cout<<"Server error: undefined message type";
                            return;
                    }
                }
                else
                {
                    close(m_InputPipes[clientCount]);
                    close(m_OutputPipes[clientCount]);

                    m_Sem1[clientCount]->unlink();
                    delete(m_Sem1[clientCount]);
                    m_Sem2[clientCount]->unlink();
                    delete(m_Sem2[clientCount]);

                    //remove
                    m_InputPipes.erase(m_InputPipes.begin()+clientCount);
                    m_OutputPipes.erase(m_OutputPipes.begin()+clientCount);
                    m_Sem1.erase(m_Sem1.begin()+clientCount);
                    m_Sem2.erase(m_Sem2.begin()+clientCount);

                    --clientCount;
                }
            }
            ++clientCount;
        }
    }
    return ;
}

bool linda::TupleServer::doInputRead(int ClientNo, bool InputReqest,TupleQuery TQ, Tuple t)
{
    if(Globals::c_Debug)
        cout<<"doInputRead" << endl;
    if(InputReqest)
    {
        if(t.size() == 0)
            Tuple t = m_DB.input(TQ);

        if(t.size()>0)
        {
            bool Result = sendTupleIfStillRequested(ClientNo,t);
            if(false == Result)
            {
                if(Globals::c_Debug)
                    cout<<"send failed, return tuple";
                m_DB.output(t);
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(t.size() == 0)
            Tuple t = m_DB.read(TQ);
        if(t.size()>0)
        {
            sendTupleIfStillRequested(ClientNo,t);
            return true;
        }
        else
        {
            return false;
        }
    }
}

void linda::TupleServer::handle_input_read(int ClientNo, bool InputReqest)
{
    if(Globals::c_Debug)
    {
        cout << "\n  Server: received data access message"<<endl;
        cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
        cout << "    m_Msg.length: " << m_Msg.length <<endl;
    }

    int rv = read(m_InputPipes[ClientNo],&m_Msg.data,m_Msg.length);
    if(rv == -1)
    {
        if(Globals::c_Debug)
            cout << "Read test data failed. Errno is: " << errno ;
    }
    else if (rv == m_Msg.length)
    {
        //valid input request.
        if(Globals::c_Debug)
            cout <<"    Read returned: " << rv << " bytes. Data is: " << string(m_Msg.data) << endl;

        //if can be satisied now, do it now (unsynchonized on DB level)
        {
            //mutex needed if mt-server
            string query = string(m_Msg.data);
            TupleQuery tq;
            try
            {
                tq = TupleQuery(query);
            }
            catch(std::logic_error)
            {
                //wrong pattern
                m_Msg.id = EMessageType::WRONG_PATTERN;
                m_Msg.length=0;
                int rv = write(m_OutputPipes[ClientNo],&m_Msg.data,m_Msg.messageSize());
                return;
            }

            if(!doInputRead(ClientNo,InputReqest,tq))
            {
                if(Globals::c_Debug)
                    cout<<"can't be sat now\n";

                ParsedClientRequest pr;
                pr.clientId = ClientNo;
                pr.id=m_Msg.id;
                pr.tag=m_Msg.tag;
                pr.tq = tq;
                m_WaitingRequests.push_back(pr);
            }
        }
    }
}

void linda::TupleServer::handle_cancel(int ClientNo)
{
    if(Globals::c_Debug)
    {
        cout << "\n  Server: received cancel message"<<endl;
        cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
        cout << "    m_Msg.length: " << m_Msg.length <<endl;
    }

    auto i = std::begin(m_WaitingRequests);
    while (i != std::end(m_WaitingRequests)) {
        if(i->clientId == ClientNo)
        {
            if(Globals::c_Debug)
                cout<<"Client request cancelled by client.\n";
        	m_WaitingRequests.erase(i);
            break;
        }
    }

}

void linda::TupleServer::handle_output(int ClientNo)
{
    if(Globals::c_Debug)
    {
        cout << "\n  Server: received output message"<<endl;
        cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
        cout << "    m_Msg.length: " << m_Msg.length <<endl;
    }

    int rv = read(m_InputPipes[ClientNo],&m_Msg.data,m_Msg.length);
    if(rv == -1)
    {
        if(Globals::c_Debug)
            cout << "Read test data failed. Errno is: " << errno ;
    }
    else if (rv == m_Msg.length)
    {
        //valid input request.
        if(Globals::c_Debug)
            cout <<"    Read returned: " << rv << " bytes. Data is: " << string(m_Msg.data) << endl;

        Tuple t;
        try{
            t.deserialize(string(m_Msg.data));
        }
        catch(std::invalid_argument)
        {
            //ack output
            m_Msg.id = EMessageType::OUTPUT_ERROR;
            m_Msg.length = 0;
            //tag is the same
            write(m_OutputPipes[ClientNo],&m_Msg,m_Msg.messageSize());
        }

        //ack output
        m_Msg.id = EMessageType::OUTPUT_ACK;
        m_Msg.length = 0;
        //tag is the same
        write(m_OutputPipes[ClientNo],&m_Msg,m_Msg.messageSize());

        //redo all queries
        bool inputFlag = false;
        auto i = TupleDatabase::searchParsedClientRequest(m_WaitingRequests,t);
        while(i != std::end(m_WaitingRequests)) {
            if(Globals::c_Debug)
                cout<<"    Tuple request for output tuple found!" << endl;

            if(i->id == EMessageType::INPUT)
            {
                bool Result = doInputRead(i->clientId,true,i->tq,t);
                if(Globals::c_Debug)
                    cout<<"    Tuple sent? "  << Result << endl;
                i = m_WaitingRequests.erase(i);
                inputFlag = true;
                break;
            }
            else if(i->id == EMessageType::READ)
            {

                bool Result = doInputRead(i->clientId,false,i->tq,t);
                if(Globals::c_Debug)
                    cout<<"    Tuple sent? "  << Result << endl;
                i = m_WaitingRequests.erase(i);
                continue;
            }
            else
            {
                if(Globals::c_Debug)
                    cout<<"Wrong message type in requests." << endl;
            }

            i = TupleDatabase::searchParsedClientRequest(m_WaitingRequests,t);
        }
        if(!inputFlag)
        {
            if(Globals::c_Debug)
                cout<<"No Tuple input request maches output tuple, put it to db." << endl;

            //if there was no input request, output the tuple
            m_DB.output(t);
        }
    }
}


bool linda::TupleServer::sendTupleIfStillRequested(int ClientNo, linda::Tuple &t)
{
    m_Sem2[ClientNo]->lock();
    bool returnValue = false;

    //sprawdź czy zapytanie klienta o krotkę jest wciąż aktualne.
    //Jeżeli odczytana zostanie wiadomość cancel_request z id zgodnym z id zapytania zapytanie powinno zostać anulowane
    int flags = fcntl(m_InputPipes[ClientNo], F_GETFL, 0);
    fcntl(m_InputPipes[ClientNo], F_SETFL, flags | O_NONBLOCK);
    int readRV = read(m_InputPipes[ClientNo],&m_Msg,sizeof(MessageHeader));
    fcntl(m_InputPipes[ClientNo], F_SETFL, flags);

    if(readRV == sizeof(MessageHeader))
    {
        if(Globals::c_Debug)
            cout<<"Cancelled?"<<endl;
        //new message received!
        if(m_Msg.id == EMessageType::CANCEL_REQUEST)
        {
            //ewidentnie cancel.
            if(Globals::c_Debug)
                cout<<"Cancelled;"<<endl;
        }
        else
        {
            if(Globals::c_Debug)
                cout<<"sth different"<<endl;
        }
        returnValue = false;
    }
    else if(m_Sem1[ClientNo]->isLocked())
    {
        //client is ready

        //construct answer message
        //set message type to tuple return
        m_Msg.id = EMessageType::TUPLE_RETURN;

        //insert tuple to message
        m_Msg.insertString(t.serialize());

        //answer with tuple
        write(m_OutputPipes[ClientNo],&m_Msg,m_Msg.messageSize());

        returnValue = true;
    }
    else
    {
        returnValue=false;
    }
    m_Sem2[ClientNo]->unlock();
    return returnValue;
}
    int
main ( int argc, char *argv[] )
{

    if(argc<2)
    {
        cout	<< "You should provide names of programs to launch (for example absolute 'client' path from make)" << endl;
    }

    linda::TupleServer Cts;
    vector<string> Vs;
    vector<char**> Args;



    for(int i=1;i<argc;++i)
    {
        char* c_ClientProgramName = argv[i];
        Vs.push_back(c_ClientProgramName);

        char** CharArgs = new char*[2]; // null terminated array of c strings
        CharArgs[1] = 0;
        CharArgs[0] = c_ClientProgramName;
        Args.push_back(CharArgs);
    }

    Cts.init(Vs,Args);
    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
