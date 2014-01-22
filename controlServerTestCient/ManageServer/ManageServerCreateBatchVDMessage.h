#ifndef ManageServerCreateBatchVDMessage_H
#define ManageServerCreateBatchVDMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include "ManageServerCreateVDMessage.h"
#include <stdint.h>

#define FAT_16 1
#define FAT_32 2
#define NTFS 3


using namespace std;

class ManageServerCreateBatchVDMessage : public Message
{
public:	
	ManageServerCreateBatchVDMessage() : Message(MSG_MANAGER_SERVER_BATCH_CREATE_VD)
	{

	}
	ManageServerCreateBatchVDMessage(uint32_t num, ManageServerCreateVDMessage *pInfo) : //out new the info obj
		Message(MSG_MANAGER_SERVER_BATCH_CREATE_VD), m_iDiskNum(num), m_diskInfoList(pInfo)
	{

	}

	~ManageServerCreateBatchVDMessage()
	{
		if(m_diskInfoList)
		{
			if(m_iDiskNum > 1)
				delete [] m_diskInfoList;
			else
				delete m_diskInfoList;
		}
			
	}

public:
	uint32_t  m_iDiskNum;
	ManageServerCreateVDMessage *m_diskInfoList; 
	// uint32_t  m_iEchoID;
	// uint32_t  m_FileSysType; //表示文件系统类型的值
	// uint64_t  m_VDCapacity; //创建虚盘的大小
};


class ManageServerCreateBatchVDMsgSerializer : public MessageSerializer
{
public:
    ManageServerCreateBatchVDMsgSerializer()
    {}

    virtual ~ManageServerCreateBatchVDMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerCreateBatchVDMessage *p = dynamic_cast<ManageServerCreateBatchVDMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	ManageServerCreateVDMsgSerializer OneMsgSerializer;
    	int oneMsgBufLen = 0;
    	char *pOneBuf;
    	pOneBuf = OneMsgSerializer.Serialize(&((p->m_diskInfoList)[0]), &oneMsgBufLen);

    	if(oneMsgBufLen == 0)
    		return NULL;

    	int len = sizeof(uint32_t) + (p->m_iDiskNum) * oneMsgBufLen;
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pNum = (uint32_t *)pBuf;
    	*pNum = p->m_iDiskNum;
    	
    	memcpy(pBuf + sizeof(uint32_t), pOneBuf, oneMsgBufLen);
    	delete [] pOneBuf;

    	for (uint32_t i = 1; i < (p->m_iDiskNum); ++i)
    	{
    		pOneBuf = OneMsgSerializer.Serialize(&((p->m_diskInfoList)[i]), &oneMsgBufLen);
    		if(oneMsgBufLen == 0)
    			return NULL;

    		memcpy(pBuf + sizeof(uint32_t) + (i * oneMsgBufLen), pOneBuf, oneMsgBufLen);
    		delete [] pOneBuf;
    	}

    	return pBuf;
    }
};


class ManageServerCreateBatchVDMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerCreateBatchVDMsgDeserializer()
	{}

	virtual ~ManageServerCreateBatchVDMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t num = *((uint32_t *)pBuffer);

		ManageServerCreateVDMessage *pInfoList = new ManageServerCreateVDMessage[num];

		ManageServerCreateVDMsgDeserializer OneMsgDeserializer;

		int oneMsgBufLen = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint64_t);

		for (uint32_t i = 0; i < num; ++i)
		{
			Message *pM =  OneMsgDeserializer.Deserialize(pBuffer + sizeof(uint32_t) + (i * oneMsgBufLen));
			ManageServerCreateVDMessage *p = dynamic_cast<ManageServerCreateVDMessage *>(pM);
			if(!p)
				return NULL;

			pInfoList[i].m_iEchoID = p->m_iEchoID;
			pInfoList[i].m_FileSysType = p->m_FileSysType;
			pInfoList[i].m_VDCapacity = p->m_VDCapacity;

			delete p;
			p = NULL;
		}

		ManageServerCreateBatchVDMessage *pMsg = new ManageServerCreateBatchVDMessage(num, pInfoList);

		return pMsg;
	}

};

class ManageServerCreateBatchVDACKMessage : public Message
{
public:
	ManageServerCreateBatchVDACKMessage() : Message(MSG_ACK_MANAGER_SERVER_BATCH_CREATE_VD)
	{

	}
	ManageServerCreateBatchVDACKMessage(uint32_t num, ManageServerCreateVDACKMessage *pRes) : Message(MSG_ACK_MANAGER_SERVER_BATCH_CREATE_VD), m_iDiskNum(num), m_pResList(pRes)
	{
		
	}

	virtual ~ManageServerCreateBatchVDACKMessage()
	{
		if(m_pResList)
		{
			if(m_iDiskNum > 1)
				delete [] m_pResList;
			else
				delete m_pResList;
		}
			
	}	

public:
	uint32_t  m_iDiskNum;
	ManageServerCreateVDACKMessage *m_pResList;
};

class ManageServerCreateBatchVDACKMsgSerializer : public MessageSerializer
{
public:
    ManageServerCreateBatchVDACKMsgSerializer()
    {}

    virtual ~ManageServerCreateBatchVDACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerCreateBatchVDACKMessage *p = dynamic_cast<ManageServerCreateBatchVDACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	ManageServerCreateVDACKMsgSerializer OneMsgSerializer;
    	int oneMsgBufLen = 0;
    	char *pOneBuf = OneMsgSerializer.Serialize(&((p->m_pResList)[0]), &oneMsgBufLen);

    	if(oneMsgBufLen == 0)
    		return NULL;

    	int len = sizeof(uint32_t) + (p->m_iDiskNum) * oneMsgBufLen;
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pNum = (uint32_t *)pBuf;
    	*pNum = p->m_iDiskNum;
    	
    	memcpy(pBuf + sizeof(uint32_t), pOneBuf, oneMsgBufLen);
    	delete [] pOneBuf;

    	for (uint32_t i = 1; i < (p->m_iDiskNum); ++i)
    	{
    		pOneBuf = OneMsgSerializer.Serialize(&((p->m_pResList)[i]), &oneMsgBufLen);
    		if(oneMsgBufLen == 0)
    			return NULL;

    		memcpy(pBuf + sizeof(uint32_t) + (i * oneMsgBufLen), pOneBuf, oneMsgBufLen);
    		delete [] pOneBuf;
    	}

    	return pBuf;
    }
};

class ManageServerCreateBatchVDACKMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerCreateBatchVDACKMsgDeserializer()
	{}

	virtual ~ManageServerCreateBatchVDACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t num = *((uint32_t *)pBuffer);

		ManageServerCreateVDACKMessage *pResList = new ManageServerCreateVDACKMessage[num];

		ManageServerCreateVDACKMsgDeserializer OneMsgDeserializer;

		int oneMsgBufLen = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

		for (uint32_t i = 0; i < num; ++i)
		{
			Message *pM =  OneMsgDeserializer.Deserialize(pBuffer + sizeof(uint32_t) + (i * oneMsgBufLen));
			ManageServerCreateVDACKMessage *p = dynamic_cast<ManageServerCreateVDACKMessage*>(pM);

			if(!p)
				return NULL;

			pResList[i].m_iEchoID = p->m_iEchoID;
			pResList[i].m_bIsSuccess = p->m_bIsSuccess;
			pResList[i].m_iDiskID = p->m_iDiskID;

			delete p;
			p = NULL;
		}

		ManageServerCreateBatchVDACKMessage *pMsg = new ManageServerCreateBatchVDACKMessage(num, pResList);

		return pMsg;
	}
};

#endif
