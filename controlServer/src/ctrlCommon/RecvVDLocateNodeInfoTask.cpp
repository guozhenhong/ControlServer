#include "common/sys/ThreadPoolDispatcher.h"                                     
#include "common/comm/BaseReq.h"                                                 
#include "common/comm/Agent.h"                                                   
#include "common/comm/SocketAddress.h"                                           
#include "ctrlCommon/RecvVDLocateNodeInfoTask.h"                                   
#include "ctrlCommon/ControlAgent.h"                                                                                  
#include "ctrlCommon/GetVDLocateNodeInfoMessage.h"                                    
                                                                                 
RecvVDLocateNodeInfoTask::RecvVDLocateNodeInfoTask()                                 
{                                                                                                                                                              
    m_pMsg = NULL;                                                               
}                                                                                
                                                                                 
                                                                                 
RecvVDLocateNodeInfoTask::~RecvVDLocateNodeInfoTask()                                
{                                                                                
                                                                                 
}                                                                                
                                                                                 
int RecvVDLocateNodeInfoTask::recvMsg(Message *pMsg)                               
{                                                                                
    m_pMsg = pMsg;                                                               
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
int RecvVDLocateNodeInfoTask::goNext()                                             
{                                                                                
    GetVDLocateNodeInfoACKMessage *pMsg = dynamic_cast<GetVDLocateNodeInfoACKMessage *> (m_pMsg);
    if(!pMsg)                                                                    
        return -1;  
                                                                                 
    BaseTask *pTask = TaskManager::getInstance()->get(pMsg->m_iEchoID);
    pTask->recvMsg(pMsg);

    pTask->goNext();       

    TaskManager::getInstance()->recycle(getID());            
                                                                                                                                                              
    return 0;                                                                    
}                                                                                                                                                            
                                                                                 
int RecvVDLocateNodeInfoTask::setAgent(ControlAgent *agent)                        
{                                                                                
    m_pAgent = agent;                                                            
                                                                                 
    return 0;                                                                    
}  


// BaseTask* RecvVDLocateNodeInfoTask::createTask()
// {
//     return TaskManager::getInstance()->create<RecvVDLocateNodeInfoTask>();
// }
