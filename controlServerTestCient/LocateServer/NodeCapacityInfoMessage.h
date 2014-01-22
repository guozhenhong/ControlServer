#ifndef NODECAPACITYINFOMESSAGE_H
#define NODECAPACITYINFOMESSAGE_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

using namespace std;

class NodeCapacityInfoMessage : public Message
{
public:	
	NodeCapacityInfoMessage() : Message(MSG_UPDATE_NODE_CAP_INFO)
	{

	}
	NodeCapacityInfoMessage(uint64_t s, uint64_t d, uint64_t u) : 
		Message(MSG_UPDATE_NODE_CAP_INFO), 
		m_SumCap(s),
		m_DecCap(d),
		m_UsedCap(u)
	{

	}

	~NodeCapacityInfoMessage()
	{

	}

public:
	uint64_t m_SumCap;
	uint64_t m_DecCap;
	uint64_t m_UsedCap;
};


class NodeCapacityInfoMsgSerializer : public MessageSerializer
{
public:
    NodeCapacityInfoMsgSerializer()
    {}

    virtual ~NodeCapacityInfoMsgSerializer()
    {}

    virtual char *Serialize(Message *pMsg, int *pFullLength)
    {
    	NodeCapacityInfoMessage *p = dynamic_cast<NodeCapacityInfoMessage *> (pMsg);
    	if(!p)
    	{
    		*pFullLength = 0;
    		return NULL;
    	}	

    	int len = 3 * sizeof(uint64_t);
    	*pFullLength = len;

    	char *pBuf = new char[len];
    	uint64_t *pValue = (uint64_t *)pBuf;
    	*pValue = p->m_SumCap;
    	pValue = (uint64_t*)(pBuf + sizeof(uint64_t));
    	*pValue = p->m_DecCap;
    	pValue = (uint64_t*)(pBuf + sizeof(uint64_t) + sizeof(uint64_t));
    	*pValue = p->m_UsedCap;

    	return pBuf;
    }
};


class NodeCapacityInfoMsgDeserializer : public MessageDeserializer
{
public:
	NodeCapacityInfoMsgDeserializer()
	{}

	virtual ~NodeCapacityInfoMsgDeserializer()
	{}

	virtual Message *Deserialize(char *pBuffer)
	{
		uint64_t s = *((uint64_t *)pBuffer);
		uint64_t d = *((uint64_t *)(pBuffer + sizeof(uint64_t)));
		uint64_t u = *((uint64_t *)(pBuffer + sizeof(uint64_t) + sizeof(uint64_t)));

		NodeCapacityInfoMessage *pMsg = new NodeCapacityInfoMessage(s, d, u);

		return pMsg;
	}

};

//info:: ACK is send by heart?? so the msg below is not used??

// class NodeCapInfoACKMessage : public Message
// {
// public:
// 	NodeCapInfoACKMessage() : Message(MSG_ACK_NODE_CAP_INFO)
// 	{

// 	}

// 	virtual ~NodeCapInfoACKMessage()
// 	{

// 	}	

// public:

// };

// class NodeCapInfoACKMsgSerializer : public MessageSerializer
// {
// public:
//     NodeCapInfoACKMsgSerializer()
//     {}

//     virtual ~NodeCapInfoACKMsgSerializer()
//     {}

//     virtual char *Serialize(Message *pMsg, int *pFullLength)
//     {
//     	*pFullLength = 0;
//       	return NULL;
//     }
// };

// class NodeCapInfoACKMsgDeserializer : public MessageDeserializer
// {
// public:
// 	NodeCapInfoACKMsgDeserializer()
// 	{}

// 	virtual ~NodeCapInfoACKMsgDeserializer()
// 	{}

// 	virtual Message *Deserialize(char *pBuffer)
// 	{
// 		NodeCapInfoACKMessage *pMsg = new NodeCapInfoACKMessage();

// 		return pMsg;
// 	}
// };

#endif
