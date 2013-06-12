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

    /**
     * \brief Simple structure used to pass messages through unix pipes
     */
    struct Message
    {
        // BEGIN OF INSERTED MessageHeader (X11 Style)
        char id;
        //(one byte of alignment)
        short tag;
        short length;
        // END OF INSERTED MessageHeader

        char data[PIPE_BUF-sizeof(MessageHeader)];

        /**
         * \brief Used for pipe write calls.
         *
         * \returns Whole message size
         */
        int messageSize() {
             return sizeof(MessageHeader) + length;
        }

        /**
         * \brief Inserts string to data buffer. 
         *
         * \param InsString string to insert
         *
         * \returns whether it succeeded
         */
        bool insertString(string InsString) {
			if(InsString.length()+1 < PIPE_BUF-sizeof(MessageHeader))
			{
				strcpy(data,InsString.c_str());
				length = InsString.length()+1; // end char \0 => +1
				return true;
			}
			return false;
		}

    };

}
#endif // MESSAGE_HPP
