#include <iostream>
#include <unistd.h>
#include "TupleClient.hpp"

using namespace std;


int main ( int argc, char *argv[] )
{   
    linda::TupleClient* Ctc;
    try
    {
        Ctc = new linda::TupleClient();
    }
    catch(...)
    {
        cout<< "Problem z inicjaliacj� klienta" <<endl;
        return EXIT_FAILURE;
    }
    sleep(5);
    cout	<< "! Klient zako�czy� dzia�anie powodzeniem. (Stwierdzam �mier� bo umar.)" << endl;

    return EXIT_SUCCESS;
}				// ----------  end of function main  ----------