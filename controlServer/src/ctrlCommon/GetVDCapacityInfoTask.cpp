/*
 * =====================================================================================
 *
 *       Filename:  GetVDCapacityInfoTask.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/16/13 09:59:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#include "common/sys/ThreadPoolDispatcher.h"
#include "common/comm/BaseReq.h"
#include "common/comm/Agent.h"
#include "common/comm/SocketAddress.h"
#include "ctrlCommon/GetVDCapacityInfoTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/GetVDCapacityInfoWorkItem.h"
#include "ctrlCommon/GetVDCapacityInfoMessage.h"

GetVDCapacityInfoTask::GetVDCapacityInfoTask()
{
	m_pMsg = NULL;
}


GetVDCapacityInfoTask::~GetVDCapacityInfoTask()
{

}

int GetVDCapacityInfoTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int GetVDCapacityInfoTask::goNext()
{
	GetVDCapacityInfoMessage *pMsg = dynamic_cast<GetVDCapacityInfoMessage *> (m_pMsg);
	if(!pMsg)
		return -1;

    GetVDCapacityInfoWorkItem *pWI = new GetVDCapacityInfoWorkItem(pMsg->m_iVDID);
   
    pWI->setTaskID(getID());
    g_pDispatcher->postRequest(pWI);

    return 0;
}

void GetVDCapacityInfoTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)
{
	GetVDCapacityInfoWorkItem *p = dynamic_cast<GetVDCapacityInfoWorkItem *>(pWorkItem);
	// ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);
	if(p)
	{
		GetVDCapacityInfoACKMessage* pAckMsg = p->GetACKMessage();
		if(pAckMsg != NULL)
			m_pAgent->SendMsg(pAckMsg);
	}
	else
	{
		//error operator.add next time
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

    TaskManager::getInstance()->recycle(getID());  

	return;
}

int GetVDCapacityInfoTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* GetVDCapacityInfoTask::createTask()
// {
//     return TaskManager::getInstance()->create<GetVDCapacityInfoTask>();
// }
