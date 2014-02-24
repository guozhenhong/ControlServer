#ifndef GetVDCapacityInfoMessage_H
#define GetVDCapacityInfoMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class GetVDCapacityInfoMessage : public Message
{
public:	
	GetVDCapacityInfoMessage() : Message(MSG_GET_VD_CAP_INFO)
	{

	}
	GetVDCapacityInfoMessage(uint32_t vdID) : 
		Message(MSG_GET_VD_CAP_INFO), m_iVDID(vdID)
	{

	}

	~GetVDCapacityInfoMessage()
	{

	}

public:
	uint32_t  m_iVDID; 
};


class GetVDCapacityInfoMsgSerializer : public MessageSerializer
{
public:
    GetVDCapacityInfoMsgSerializer()
    {}

    virtual ~GetVDCapacityInfoMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetVDCapacityInfoMessage *p = dynamic_cast<GetVDCapacityInfoMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
        	
    	uint32_t *pVDID = (uint32_t *)(pBuf);
    	*pVDID = p->m_iVDID;

    	return pBuf;
    }
};


class GetVDCapacityInfoMsgDeserializer : public MessageDeserializer
{
public:
	GetVDCapacityInfoMsgDeserializer()
	{}

	virtual ~GetVDCapacityInfoMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);

		GetVDCapacityInfoMessage *pMsg = new GetVDCapacityInfoMessage(id);

		return pMsg;
	}

};

class GetVDCapacityInfoACKMessage : public Message
{
public:
	GetVDCapacityInfoACKMessage() : Message(MSG_ACK_VD_CAP_INFO)
	{

	}
	GetVDCapacityInfoACKMessage(uint32_t id, uint64_t decCap, uint64_t usedCap, uint64_t realUsedCap):
		Message(MSG_ACK_VD_CAP_INFO), m_iVDID(id), m_decCapacity(decCap), m_usedCapacity(usedCap), m_realUsedCapacityByFileSys(realUsedCap)
	{
	}

	virtual ~GetVDCapacityInfoACKMessage()
	{

	}	

public:
	uint32_t m_iVDID; 
	uint64_t m_decCapacity;
	uint64_t m_usedCapacity;
	uint64_t m_realUsedCapacityByFileSys;

};

class GetVDCapacityInfoACKMsgSerializer : public MessageSerializer
{
public:
    GetVDCapacityInfoACKMsgSerializer()
    {}

    virtual ~GetVDCapacityInfoACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetVDCapacityInfoACKMessage *p = dynamic_cast<GetVDCapacityInfoACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int len = sizeof(uint32_t) + 3 * sizeof(uint64_t);
    	char *pBuffer = new char[len];

    	uint32_t *pVDID;
    	pVDID = (uint32_t*)(pBuffer);
    	*pVDID = p->m_iVDID;

    	uint64_t *pValue;
    	pValue = (uint64_t *)(pBuffer + sizeof(uint32_t));
    	*pValue = p->m_decCapacity;

    	pValue = (uint64_t *)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t));
    	*pValue = p->m_usedCapacity;
		
		pValue = (uint64_t *)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint64_t));
    	*pValue = p->m_realUsedCapacityByFileSys;

		*pFullLength = len;

    	return pBuffer;
    }
};

class GetVDCapacityInfoACKMsgDeserializer : public MessageDeserializer
{
public:
	GetVDCapacityInfoACKMsgDeserializer()
	{}

	virtual ~GetVDCapacityInfoACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint64_t dC = *((uint32_t*)(pBuffer + sizeof(uint32_t)));
		uint64_t uC= *((uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t)));
		uint64_t ruC= *((uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint64_t)));
		

		GetVDCapacityInfoACKMessage *pMsg = new GetVDCapacityInfoACKMessage(id, dC, uC, ruC);

		return pMsg;
	}
};

#endif
