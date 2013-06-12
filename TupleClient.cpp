#include "TupleClient.hpp"

using std::cout;
using std::endl;
using std::string;
using linda::Tuple;

linda::TupleClient::TupleClient (): m_ReadFD(Globals::c_ReadFD), m_WriteFD(Globals::c_WriteFD), m_Tag(0)
{
    //open semaphore
    string Sem1Name = getSemName(getpid(),1);
    string Sem2Name = getSemName(getpid(),2);
    
    m_Sem1 = new PosixSemaphore(Sem1Name.c_str(),0);
    m_Sem2 = new PosixSemaphore(Sem2Name.c_str(),0);

    if(is_valid_fd(m_ReadFD) && is_valid_fd(m_WriteFD))
    {
        //cout <<"twój deskryptor jest ok\n";
    }
    else
    {
        cout <<"ERROR: twój deskryptor jest inwalidą\n";
    }

    //mam nadzieje ze nie usunalem nic waznego z konstruktora
    //przeniesiono do SampleClient.cpp
}


linda::TupleClient::~TupleClient()
{
    delete m_Sem1;
    delete m_Sem2;
}


Tuple linda::TupleClient::input(string pattern, timeval* timeout)
{
    return readTupleImpl(EMessageType::INPUT,pattern, timeout);
}

Tuple linda::TupleClient::readd(string pattern, timeval* timeout)
{
    return readTupleImpl(EMessageType::READ,pattern, timeout);
}

Tuple linda::TupleClient::readTupleImpl(EMessageType type, string pattern, timeval* timeout)
{
    m_Sem1->lock();

    //send message to server
    m_Msg.id = type;
    m_Msg.tag = m_Tag++;
    m_Msg.insertString(pattern.c_str());
    write(m_WriteFD,&m_Msg,m_Msg.messageSize());

    //wait with semaphore up with timeout
    fd_set set;

    int rv; //select return value

    FD_ZERO(&set); // clear the set
    FD_SET(m_ReadFD, &set); // add our file descriptor to the set

    //set timeout to <globals.hpp> seconds
    //changed -> closer to blinowski style
   // timeout.tv_sec = Globals::c_ClientTimeoutSeconds;
    //timeout.tv_usec = 0;

    //1st arg of select is "the highest-numbered file descriptor in any of the three sets, plus 1."
    rv = select(m_ReadFD + 1, &set, NULL, NULL, timeout);
    if(rv == -1)
    {
        //select error
        cout <<"Select fatal error. errno of select is " <<errno<<endl;
        m_Sem1->unlock();
        return Tuple(); //return false
    }
    else if(rv == 0)
    {
        cout<<"TIMEOUT"<<endl;
        //timeout. One needs to check whether tuple was send between timeout and sending cancel request
        m_Sem1->unlock();
        m_Sem2->lock();

        //async read
        int flags = fcntl(m_ReadFD, F_GETFL, 0);
        fcntl(m_ReadFD, F_SETFL, flags | O_NONBLOCK);
        int readRV = read(m_ReadFD,&m_Msg,sizeof(MessageHeader));
        fcntl(m_ReadFD, F_SETFL, flags);

        if(readRV == sizeof(MessageHeader)) //wiadomość wysłana wyslana po select przed s1.unlock
        {
            if(m_Msg.id == EMessageType::TUPLE_RETURN)
            {
                 bool ReadSuccess = (read(m_ReadFD,&m_Msg.data,m_Msg.length) == m_Msg.length);
                 Tuple t;
                 if(ReadSuccess)
                 {
                     t.deserialize(string(m_Msg.data));
                 }
                 m_Sem2->unlock();
                 return t;
            }
            else
            {
               //TODO: else if NOT tuple_return, ex.server error..
                cout<<"server returned error"  <<endl;
                return Tuple();
            }
        }
        else{
            //send cancel request
            m_Msg.id = EMessageType::CANCEL_REQUEST;
            m_Msg.tag = m_Tag;
            m_Msg.length = 0;
            write(m_WriteFD,&m_Msg,m_Msg.messageSize());
        }
        m_Sem2->unlock();
        return Tuple();
    }
    else
    {
        cout<<"Client select success!\n";

        //select returned due to existence of something to read.
        read( m_ReadFD, &m_Msg, sizeof(MessageHeader));
        if(m_Msg.id == EMessageType::TUPLE_RETURN)
        {
            bool ReadSuccess = (read(m_ReadFD,&m_Msg.data,m_Msg.length) == m_Msg.length);
            Tuple t;
            if(ReadSuccess)
            {
                t.deserialize(string(m_Msg.data));
            }
            cout<<"Client received a beautiful Tuple: "<<t.serialize()<<endl;

            m_Sem1->unlock();
            return t;
        }
        else
        {
            //TODO: else if NOT tuple_return, ex.server error..
            cout<<"server returned error"  <<endl;
            m_Sem1->unlock();
            return Tuple();
        }
    }
}

bool linda::TupleClient::output(const Tuple& t)
{
    //send message to server
    m_Msg.id = EMessageType::OUTPUT;
    m_Msg.tag = m_Tag++;
    m_Msg.insertString(t.serialize());
    write(m_WriteFD,&m_Msg,m_Msg.messageSize());

    //read output ack!
    read(m_ReadFD,&m_Msg,sizeof(MessageHeader));
    if(m_Msg.id == EMessageType::OUTPUT_ACK)
    {
        cout <<"output ok"<<endl;
        return true;
    }
    else if(m_Msg.id == EMessageType::OUTPUT_ERROR)
    {
        cout <<"output error"<<endl;
        return false;
    }
    else
    {
        cout <<"unexpected output answer"<<endl;
        return false;
    }
   return true;

}


/*
//moved to SampleClient.cpp

int main ( int argc, char *argv[] )
{   
    linda::TupleClient* Ctc;
    try
    {
        Ctc = new linda::TupleClient();
    }
    catch(...)
    {
        cout<< "Problem z inicjaliacją klienta" <<endl;
        return EXIT_FAILURE;
    }
    sleep(5);
    cout	<< "! Klient zakończył działanie powodzeniem. (Stwierdzam śmierć bo umar.)" << endl;

    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
*/
