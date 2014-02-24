#include "common/sys/ThreadPoolDispatcher.h"                                     
#include "common/comm/BaseReq.h"                                                 
#include "common/comm/Agent.h"                                                   
#include "common/comm/SocketAddress.h"                                           
#include "ctrlCommon/RecvVDSumFormatCapChangeResTask.h"                                   
#include "ctrlCommon/ControlAgent.h"                                                                                  
#include "ctrlCommon/ChangeVDSumFormatCapMessage.h"                                    
                                                                                 
RecvVDSumFormatCapChangeResTask::RecvVDSumFormatCapChangeResTask()                                 
{                                                                                                                                                              
    m_pMsg = NULL;                                                               
}                                                                                
                                                                                 
                                                                                 
RecvVDSumFormatCapChangeResTask::~RecvVDSumFormatCapChangeResTask()                                
{                                                                                
                                                                                 
}                                                                                
                                                                                 
int RecvVDSumFormatCapChangeResTask::recvMsg(Message *pMsg)                               
{                                                                                
    m_pMsg = pMsg;                                                               
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
int RecvVDSumFormatCapChangeResTask::goNext()                                             
{                                                                                
    ChangeVDSumFormatCapACKMessage *pMsg = dynamic_cast<ChangeVDSumFormatCapACKMessage *> (m_pMsg);
    if(!pMsg)                                                                    
        return -1;  
                                                                                 
    BaseTask *pTask = TaskManager::getInstance()->get(pMsg->m_iEchoID);
    pTask->recvMsg(pMsg);

    pTask->goNext();       

    TaskManager::getInstance()->recycle(getID());            
                                                                                                                                                              
    return 0;                                                                    
}                                                                                                                                                            
                                                                                 
int RecvVDSumFormatCapChangeResTask::setAgent(ControlAgent *agent)                        
{                                                                                
    m_pAgent = agent;                                                            
                                                                                 
    return 0;                                                                    
}  


// BaseTask* RecvVDSumFormatCapChangeResTask::createTask()
// {
//     return TaskManager::getInstance()->create<RecvVDSumFormatCapChangeResTask>();
// }
