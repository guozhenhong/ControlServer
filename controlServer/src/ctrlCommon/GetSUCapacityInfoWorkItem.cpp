#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CStorageUnitInfoTable.h"
#include "ctrlCommon/GetSUCapacityInfoWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

GetSUCapacityInfoWorkItem::GetSUCapacityInfoWorkItem(GetSUCapacityInfoMessage *p):
     m_pMsg(p), m_pSUCapInfo(NULL),  m_strNodeName(p->m_strNodeName)
{
   
}

GetSUCapacityInfoWorkItem::GetSUCapacityInfoWorkItem(string& strNodeName) : m_pMsg(NULL), m_pSUCapInfo(NULL), m_strNodeName(strNodeName)
{

}

GetSUCapacityInfoWorkItem::~GetSUCapacityInfoWorkItem()
{
    if(m_pSUCapInfo)
        delete m_pSUCapInfo;
}

int
GetSUCapacityInfoWorkItem::process()
{
    std::cout<<"In GetSUCapacityInfoWorkItem::process()"<<std::endl;
    std::cout<<"The want SU Cap Name is "<<m_strNodeName<<std::endl;
    // std::cout<<"The echo ID is "<<m_pMsg->m_iEchoID<<std::endl;

    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_STORAGE_INFO_TABLE);
    if(!p)
        return -1;
    CStorageUnitInfoTable *pSUInfoTable = dynamic_cast<CStorageUnitInfoTable* >(p);
    if(!pSUInfoTable)
        return -1;

    m_pSUCapInfo = new sCapacityInfo;
    int rt = pSUInfoTable->GetNodeCapacityInfo(m_strNodeName, m_pSUCapInfo);
    if(0 > rt)
    {
        std::cout<<"GetNodeCapacityInfo db operator error!"<<std::endl;
        return -1;
    }

    std::cout<<"result is "<<m_pSUCapInfo->m_sumCap<<" "<<m_pSUCapInfo->m_declaredCap<<" "<<m_pSUCapInfo->m_usedCap<<std::endl;
    
    return 0;
}

uint64_t GetSUCapacityInfoWorkItem::GetSumCapacity()
{
    return m_pSUCapInfo->m_sumCap;
}

uint64_t GetSUCapacityInfoWorkItem::GetDeclareCapacity()
{
    return m_pSUCapInfo->m_declaredCap;
}

uint64_t GetSUCapacityInfoWorkItem::GetUsedCapacity()
{
    return m_pSUCapInfo->m_usedCap;
}

GetSUCapacityInfoACKMessage* GetSUCapacityInfoWorkItem::GetACKMsg()
{
    return (new GetSUCapacityInfoACKMessage(m_pMsg->m_iEchoID, m_pSUCapInfo->m_sumCap, m_pSUCapInfo->m_declaredCap, m_pSUCapInfo->m_usedCap));
}