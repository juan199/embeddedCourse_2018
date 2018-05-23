#ifndef MAILBOX_HPP_
#define MAILBOX_HPP_
#include "msp.h"
#define MAX_MESSAGE_QUEUE 50

struct st_Message
{
    bool     bMessageValid = false;   // - True when message is valid
    uint8_t  u8DestinationID = 0U; // - Destination Task ID
    uint8_t  u8SourceID = 0U;      // - Source Task ID
    uint8_t  u8MessageCode = 0U;   // - Message code, interpreted by Destination
    uint32_t u32MessageData = 0U;  // - Message data, interpreted by Destination
    int16_t i16MessageData1 = 0;  // - Message data, interpreted by Destination
    int16_t i16MessageData2 = 0;  // - Message data, interpreted by Destination
    int16_t * pPayload = (uintptr_t) 0;       // - Message Payload, interpreted by Destination
};

class Mailbox
{
public:
    static Mailbox* getMailbox();
    bool sendMessage(st_Message i_stMessage);
    st_Message getMessage(uint8_t i_u8MailboxID);

private:
    Mailbox(){};
    static Mailbox* MailObj;
    st_Message m_stMessageQueue[MAX_MESSAGE_QUEUE];
};

#endif /* MAILBOX_HPP_ */
