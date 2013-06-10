#include "ParsedRequest.hpp"

//using namespace linda;

/*
deserializacja danych z postaci binarnej
param char* buffer - zapytanie zserializowane do postaci binarnej
return ParsedRequest - strukturka umozliwiajaca przechowywanie zapytania
*/

linda::ParsedRequest linda::deserializeReq(char *buffer)
{
	linda::ParsedRequest out;
	linda::MessageHeader mh;
	memcpy(&mh, buffer, sizeof(MessageHeader));
	char* query = new char[mh.length];
	memcpy(query, buffer + sizeof(MessageHeader), mh.length);
	out.id = mh.id;
	out.tag = mh.tag;
	out.tq = TupleQuery(query);
	delete query;
	return out;	
}
