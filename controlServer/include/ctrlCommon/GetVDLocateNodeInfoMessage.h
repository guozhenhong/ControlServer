#ifndef GetVDLocateNodeInfoMessage_H
#define GetVDLocateNodeInfoMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class GetVDLocateNodeInfoMessage : public Message
{
public:	
	GetVDLocateNodeInfoMessage() : Message(MSG_GET_VD_LOCATE_NODE)
	{

	}
	GetVDLocateNodeInfoMessage(uint32_t id, uint32_t vdID) : 
		Message(MSG_GET_VD_LOCATE_NODE), m_iEchoID(id), m_iVDID(vdID)
	{

	}

	~GetVDLocateNodeInfoMessage()
	{

	}

public:
	uint32_t  m_iEchoID;//the tast ID
	uint32_t  m_iVDID; 
};


class GetVDLocateNodeInfoMsgSerializer : public MessageSerializer
{
public:
    GetVDLocateNodeInfoMsgSerializer()
    {}

    virtual ~GetVDLocateNodeInfoMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetVDLocateNodeInfoMessage *p = dynamic_cast<GetVDLocateNodeInfoMessage *> (pMsg);
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


class GetVDLocateNodeInfoMsgDeserializer : public MessageDeserializer
{
public:
	GetVDLocateNodeInfoMsgDeserializer()
	{}

	virtual ~GetVDLocateNodeInfoMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);
		uint32_t vd = *((uint32_t *)(pBuffer + sizeof(uint32_t)));

		GetVDLocateNodeInfoMessage *pMsg = new GetVDLocateNodeInfoMessage(id, vd);

		return pMsg;
	}

};

class GetVDLocateNodeInfoACKMessage : public Message
{
public:
	GetVDLocateNodeInfoACKMessage() : Message(MSG_ACK_VD_LOCATE_NODE)
	{

	}
	GetVDLocateNodeInfoACKMessage(uint32_t id, uint32_t vd, string nodeName):
		Message(MSG_ACK_VD_LOCATE_NODE), m_iEchoID(id), m_iVDID(vd), m_strLocateNodeName(nodeName)
	{
	}

	virtual ~GetVDLocateNodeInfoACKMessage()
	{

	}	

public:
	uint32_t  m_iEchoID;//the tast ID
	uint32_t  m_iVDID; 
	string    m_strLocateNodeName;
};

class GetVDLocateNodeInfoACKMsgSerializer : public MessageSerializer
{
public:
    GetVDLocateNodeInfoACKMsgSerializer()
    {}

    virtual ~GetVDLocateNodeInfoACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetVDLocateNodeInfoACKMessage *p = dynamic_cast<GetVDLocateNodeInfoACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int nameLen = p->m_strLocateNodeName.length();

    	int len = 3 * sizeof(uint32_t) + nameLen;
    	char *pBuffer = new char[len];

    	uint32_t *pID = (uint32_t*)(pBuffer);
    	*pID = p->m_iEchoID;

    	uint32_t *pVDID;
    	pVDID = (uint32_t*)(pBuffer + sizeof(uint32_t));
    	*pVDID = p->m_iVDID;

    	uint32_t *pNameLen = (uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t));
    	*pNameLen = nameLen;

    	memcpy((pBuffer + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t)), p->m_strLocateNodeName.c_str(), nameLen);

		*pFullLength = len;

    	return pBuffer;
    }
};

class GetVDLocateNodeInfoACKMsgDeserializer : public MessageDeserializer
{
public:
	GetVDLocateNodeInfoACKMsgDeserializer()
	{}

	virtual ~GetVDLocateNodeInfoACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint32_t vd = *((uint32_t*)(pBuffer + sizeof(uint32_t)));
		uint32_t nameLen = *((uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));
		
		string name((pBuffer + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t)), nameLen);

		GetVDLocateNodeInfoACKMessage *pMsg = new GetVDLocateNodeInfoACKMessage(id, vd, name);

		return pMsg;
	}
};

#endif
