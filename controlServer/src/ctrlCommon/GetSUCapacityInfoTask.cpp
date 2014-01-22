/*
 * =====================================================================================
 *
 *       Filename:  GetSUCapacityInfoTask.cpp
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
#include "ctrlCommon/GetSUCapacityInfoTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/GetSUCapacityInfoWorkItem.h"
#include "ctrlCommon/GetSUCapacityInfoMessage.h"

GetSUCapacityInfoTask::GetSUCapacityInfoTask()
{

	m_pMsg = NULL;
}


GetSUCapacityInfoTask::~GetSUCapacityInfoTask()
{

}

int GetSUCapacityInfoTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int GetSUCapacityInfoTask::goNext()
{
	GetSUCapacityInfoMessage *pMsg = dynamic_cast<GetSUCapacityInfoMessage *> (m_pMsg);
	if(!pMsg)
		return -1;

    GetSUCapacityInfoWorkItem *pWI = new GetSUCapacityInfoWorkItem(pMsg);
   
    pWI->setTaskID(getID());
    g_pDispatcher->postRequest(pWI);

    return 0;
}

void GetSUCapacityInfoTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)
{
	GetSUCapacityInfoWorkItem *p = dynamic_cast<GetSUCapacityInfoWorkItem *>(pWorkItem);
	// ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);
	if(p)
	{
		GetSUCapacityInfoACKMessage *pACKMsg = p->GetACKMsg();
		if(pACKMsg)
			m_pAgent->SendMsg(pACKMsg);	
	}
	else
	{
		//error
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

int GetSUCapacityInfoTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* GetSUCapacityInfoTask::createTask()
// {
//     return TaskManager::getInstance()->create<GetSUCapacityInfoTask>();
// }
