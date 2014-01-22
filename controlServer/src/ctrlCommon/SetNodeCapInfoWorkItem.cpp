#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CStorageUnitInfoTable.h"
#include "controlDB/CTableHandler.h"
#include "ctrlCommon/SetNodeCapInfoWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

SetNodeCapInfoWorkItem::SetNodeCapInfoWorkItem(string& name, NodeCapacityInfoMessage *p):
     m_pMsg(p), m_strNodeName(name)
{
}

SetNodeCapInfoWorkItem::~SetNodeCapInfoWorkItem()
{
}

int
SetNodeCapInfoWorkItem::process()
{
	if(!m_pMsg)
		return -1;

    std::cout<<"In SetNodeCapInfoWorkItem::process()"<<std::endl;
    std::cout<<"The Node Name is "<<m_strNodeName<<std::endl;
    std::cout<<"The Node sum cap is "<<m_pMsg->m_SumCap<<std::endl;
    std::cout<<"The Node dec cap is "<<m_pMsg->m_DecCap<<std::endl;
    std::cout<<"The Node used cap is "<<m_pMsg->m_UsedCap<<std::endl;

    //register the server info into DB
    CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_STORAGE_INFO_TABLE);
    if(!p)
        return -1;
    CStorageUnitInfoTable *pSUInfoTable = dynamic_cast<CStorageUnitInfoTable* >(p);
    if(!pSUInfoTable)
        return -1;

    int rt = pSUInfoTable->SetNodeCapacityInfo(m_strNodeName, new sCapacityInfo(m_pMsg->m_SumCap, m_pMsg->m_DecCap, m_pMsg->m_UsedCap));
    if(0 > rt)
    {
        std::cout<<"SetNodeCapacityInfo db operator error!"<<std::endl;
        return -1;
    }

    return 0;
}

