#ifndef ChangeVDSumFormatCapMessage_H
#define ChangeVDSumFormatCapMessage_H

/* io with the node server*/

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class ChangeVDSumFormatCapMessage : public Message
{
public:	
	ChangeVDSumFormatCapMessage() : Message(MSG_CHANGE_VD_FORMAT_CAP)
	{

	}
	ChangeVDSumFormatCapMessage(uint32_t id, uint32_t vdID, uint64_t newCap) : 
		Message(MSG_CHANGE_VD_FORMAT_CAP), m_iEchoID(id), m_iVDID(vdID), m_newCap(newCap)
	{

	}

	~ChangeVDSumFormatCapMessage()
	{

	}

public:
	uint32_t  m_iEchoID;
	uint32_t  m_iVDID;
	uint64_t  m_newCap; 
};


class ChangeVDSumFormatCapMsgSerializer : public MessageSerializer
{
public:
    ChangeVDSumFormatCapMsgSerializer()
    {}

    virtual ~ChangeVDSumFormatCapMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ChangeVDSumFormatCapMessage *p = dynamic_cast<ChangeVDSumFormatCapMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint64_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
        memset(pBuf, 0, len);

        uint32_t *pEchoID = (uint32_t *)(pBuf);
        *pEchoID = p->m_iEchoID;

    	uint32_t *pVDID = (uint32_t *)(pBuf + sizeof(uint32_t));
    	*pVDID = p->m_iVDID;

    	uint64_t *pCap = (uint64_t *)(pBuf + sizeof(uint32_t) + sizeof(uint32_t));
 		*pCap = p->m_newCap;

    	return pBuf;
    }
};


class ChangeVDSumFormatCapMsgDeserializer : public MessageDeserializer
{
public:
	ChangeVDSumFormatCapMsgDeserializer()
	{}

	virtual ~ChangeVDSumFormatCapMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);
		uint32_t vdID = *((uint32_t *)(pBuffer + sizeof(uint32_t)));
		uint64_t newCap = *((uint64_t *)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));

		ChangeVDSumFormatCapMessage *pMsg = new ChangeVDSumFormatCapMessage(id, vdID, newCap);

		return pMsg;
	}

};

class ChangeVDSumFormatCapACKMessage : public Message
{
public:
	ChangeVDSumFormatCapACKMessage() : Message(MSG_ACK_CH_VD_FORMAT_CAP)
	{

	}
	ChangeVDSumFormatCapACKMessage(uint32_t id, uint32_t res):
		Message(MSG_ACK_CH_VD_FORMAT_CAP), m_iEchoID(id), m_iRes(res)
	{
	}

	virtual ~ChangeVDSumFormatCapACKMessage()
	{

	}	

public:
	uint32_t m_iEchoID; 
	uint32_t m_iRes;
};

class ChangeVDSumFormatCapACKMsgSerializer : public MessageSerializer
{
public:
    ChangeVDSumFormatCapACKMsgSerializer()
    {}

    virtual ~ChangeVDSumFormatCapACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ChangeVDSumFormatCapACKMessage *p = dynamic_cast<ChangeVDSumFormatCapACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int len = 2  * sizeof(uint32_t);
    	char *pBuffer = new char[len];

    	uint32_t *pEchoID;
    	pEchoID = (uint32_t*)(pBuffer);
    	*pEchoID = p->m_iEchoID;

    	uint32_t *pRes;
    	pRes = (uint32_t *)(pBuffer + sizeof(uint32_t));
    	*pRes = p->m_iRes;

		*pFullLength = len;

    	return pBuffer;
    }
};

class ChangeVDSumFormatCapACKMsgDeserializer : public MessageDeserializer
{
public:
	ChangeVDSumFormatCapACKMsgDeserializer()
	{}

	virtual ~ChangeVDSumFormatCapACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint32_t res = *((uint32_t*)(pBuffer + sizeof(uint32_t)));
		
		ChangeVDSumFormatCapACKMessage *pMsg = new ChangeVDSumFormatCapACKMessage(id, res);

		return pMsg;
	}
};

#endif
