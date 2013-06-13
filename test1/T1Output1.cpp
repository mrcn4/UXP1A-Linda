#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "../TupleClient.hpp"

using namespace std;
using namespace linda;

int main ( int argc, char *argv[] )
{
	timeval timeout;
    timeout.tv_sec = 5;
	timeout.tv_usec= 0;


    TupleClient* Ctc;
    try
    {
        Ctc = new TupleClient();
    }
    catch(...)
    {
        cout<< "Client \"" << argv[0] << "\" initalization failure." <<endl;
        return EXIT_FAILURE;
    }

    // CLIENT REQUESTS GO HERE \/

    Tuple t;
    t.push_back(1);

    sleep(2);

    bool Result = Ctc->output(t);
    cout<< "Client \"" << argv[0] << "\" tuple send success? " <<Result <<endl;

    //END OF CLIENT REQUESTS

    cout<< "Client \"" << argv[0] << "\" returns success." <<endl;

    return EXIT_SUCCESS;
}
// ----------  end of function main  ----------

