#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CVirtualDiskInfoTable.h"
#include "controlDB/CTableHandler.h"
#include "ctrlCommon/GetVDCapacityInfoWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

GetVDCapacityInfoWorkItem::GetVDCapacityInfoWorkItem(GetVDCapacityInfoMessage *p) : m_pMsg(p), m_pACKMsg(NULL)
{

}

GetVDCapacityInfoWorkItem::~GetVDCapacityInfoWorkItem()
{
}

int
GetVDCapacityInfoWorkItem::process()
{
	if(!m_pMsg)
		return -1;

    std::cout<<"In GetVDCapacityInfoWorkItem::process()"<<std::endl;

    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_DISK_INFO_TABLE);
    if(!p)
        return -1;
    CVirtualDiskInfoTable *pVDInfoTable = dynamic_cast<CVirtualDiskInfoTable* >(p);
    if(!pVDInfoTable)
        return -1;

    sCapacityInfo *pInfo = new sCapacityInfo();
    int rt = pVDInfoTable->GetVDCapacityInfo(m_pMsg->m_iVDID, pInfo);
    if(0 > rt)
    {
        std::cout<<"GetVDCapacityInfo db operator error!"<<std::endl;
        return -1;
    }

	m_pACKMsg = new GetVDCapacityInfoACKMessage();

    m_pACKMsg->m_iVDID = m_pMsg->m_iVDID; 
    m_pACKMsg->m_decCapacity = pInfo->m_sumCap;
    m_pACKMsg->m_usedCapacity = pInfo->m_declaredCap;
    m_pACKMsg->m_realUsedCapacityByFileSys = pInfo->m_usedCap;

    std::cout<<"result is "<<m_pMsg->m_iVDID<<" "<<pInfo->m_sumCap<<" "<<pInfo->m_declaredCap<<" "<<pInfo->m_usedCap<<std::endl;

    delete pInfo;
	 
    return 0;
}

GetVDCapacityInfoACKMessage* 
GetVDCapacityInfoWorkItem::GetACKMessage()
{
	return m_pACKMsg;
}
