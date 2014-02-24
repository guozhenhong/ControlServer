#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CVirtualDiskInfoTable.h"
#include "ctrlCommon/GetVDCapacityInfoWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

GetVDCapacityInfoWorkItem::GetVDCapacityInfoWorkItem(uint32_t vdID) : m_iVDID(vdID), m_pCapInfo(NULL)
{

}

GetVDCapacityInfoWorkItem::~GetVDCapacityInfoWorkItem()
{
    if(m_pCapInfo)
        delete m_pCapInfo;
}

int
GetVDCapacityInfoWorkItem::process()
{
    std::cout<<"In GetVDCapacityInfoWorkItem::process()"<<std::endl;

    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_DISK_INFO_TABLE);
    if(!p)
        return -1;
    CVirtualDiskInfoTable *pVDInfoTable = dynamic_cast<CVirtualDiskInfoTable* >(p);
    if(!pVDInfoTable)
        return -1;

    m_pCapInfo = new sCapacityInfo();
    int rt = pVDInfoTable->GetVDCapacityInfo(m_iVDID, m_pCapInfo);
    if(0 > rt)
    {
        std::cout<<"GetVDCapacityInfo db operator error!"<<std::endl;
        return -1;
    }

    std::cout<<"result is "<<m_iVDID<<" "<<m_pCapInfo->m_sumCap<<" "<<m_pCapInfo->m_declaredCap<<" "<<m_pCapInfo->m_usedCap<<std::endl;

    return 0;
}

GetVDCapacityInfoACKMessage* 
GetVDCapacityInfoWorkItem::GetACKMessage()
{   
    GetVDCapacityInfoACKMessage*  pACKMsg = new GetVDCapacityInfoACKMessage();

    pACKMsg->m_iVDID = m_iVDID; 
    pACKMsg->m_decCapacity = m_pCapInfo->m_sumCap;
    pACKMsg->m_usedCapacity = m_pCapInfo->m_declaredCap;
    pACKMsg->m_realUsedCapacityByFileSys = m_pCapInfo->m_usedCap;

	return pACKMsg;
}

uint64_t GetVDCapacityInfoWorkItem::GetDeclaredCapacity()
{
    return m_pCapInfo->m_sumCap;
}

uint64_t GetVDCapacityInfoWorkItem::GetUsedCapacity()
{
    return m_pCapInfo->m_declaredCap;
}

uint64_t GetVDCapacityInfoWorkItem::GetRealUsedCapacity()
{
    return m_pCapInfo->m_usedCap;
}
