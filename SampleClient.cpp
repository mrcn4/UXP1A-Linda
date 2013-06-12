#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "TupleClient.hpp"

using namespace std;
using namespace linda;

int main ( int argc, char *argv[] )
{
	timeval timeout;
	timeout.tv_sec = 3;
	timeout.tv_usec= 0;


    TupleClient* Ctc;
    try
    {
        Ctc = new TupleClient();

        /*unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed1);
        std::uniform_int_distribution<int> unif(0,5);*/

        if(getpid()%2)
        {
            Tuple tuple666;
            tuple666.push_back(0);
            Ctc->readd(string("INT == 1"),&timeout);
            Ctc->output(tuple666);
        }
        else
        {
            Tuple tuple666;
            tuple666.push_back(1);
            Ctc->output(tuple666);
            Ctc->input(string("INT == 0"),&timeout);
        }
        Ctc->readd(string("YNT == pińcet tysięcy"),&timeout);
    /*
        //send sth through pipe
        for(int i=0;i<1;++i)
        {

            int inputChoice = unif(generator);

            switch(inputChoice)
            {
            case 0:
                input(string("INT == * INT == * INT == *"));
                break;
            case 1:
                input(string("INT == * INT == *"));
                break;
            case 2:
                readd(string("INT == 0"));
                break;
            case 3:
                input(string("INT == 0"));
                break;
            default:
                input(string("STR == *"));
                break;
            }
        }*/

    }
    catch(...)
    {
        cout<< "Problem z inicjaliacj� klienta" <<endl;
        return EXIT_FAILURE;
    }
    sleep(5);
    cout	<< "! Klient zako�czy� dzia�anie powodzeniem. (Stwierdzam �mier� bo umar.)" << endl;

    return EXIT_SUCCESS;
}
// ----------  end of function main  ----------

