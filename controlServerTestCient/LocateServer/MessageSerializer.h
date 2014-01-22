#ifndef MessageSerializer_H
#define MessageSerializer_H

class Message;

class MessageSerializer
{
public:
        MessageSerializer()
        {}
        virtual ~MessageSerializer()
        {}

        virtual char *Serialize(Message *pMsg, int *pFullLength) = 0;

private:
        MessageSerializer(const MessageSerializer&);
        MessageSerializer& operator=(const MessageSerializer&);
};

#endif
