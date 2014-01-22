#include "MultiMsgDeserializer.h"
#include "Message.h"
#include "MessageDeserializer.h"

MultiMsgDeserializer::MultiMsgDeserializer()
{

}

MultiMsgDeserializer::~MultiMsgDeserializer()
{
	m_it = m_MsgDeserializerTable.begin();
	while(m_it != m_MsgDeserializerTable.end())
	{	
		if(m_it->second)
			delete m_it->second;
		
		++m_it;
	}
}

int MultiMsgDeserializer::
Register(unsigned long msgID, MessageDeserializer* pDeserializer)
{
	m_it = m_MsgDeserializerTable.find(msgID);

	if(m_it != m_MsgDeserializerTable.end())
		return -1;

	m_MsgDeserializerTable[msgID] = pDeserializer;

	return 0;
}

Message* MultiMsgDeserializer::
Deserialize(unsigned long msgID, char *pBuffer)
{
	m_it = m_MsgDeserializerTable.find(msgID);

	if(m_it == m_MsgDeserializerTable.end())
		return NULL;

	return (m_it->second)->Deserialize(pBuffer);
}
