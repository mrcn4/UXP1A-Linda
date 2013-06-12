#ifndef TUPLECLIENTREQUEST_HPP
#define TUPLECLIENTREQUEST_HPP

#include "TupleQuery.hpp"
//#include "Message.hpp"
//#include <cstring>

namespace linda
{
	//EQueryType {INPUT, OUTPUT, READ};

    /**
     * \brief Structure used to save client requests to fulfill them later
     */
	struct ParsedClientRequest
	{
        /**
         * \brief Request type
         */
		char id;
        /**
         * \brief Unique request id (per process)
         */
		short tag;
        /**
         * \brief Server's client ID
         */
        int clientId;
        /**
         * \brief Parsed client request ready to query DB.
         */
        TupleQuery tq; //TODO: zrobic w ParsedClientRequest poprawny konstruktor, puste TupleQuery jest semantycznie niepoprawne!!!
	};

    //extern ParsedClientRequest deserializeClientRequest(const char *buffer); // is it useful?
}


#endif //TUPLECLIENTREQUEST_HPP
