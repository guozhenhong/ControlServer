#include "common/sys/ThreadPoolDispatcher.h"                                     
#include "common/comm/BaseReq.h"                                                 
#include "common/comm/Agent.h"                                                   
#include "common/comm/SocketAddress.h"                                           
#include "ctrlCommon/RecvVDFormatResTask.h"                                   
#include "ctrlCommon/ControlAgent.h"                                                                                  
#include "ctrlCommon/VirtualDiskFormatMessage.h"                                    
                                                                                 
RecvVDFormatResTask::RecvVDFormatResTask()                                 
{                                                                                                                                                              
    m_pMsg = NULL;                                                               
}                                                                                
                                                                                 
                                                                                 
RecvVDFormatResTask::~RecvVDFormatResTask()                                
{                                                                                
                                                                                 
}                                                                                
                                                                                 
int RecvVDFormatResTask::recvMsg(Message *pMsg)                               
{                                                                                
    m_pMsg = pMsg;                                                               
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
int RecvVDFormatResTask::goNext()                                             
{                                                                                
    VirtualDiskFormatACKMessage *pMsg = dynamic_cast<VirtualDiskFormatACKMessage *> (m_pMsg);
    if(!pMsg)                                                                    
        return -1;  
                                                                                 
    BaseTask *pTask = TaskManager::getInstance()->get(pMsg->m_iEchoID);
    pTask->recvMsg(pMsg);

    pTask->goNext();       

    TaskManager::getInstance()->recycle(getID());            
                                                                                                                                                              
    return 0;                                                                    
}                                                                                                                                                            
                                                                                 
int RecvVDFormatResTask::setAgent(ControlAgent *agent)                        
{                                                                                
    m_pAgent = agent;                                                            
                                                                                 
    return 0;                                                                    
}  


// BaseTask* RecvVDFormatResTask::createTask()
// {
//     return TaskManager::getInstance()->create<RecvVDFormatResTask>();
// }
