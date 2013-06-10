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
    cout<<"List of procs to run:\n";
    for(auto it = ChildrenProcesses.begin(); it != ChildrenProcesses.end(); ++it)
    {
        cout << *it << endl;
    }

    if(ChildrenProcesses.size() != ChildrenArgs.size())
    {
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
            cout << "failure creating pipe" << endl;
            return;
        }
        if(pipe(PipeDIn) < 0) 
        {
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

            cout << "Error: unknown command or wrong arguments!" << endl;
            return;
        }//end child case

    }

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
            cout<<"All clients disconnected"<<endl;
            return;
        }
        //endof select preparation

        //1st arg: "the highest-numbered file descriptor in any of the three sets, plus 1."
        //2nd arg: read set; 3rd arg: write set (0 FD); 4th arg: alarm set (0 FD)
        //5th arg: timeout (no timeout - till infinity)
        rv = select(max_fd + 1, &set, NULL, NULL, NULL);
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
                            handle_input(clientCount);
                        break;
                        case EMessageType::OUTPUT:
                            handle_output(clientCount);
                        break;
                        case EMessageType::CANCEL_REQUEST:
                            handle_cancel(clientCount);
                        break;
                        case EMessageType::READ:
                            handle_read(clientCount);
                        break;
                        default:
                            cout<<"Sever error: undefined message type";
                            return;
                    }
                }
                else
                {
                    close(m_InputPipes[clientCount]);
                    close(m_OutputPipes[clientCount]);
                    delete(m_Sem1[clientCount]);
                    delete(m_Sem2[clientCount]);

                    //remove
                    m_InputPipes.erase(m_InputPipes.begin()+clientCount);
                    m_OutputPipes.erase(m_OutputPipes.begin()+clientCount);
                    m_Sem1.erase(m_Sem1.begin()+clientCount);
                    m_Sem2.erase(m_Sem2.begin()+clientCount);
                }
            }
            ++clientCount;
        }
    }
    return ;
    }

void linda::TupleServer::handle_input(int ClientNo)
{
    cout << "\n  Server: received input message"<<endl;
    cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
    cout << "    m_Msg.length: " << m_Msg.length <<endl;

    int rv = read(m_InputPipes[ClientNo],&m_Msg.data,m_Msg.length);
    if(rv == -1)
    {
        cout << "Read test data failed. Errno is: " << errno ;
    }
    else if (rv == m_Msg.length)
    {
        //valid input request.
        cout <<"    Read returned: " << rv << " bytes. Data is: " << string(m_Msg.data) << endl;

        //if can be satisied now, do it now (unsynchonized on DB level)
        {
            //mutex needed if mt-server
            string query = string(m_Msg.data);
            TupleQuery tq(query);
            Tuple t = m_DB.input(tq);
            if(t.size()>0)
            {
                bool Result = sendTupleIfStillRequested(ClientNo,t);
                if(false == Result)
                {
                    cout<<"send failed, return tuple";
                    m_DB.output(t);
                }
            }
            else
            {
                cout<<"can't be sat now\n";
                ParsedRequest pr;
                pr.clientId = ClientNo;
                pr.id=m_Msg.id;
                pr.tag=m_Msg.tag;
                pr.tq = tq;
                m_Requests.push_back(pr);
            }
        }
    }

}

void linda::TupleServer::handle_read(int ClientNo)
{
    cout << "\n  Server: received read message"<<endl;
    cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
    cout << "    m_Msg.length: " << m_Msg.length <<endl;

    int rv = read(m_InputPipes[ClientNo],&m_Msg.data,m_Msg.length);
    if(rv == -1)
    {
        cout << "Read test data failed. Errno is: " << errno ;
    }
    else if (rv == m_Msg.length)
    {
        //valid input request.
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

            Tuple t = m_DB.read(tq);
            if(t.size()>0)
            {
                bool Result = sendTupleIfStillRequested(ClientNo,t);
            }
            else
            {
                cout<<"can't be sat now\n";

                ParsedRequest pr;
                pr.clientId = ClientNo;
                pr.id=m_Msg.id;
                pr.tag=m_Msg.tag;
                pr.tq = tq;
                m_Requests.push_back(pr);
            }
        }
    }
}

void linda::TupleServer::handle_cancel(int ClientNo)
{
    cout << "\n  Server: received cancel message"<<endl;
    cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
    cout << "    m_Msg.length: " << m_Msg.length <<endl;

    auto i = std::begin(m_Requests);
    while (i != std::end(m_Requests)) {
        if(i->clientId == ClientNo)
        {
            m_Requests.erase(i);
            break;
        }
    }

}

void linda::TupleServer::handle_output(int ClientNo)
{
    cout << "\n  Server: received output message"<<endl;
    cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
    cout << "    m_Msg.length: " << m_Msg.length <<endl;

    int rv = read(m_InputPipes[ClientNo],&m_Msg.data,m_Msg.length);
    if(rv == -1)
    {
        cout << "Read test data failed. Errno is: " << errno ;
    }
    else if (rv == m_Msg.length)
    {
        //valid input request.
        cout <<"    Read returned: " << rv << " bytes. Data is: " << string(m_Msg.data) << endl;

        {
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

            m_DB.output(t);

            //ack output
            m_Msg.id = EMessageType::OUTPUT_ACK;
            m_Msg.length = 0;
            //tag is the same
            write(m_OutputPipes[ClientNo],&m_Msg,m_Msg.messageSize());

            //redo all queries (TODO: should be before output and only new typle with requests)
            auto i = std::begin(m_Requests);
            while (i != std::end(m_Requests)) {
                if(i->id == EMessageType::INPUT)
                {
                    Tuple t = m_DB.input(i->tq);
                    if(t.size()>0)
                    {
                        //tuple found
                        bool Result = sendTupleIfStillRequested(i->clientId,t);
                        if(false == Result)
                        {
                            m_DB.output(t);
                        }
                        i = m_Requests.erase(i);
                        continue;
                    }
                }
                else if(i->id == EMessageType::READ)
                {
                    Tuple t = m_DB.read(i->tq);
                    if(t.size()>0)
                    {
                        sendTupleIfStillRequested(i->clientId,t);
                        i = m_Requests.erase(i);
                        continue;
                    }
                }
                else
                {
                    cout<<"Wrong message type in requests." << endl;
                }

                ++i;
            }
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
        cout<<"Cancelled?"<<endl;
        //new message received!
        if(m_Msg.id == EMessageType::CANCEL_REQUEST)
        {
            //ewidentnie cancel.
            cout<<"Cancelled;"<<endl;
        }
        else
        {
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
        cout	<< "You should provide name of program to launch (for example absolute 'client' path from make)" << endl;
    }

    linda::TupleServer Cts;
    vector<string> Vs;
    char* c_LsChar = argv[1];
    Vs.push_back(c_LsChar);
    Vs.push_back(c_LsChar);
    vector<char**> Args;
    
    char* CharArgs[2];                          // null terminated array of c strings
    CharArgs[0] = c_LsChar;
    CharArgs[1] = 0;

    Args.push_back(CharArgs);
    Args.push_back(CharArgs);
    Cts.init(Vs,Args);
    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
