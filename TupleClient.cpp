#include "HelperFunctions.hpp"
#include "TupleClient.hpp"
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <sys/resource.h>
#include "PosixSemaphore.hpp"
#include "Message.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <cstring>
#include "errno.h"

using std::cout;
using std::endl;
using std::string;
using linda::Tuple;

linda::TupleClient::TupleClient (): m_ReadFD(RLIMIT_NOFILE-2), m_WriteFD(RLIMIT_NOFILE-1), m_Tag(0)
{
    //open semaphore
    string Sem1Name = getSemName(getpid(),1);
    string Sem2Name = getSemName(getpid(),2);
    
    m_Sem1 = new PosixSemaphore(Sem1Name.c_str(),0);
    m_Sem1 = new PosixSemaphore(Sem2Name.c_str(),0);

    //send sth through pipe
    //char buf = 'k';
    //write(m_WriteFD,&buf,1);
    sleep(1);
    input(string("test"));
}

Tuple linda::TupleClient::input(std::string pattern)
{
    //send message to server
    m_Msg.id = EMessageType::INPUT;
    m_Msg.tag = m_Tag++;
    m_Msg.length = pattern.length()+1;//header len + data length
    strcpy(m_Msg.data,pattern.c_str());
    cout<<"m_Msg.data is "<<m_Msg.data<<endl;
    write(m_WriteFD,&m_Msg,sizeof(MessageHeader) + m_Msg.length);

    m_Sem1->lock();
    //wait with semaphore up with timeout
    fd_set set;
    struct timeval timeout;
    int rv; //select return value

    FD_ZERO(&set); // clear the set
    FD_SET(m_ReadFD, &set); // add our file descriptor to the set

    //set timeout to one second
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    //1st arg of select is "the highest-numbered file descriptor in any of the three sets, plus 1."
    rv = select(m_ReadFD + 1, &set, NULL, NULL, &timeout);
    cout<<"select returned>>>>>>>>>>>>>>>>>>>>>>>>>>>>> "<< rv <<endl;
    cout <<"errno of select is " <<errno <<endl;
    if(errno == EBADF)
    {
        cout <<"errno of select is EBADF" <<endl;
    }
    if(rv == -1)
    {
        //select error
        m_Sem1->unlock();
        return Tuple(); //return false
    }
    else if(rv == 0)
    {
        //timeout. One needs to check whether tuple was send between timeout and sending cancel request
        m_Sem1->unlock();
        m_Sem2->lock();

        int flags = fcntl(m_ReadFD, F_GETFL, 0);
        fcntl(m_ReadFD, F_SETFL, flags | O_NONBLOCK);
        int readRV = read(m_ReadFD,&m_Msg,sizeof(MessageHeader));
        fcntl(m_ReadFD, F_SETFL, flags | O_NONBLOCK);

        if(readRV == sizeof(MessageHeader)) //krotka wyslana po select przed s1.unlock
        {
            if(m_Msg.id == EMessageType::TUPLE_RETURN)
            {
                 read(m_ReadFD,&m_Msg+sizeof(MessageHeader),m_Msg.length);
                 //TODO: error handle, tuple read, deserialize and return it
                 m_Sem2->unlock();
                 return Tuple()/* read_tuple()*/;

            }
        }
        else{
            //send_cancel_request()
        }
        m_Sem2->unlock();
    }
    else
    {
        Message Msg;
        //good selection
        read( m_ReadFD, &Msg, sizeof(MessageHeader));
        //if good then good TODO:check if return tuple
        read( m_ReadFD, &Msg+sizeof(MessageHeader), Msg.length);

        //deserialize tuple
        // HERE

        m_Sem1->unlock();
        return Tuple(); //TODO:deserialize
    }
}

    int
main ( int argc, char *argv[] )
{   
    linda::TupleClient* Ctc;
    try
    {
        Ctc = new linda::TupleClient();
    }
    catch(std::logic_error& e)
    {

        cout	<< e.what() << endl;
        return EXIT_FAILURE;
    }
sleep(4);
    cout	<< "Koniec pracy klienta, sukces." << endl;

    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
