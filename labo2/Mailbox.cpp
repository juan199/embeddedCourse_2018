#include <Mailbox.hpp>

Mailbox* Mailbox::MailObj = nullptr;

extern "C"
{
    Mailbox* Mailbox::getMailbox()
    {
        if(MailObj == nullptr)
        {
            MailObj = new Mailbox();
        }

        return MailObj;
    }

    st_Message Mailbox::getMessage(uint8_t i_u8MailboxID)
    {
        st_Message l_stMessage;

        l_stMessage = m_stMessageQueue[i_u8MailboxID];
        m_stMessageQueue[l_stMessage.u8DestinationID].bMessageValid = false;

        return(l_stMessage);
    }

    bool Mailbox::sendMessage(st_Message i_stMessage)
    {
        if(m_stMessageQueue[i_stMessage.u8DestinationID].bMessageValid == false)
        {
            m_stMessageQueue[i_stMessage.u8DestinationID] = i_stMessage;
            return(true);
        }
        else
        {
            return(false);
        }
    }
}


