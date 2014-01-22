#ifndef MessageDeserializer_H
#define MessageDeserializer_H

class Message;

class MessageDeserializer
{
public:
    MessageDeserializer()
    {}

    virtual ~MessageDeserializer()
    {}

    virtual Message *Deserialize(char *pBuffer) = 0;

private:
    MessageDeserializer(const MessageDeserializer&);
    MessageDeserializer& operator=(const MessageDeserializer&);
};

#endif
