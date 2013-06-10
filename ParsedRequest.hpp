#include "TupleDatabase.hpp"
#include "Message.hpp"
#include <cstring>

namespace linda
{
	//EQueryType {INPUT, OUTPUT, READ};

	struct ParsedRequest
	{
		char id;
		short tag;
        int clientId;
        TupleQuery tq;
	};

    extern ParsedRequest deserializeReq(char *buffer); // is it useful?
}
