#ifndef VirtualDiskFormatMessage_H
#define VirtualDiskFormatMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class VirtualDiskFormatMessage : public Message
{
public:	
	VirtualDiskFormatMessage() : Message(MSG_NODE_FORMAT_VD)
	{

	}
	VirtualDiskFormatMessage(uint32_t id, uint32_t vdID, uint64_t cap, uint32_t type) : 
		Message(MSG_NODE_FORMAT_VD), m_iEchoID(id), m_iVDID(vdID), m_cap(cap), m_iFSType(type)
	{

	}

	~VirtualDiskFormatMessage()
	{

	}

public:
	uint32_t  m_iEchoID;//the tast ID
	uint32_t  m_iVDID; 
	uint64_t  m_cap;
	uint32_t  m_iFSType;
};


class VirtualDiskFormatMsgSerializer : public MessageSerializer
{
public:
    VirtualDiskFormatMsgSerializer()
    {}

    virtual ~VirtualDiskFormatMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	VirtualDiskFormatMessage *p = dynamic_cast<VirtualDiskFormatMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint64_t) + sizeof(uint32_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pID = (uint32_t *)pBuf;
    	*pID = p->m_iEchoID;
    	
    	uint32_t *pVDID = (uint32_t *)(pBuf + sizeof(uint32_t));
    	*pVDID = p->m_iVDID;

    	uint64_t *pCap = (uint64_t *)(pBuf + sizeof(uint32_t) + sizeof(uint32_t));
    	*pCap = p->m_cap;

    	uint32_t *pType = (uint32_t *)(pBuf + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint64_t));
    	*pType = p->m_iFSType;

    	return pBuf;
    }
};


class VirtualDiskFormatMsgDeserializer : public MessageDeserializer
{
public:
	VirtualDiskFormatMsgDeserializer()
	{}

	virtual ~VirtualDiskFormatMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);
		uint32_t vd = *((uint32_t *)(pBuffer + sizeof(uint32_t)));
		uint64_t cap = *((uint64_t *)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));
		uint32_t type = *((uint32_t *)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint64_t)));

		VirtualDiskFormatMessage *pMsg = new VirtualDiskFormatMessage(id, vd, cap, type);

		return pMsg;
	}

};

class VirtualDiskFormatACKMessage : public Message
{
public:
	VirtualDiskFormatACKMessage() : Message(MSG_ACK_FORMAT_VD)
	{

	}
	VirtualDiskFormatACKMessage(uint32_t id, uint32_t vd, uint32_t res):
		Message(MSG_ACK_FORMAT_VD), m_iEchoID(id), m_iVDID(vd), m_iRes(res)
	{
	}

	virtual ~VirtualDiskFormatACKMessage()
	{

	}	

public:
	uint32_t  m_iEchoID;//the tast ID
	uint32_t  m_iVDID; 
	uint32_t  m_iRes;//0: failed, 1:success
};

class VirtualDiskFormatACKMsgSerializer : public MessageSerializer
{
public:
    VirtualDiskFormatACKMsgSerializer()
    {}

    virtual ~VirtualDiskFormatACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	VirtualDiskFormatACKMessage *p = dynamic_cast<VirtualDiskFormatACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int len = 3 * sizeof(uint32_t);
    	char *pBuffer = new char[len];

    	uint32_t *pID = (uint32_t*)(pBuffer);
    	*pID = p->m_iEchoID;

    	uint32_t *pVDID;
    	pVDID = (uint32_t*)(pBuffer + sizeof(uint32_t));
    	*pVDID = p->m_iVDID;

    	uint32_t *pRes = (uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t));
    	*pRes= p->m_iRes;

		*pFullLength = len;

    	return pBuffer;
    }
};

class VirtualDiskFormatACKMsgDeserializer : public MessageDeserializer
{
public:
	VirtualDiskFormatACKMsgDeserializer()
	{}

	virtual ~VirtualDiskFormatACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint32_t vd = *((uint32_t*)(pBuffer + sizeof(uint32_t)));

		uint32_t res = *((uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));
		
		VirtualDiskFormatACKMessage *pMsg = new VirtualDiskFormatACKMessage(id, vd, res);

		return pMsg;
	}
};

#endif
