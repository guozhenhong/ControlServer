#ifndef GETSYSCAPMESSAGE_H
#define GETSYSCAPMESSAGE_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class GetSysCapMessage : public Message
{
public:	
	GetSysCapMessage() : Message(MSG_GET_SYS_CAP_INFO)
	{

	}

	~GetSysCapMessage()
	{

	}

public:
	
};


class GetSysCapMsgSerializer : public MessageSerializer
{
public:
    GetSysCapMsgSerializer()
    {}

    virtual ~GetSysCapMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	*pFullLength = 0;
    	return NULL;
    }
};


class GetSysCapMsgDeserializer : public MessageDeserializer
{
public:
	GetSysCapMsgDeserializer()
	{}

	virtual ~GetSysCapMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		return (new GetSysCapMessage());
	}

};

class GetSysCapACKMessage : public Message
{
public:
	GetSysCapACKMessage() : Message(MSG_ACK_SYS_CAP_INFO)
	{

	}

	virtual ~GetSysCapACKMessage()
	{

	}	

public:
	uint64_t m_SumCap;
	uint64_t m_DecCap;
	uint64_t m_UsedCap;
};

class GetSysCapACKMsgSerializer : public MessageSerializer
{
public:
    GetSysCapACKMsgSerializer()
    {}

    virtual ~GetSysCapACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetSysCapACKMessage *p = dynamic_cast<GetSysCapACKMessage *>(pMsg);
    	if(!p)
    		return NULL;

    	int len = 3 * sizeof(uint64_t);
    	*pFullLength = len;

    	char *pBuffer = new char[len];
    	uint64_t *pSum = (uint64_t*)pBuffer;
    	*pSum = p->m_SumCap;
		uint64_t *pDec = (uint64_t*)(pBuffer + sizeof(uint64_t));
    	*pDec = p->m_DecCap;
		uint64_t *pUse = (uint64_t*)(pBuffer + sizeof(uint64_t) + sizeof(uint64_t));
    	*pUse = p->m_UsedCap;

    	return pBuffer;
    }
};

class GetSysCapACKMsgDeserializer : public MessageDeserializer
{
public:
	GetSysCapACKMsgDeserializer()
	{}

	virtual ~GetSysCapACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		GetSysCapACKMessage *pMsg = new GetSysCapACKMessage();

		pMsg->m_SumCap = *((uint64_t*)pBuffer);
    	
		pMsg->m_DecCap = *((uint64_t*)(pBuffer + sizeof(uint64_t)));
    	
		pMsg->m_UsedCap = *((uint64_t*)(pBuffer + sizeof(uint64_t) + sizeof(uint64_t)));

		return pMsg;
	}
};

#endif
