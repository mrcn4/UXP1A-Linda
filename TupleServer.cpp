#include "TupleServer.hpp"
#include "HelperFunctions.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <pthread.h>
#include "PosixSemaphore.hpp"
#include "errno.h"
#include "Globals.hpp"

using std::string;
using std::cout;
using std::endl;

#define READ 0
#define WRITE 1

    void
linda::TupleServer::init ( vector<string> ChildrenProcesses, vector<char**> ChildrenArgs )
{
    for(auto it = ChildrenProcesses.begin(); it != ChildrenProcesses.end(); ++it)
    {
        cout << *it << endl;
    }

    if(ChildrenProcesses.size() != ChildrenArgs.size())
    {

        cout << "not matching vector sizes" << endl;
        return;
    }

    cout <<"Rlimit is " << RLIMIT_NOFILE << endl;

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
        m_InputPipes.push_back(PipeDIn[READ]);
        m_OutputPipes.push_back(PipeDOut[WRITE]);

        CreationSemaphore->lock();
        if((ChildPid = fork()) != 0)
        {
            //parent case
            //close child ends
            close(PipeDIn[WRITE]);
            close(PipeDOut[READ]);
            
            //create semaphores
            string Sem1Name = getSemName(ChildPid,1);
            string Sem2Name = getSemName(ChildPid,2);
            try
            {
                Semaphore* Sem1 = new PosixSemaphore(Sem1Name.c_str(),O_CREAT,S_IRUSR | S_IWUSR,1);
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
            //wait for parent to establish semaphores basing on pid
            CreationSemaphore->lock();
            CreationSemaphore->unlock();
            //child case

            //cout<<"descs1: " <<PipeDIn[WRITE] << Globals::c_MaxFDExclusive-1<<endl;

            dup2(PipeDIn[WRITE],Globals::c_MaxFDExclusive-1);
            dup2(PipeDOut[READ],Globals::c_MaxFDExclusive-2);
            close(PipeDIn[WRITE]);
            close(PipeDOut[READ]);

            execvp(it->c_str(),*it2);
            cout << "Error: unknown command or wrong arguments!" << endl;
            return;
        }//end child case

        //test read from buffer
        if(read(PipeDIn[READ],&m_Msg,sizeof(MessageHeader)) == sizeof(MessageHeader))
        {
            cout << "    Server: received a message"<<endl;
            cout << "    m_Msg.tag: " << m_Msg.tag <<endl;
            cout << "    m_Msg.length: " << m_Msg.length <<endl;
            cout << "    ((bool)m_Msg.id==EMessageType::INPUT): " << ((bool)m_Msg.id==EMessageType::INPUT) << endl;

            int rv = read(PipeDIn[READ],&m_Msg.data,m_Msg.length);
            if(rv == -1)
            {
                cout << "errno is: " << errno ;
                if(errno == EAGAIN)
                    cout <<"(EAGAIN)" ;
                if(errno == EBADF)
                    cout <<"(EAGAIN)" ;
                if(errno == EFAULT)
                    cout <<"(EAGAIN)" ;
                cout <<endl;
            }
            cout <<"Read returned: " << rv << ". Data is: " << m_Msg.data << endl;
        }
        else
        {
            cout << "failure receiving test data" <<endl;
        }
    }

    //begin processing events: enter infinite loop
    //
    //
    return ;
}		// -----  end of method linda::CTupleServer::init  -----


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
    Vs.push_back(c_LsChar);
    Vs.push_back(c_LsChar);
    vector<char**> Args;
    
    char* CharArgs[2];                          // null terminated array of c strings
    CharArgs[0] = c_LsChar;
    CharArgs[1] = 0;

    Args.push_back(CharArgs);
    Args.push_back(CharArgs);
    Args.push_back(CharArgs);
    Args.push_back(CharArgs);

    Cts.init(Vs,Args);
    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
