#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CStorageUnitInfoTable.h"
#include "controlDB/CTableHandler.h"
#include "ctrlCommon/GetSysCapInfoWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

GetSysCapInfoWorkItem::GetSysCapInfoWorkItem(GetSysCapMessage *p) : m_pMsg(p), m_pACKMsg(NULL)
{

}

GetSysCapInfoWorkItem::~GetSysCapInfoWorkItem()
{
}

int
GetSysCapInfoWorkItem::process()
{
	if(!m_pMsg)
		return -1;

    std::cout<<"In GetSysCapInfoWorkItem::process()"<<std::endl;
    
    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_STORAGE_INFO_TABLE);
    if(!p)
        return -1;
    CStorageUnitInfoTable *pSUInfoTable = dynamic_cast<CStorageUnitInfoTable* >(p);
    if(!pSUInfoTable)
        return -1;

    sCapacityInfo *pInfo = new sCapacityInfo();
    int rt = pSUInfoTable->GetSysCapacityInfo(pInfo);
    if(0 > rt)
    {
        std::cout<<"GetSysCapacityInfo db operator error!"<<std::endl;
        return -1;
    }

	m_pACKMsg = new GetSysCapACKMessage();
	m_pACKMsg->m_SumCap = pInfo->m_sumCap;
	m_pACKMsg->m_DecCap = pInfo->m_declaredCap;
	m_pACKMsg->m_UsedCap = pInfo->m_usedCap;

    delete pInfo;
	 
    return 0;
}

GetSysCapACKMessage* 
GetSysCapInfoWorkItem::GetACKMessage()
{
	return m_pACKMsg;
}
