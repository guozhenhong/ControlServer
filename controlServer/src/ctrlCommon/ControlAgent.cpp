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
#include "common/comm/AgentManager.h"
#include "ctrlCommon/RegisterServerInfoTask.h"
#include "ctrlCommon/GetSysCapacityInfoTask.h"
#include "ctrlCommon/GetSysCapMessage.h"
#include "ctrlCommon/RegisterServerMessage.h"
#include "ctrlCommon/MessageDeserializer.h"
#include "ctrlCommon/MessageSerializer.h"
#include "ctrlCommon/MultiMsgDeserializer.h"
#include "ctrlCommon/MultiMsgSerializer.h"
#include "ctrlCommon/Message.h"
#include "ctrlCommon/GetServerInfoMessage.h"
#include "ctrlCommon/GetServerInfoTask.h"
#include "ctrlCommon/NodeCapacityInfoMessage.h"
#include "ctrlCommon/SetNodeCapacityInfoTask.h"
#include "ctrlCommon/SetNodeCapInfoWorkItem.h"
#include "ctrlCommon/GetSUCapacityInfoTask.h"
#include "ctrlCommon/GetSUCapacityInfoMessage.h"
#include "ctrlCommon/ManageServerCreateVDTask.h"
#include "ctrlCommon/ManageServerCreateBatchVDTask.h"
#include "ctrlCommon/ManageServerDeleteVDTask.h"
#include "ctrlCommon/ManageServerDeleteBatchVDTask.h"
#include "ctrlCommon/ManageServerCreateVDMessage.h"
#include "ctrlCommon/ManageServerCreateBatchVDMessage.h"
#include "ctrlCommon/ManageServerDeleteVDMessage.h"
#include "ctrlCommon/ManageServerDeleteBatchVDMessage.h"
#include "ctrlCommon/VirtualDiskFormatMessage.h"
#include "ctrlCommon/VirtualDiskDeleteMessage.h"
#include "ctrlCommon/SetVDStatusMessage.h"
#include "ctrlCommon/GetVDLocateNodeInfoMessage.h"
#include "ctrlCommon/RecvVDFormatResTask.h"
#include "ctrlCommon/RecvVDLocateNodeInfoTask.h"
#include "ctrlCommon/RecvVDDeleteResTask.h"
#include "ctrlCommon/ChangeVDSumCapMessage.h"
#include "ctrlCommon/ChangeVDSumCapTask.h"
#include "ctrlCommon/ChangeVDSumFormatCapMessage.h"
#include "ctrlCommon/RecvVDSumFormatCapChangeResTask.h"

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
    
    this->RegisterDeserializer(MSG_GET_SYS_CAP_INFO, new GetSysCapMsgDeserializer());
    this->RegisterSerializer(MSG_ACK_SYS_CAP_INFO, new GetSysCapACKMsgSerializer());
    
    this->RegisterDeserializer(MSG_UPDATE_NODE_CAP_INFO, new NodeCapacityInfoMsgDeserializer());

    this->RegisterDeserializer(MSG_GET_SU_CAP_INFO, new GetSUCapacityInfoMsgDeserializer());
    this->RegisterSerializer(MSG_ACK_SU_CAP_INFO, new GetSUCapacityInfoACKMsgSerializer());

    this->RegisterSerializer(MSG_ACK_MANAGE_SERVER_CREATE_VD, new ManageServerCreateVDACKMsgSerializer());
    this->RegisterDeserializer(MSG_MANAGE_SERVER_CREATE_VD, new ManageServerCreateVDMsgDeserializer());

    this->RegisterSerializer(MSG_ACK_MANAGER_SERVER_BATCH_CREATE_VD, new ManageServerCreateBatchVDACKMsgSerializer());
    this->RegisterDeserializer(MSG_MANAGER_SERVER_BATCH_CREATE_VD, new ManageServerCreateBatchVDMsgDeserializer());

    this->RegisterSerializer(MSG_GET_VD_LOCATE_NODE, new GetVDLocateNodeInfoMsgSerializer());
    this->RegisterDeserializer(MSG_ACK_VD_LOCATE_NODE, new GetVDLocateNodeInfoACKMsgDeserializer());

    this->RegisterSerializer(MSG_NODE_FORMAT_VD, new VirtualDiskFormatMsgSerializer());
    this->RegisterDeserializer(MSG_ACK_FORMAT_VD, new VirtualDiskFormatACKMsgDeserializer());

    this->RegisterSerializer(MSG_NODE_DEL_VD, new VirtualDiskDeleteMsgSerializer());
    this->RegisterDeserializer(MSG_ACK_DEL_VD, new VirtualDiskDeleteACKMsgDeserializer());

    this->RegisterSerializer(MSG_ACK_MANAGE_SERVER_DELETE_VD, new ManageServerDeleteVDACKMsgSerializer());
    this->RegisterDeserializer(MSG_MANAGE_SERVER_DELETE_VD, new ManageServerDeleteVDMsgDeserializer());

    this->RegisterSerializer(MSG_ACK_MANAGER_SERVER_BATCH_DELETE_VD, new ManageServerDeleteBatchVDACKMsgSerializer());
    this->RegisterDeserializer(MSG_MANAGER_SERVER_BATCH_DELETE_VD, new ManageServerDeleteBatchVDMsgDeserializer());

    this->RegisterSerializer(MSG_ACK_CH_VD_CAP, new ChangeVDSumCapACKMsgSerializer());
    this->RegisterDeserializer(MSG_CHANGE_VD_CAP, new ChangeVDSumCapMsgDeserializer());

    this->RegisterSerializer(MSG_CHANGE_VD_FORMAT_CAP, new ChangeVDSumFormatCapMsgSerializer());
    this->RegisterDeserializer(MSG_ACK_CH_VD_FORMAT_CAP, new ChangeVDSumFormatCapACKMsgDeserializer());
    
    this->RegisterTaskCreateFunc(MSG_REGISTER_SERVER, RegisterServerInfoTask::createTask<RegisterServerInfoTask>);

    this->RegisterTaskCreateFunc(MSG_GET_SERVER_INFO, GetServerInfoTask::createTask<GetServerInfoTask>);
    
    this->RegisterTaskCreateFunc(MSG_GET_SYS_CAP_INFO, GetSysCapacityInfoTask::createTask<GetSysCapacityInfoTask>);
    
    this->RegisterTaskCreateFunc(MSG_UPDATE_NODE_CAP_INFO, SetNodeCapacityInfoTask::createTask<SetNodeCapacityInfoTask>);

    this->RegisterTaskCreateFunc(MSG_GET_SU_CAP_INFO, GetSUCapacityInfoTask::createTask<GetSUCapacityInfoTask>);

    this->RegisterTaskCreateFunc(MSG_MANAGE_SERVER_CREATE_VD, ManageServerCreateVDTask::createTask<ManageServerCreateVDTask>);

    this->RegisterTaskCreateFunc(MSG_MANAGER_SERVER_BATCH_CREATE_VD, ManageServerCreateBatchVDTask::createTask<ManageServerCreateBatchVDTask>);

    this->RegisterTaskCreateFunc(MSG_MANAGE_SERVER_DELETE_VD, ManageServerDeleteVDTask::createTask<ManageServerDeleteVDTask>);

    this->RegisterTaskCreateFunc(MSG_MANAGER_SERVER_BATCH_DELETE_VD, ManageServerDeleteBatchVDTask::createTask<ManageServerDeleteBatchVDTask>);

    this->RegisterTaskCreateFunc(MSG_ACK_VD_LOCATE_NODE, RecvVDLocateNodeInfoTask::createTask<RecvVDLocateNodeInfoTask>);

    this->RegisterTaskCreateFunc(MSG_ACK_FORMAT_VD, RecvVDFormatResTask::createTask<RecvVDFormatResTask>);

    this->RegisterTaskCreateFunc(MSG_ACK_DEL_VD, RecvVDDeleteResTask::createTask<RecvVDDeleteResTask>);

    this->RegisterTaskCreateFunc(MSG_CHANGE_VD_CAP, ChangeVDSumCapTask::createTask<ChangeVDSumCapTask>);

    this->RegisterTaskCreateFunc(MSG_ACK_CH_VD_FORMAT_CAP, RecvVDSumFormatCapChangeResTask::createTask<RecvVDSumFormatCapChangeResTask>);

    return 0;    
}

