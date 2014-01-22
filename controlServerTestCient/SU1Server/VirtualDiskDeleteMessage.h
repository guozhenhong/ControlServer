#ifndef VirtualDiskDeleteMessage_H
#define VirtualDiskDeleteMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class VirtualDiskDeleteMessage : public Message
{
public:	
	VirtualDiskDeleteMessage() : Message(MSG_NODE_DEL_VD)
	{

	}
	VirtualDiskDeleteMessage(uint32_t id, uint32_t vdID) : 
		Message(MSG_NODE_DEL_VD), m_iEchoID(id), m_iVDID(vdID)
	{

	}

	~VirtualDiskDeleteMessage()
	{

	}

public:
	uint32_t  m_iEchoID;//the tast ID
	uint32_t  m_iVDID; 
};


class VirtualDiskDeleteMsgSerializer : public MessageSerializer
{
public:
    VirtualDiskDeleteMsgSerializer()
    {}

    virtual ~VirtualDiskDeleteMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	VirtualDiskDeleteMessage *p = dynamic_cast<VirtualDiskDeleteMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t) + sizeof(uint32_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pID = (uint32_t *)pBuf;
    	*pID = p->m_iEchoID;
    	
    	uint32_t *pVDID = (uint32_t *)(pBuf + sizeof(uint32_t));
    	*pVDID = p->m_iVDID;

    	return pBuf;
    }
};


class VirtualDiskDeleteMsgDeserializer : public MessageDeserializer
{
public:
	VirtualDiskDeleteMsgDeserializer()
	{}

	virtual ~VirtualDiskDeleteMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);
		uint32_t vd = *((uint32_t *)(pBuffer + sizeof(uint32_t)));
	
		VirtualDiskDeleteMessage *pMsg = new VirtualDiskDeleteMessage(id, vd);

		return pMsg;
	}

};

class VirtualDiskDeleteACKMessage : public Message
{
public:
	VirtualDiskDeleteACKMessage() : Message(MSG_ACK_DEL_VD)
	{

	}
	VirtualDiskDeleteACKMessage(uint32_t id, uint32_t vd, uint32_t res):
		Message(MSG_ACK_DEL_VD), m_iEchoID(id), m_iVDID(vd), m_iRes(res)
	{
	}

	virtual ~VirtualDiskDeleteACKMessage()
	{

	}	

public:
	uint32_t  m_iEchoID;//the tast ID
	uint32_t  m_iVDID; 
	uint32_t  m_iRes;//0: failed, 1:success
};

class VirtualDiskDeleteACKMsgSerializer : public MessageSerializer
{
public:
    VirtualDiskDeleteACKMsgSerializer()
    {}

    virtual ~VirtualDiskDeleteACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	VirtualDiskDeleteACKMessage *p = dynamic_cast<VirtualDiskDeleteACKMessage *> (pMsg);
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

class VirtualDiskDeleteACKMsgDeserializer : public MessageDeserializer
{
public:
	VirtualDiskDeleteACKMsgDeserializer()
	{}

	virtual ~VirtualDiskDeleteACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint32_t vd = *((uint32_t*)(pBuffer + sizeof(uint32_t)));

		uint32_t res = *((uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));
		
		VirtualDiskDeleteACKMessage *pMsg = new VirtualDiskDeleteACKMessage(id, vd, res);

		return pMsg;
	}
};

#endif
