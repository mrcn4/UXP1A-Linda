#ifndef TUPLECLIENTREQUEST_HPP
#define TUPLECLIENTREQUEST_HPP

#include "TupleQuery.hpp"
//#include "Message.hpp"
//#include <cstring>

namespace linda
{
	//EQueryType {INPUT, OUTPUT, READ};

	struct ParsedClientRequest
	{
		char id;
		short tag;
        int clientId;
        TupleQuery tq; //TODO: zrobic w ParsedClientRequest poprawny konstruktor, puste TupleQuery jest semantycznie niepoprawne!!!
	};

    //extern ParsedClientRequest deserializeClientRequest(const char *buffer); // is it useful?
}


#endif //TUPLECLIENTREQUEST_HPP