void
ControlAgent::readBack(InReq& req)
{
    unsigned long cmd = req.m_msgHeader.cmd;
    Message *pMsg;
    pMsg = m_pDeserializer->Deserialize(cmd, req.ioBuf);
    if(!pMsg)
        return ;
    
    return Dispatch(cmd, pMsg);
   /* map<unsigned long, BaseTask *>::iterator it = m_WorkTaskCreaterTable.find(cmd);
    if(it == m_WorkTaskCreaterTable.end())
        return ;

    it->second->recvMsg(pMsg);
    it->second->goNext();
    */
}

//this function is made for other`s to call.Maybe the Task will call this
//dispatch func.
//cause one iobuf may include several msgs here for the heart from node!!

void 
ControlAgent::Dispatch(uint32_t cmd, Message *pMsg)
{
    BaseTask *p = GetTaskObject(cmd);
    if(p)
    {
        p->setAgent(this);
        p->recvMsg(pMsg);
        p->goNext();
    }

    return ;
}

BaseTask* 
ControlAgent::GetTaskObject(uint32_t cmd)
{
    map<unsigned long, pTaskCreateFunction>::iterator it = m_WorkTaskCreaterTable.find(cmd);
    if(it == m_WorkTaskCreaterTable.end())
        return NULL;
    
    pTaskCreateFunction pFunction = it->second;
    BaseTask* pTask = (*pFunction)();

    return pTask;

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
    if(ctxLen > 0)
    {
        memcpy(sendBuf + sizeof(MsgHeader), pBuffer, ctxLen);
        delete [] pBuffer;
    }    
    if (this->writeToBuffer(sendBuf, bufLength) == FAILED)
    {   
        return ;
    }
    
    if(pMsg)
    {
        delete pMsg;
        pMsg = NULL;
    }

    return ;
}


void ControlAgent::
SendMsgToServer(const std::string& name, Message *pMsg)
{
    Agent *p = ( AgentManager::getInstance() )->get( name );
    if(NULL == p)
    {
        //this connection is not exist.must new connect to locate server!!!
        return ;
    }

    ControlAgent *pAgent = dynamic_cast<ControlAgent *> (p);
    if(!pAgent)
    {
        return ;
    }   

    return pAgent->SendMsg(pMsg);
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
ControlAgent::RegisterTaskCreateFunc(unsigned long msgID, pTaskCreateFunction pFunc)
{
    map<unsigned long, pTaskCreateFunction>::iterator it = m_WorkTaskCreaterTable.find(msgID);

    if(it != m_WorkTaskCreaterTable.end())
        return -1;

    m_WorkTaskCreaterTable[msgID] = pFunc;

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

    return ( (AgentManager::getInstance() )->add(name, this) );
}
