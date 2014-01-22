#include <iostream>                                                              
#include "controlDB/CCtrlDBHandler.h"                                            
#include "controlDB/CServerInfoTable.h"                                          
#include "controlDB/CTableHandler.h"                                             
#include "ctrlCommon/GetServerInfoWorkItem.h"                                         
#include "common/comm/Error.h"                                                   
#include "common/log/log.h"                                                      
                                                                                 
GetServerInfoWorkItem::GetServerInfoWorkItem(GetServerInfoMessage *p):        
     m_pMsg(p), m_strIP("")                            
{                                                                                
}                                                                                
                                                                                 
GetServerInfoWorkItem::~GetServerInfoWorkItem()                                            
{                                                                                
}                                                                                
                                                                                 
int                                                                              
GetServerInfoWorkItem::process()                                                      
{                                                                                
    if(!m_pMsg)                                                                  
        return -1;                                                               
                                                                                 
    std::cout<<"In GetServerInfoWorkItem::process()"<<std::endl;                      
    std::cout<<"The request Server Name is "<<m_pMsg->m_strServerName<<std::endl;           
    std::cout<<"The request echo id is "<<m_pMsg->m_iEchoID<<std::endl;  

    m_iEchoID = m_pMsg->m_iEchoID;
                                                                                 
    //register the server info into DB                                           
    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_SERVER_INFO_TABLE);
    if(!p)                                                                       
        return -1;                                                               
    CServerInfoTable *pServerInfoTable = dynamic_cast<CServerInfoTable* >(p);    
    if(!pServerInfoTable)                                                        
        return -1;                                                               
                                                                                 
    int rt = pServerInfoTable->GetServerInfo(m_pMsg->m_strServerName, m_strIP, m_iServerPort);

    if(0 > rt)                                                                   
    {                                                                            
        std::cout<<"SetServerInfo db operator error!"<<std::endl;                
        return -1;                                                               
    }                                                                            
                                                                                                                            
    return 0;                                                                    
}                                 


const int& GetServerInfoWorkItem::ID() const
{
	return m_iEchoID;
}

const int& GetServerInfoWorkItem::Port() const
{
	return m_iServerPort;
}

const string& GetServerInfoWorkItem::IP() const
{
	return m_strIP;
}                                                           
