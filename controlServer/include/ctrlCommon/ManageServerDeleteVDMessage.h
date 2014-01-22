#ifndef ManageServerDeleteVDMessage_H
#define ManageServerDeleteVDMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>


using namespace std;

class ManageServerDeleteVDMessage : public Message
{
public:	
	ManageServerDeleteVDMessage() : Message(MSG_MANAGE_SERVER_DELETE_VD)
	{

	}
	ManageServerDeleteVDMessage(uint32_t id) : 
		Message(MSG_MANAGE_SERVER_DELETE_VD), m_iVDID(id)
	{

	}

	~ManageServerDeleteVDMessage()
	{

	}

public:
	uint32_t  m_iVDID;
};


class ManageServerDeleteVDMsgSerializer : public MessageSerializer
{
public:
    ManageServerDeleteVDMsgSerializer()
    {}

    virtual ~ManageServerDeleteVDMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerDeleteVDMessage *p = dynamic_cast<ManageServerDeleteVDMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pID = (uint32_t *)pBuf;
    	*pID = p->m_iVDID;

    	return pBuf;
    }
};


class ManageServerDeleteVDMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerDeleteVDMsgDeserializer()
	{}

	virtual ~ManageServerDeleteVDMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);

		ManageServerDeleteVDMessage *pMsg = new ManageServerDeleteVDMessage(id);

		return pMsg;
	}

};

class ManageServerDeleteVDACKMessage : public Message
{
public:
	ManageServerDeleteVDACKMessage() : Message(MSG_ACK_MANAGE_SERVER_DELETE_VD)
	{

	}
	ManageServerDeleteVDACKMessage(uint32_t id, uint32_t res) : Message(MSG_ACK_MANAGE_SERVER_DELETE_VD), m_iVDID(id), m_bIsSuccess(res)
	{

	}

	virtual ~ManageServerDeleteVDACKMessage()
	{

	}	

public:
	uint32_t m_iVDID;
	uint32_t m_bIsSuccess; //success :0; faile:>=1
};

class ManageServerDeleteVDACKMsgSerializer : public MessageSerializer
{
public:
    ManageServerDeleteVDACKMsgSerializer()
    {}

    virtual ~ManageServerDeleteVDACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerDeleteVDACKMessage *p = dynamic_cast<ManageServerDeleteVDACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int len = 2 * sizeof(uint32_t);
    	char *pBuffer = new char[len];

    	uint32_t *pID = (uint32_t*)(pBuffer);
    	*pID = p->m_iVDID;

    	uint32_t *pValue;
    	pValue = (uint32_t*)(pBuffer + sizeof(uint32_t));
    	*pValue = p->m_bIsSuccess;

		*pFullLength = len;

    	return pBuffer;
    }
};

class ManageServerDeleteVDACKMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerDeleteVDACKMsgDeserializer()
	{}

	virtual ~ManageServerDeleteVDACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint32_t res = *((uint32_t*)(pBuffer + sizeof(uint32_t)));
		
		ManageServerDeleteVDACKMessage *pMsg = new ManageServerDeleteVDACKMessage(id, res);

		return pMsg;
	}
};

#endif
