#include "common/sys/ThreadPoolDispatcher.h"                                     
#include "common/comm/BaseReq.h"                                                 
#include "common/comm/Agent.h"                                                   
#include "common/comm/SocketAddress.h"                                           
#include "ctrlCommon/RecvVDDeleteResTask.h"                                   
#include "ctrlCommon/ControlAgent.h"                                                                                  
#include "ctrlCommon/VirtualDiskDeleteMessage.h"                                    
                                                                                 
RecvVDDeleteResTask::RecvVDDeleteResTask()                                 
{                                                                                                                                                              
    m_pMsg = NULL;                                                               
}                                                                                
                                                                                 
                                                                                 
RecvVDDeleteResTask::~RecvVDDeleteResTask()                                
{                                                                                
                                                                                 
}                                                                                
                                                                                 
int RecvVDDeleteResTask::recvMsg(Message *pMsg)                               
{                                                                                
    m_pMsg = pMsg;                                                               
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
int RecvVDDeleteResTask::goNext()                                             
{                                                                                
    VirtualDiskDeleteACKMessage *pMsg = dynamic_cast<VirtualDiskDeleteACKMessage *> (m_pMsg);
    if(!pMsg)                                                                    
        return -1;  
                                                                                 
    BaseTask *pTask = TaskManager::getInstance()->get(pMsg->m_iEchoID);
    pTask->recvMsg(pMsg);

    pTask->goNext();       

    TaskManager::getInstance()->recycle(getID());            
                                                                                                                                                              
    return 0;                                                                    
}                                                                                                                                                            
                                                                                 
int RecvVDDeleteResTask::setAgent(ControlAgent *agent)                        
{                                                                                
    m_pAgent = agent;                                                            
                                                                                 
    return 0;                                                                    
}  


// BaseTask* RecvVDDeleteResTask::createTask()
// {
//     return TaskManager::getInstance()->create<RecvVDDeleteResTask>();
// }
