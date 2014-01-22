#ifndef MULTIMSGDESERIALIZER_H
#define MULTIMSGDESERIALIZER_H

#include <iostream>
#include <map>
class Message;
class MessageDeserializer;

class MultiMsgDeserializer
{
public:
	MultiMsgDeserializer();
	virtual ~MultiMsgDeserializer();

	int Register(unsigned long msgID, MessageDeserializer* pDeserializer);
	Message* Deserialize(unsigned long msgID, char *pBuffer);

private:
	std::map<unsigned long, MessageDeserializer*> m_MsgDeserializerTable;
	std::map<unsigned long, MessageDeserializer*>::iterator m_it;
};

#endif
