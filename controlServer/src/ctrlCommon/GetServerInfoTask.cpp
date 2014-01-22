#include "common/sys/ThreadPoolDispatcher.h"                                     
#include "common/comm/BaseReq.h"                                                 
#include "common/comm/Agent.h"                                                   
#include "common/comm/SocketAddress.h"                                           
#include "ctrlCommon/GetServerInfoTask.h"                                   
#include "ctrlCommon/ControlAgent.h"                                             
#include "ctrlCommon/GetServerInfoWorkItem.h"                                         
#include "ctrlCommon/GetServerInfoMessage.h"                                    
                                                                                 
GetServerInfoTask::GetServerInfoTask()                                 
{                                                                                                                                                              
    m_pMsg = NULL;                                                               
}                                                                                
                                                                                 
                                                                                 
GetServerInfoTask::~GetServerInfoTask()                                
{                                                                                
                                                                                 
}                                                                                
                                                                                 
int GetServerInfoTask::recvMsg(Message *pMsg)                               
{                                                                                
    m_pMsg = pMsg;                                                               
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
int GetServerInfoTask::goNext()                                             
{                                                                                
    GetServerInfoMessage *pMsg = dynamic_cast<GetServerInfoMessage *> (m_pMsg);
    if(!pMsg)                                                                    
        return -1;  
                                                                                 
    GetServerInfoWorkItem *pWI = new GetServerInfoWorkItem(pMsg);                   
                                                                                 
    pWI->setTaskID(getID());                                                     
    g_pDispatcher->postRequest(pWI);                                             
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
void GetServerInfoTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)        
{                                                                                
    GetServerInfoWorkItem *p = dynamic_cast<GetServerInfoWorkItem *>(pWorkItem);           
    // ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);            
    if(p)                                                                        
    {                     
    	GetServerInfoACKMsg *pACK = new GetServerInfoACKMsg();                                                       
    	pACK->m_iEchoID = p->ID();
    	pACK->m_iListenPort = p->Port();
    	pACK->m_strServerIP = p->IP();

        m_pAgent->SendMsg(pACK);     
    }                                                                                                                                                      
                                                                                 
    if(m_pMsg)                                                                   
    {                                                                            
        delete m_pMsg;                                                           
        m_pMsg = NULL;                                                           
    }                                                                            
                                                                                 
    if (pWorkItem != NULL)                                                       
    {                                                                            
        delete pWorkItem;                                                        
        pWorkItem = NULL;                                                        
    }                                                                            
             
    TaskManager::getInstance()->recycle(getID());  
                                                                        
    return;                                                                      
}                                                                                
                                                                                 
int GetServerInfoTask::setAgent(ControlAgent *agent)                        
{                                                                                
    m_pAgent = agent;                                                            
                                                                                 
    return 0;                                                                    
}  


// BaseTask* GetServerInfoTask::createTask()
// {
//     return TaskManager::getInstance()->create<GetServerInfoTask>();
// }
