#include "MultiMsgSerializer.h"
#include "Message.h"
#include "MessageSerializer.h"

MultiMsgSerializer::MultiMsgSerializer()
{

}

MultiMsgSerializer::~MultiMsgSerializer()
{
	m_it = m_MsgSerializerTable.begin();
	while(m_it != m_MsgSerializerTable.end())
	{	
		if(m_it->second)
			delete m_it->second;

		++m_it;
	}
}

int MultiMsgSerializer::
Register(unsigned long msgID, MessageSerializer *pSerializer)
{
	m_it = m_MsgSerializerTable.find(msgID);

	if(m_it != m_MsgSerializerTable.end())
		return -1;

	m_MsgSerializerTable[msgID] = pSerializer;

	return 0;
}

char* MultiMsgSerializer::
Serialize(unsigned long ID, Message *pMsg, int *pBufLen)
{
	m_it = m_MsgSerializerTable.find(ID);

	if(m_it == m_MsgSerializerTable.end())
		return NULL;

	return (m_it->second)->Serialize(pMsg, pBufLen);
}
