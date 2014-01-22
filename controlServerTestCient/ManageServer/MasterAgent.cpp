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
#include "ManageServerCreateBatchVDMessage.h"
#include "Message.h"
#include "MessageID.h"
#include "ManageServerDeleteBatchVDMessage.h"
#include "ManageServerDeleteVDMessage.h"
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
    } 
    ManageServerCreateBatchVDACKMessage *pCreateACK = dynamic_cast<ManageServerCreateBatchVDACKMessage *> (pMsg);
    if(pCreateACK)
    {
        std::cout<<"batch create number is: "<<pCreateACK->m_iDiskNum<<std::endl;
        for(uint32_t i = 0; i < pCreateACK->m_iDiskNum; i++)
        {
            ManageServerCreateVDACKMessage *pACK = &(pCreateACK->m_pResList[i]);
            std::cout<<"the Create ACK info is: "<<pACK->m_iEchoID<<" res is : "<<pACK->m_bIsSuccess<<" the disk ID is : "<<pACK->m_iDiskID<<std::endl;

        }
    }

    ManageServerDeleteBatchVDACKMessage *pDesMsg = dynamic_cast<ManageServerDeleteBatchVDACKMessage*> (pMsg);
    if(pDesMsg)
    {
        ManageServerDeleteVDACKMessage *pDesList = pDesMsg->m_pResList;
        if(pDesList);
        {
            std::cout<<"the Batch Del ACK info is: "<<std::endl;
            for(uint32_t i = 0; i < pDesMsg->m_iDiskNum; i++)
            {
                std::cout<<"the Disk ID is: "<<pDesList[i].m_iVDID<<" res is : "<<pDesList[i].m_bIsSuccess<<std::endl;
            }
        }
    }
    ManageServerDeleteVDACKMessage *pSglDelMsg = dynamic_cast<ManageServerDeleteVDACKMessage*> (pMsg);
    if(pSglDelMsg)
    {
        std::cout<<"the single del ACK info is: "<<std::endl;
        std::cout<<"the Disk ID is: "<<pSglDelMsg->m_iVDID<<" res is : "<<pSglDelMsg->m_bIsSuccess<<std::endl;
    }

}

void MasterAgent::writeBack(bool result)
{

}

int MasterAgent::connectAfter(bool result)
{
    RegisterServerMessage *pMsg = new RegisterServerMessage("ManageServer", 4567);
    //RegisterServerMessage *pMsg = new RegisterServerMessage(LOCATE_SERVER_NAME, 2345);
    SendMsg(pMsg);
//    SendMsg(new NodeCapacityInfoMessage(100000000000, 50000000000, 20000000000));
  //  SendMsg(new GetSUCapacityInfoMessage(3, "SU2"));

    ManageServerCreateVDMessage *pCreateList = new ManageServerCreateVDMessage [5];
    for(int i = 0; i < 5; i++)
    {
        pCreateList[i].m_iEchoID = i + 1;
        pCreateList[i].m_FileSysType = 3;
        pCreateList[i].m_VDCapacity = 10000000000 * i;

    }

    ManageServerCreateBatchVDMessage *pBatchCreateMsg = new ManageServerCreateBatchVDMessage(5, pCreateList);

    SendMsg(pBatchCreateMsg);

    // int num = 4;
    // uint32_t *pDelIDList = new uint32_t[4];
    // for(int i = 0; i < num; i++)
    // {
    //     pDelIDList[i] = i + 3;
    // }

    int num = 2;
    uint32_t *pDelIDList = new uint32_t[2];
    for(int i = 0; i < num; i++)
    {
        pDelIDList[0] = 1;
        pDelIDList[1] = 4;
    }

    ManageServerDeleteBatchVDMessage *pBatchDelMsg = new ManageServerDeleteBatchVDMessage(num, pDelIDList);

    sleep(30);

    SendMsg(pBatchDelMsg);

    // ManageServerDeleteVDMessage *pDelMsg = new ManageServerDeleteVDMessage(2);
    // SendMsg(pDelMsg);

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
