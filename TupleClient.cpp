// =====================================================================================
// 
//       Filename:  TupleClient.cpp
// 
//    Description:  Implementation of class CTupleClient
// 
//        Version:  1.0
//        Created:  03.06.2013 16:27:59
//       Revision:  none
//       Compiler:  g++
//         Author:  Marcin Swend (ms), M.Swend@mion.elka.pw.edu.pl
// 
// =====================================================================================

#include "HelperFunctions.hpp"
#include "TupleClient.hpp"
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <sys/resource.h>
#include "PosixSemaphore.hpp"

using std::cout;
using std::endl;
using std::string;

linda::TupleClient::TupleClient (): m_ReadFD(RLIMIT_NOFILE-2), m_WriteFD(RLIMIT_NOFILE-1)
{
    //open semaphore
    string Sem1Name = getSemName(getpid(),1);
    string Sem2Name = getSemName(getpid(),2);
    
    m_Sem1 = new PosixSemaphore(Sem1Name.c_str(),0);
    m_Sem1 = new PosixSemaphore(Sem2Name.c_str(),0);

    //send sth through pipe
    char buf = 'k';
    write(m_WriteFD,&buf,1);
}  // -----  end of method CTupleClient::CTupleClient  (constructor)  -----

    int
main ( int argc, char *argv[] )
{   
    try
    {
        linda::TupleClient Ctc;
    }
    catch(std::logic_error& e)
    {

        cout	<< e.what() << endl;
        return EXIT_FAILURE;
    }

    cout	<< "Sukces." << endl;
    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
