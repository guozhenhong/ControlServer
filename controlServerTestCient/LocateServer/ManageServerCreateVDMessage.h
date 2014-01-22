#ifndef ManageServerCreateVDMessage_H
#define ManageServerCreateVDMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

#define FAT_16 1
#define FAT_32 2
#define NTFS 3


using namespace std;

class ManageServerCreateVDMessage : public Message
{
public:	
	ManageServerCreateVDMessage() : Message(MSG_MANAGE_SERVER_CREATE_VD)
	{

	}
	ManageServerCreateVDMessage(uint32_t id, uint32_t type, uint64_t vdCap) : 
		Message(MSG_MANAGE_SERVER_CREATE_VD), m_iEchoID(id), m_FileSysType(type), m_VDCapacity(vdCap)
	{

	}

	~ManageServerCreateVDMessage()
	{

	}

public:
	uint32_t  m_iEchoID;
	uint32_t  m_FileSysType; //表示文件系统类型的值
	uint64_t  m_VDCapacity; //创建虚盘的大小
};


class ManageServerCreateVDMsgSerializer : public MessageSerializer
{
public:
    ManageServerCreateVDMsgSerializer()
    {}

    virtual ~ManageServerCreateVDMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerCreateVDMessage *p = dynamic_cast<ManageServerCreateVDMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint64_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pID = (uint32_t *)pBuf;
    	*pID = p->m_iEchoID;
    	
    	uint32_t *pType = (uint32_t *)(pBuf + sizeof(uint32_t));
    	*pType = p->m_FileSysType;

    	uint64_t *pVDCap = (uint64_t *)(pBuf + sizeof(uint32_t) + sizeof(uint32_t));
    	*pVDCap = p->m_VDCapacity;

    	return pBuf;
    }
};


class ManageServerCreateVDMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerCreateVDMsgDeserializer()
	{}

	virtual ~ManageServerCreateVDMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t *)pBuffer);
		uint32_t type = *((uint32_t *)(pBuffer + sizeof(uint32_t)));
		uint64_t cap = *((uint64_t *)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));

		ManageServerCreateVDMessage *pMsg = new ManageServerCreateVDMessage(id, type, cap);

		return pMsg;
	}

};

class ManageServerCreateVDACKMessage : public Message
{
public:
	ManageServerCreateVDACKMessage() : Message(MSG_ACK_MANAGE_SERVER_CREATE_VD)
	{

	}
	ManageServerCreateVDACKMessage(uint32_t id, uint32_t res, uint32_t diskID) : Message(MSG_ACK_MANAGE_SERVER_CREATE_VD), m_iEchoID(id), m_bIsSuccess(res), m_iDiskID(diskID)
	{

	}

	virtual ~ManageServerCreateVDACKMessage()
	{

	}	

public:
	uint32_t m_iEchoID;
	uint32_t m_bIsSuccess;
	uint32_t m_iDiskID;

};

class ManageServerCreateVDACKMsgSerializer : public MessageSerializer
{
public:
    ManageServerCreateVDACKMsgSerializer()
    {}

    virtual ~ManageServerCreateVDACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerCreateVDACKMessage *p = dynamic_cast<ManageServerCreateVDACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}

    	int len = 3 * sizeof(uint32_t);
    	char *pBuffer = new char[len];

    	uint32_t *pID = (uint32_t*)(pBuffer);
    	*pID = p->m_iEchoID;

    	uint32_t *pValue;
    	pValue = (uint32_t*)(pBuffer + sizeof(uint32_t));
    	*pValue = p->m_bIsSuccess;

    	pValue = (uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t));
    	*pValue = p->m_iDiskID;

		*pFullLength = len;

    	return pBuffer;
    }
};

class ManageServerCreateVDACKMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerCreateVDACKMsgDeserializer()
	{}

	virtual ~ManageServerCreateVDACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t id = *((uint32_t*)pBuffer);

		uint32_t res = *((uint32_t*)(pBuffer + sizeof(uint32_t)));
		uint32_t dID = *((uint32_t*)(pBuffer + sizeof(uint32_t) + sizeof(uint32_t)));
		
		ManageServerCreateVDACKMessage *pMsg = new ManageServerCreateVDACKMessage(id, res, dID);

		return pMsg;
	}
};

#endif
