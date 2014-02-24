#include "common/sys/ThreadPoolDispatcher.h"                                     
#include "common/comm/BaseReq.h"                                                 
#include "common/comm/Agent.h"                                                   
#include "common/comm/SocketAddress.h"                                           
#include "ctrlCommon/ChangeVDSumCapTask.h"                                   
#include "ctrlCommon/ControlAgent.h"                                             
#include "ctrlCommon/GetVDCapacityInfoWorkItem.h"    
#include "ctrlCommon/GetSUCapacityInfoWorkItem.h"                                     
#include "ctrlCommon/ChangeVDSumCapMessage.h"
#include "ctrlCommon/ChangeVDSumFormatCapMessage.h"       
#include "ctrlCommon/GetVDLocateNodeInfoMessage.h"    
#include "ctrlCommon/SetVDInfoWorkItem.h"                         
#include <iostream>
                                                                                 
ChangeVDSumCapTask::ChangeVDSumCapTask()                                 
{                                                                                                                                                              
    m_pMsg = NULL;
    m_iRes = 3;
    m_state = DO_CHECK_VD_CAP;                                                               
}                                                                                
                                                                                 
                                                                                 
ChangeVDSumCapTask::~ChangeVDSumCapTask()                                
{                                                                                
                                                                                 
}                                                                                
                                                                                 
int ChangeVDSumCapTask::recvMsg(Message *pMsg)                               
{                                                                                
    m_pMsg = pMsg;                                                               
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
int ChangeVDSumCapTask::goNext()                                             
{                                                                                
    switch(m_state)
    {
        case DO_CHECK_VD_CAP:
        {
            ChangeVDSumCapMessage *pMsg = dynamic_cast<ChangeVDSumCapMessage *> (m_pMsg);
            if(!pMsg)                                                                    
                return -1;  
            
            m_iVDID = pMsg->m_iVDID;
            m_vdNewSumCap = pMsg->m_newCap;     

            GetVDCapacityInfoWorkItem *pWI = new GetVDCapacityInfoWorkItem(m_iVDID);                   
                                                                                 
            pWI->setTaskID(getID());                                                     
            g_pDispatcher->postRequest(pWI);    

            m_state = DO_GET_VD_LOCATE;
            //goNext();
            break;
        }
        case DO_GET_VD_LOCATE:
        {
            //get Agent* for location server.and send msg to it to get a node info where to locate the vd
            //and the msg should have a word to sign the task ID
            // cause of the heart connection, it is supporst to be a connection with every server
            // but if the connect is break? that means the heart is breaK, so i must new one connect to locateserver!!!!
            m_pAgent->SendMsgToServer(LOCATE_SERVER_NAME, new GetVDLocateNodeInfoMessage(getID(), m_iVDID));

            m_state = DO_CHECK_NODE_CAP;
            break;
        }
        case DO_CHECK_NODE_CAP:
        {
            if(!m_pMsg)
                return -1;
            GetVDLocateNodeInfoACKMessage *pMsg = dynamic_cast<GetVDLocateNodeInfoACKMessage *> (m_pMsg);
            if(!pMsg)
                return -1;

            m_strVDLocateNodeName = pMsg->m_strLocateNodeName;
            m_state = DO_CHANGE_FORMAT_CAP;

            // m_pAgent->SendMsgToServer(pMsg->m_strLocateNodeName, new VirtualDiskFormatMessage(getID(), m_iVDID, m_iCap, m_iFSType/*, fsType*/));
            if(m_vdNewSumCap > m_vdOldSumCap)
            {
                GetSUCapacityInfoWorkItem *pWI = new GetSUCapacityInfoWorkItem(m_strVDLocateNodeName);                   
                                                                                 
                pWI->setTaskID(getID());                                                     
                g_pDispatcher->postRequest(pWI);   
            }
            else
            {
                delete m_pMsg;
                m_pMsg = NULL;
                goNext();
            }
            
            // delete m_pMsg;
            // m_pMsg = NULL;

            break;
        }
        case DO_CHANGE_FORMAT_CAP:
        {
            m_pAgent->SendMsgToServer(m_strVDLocateNodeName, new ChangeVDSumFormatCapMessage(getID(), m_iVDID, m_vdNewSumCap));
            
            m_state = DO_CHANGE_DB_INFO;
            break;
        }
        case DO_CHANGE_DB_INFO:
        {
            m_state = FINISHED;
            if(m_pMsg)
            {
                ChangeVDSumFormatCapACKMessage *pMsg = dynamic_cast<ChangeVDSumFormatCapACKMessage *>(m_pMsg);
                if(pMsg)
                {
                    std::cout<<"the res from Node is : "<<pMsg->m_iRes<<std::endl;
                    if((pMsg->m_iRes) == 0)
                    {
                        // m_iRes = pMsg->m_iRes;
                        SetVDInfoWorkItem *pWI = new SetVDInfoWorkItem(CH_DEC_CAP, m_iVDID, m_vdNewSumCap);                   
                                                                                 
                        pWI->setTaskID(getID());                                                     
                        g_pDispatcher->postRequest(pWI);   
                    }
                    else
                    {
                        delete m_pMsg;
                        m_pMsg = NULL;
                        goNext();
                    }
                }

            }
            break;
        }
        case FINISHED:
        {
            m_pAgent->SendMsg(new ChangeVDSumCapACKMessage(m_iVDID, m_iRes));
            
            TaskManager::getInstance()->recycle(getID());  

            break;
        }

    }                                        
                                                                                 
    return 0;                                                                    
}                                                                                
                                                                                 
void ChangeVDSumCapTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)        
{                                                                                
    switch(m_state)
    {
        case DO_GET_VD_LOCATE:
        {
            GetVDCapacityInfoWorkItem *p = dynamic_cast<GetVDCapacityInfoWorkItem *>(pWorkItem);           
            // ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);            
            if(p)                                                                        
            {                     
                uint64_t vdUsedCap = p->GetUsedCapacity();
                // std::cout<<"vd used cap is "<<vdUsedCap<<"  new cap is "<<m_vdNewSumCap<<endl;
                m_vdOldSumCap = p->GetDeclaredCapacity();
                if(vdUsedCap > m_vdNewSumCap)
                {
                    m_iRes = 1;
                    m_state = FINISHED;
                }
            }
            break;      
        }
        case DO_CHANGE_FORMAT_CAP:
        {
            GetSUCapacityInfoWorkItem *p = dynamic_cast<GetSUCapacityInfoWorkItem *>(pWorkItem);
            if(p)
            {
                m_nodeRestCap = p->GetSumCapacity() - (p->GetDeclareCapacity());
                if(m_nodeRestCap < (m_vdNewSumCap - m_vdOldSumCap))
                {
                    m_iRes = 2;
                    m_state = FINISHED;
                }
            }
            break;
        }
        case FINISHED:
        {
            SetVDInfoWorkItem *p = dynamic_cast<SetVDInfoWorkItem *>(pWorkItem);
            if(p)
            {
                if(1 == p->getRes())
                    m_iRes = 0;
            }
            break;
        }
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
             
    goNext();
                                                                        
    return;                                                                      
}                                                                                
                                                                                 
int ChangeVDSumCapTask::setAgent(ControlAgent *agent)                        
{                                                                                
    m_pAgent = agent;                                                            
                                                                                 
    return 0;                                                                    
}  


// BaseTask* ChangeVDSumCapTask::createTask()
// {
//     return TaskManager::getInstance()->create<ChangeVDSumCapTask>();
// }
