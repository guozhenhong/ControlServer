/*
 * =====================================================================================
 *
 *       Filename:  ControlAgent.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/13 15:45:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#include <iostream>
#include <map>
#include "ctrlCommon/ControlAgent.h"
#include "common/sys/ThreadPoolDispatcher.h"
#include "common/comm/TaskManager.h"
#include "common/log/log.h"
#include "common/comm/SocketAddress.h"
#include "ctrlCommon/RegisterServerInfoTask.h"
#include "ctrlCommon/RegisterServerMessage.h"
#include "ctrlCommon/MessageDeserializer.h"
#include "ctrlCommon/MessageSerializer.h"
#include "ctrlCommon/MultiMsgDeserializer.h"
#include "ctrlCommon/MultiMsgSerializer.h"
#include "ctrlCommon/Message.h"
#include "ctrlCommon/GetServerInfoMessage.h"
#include "ctrlCommon/GetServerInfoTask.h"


using namespace std;

ControlAgent::ControlAgent()
{

    m_pSerializer = new MultiMsgSerializer();
    m_pDeserializer = new MultiMsgDeserializer();
}

ControlAgent::ControlAgent(const SocketAddress &addr) : TCPAgent(addr)
{
    m_pSerializer = new MultiMsgSerializer();
    m_pDeserializer = new MultiMsgDeserializer();

}

ControlAgent::ControlAgent(const TCPSocket& socket, const SocketAddress& addr) :
    TCPAgent(socket, addr)
{   

    m_pSerializer = new MultiMsgSerializer();
    m_pDeserializer = new MultiMsgDeserializer();
}

ControlAgent::~ControlAgent()
{
    if(m_pSerializer)
        delete m_pSerializer;
    if(m_pDeserializer)
        delete m_pDeserializer; 

    if(!m_WorkTaskTable.empty())
    {
        map<unsigned long, BaseTask *>::iterator it = m_WorkTaskTable.begin();
        for(; it != m_WorkTaskTable.end(); ++it)
        {
            TaskManager::getInstance()->recycle(it->second->getID());
        }
    }
}

int ControlAgent::init()
{
    TCPAgent::init();

   // this->RegisterSerializer(MSG_REGISTER_SERVER, new RegisterServerMsgSerializer());
    this->RegisterDeserializer(MSG_REGISTER_SERVER, new RegisterServerMsgDeserializer());
    this->RegisterSerializer(MSG_ACK_REGISTER_SERVER, new RegisterServerACKMsgSerializer());
   // this->RegisterDeserializer(MSG_ACK_REGISTER_SERVER, new RegisterServerACKMsgDeserializer());
    this->RegisterDeserializer(MSG_GET_SERVER_INFO, new GetServerInfoMsgDeserializer());
    this->RegisterSerializer(MSG_ACK_GET_SERVER_INFO, new GetServerInfoACKMsgSerializer());
    
    RegisterServerInfoTask *pTask = TaskManager::getInstance()->create<RegisterServerInfoTask>();
    pTask->setAgent(this);
    this->RegisterWorkTask(MSG_REGISTER_SERVER, pTask);

    GetServerInfoTask *pGetTask = TaskManager::getInstance()->create<GetServerInfoTask>();
    pGetTask->setAgent(this);
    this->RegisterWorkTask(MSG_GET_SERVER_INFO, pGetTask);
    

    return 0;    
}

void
ControlAgent::readBack(InReq& req)
{
    unsigned long cmd = req.m_msgHeader.cmd;

    Message *pMsg = m_pDeserializer->Deserialize(cmd, req.ioBuf);
    if(!pMsg)
        return ;

    map<unsigned long, BaseTask *>::iterator it = m_WorkTaskTable.find(cmd);
    if(it == m_WorkTaskTable.end())
        return ;

    it->second->recvMsg(pMsg);
    it->second->goNext();

    return ;
}

void 
ControlAgent::writeBack(bool result)
{
    return ;
}

void
ControlAgent::SendMsg(Message *pMsg)
{
    int ctxLen = 0;
    char *pBuffer;

    if(pMsg)
    {
        unsigned long ID = pMsg->m_clMsgID;
        pBuffer = m_pSerializer->Serialize(ID, pMsg, &ctxLen);
    }

    int bufLength = sizeof(MsgHeader) + ctxLen;
    char *sendBuf = new char [bufLength];

    MsgHeader header;
    header.cmd = pMsg->m_clMsgID;
    header.length = ctxLen;

    memcpy(sendBuf, &header, sizeof(MsgHeader));
    if(pBuffer)
    {
        memcpy(sendBuf + sizeof(MsgHeader), pBuffer, ctxLen);
        delete [] pBuffer;
    }    
    if (this->writeToBuffer(sendBuf, bufLength) == FAILED)
    {   
        return ;
    }

    return ;
}

int ControlAgent::
RegisterSerializer(unsigned long msgID, MessageSerializer *pSerializer)
{
    return m_pSerializer->Register(msgID, pSerializer);
}

int ControlAgent::
RegisterDeserializer(unsigned long msgID, MessageDeserializer *pDeserializer)
{
    return m_pDeserializer->Register(msgID, pDeserializer);
}

int 
ControlAgent::RegisterWorkTask(unsigned long msgID, BaseTask *pTask)
{
    map<unsigned long, BaseTask *>::iterator it = m_WorkTaskTable.find(msgID);

    if(it != m_WorkTaskTable.end())
        return -1;

    m_WorkTaskTable[msgID] = pTask;

    return 0;
}

const SocketAddress& 
ControlAgent::GetOppositeAddr() const
{
    return m_addr;
}

const string&
ControlAgent::GetOppositeName() const
{
    return m_strOppositeName;
}

int
ControlAgent::SetOppositeName(string& name)
{
    m_strOppositeName = name;

    return 0;
}
