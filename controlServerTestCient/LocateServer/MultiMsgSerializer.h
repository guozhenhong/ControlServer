#ifndef MultiMsgSerializer_H
#define MultiMsgSerializer_H

#include <map>

class MessageSerializer;
class Message;

class MultiMsgSerializer
{
public:
	MultiMsgSerializer();
	virtual ~MultiMsgSerializer();

	char * Serialize(unsigned long ID, Message *pMsg, int *pBufLen);
	int Register(unsigned long msgID, MessageSerializer *pSerializer);

private:
	std::map<unsigned long, MessageSerializer*> m_MsgSerializerTable;	
	std::map<unsigned long, MessageSerializer*>::iterator m_it;
};


#endif