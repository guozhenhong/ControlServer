/*
 * =====================================================================================
 *
 *       Filename:  MasterAgent.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/26/2013 09:41:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#include "RegisterServerMessage.h"
#include "MasterAgent.h"
#include "common/comm/SocketAddress.h"
#include "common/comm/BaseHeader.h"
#include "common/sys/ThreadPoolDispatcher.h"
#include "common/comm/TaskManager.h"
#include "common/log/log.h"
#include "MessageDeserializer.h"
#include "MessageSerializer.h"
#include "MultiMsgDeserializer.h"
#include "MultiMsgSerializer.h"
#include "GetServerInfoMessage.h"
#include "GetVDLocateNodeInfoMessage.h"
#include "NodeCapacityInfoMessage.h"
#include "GetSUCapacityInfoMessage.h"
#include "GetSysCapMessage.h"
#include "ManageServerCreateVDMessage.h"
#include "Message.h"
#include "MessageID.h"
#include <iostream>

using namespace std;

MasterAgent::MasterAgent(const SocketAddress& addr) : TCPAgent(addr)
{
    m_pSerializer = new MultiMsgSerializer();
    m_pDeserializer = new MultiMsgDeserializer();
}

MasterAgent::~MasterAgent()
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

void MasterAgent::readBack(InReq& req)
{
    unsigned long cmd = req.m_msgHeader.cmd;

    std::cout<<"the cmd is : "<<cmd<<endl;
    // int a = *((int*)req.ioBuf);
    // std::cout<<"the buffer value is : "<<a<<endl;
    Message *pMsg = m_pDeserializer->Deserialize(cmd, req.ioBuf);
    if(!pMsg)
        return ;
    GetSUCapacityInfoACKMessage *p = dynamic_cast<GetSUCapacityInfoACKMessage*>(pMsg);
    if(p)
    {
        std::cout<<"the SU cap info is: "<<p->m_iEchoID<<" "<<p->m_MaxCapacity<<" "<<p->m_DiskDecCapacity<<" "<<p->m_DataUsedCapacity<<std::endl;
    } 
    GetVDLocateNodeInfoMessage *pLocate = dynamic_cast<GetVDLocateNodeInfoMessage*>(pMsg);
    if(pLocate)
    {
        std::cout<<"the locate info is: "<<pLocate->m_iEchoID<<" vd ID is : "<<pLocate->m_iVDID<<std::endl;
        SendMsg(new GetVDLocateNodeInfoACKMessage(pLocate->m_iEchoID, pLocate->m_iVDID, STORAGE_SERVER_ONE_NAME));
    } 
}

void MasterAgent::writeBack(bool result)
{

}

int MasterAgent::connectAfter(bool result)
{
    RegisterServerMessage *pMsg = new RegisterServerMessage(LOCATE_SERVER_NAME, 2345);
    SendMsg(pMsg);
//    SendMsg(new NodeCapacityInfoMessage(100000000000, 50000000000, 20000000000));
  //  SendMsg(new GetSUCapacityInfoMessage(3, "SU2"));

    return 0;
}

void
MasterAgent::SendMsg(Message *pMsg)
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
    delete pMsg;

    return ;
}

int MasterAgent::
RegisterSerializer(unsigned long msgID, MessageSerializer *pSerializer)
{
    return m_pSerializer->Register(msgID, pSerializer);
}

int MasterAgent::
RegisterDeserializer(unsigned long msgID, MessageDeserializer *pDeserializer)
{
    return m_pDeserializer->Register(msgID, pDeserializer);
}

int 
MasterAgent::RegisterWorkTask(unsigned long msgID, BaseTask *pTask)
{
    map<unsigned long, BaseTask *>::iterator it = m_WorkTaskTable.find(msgID);

    if(it != m_WorkTaskTable.end())
        return -1;

    m_WorkTaskTable[msgID] = pTask;

    return 0;
}
