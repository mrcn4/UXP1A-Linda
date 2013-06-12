//auto-tests
#include <iostream>
#include <unistd.h>
#include "TupleClient.hpp"

using namespace std;



void gen_random(char *s, const int len) {
    static const char alphanum[] = {
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"};

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}



// usage client produce consume
// produce/(produce+consume) -> perctentage of producer tasks
// consume/(produce+consume) -> perctentage of consumer tasks (input, read)
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


    //cout	<<  "Klient zakończył działanie powodzeniem" << endl;

    return EXIT_SUCCESS;
}
// ----------  end of function main  ----------
