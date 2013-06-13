//auto-tests
#include "TupleClient.hpp"

#include <iostream>
#include <unistd.h>
#include <string>
#include <signal.h>
#include <time.h>

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
//*/


Tuple create_random_tuple(int type) {
	Tuple t;
	type %= 2; //2 types avaliable

	if(type==0) {
		//int int
		t.push_back(rand()%101); //0-100
		t.push_back(rand()%101); //0-100
	}
	else if(type==1) {
		//int string
		t.push_back(rand()%11); //0-10
		string stri;
		random_str(stri,3); //stri.size() == 3
		t.push_back(stri); //3 chars
	}

	return t;
}


string create_random_query(int type) {
	string q;
	type %= 2; //2 types avaliable

	if(type==0) {
		//int 0-100 int 0-100

		for(int i=0;i<2;++i) {
			q += "INT ";
			int op_type = rand()%3;
			if(op_type==0)
				q+="<= ";
			else if(op_type==1)
				q+=">= ";
			else if(op_type==2) {
				q+="== ";
				if(rand()%3 == 0) //33% chance
				{
					//accept all
					q+="* ";
					continue;
				}
			}

			q+= to_string(rand()%101) + " ";
		}

	}
	else if(type==1) {
		//int 0-10 string 3

		for(int i=0;i<1;++i) {
			q += "INT ";
			int op_type = rand()%3;
			if(op_type==0)
				q+="<= ";
			else if(op_type==1)
				q+=">= ";
			else if(op_type==2) {
				q+="== ";
				if(rand()%3 == 0) //33% chance
				{
					//accept all
					q+="* ";
					continue;
				}
			}

			q+= to_string(rand()%11) + " ";
		}

		q+="STR ";

		int op_type = rand()%3;
		if(op_type==0)
			q+="<= ";
		else if(op_type==1)
			q+=">= ";
		else if(op_type==2) {
			q+="== ";
			if(rand()%3 == 0) //33% chance
			{
				//accept all
				q+="* ";
				return q;
			}
		}

		string stri;
		random_str(stri,3); //stri.size() == 3
		q+= stri + " ";
	}


	return q;
}

void do_random_in() {
	Tuple t = create_random_tuple(rand());

	cout << "OUTPUT "<< t.serialize() << endl;

	//Tuple t2;
	//t2.deserialize(t.serialize());
	//cout << "TEST "<< (t == t2) << endl;

}


void do_random_out() {
	string q = create_random_query(rand());

	cout << "INPUT "<< q << endl;
}

// usage: client produce_uint consume_uint
// p/(p+c) -> percentage of producer tasks (output)
// c/(p+c) -> percentage of consumer tasks (input, read)
int main ( int argc, char *argv[] )
{
	//catchSignals();
	srand(time(NULL));

    try
    {
        Ctc = new TupleClient();
    }
    catch(...)
    {
        cout<< "Problem z inicjaliacj� klienta" <<endl;
        return EXIT_FAILURE;
    }

//    if(argc < 3) {
//    	cout<<"usage: client produce_uint consume_uint"<<endl;
//
//    	return EXIT_FAILURE;
//    }
//
    unsigned int produce = 1;//stoi(string(argv[1]));
    unsigned int consume = 2;//stoi(string(argv[2]));


    while(1) {
    	int rnd = rand() % (produce + consume);

    	if(rnd < produce) {
    		//do produce
    		do_random_in();
    	} else {
    		//do consume
    		do_random_out();
    	}

    	usleep(1000*(rand()%501)); //0-500ms;
    }

    //cout	<<  "Klient zako�czy� dzia�anie powodzeniem" << endl;

    return EXIT_SUCCESS;
}
// ----------  end of function main  ----------
