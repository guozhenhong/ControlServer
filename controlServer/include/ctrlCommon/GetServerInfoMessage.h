#ifndef GETSERVERINFOMESSAGE_H
#define GETSERVERINFOMESSAGE_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"

using namespace std;

class GetServerInfoMessage : public Message
{
public:	
	GetServerInfoMessage() : Message(MSG_GET_SERVER_INFO),  
		m_strServerName(""), m_iEchoID(0)
	{

	}

	GetServerInfoMessage(const char* name, int echoID = 1):
		Message(MSG_GET_SERVER_INFO), m_strServerName(name), m_iEchoID(echoID)
	{

	}

	~GetServerInfoMessage()
	{

	}

public:
	string m_strServerName;	
	int m_iEchoID;	
};

class GetServerInfoMsgSerializer : public MessageSerializer
{
public:
	GetServerInfoMsgSerializer()
    {}

    virtual ~GetServerInfoMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetServerInfoMessage *p = dynamic_cast<GetServerInfoMessage* >(pMsg);
    	if(!p)
    		return NULL;

    	int len = sizeof(int) + sizeof(int) + (p->m_strServerName).length();
    	*pFullLength = len;
    	char *pBuffer = new char[len];

    	int *pID = (int *)pBuffer;
    	*pID = p->m_iEchoID;

    	int *pStrLen = (int *)(pBuffer + sizeof(int));
    	*pStrLen = (p->m_strServerName).length();

    	memcpy(pBuffer + sizeof(int) + sizeof(int), (p->m_strServerName).c_str(), *pStrLen);

    	return pBuffer;
    }
};

class GetServerInfoMsgDeserializer : public MessageDeserializer
{
public:
	GetServerInfoMsgDeserializer()
	{}

	virtual ~GetServerInfoMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		if(!pBuffer)
			return NULL;

		GetServerInfoMessage *p = new GetServerInfoMessage();

		int *pID = &(p->m_iEchoID);
		*pID = *((int*)pBuffer);

		int pStrLen = *((int*)(pBuffer + sizeof(int)));
		p->m_strServerName = string(pBuffer + sizeof(int) + sizeof(int), pStrLen);

		return p;
	}
};

class GetServerInfoACKMsg : public Message
{
public:
	GetServerInfoACKMsg() : Message(MSG_ACK_GET_SERVER_INFO)
	{

	}

	~GetServerInfoACKMsg()
	{

	}

public:
	int m_iEchoID;
	int m_iListenPort;
	string m_strServerIP;
};

class GetServerInfoACKMsgSerializer : public MessageSerializer
{
public:
	GetServerInfoACKMsgSerializer()
    {}

    virtual ~GetServerInfoACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	GetServerInfoACKMsg *p = dynamic_cast<GetServerInfoACKMsg *> (pMsg);
    	if(!p)
    		return NULL;

    	int ipLen = (p->m_strServerIP).length();
    	int len = sizeof(int) + sizeof(int) + sizeof(int) + ipLen;
    	
    	char *pBuffer = new char[len];
    	int *pID = (int*)pBuffer;
    	*pID = p->m_iEchoID;

    	int *pPort = (int*)(pBuffer + sizeof(int));
    	*pPort = p->m_iListenPort;

    	int *pIPLength = (int*)(pBuffer + sizeof(int) + sizeof(int));
    	*pIPLength = ipLen;

    	memcpy(pBuffer + sizeof(int) + sizeof(int) + sizeof(int), (p->m_strServerIP).c_str(), ipLen);

    	*pFullLength = len;
    	return pBuffer;
    }
};

class GetServerInfoACKMsgDeserializer : public MessageDeserializer
{
public:
	GetServerInfoACKMsgDeserializer()
	{}

	virtual ~GetServerInfoACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		if(!pBuffer)
			return NULL;

		GetServerInfoACKMsg *pMsg = new GetServerInfoACKMsg();

		int *pID = &(pMsg->m_iEchoID);
		*pID = *((int*)pBuffer);

		int *pPort = &(pMsg->m_iListenPort);
		*pPort = *((int*)(pBuffer + sizeof(int)));

		int ipLen = *((int*)(pBuffer + sizeof(int) + sizeof(int)));
		pMsg->m_strServerIP = string(pBuffer + + sizeof(int) + sizeof(int) + sizeof(int), ipLen);

		return pMsg;
	}	
};

#endif
