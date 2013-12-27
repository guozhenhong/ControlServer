#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CServerInfoTable.h"
#include "controlDB/CTableHandler.h"
#include "ctrlCommon/RegisterWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

RegisterWorkItem::RegisterWorkItem(RegisterServerMessage *p, string& ip):
     m_pMsg(p), m_bRegisterResult(false), m_strIP(ip)
{
}

RegisterWorkItem::~RegisterWorkItem()
{
}

int
RegisterWorkItem::process()
{
	if(!m_pMsg)
		return -1;

    std::cout<<"In RegisterWorkItem::process()"<<std::endl;
    std::cout<<"The Register Server Name is "<<m_pMsg->m_strServerName<<std::endl;
    std::cout<<"The Register Server IP is "<<m_strIP<<std::endl;
    std::cout<<"The Register Server port is "<<m_pMsg->m_serverPort<<std::endl;

    //register the server info into DB
    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_SERVER_INFO_TABLE);
    if(!p)
        return -1;
    CServerInfoTable *pServerInfoTable = dynamic_cast<CServerInfoTable* >(p);
    if(!pServerInfoTable)
        return -1;

    int rt = pServerInfoTable->SetServerInfo(m_pMsg->m_strServerName, m_strIP, m_pMsg->m_serverPort);
    if(0 > rt)
    {
        std::cout<<"SetServerInfo db operator error!"<<std::endl;
        return -1;
    }

    m_bRegisterResult = true;
    return 0;
}

bool RegisterWorkItem::IsRegisterSuccess() const
{
    return m_bRegisterResult;
}