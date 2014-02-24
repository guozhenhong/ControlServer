#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CVirtualDiskInfoTable.h"
#include "controlDB/CTableHandler.h"
#include "ctrlCommon/SetVDInfoWorkItem.h"
#include "common/comm/Error.h"
#include "common/log/log.h"

SetVDInfoWorkItem::SetVDInfoWorkItem(ManageServerCreateVDMessage *p):
     m_pMsg(p), m_iOPRes(0), m_iCmd(CREATE_VD), m_iValue(0)
{

}

SetVDInfoWorkItem::SetVDInfoWorkItem(SetVDStatusMessage *p):
    m_pMsg(p), m_iOPRes(0), m_iCmd(CH_STATUS), m_iValue(0)
{

}

SetVDInfoWorkItem::SetVDInfoWorkItem(uint32_t cmd, uint32_t vdID, uint64_t value)
{
    m_pMsg = NULL;
    m_iVDID = vdID;
    m_iCmd = cmd;
    m_iValue = value;
    m_iOPRes = 0;
}

// SetVDInfoWorkItem::SetVDInfoWorkItem(uint32_t vdID, uint32_t cmd, const string& value)
// {
//     m_pMsg = NULL;
//     m_iVDID = vdID;
//     m_iCmd = cmd;
//     m_strValue = value;
//     m_iOPRes = 0;
// }

SetVDInfoWorkItem::~SetVDInfoWorkItem()
{
}

int
SetVDInfoWorkItem::process()
{
	CTableHandler* p = CCtrlDBHandler::getInstance()->GetTableHandler(DEFAULT_DISK_INFO_TABLE);
    if(!p)
        return -1;
    CVirtualDiskInfoTable *pVDInfoTable = dynamic_cast<CVirtualDiskInfoTable* >(p);
    if(!pVDInfoTable)
        return -1;

    switch (m_iCmd)
    {
        case CH_STATUS:
        {
            if(m_pMsg == NULL)
            {
                return -1;
            }
            SetVDStatusMessage *pMsg = dynamic_cast<SetVDStatusMessage *> (m_pMsg);
            if(pMsg)
            {
                if(pMsg->m_strStatus == DELETE_END)
                {
                    int rt = pVDInfoTable->DeleteVirtualDiskInfo(pMsg->m_iVDID);
                    if(0 > rt)
                    {
                        std::cout<<"SetVDInfoWorkItem db operator error!"<<std::endl;
                        return -1;
                    }
                    std::cout<<"In SetVDInfoWorkItem::process()"<<std::endl;
                    std::cout<<"The Delete VD Success! "<<std::endl;   
                    break;
                }
                else
                {
                    int rt = pVDInfoTable->SetVDStatus(pMsg->m_iVDID, pMsg->m_strStatus);
                    if(0 > rt)
                    {
                        std::cout<<"SetVDInfoWorkItem db operator error!"<<std::endl;
                        return -1;
                    }
                    std::cout<<"In SetVDInfoWorkItem::process()"<<std::endl;
                    std::cout<<"The CH_STATUS Success! "<<std::endl;   
                }
                
                m_iOPRes = 1;
            }

            break;
        }
        case CREATE_VD:
        {
            if(m_pMsg == NULL)
            {
                return -1;
            }
            ManageServerCreateVDMessage *pMsg = dynamic_cast<ManageServerCreateVDMessage *> (m_pMsg);
            if(!pMsg)
                break;

            int rt = pVDInfoTable->CreateVirtualDiskInfo(new sCapacityInfo(pMsg->m_VDCapacity, 0, 0), pMsg->m_FileSysType, FORMAT_ING, m_iVDID);
            if(0 > rt)
            {
                std::cout<<"SetVDInfoWorkItem db operator error!"<<std::endl;
                return -1;
            }
            m_iOPRes = 1;

            std::cout<<"In SetVDInfoWorkItem::process()"<<std::endl;
            std::cout<<"The VD ID is "<<m_iVDID<<std::endl;
            std::cout<<"The vd sum cap is "<<pMsg->m_VDCapacity<<std::endl;
            std::cout<<"The vd fs type is "<<pMsg->m_FileSysType<<std::endl; 

            break;
        }
        case CH_DEC_CAP:
        {
            int rt = pVDInfoTable->SetVDDeclaredCapacity(m_iVDID, m_iValue);
            if(0 > rt)
            {
                std::cout<<"SetVDInfoWorkItem db operator error!"<<std::endl;
                return -1;
            }
            std::cout<<"In SetVDInfoWorkItem::process()"<<std::endl;
            std::cout<<"The CH_DEC_CAP Success! "<<std::endl; 

            m_iOPRes = 1;

            break;  
        }
    }

    //register the server info into DB
    return 0;
}

uint32_t SetVDInfoWorkItem::getVDID()
{
    return m_iVDID;
}

uint32_t SetVDInfoWorkItem::getRes()
{
    return m_iOPRes;
}



