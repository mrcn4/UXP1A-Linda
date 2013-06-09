// =====================================================================================
// 
//       Filename:  TupleServer.cpp
// 
//    Description:  Implementation of class CTupleServer
// 
//        Version:  1.0
//        Created:  03.06.2013 11:21:47
//       Revision:  none
//       Compiler:  g++
//         Author:  Marcin Swend (ms), M.Swend@mion.elka.pw.edu.pl
// 
// =====================================================================================

#include "TupleServer.hpp"
#include "HelperFunctions.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/resource.h>
#include <pthread.h>
#include "PosixSemaphore.hpp"

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

    auto it = ChildrenProcesses.begin();
    auto it2 = ChildrenArgs.begin();
    for(;it != ChildrenProcesses.end(); ++it,++it2)
    {
        int PipeDOut[2];
        int PipeDIn[2];
        int ChildPid;

        if(pipe(PipeDIn) < 0) 
        {
            cout << "failure creating pipe" << endl;
            return;
        }
        if(pipe(PipeDOut) < 0) 
        {
            cout << "failure creating pipe" << endl;
            return;
        }
        m_InputPipes.push_back(PipeDIn[READ]);
        m_OutputPipes.push_back(PipeDOut[WRITE]);

        if((ChildPid = fork()) != 0)
        {
            //parent case
            close(PipeDIn[WRITE]);
            close(PipeDOut[READ]);
            
            string Sem1Name = getSemName(ChildPid,1);
            string Sem2Name = getSemName(ChildPid,2);  
            cout << Sem1Name << endl;
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
                return;
            }
        } //end parent case
        else
        {
            //child case
            dup2(PipeDIn[WRITE],RLIMIT_NOFILE-1);
            close(PipeDIn[WRITE]);
            dup2(PipeDOut[READ],RLIMIT_NOFILE-2);
            close(PipeDOut[READ]);

            execvp(it->c_str(),*it2);
            cout << "Error: unknown command or wrong arguments!" << endl;
            return;
        }//end child case

        //test read from buffer
        char buf = 0;
        if(read(PipeDIn[READ],&buf,1)>0)
        {
            cout << buf << endl;
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
