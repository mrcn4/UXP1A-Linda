#ifndef MESSAGE_HPP
#define MESSAGE_HPP
#include <limits.h>

namespace linda
{
    enum EMessageType {INPUT,OUTPUT,READ,CANCEL_REQUEST,OUTPUT_ACK,TUPLE_RETURN,OUTPUT_ERROR,WRONG_PATTERN} ;

    struct MessageHeader
    {
        char id;
        //one byte of alignment
        short tag;
        short length;
    };

    struct Message
    {
        char id;
        //one byte of alignment
        short tag;
        short length;
        char data[PIPE_BUF-6];
    };
}
#endif // MESSAGE_HPP
