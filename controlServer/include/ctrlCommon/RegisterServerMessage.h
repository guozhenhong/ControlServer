#ifndef REGISTERSERVERMESSAGE_H
#define REGISTERSERVERMESSAGE_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"

using namespace std;
//注意。是不要ip的。因为连接建立后，可以得到对端的地址。这里只是测试。别忘记了要该回来
class RegisterServerMessage : public Message
{
public:	
	RegisterServerMessage() : Message(MSG_REGISTER_SERVER),  
		m_strServerName(""), m_serverPort(0)
	{

	}

	RegisterServerMessage(const char* name, unsigned short port):
		Message(MSG_REGISTER_SERVER), m_strServerName(name), m_serverPort(port)
	{

	}

	~RegisterServerMessage()
	{

	}

public:
	string m_strServerName;
	// string m_strServerIP;
	unsigned short m_serverPort;		
};


class RegisterServerMsgSerializer : public MessageSerializer
{
public:
    RegisterServerMsgSerializer()
    {}

    virtual ~RegisterServerMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	RegisterServerMessage *p = dynamic_cast<RegisterServerMessage *> (pMsg);
    	if(!p)
    		return NULL;

    	unsigned int len = sizeof(int) + (p->m_strServerName).length() + sizeof(unsigned short);
    	*pFullLength = len;

    	char *pBuffer = new char[len];

    	int *pNameLen = (int*)pBuffer;
    	*pNameLen = (p->m_strServerName).length();
    	memcpy(pBuffer + sizeof(int), (p->m_strServerName).c_str(), *pNameLen);

    	unsigned short *pPort = (unsigned short *)(pBuffer + sizeof(int) + *pNameLen);
    	*pPort = p->m_serverPort;

    	return pBuffer;
    }
};


class RegisterServerMsgDeserializer : public MessageDeserializer
{
public:
	RegisterServerMsgDeserializer()
	{}

	virtual ~RegisterServerMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		if(!pBuffer)
			return NULL;

		RegisterServerMessage *p = new RegisterServerMessage();

		int *pNameLen = (int*)(pBuffer);
		p->m_strServerName = string(pBuffer + sizeof(int), *pNameLen);

		p->m_serverPort = *((unsigned short*)(pBuffer + sizeof(int) + *pNameLen));

		return p;
	}

};

class RegisterServerACKMSG : public Message
{
public:
	RegisterServerACKMSG() : Message(MSG_ACK_REGISTER_SERVER), m_iSuccessFlag(1)
	{

	}
	RegisterServerACKMSG(int isSuccessFlag) : 
		Message(MSG_ACK_REGISTER_SERVER), m_iSuccessFlag(m_iSuccessFlag)
	{

	}

	RegisterServerACKMSG(bool isSuccess) : Message(MSG_ACK_REGISTER_SERVER)
	{
		if(isSuccess)
			m_iSuccessFlag = 1;
		else
			m_iSuccessFlag = 0;
	}

	virtual ~RegisterServerACKMSG()
	{

	}	

public:
	int m_iSuccessFlag;
};

class RegisterServerACKMsgSerializer : public MessageSerializer
{
public:
    RegisterServerACKMsgSerializer()
    {}

    virtual ~RegisterServerACKMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	RegisterServerACKMSG *p = dynamic_cast<RegisterServerACKMSG *>(pMsg);
    	if(!p)
    		return NULL;

    	int len = sizeof(int);
    	*pFullLength = len;

    	char *pBuffer = new char[len];
    	int *pFlag = (int*)pBuffer;
    	*pFlag = p->m_iSuccessFlag;

    	return pBuffer;
    }
};

class RegisterServerACKMsgDeserializer : public MessageDeserializer
{
public:
	RegisterServerACKMsgDeserializer()
	{}

	virtual ~RegisterServerACKMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		int pIsSuc = *((int*)pBuffer);

		RegisterServerACKMSG *pMsg = new RegisterServerACKMSG(pIsSuc);

		return pMsg;
	}
};

#endif
