#ifndef ChangeVDSumCapMessage_H
#define ChangeVDSumCapMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class ChangeVDSumCapMessage : public Message
{
public:	
	ChangeVDSumCapMessage() : Message(MSG_CHANGE_VD_CAP)
	{

	}
	ChangeVDSumCapMessage(uint32_t vdID, uint64_t newCap) : 
		Message(MSG_CHANGE_VD_CAP), m_iVDID(vdID), m_newCap(newCap)
	{

	}

	~ChangeVDSumCapMessage()
	{

	}

public:
	uint32_t  m_iVDID;
	uint64_t  m_newCap; 
};


class ChangeVDSumCapMsgSerializer : public MessageSerializer
{
public:
    ChangeVDSumCapMsgSerializer()
    {}

    virtual ~ChangeVDSumCapMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ChangeVDSumCapMessage *p = dynamic_cast<ChangeVDSumCapMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint64_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
        memset(pBuf, 0, len);

    	uint32_t *pVDID = (uint32_t *)(pBuf);
    	*pVDID = p->m_iVDID;

    	uint64_t *pCap = (uint64_t *)(pBuf + sizeof(uint32_t) + sizeof(uint32_t));
 		*pCap = p->m_newCap;

    	return pBuf;
    }
};


class ChangeVDSumCapMsgDeserializer : public MessageDeserializer
{
public:
	ChangeVDSumCapMsgDeserializer()
	{}

	virtual ~ChangeVDSumCapMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);
		uint64_t newCap = *((uint64_t *)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));

		ChangeVDSumCapMessage *pMsg = new ChangeVDSumCapMessage(id, newCap);

		return pMsg;
	}

};

class ChangeVDSumCapACKMessage : public Message
{
public:
	ChangeVDSumCapACKMessage() : Message(MSG_ACK_CH_VD_CAP)
	{

	}
	ChangeVDSumCapACKMessage(uint32_t id, uint32_t res):
		Message(MSG_ACK_CH_VD_CAP), m_iVDID(id), m_opRes(res)
	{
	}

	virtual ~ChangeVDSumCapACKMessage()
	{

	}	

public:
	uint32_t m_iVDID; 
	uint32_t m_opRes;
};

class ChangeVDSumCapACKMsgSerializer : public MessageSerializer
{
public:
    ChangeVDSumCapACKMsgSerializer()
    {}

    virtual ~ChangeVDSumCapACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ChangeVDSumCapACKMessage *p = dynamic_cast<ChangeVDSumCapACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int len = 2  * sizeof(uint32_t);
    	char *pBuffer = new char[len];

    	uint32_t *pVDID;
    	pVDID = (uint32_t*)(pBuffer);
    	*pVDID = p->m_iVDID;

    	uint32_t *pRes;
    	pRes = (uint32_t *)(pBuffer + sizeof(uint32_t));
    	*pRes = p->m_opRes;

		*pFullLength = len;

    	return pBuffer;
    }
};

class ChangeVDSumCapACKMsgDeserializer : public MessageDeserializer
{
public:
	ChangeVDSumCapACKMsgDeserializer()
	{}

	virtual ~ChangeVDSumCapACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint32_t res = *((uint32_t*)(pBuffer + sizeof(uint32_t)));
		
		ChangeVDSumCapACKMessage *pMsg = new ChangeVDSumCapACKMessage(id, res);

		return pMsg;
	}
};

#endif
