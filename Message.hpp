#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <limits.h>
#include <string>
#include <cstring>

using std::string;

namespace linda
{
    enum EMessageType {INPUT,OUTPUT,READ,CANCEL_REQUEST,OUTPUT_ACK,TUPLE_RETURN,OUTPUT_ERROR,WRONG_PATTERN} ;

    struct MessageHeader
    {
        char id;
        //(one byte of alignment)
        short tag;
        short length;
    };

    struct Message
    {
        // BEGIN OF INSERTED MessageHeader (X11 Style)
        char id;
        //(one byte of alignment)
        short tag;
        short length;
        // END OF INSERTED MessageHeader

        char data[PIPE_BUF-sizeof(MessageHeader)];

        bool insertString(string InsString) {
			if(InsString.length()+1 < PIPE_BUF-sizeof(MessageHeader))
			{
				strcpy(data,InsString.c_str());
				length = InsString.length()+1; // end char \0 => +1
				return true;
			}
			return false;
		}
		
		
        int messageSize() {
			 return sizeof(MessageHeader) + length;
		}
    };

}
#endif // MESSAGE_HPP
