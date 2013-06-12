//auto-tests
#include "TupleClient.hpp"

#include <iostream>
#include <unistd.h>
#include <string>
#include <signal.h>

using namespace std;
using namespace linda;


void random_str(string& s, const int len) {
    static const char alphanum[] = {
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"};

    for (int i = 0; i < len; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

}


sigset_t signalSet;
TupleClient* Ctc = NULL;

/*
void signalHandler(int signum) {
	sigprocmask(SIG_BLOCK, &signalSet, NULL);
	delete Ctc;
	printf("EXIT DUE TO SIGNAL!\n"); //printf not allowed in handler
    exit(EXIT_FAILURE);
}

void catchSignals() {
	//in case of emergency
	sigemptyset(&signalSet);
	sigset(SIGABRT|SIGBUS|SIGFPE|SIGILL|SIGINT|SIGQUIT|SIGSEGV|SIGTERM|SIGSYS, signalHandler);
	sigprocmask(SIG_SETMASK, &signalSet, NULL);
}
*/

// usage: client produce_uint consume_uint
// p/(p+c) -> percentage of producer tasks (output)
// c/(p+c) -> percentage of consumer tasks (input, read)
int main ( int argc, char *argv[] )
{
	//catchSignals();

    try
    {
        Ctc = new TupleClient();
    }
    catch(...)
    {
        cout<< "Problem z inicjaliacją klienta" <<endl;
        return EXIT_FAILURE;
    }

//    if(argc < 3) {
//    	cout<<"usage: client produce_uint consume_uint"<<endl;
//
//    	return EXIT_FAILURE;
//    }
//
//    unsigned int produce = stoi(string(argv[1]));
//    unsigned int consume = stoi(string(argv[2]));


    while(1)
    	sleep(1);


    //cout	<<  "Klient zakończył działanie powodzeniem" << endl;

    return EXIT_SUCCESS;
}
// ----------  end of function main  ----------
