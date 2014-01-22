#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CStorageUnitInfoTable.h"
#include "controlDB/CTableHandler.h"
#include "ctrlCommon/GetSUCapacityInfoWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

GetSUCapacityInfoWorkItem::GetSUCapacityInfoWorkItem(GetSUCapacityInfoMessage *p):
     m_pMsg(p), m_pACKMsg(NULL)
{
}

GetSUCapacityInfoWorkItem::~GetSUCapacityInfoWorkItem()
{
}

int
GetSUCapacityInfoWorkItem::process()
{
	if(!m_pMsg)
		return -1;

    std::cout<<"In GetSUCapacityInfoWorkItem::process()"<<std::endl;
    std::cout<<"The want SU Cap Name is "<<m_pMsg->m_strNodeName<<std::endl;
    std::cout<<"The echo ID is "<<m_pMsg->m_iEchoID<<std::endl;

    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_STORAGE_INFO_TABLE);
    if(!p)
        return -1;
    CStorageUnitInfoTable *pSUInfoTable = dynamic_cast<CStorageUnitInfoTable* >(p);
    if(!pSUInfoTable)
        return -1;

    sCapacityInfo *pResultCap = new sCapacityInfo;
    int rt = pSUInfoTable->GetNodeCapacityInfo(m_pMsg->m_strNodeName, pResultCap);
    if(0 > rt)
    {
        std::cout<<"GetNodeCapacityInfo db operator error!"<<std::endl;
        return -1;
    }

    std::cout<<"result is "<<m_pMsg->m_iEchoID<<" "<<pResultCap->m_sumCap<<" "<<pResultCap->m_declaredCap<<" "<<pResultCap->m_usedCap<<std::endl;
    
    m_pACKMsg = new GetSUCapacityInfoACKMessage(m_pMsg->m_iEchoID, pResultCap->m_sumCap, pResultCap->m_declaredCap, pResultCap->m_usedCap);

    delete pResultCap;
    
    return 0;
}

GetSUCapacityInfoACKMessage* GetSUCapacityInfoWorkItem::GetACKMsg()
{
    return m_pACKMsg;
}