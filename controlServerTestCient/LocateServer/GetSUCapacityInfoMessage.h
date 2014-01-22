#ifndef GETSUCAPACITYINFOMESSAGE_H
#define GETSUCAPACITYINFOMESSAGE_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class GetSUCapacityInfoMessage : public Message
{
public:	
	GetSUCapacityInfoMessage() : Message(MSG_GET_SU_CAP_INFO)
	{

	}
	GetSUCapacityInfoMessage(uint32_t id, const char* name) : 
		Message(MSG_GET_SU_CAP_INFO), m_iEchoID(id), m_strNodeName(name)
	{

	}

	~GetSUCapacityInfoMessage()
	{

	}

public:
	uint32_t m_iEchoID;
	string m_strNodeName;
};


class GetSUCapacityInfoMsgSerializer : public MessageSerializer
{
public:
    GetSUCapacityInfoMsgSerializer()
    {}

    virtual ~GetSUCapacityInfoMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetSUCapacityInfoMessage *p = dynamic_cast<GetSUCapacityInfoMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int nameLen = (p->m_strNodeName).length();
    	int len = sizeof(uint32_t) + sizeof(uint32_t) + nameLen;
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pID = (uint32_t *)pBuf;
    	*pID = p->m_iEchoID;
    	
    	uint32_t *pLen = (uint32_t *)(pBuf + sizeof(uint32_t));
    	*pLen = nameLen;

    	memcpy(pBuf + sizeof(uint32_t) + sizeof(uint32_t), p->m_strNodeName.c_str(), nameLen);

    	return pBuf;
    }
};


class GetSUCapacityInfoMsgDeserializer : public MessageDeserializer
{
public:
	GetSUCapacityInfoMsgDeserializer()
	{}

	virtual ~GetSUCapacityInfoMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);
		uint32_t len = *((uint32_t *)(pBuffer + sizeof(uint32_t)));

		GetSUCapacityInfoMessage *pMsg = new GetSUCapacityInfoMessage();

		pMsg->m_iEchoID = id;
		pMsg->m_strNodeName = string((pBuffer + sizeof(uint32_t) + sizeof(uint32_t)), len);

		return pMsg;
	}

};

class GetSUCapacityInfoACKMessage : public Message
{
public:
	GetSUCapacityInfoACKMessage() : Message(MSG_ACK_SU_CAP_INFO)
	{

	}
	GetSUCapacityInfoACKMessage(uint32_t id, uint64_t mC, uint64_t dC, uint64_t uC) : Message(MSG_ACK_SU_CAP_INFO), m_iEchoID(id), m_MaxCapacity(mC), m_DiskDecCapacity(dC), m_DataUsedCapacity(uC)
	{

	}

	virtual ~GetSUCapacityInfoACKMessage()
	{

	}	

public:
	uint32_t  m_iEchoID;
	uint64_t  m_MaxCapacity;
	uint64_t  m_DiskDecCapacity;
	uint64_t  m_DataUsedCapacity;

};

class GetSUCapacityInfoACKMsgSerializer : public MessageSerializer
{
public:
    GetSUCapacityInfoACKMsgSerializer()
    {}

    virtual ~GetSUCapacityInfoACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetSUCapacityInfoACKMessage *p = dynamic_cast<GetSUCapacityInfoACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int len = sizeof(uint32_t) + 3*sizeof(uint64_t);
    	char *pBuffer = new char[len];

    	uint32_t *pID = (uint32_t*)(pBuffer);
    	*pID = p->m_iEchoID;

    	uint64_t *pValue;
    	pValue = (uint64_t*)(pBuffer + sizeof(uint32_t));
    	*pValue = p->m_MaxCapacity;

    	pValue = (uint64_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t));
    	*pValue = p->m_DiskDecCapacity;

    	pValue = (uint64_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint64_t));
    	*pValue = p->m_DataUsedCapacity;

        *pFullLength = len;

    	return pBuffer;
    }
};

class GetSUCapacityInfoACKMsgDeserializer : public MessageDeserializer
{
public:
	GetSUCapacityInfoACKMsgDeserializer()
	{}

	virtual ~GetSUCapacityInfoACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint64_t m = *((uint64_t*)(pBuffer + sizeof(uint32_t)));
		uint64_t d = *((uint64_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t)));
		uint64_t u = *((uint64_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint64_t)));

		GetSUCapacityInfoACKMessage *pMsg = new GetSUCapacityInfoACKMessage(id, m, d, u);

		return pMsg;
	}
};

#endif
