#ifndef Message_H
#define Message_H

class Message
{
public:
        Message(unsigned long lMsgID) : m_clMsgID(m_lMsgID)
        {
        	m_lMsgID = lMsgID;
        }
        virtual ~Message()
        {}

public:
        const unsigned long& m_clMsgID;

private:
        Message(const Message&);
        Message& operator=(const Message&);

protected:
        unsigned long m_lMsgID;
};

#endif