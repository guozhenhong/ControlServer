#ifndef ManageServerDeleteBatchVDMessage_H
#define ManageServerDeleteBatchVDMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include "ManageServerDeleteVDMessage.h"
#include <stdint.h>


using namespace std;

class ManageServerDeleteBatchVDMessage : public Message
{
public:	
	ManageServerDeleteBatchVDMessage() : Message(MSG_MANAGER_SERVER_BATCH_DELETE_VD)
	{

	}
	ManageServerDeleteBatchVDMessage(uint32_t num, uint32_t *pVDList) : //out new the info obj
		Message(MSG_MANAGER_SERVER_BATCH_DELETE_VD), m_iDiskNum(num), m_pDiskIDList(pVDList)
	{

	}

	~ManageServerDeleteBatchVDMessage()
	{
		if(m_pDiskIDList)
		{
			if(m_iDiskNum > 1)
				delete [] m_pDiskIDList;
			else
				delete m_pDiskIDList;
		}
			
	}

public:
	uint32_t  m_iDiskNum;
	uint32_t *m_pDiskIDList; 
	// uint32_t  m_iEchoID;
	// uint32_t  m_FileSysType; //表示文件系统类型的值
	// uint64_t  m_VDCapacity; //创建虚盘的大小
};


class ManageServerDeleteBatchVDMsgSerializer : public MessageSerializer
{
public:
    ManageServerDeleteBatchVDMsgSerializer()
    {}

    virtual ~ManageServerDeleteBatchVDMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerDeleteBatchVDMessage *p = dynamic_cast<ManageServerDeleteBatchVDMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = sizeof(uint32_t) + (p->m_iDiskNum) * sizeof(uint32_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint32_t *pNum = (uint32_t *)pBuf;
    	*pNum = p->m_iDiskNum;
    	
    	for (uint32_t i = 0; i < (p->m_iDiskNum); ++i)
    	{
    		pNum = (uint32_t*)(pBuf + sizeof(uint32_t) + i * sizeof(uint32_t));
    		*pNum = (p->m_pDiskIDList)[i];
    	}

    	return pBuf;
    }
};


class ManageServerDeleteBatchVDMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerDeleteBatchVDMsgDeserializer()
	{}

	virtual ~ManageServerDeleteBatchVDMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t num = *((uint32_t *)pBuffer);

		uint32_t *pIDList = new uint32_t[num];

		for (uint32_t i = 0; i < num; ++i)
		{
			pIDList[i] = *((uint32_t *)(pBuffer + sizeof(uint32_t) + i * sizeof(uint32_t)));
		}

		ManageServerDeleteBatchVDMessage *pMsg = new ManageServerDeleteBatchVDMessage(num, pIDList);

		return pMsg;
	}

};

class ManageServerDeleteBatchVDACKMessage : public Message
{
public:
	ManageServerDeleteBatchVDACKMessage() : Message(MSG_ACK_MANAGER_SERVER_BATCH_DELETE_VD)
	{

	}
	ManageServerDeleteBatchVDACKMessage(uint32_t num, ManageServerDeleteVDACKMessage *pRes) : Message(MSG_ACK_MANAGER_SERVER_BATCH_DELETE_VD), m_iDiskNum(num), m_pResList(pRes)
	{
		
	}

	virtual ~ManageServerDeleteBatchVDACKMessage()
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
	ManageServerDeleteVDACKMessage *m_pResList;
};

class ManageServerDeleteBatchVDACKMsgSerializer : public MessageSerializer
{
public:
    ManageServerDeleteBatchVDACKMsgSerializer()
    {}

    virtual ~ManageServerDeleteBatchVDACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	ManageServerDeleteBatchVDACKMessage *p = dynamic_cast<ManageServerDeleteBatchVDACKMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	ManageServerDeleteVDACKMsgSerializer OneMsgSerializer;
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

class ManageServerDeleteBatchVDACKMsgDeserializer : public MessageDeserializer
{
public:
	ManageServerDeleteBatchVDACKMsgDeserializer()
	{}

	virtual ~ManageServerDeleteBatchVDACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint32_t num = *((uint32_t *)pBuffer);

		ManageServerDeleteVDACKMessage *pResList = new ManageServerDeleteVDACKMessage[num];

		ManageServerDeleteVDACKMsgDeserializer OneMsgDeserializer;

		int oneMsgBufLen = sizeof(uint32_t) + sizeof(uint32_t);

		for (uint32_t i = 0; i < num; ++i)
		{
			Message *pM =  OneMsgDeserializer.Deserialize(pBuffer + sizeof(uint32_t) + (i * oneMsgBufLen));
			ManageServerDeleteVDACKMessage *p = dynamic_cast<ManageServerDeleteVDACKMessage*>(pM);

			if(!p)
				return NULL;

			pResList[i].m_iVDID = p->m_iVDID;
			pResList[i].m_bIsSuccess = p->m_bIsSuccess;

			delete p;
			p = NULL;
		}

		ManageServerDeleteBatchVDACKMessage *pMsg = new ManageServerDeleteBatchVDACKMessage(num, pResList);

		return pMsg;
	}
};

#endif
