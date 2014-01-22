/*
 * =====================================================================================
 *
 *       Filename:  GetSysCapacityInfoTask.cpp
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
#include "ctrlCommon/GetSysCapacityInfoTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/GetSysCapInfoWorkItem.h"
#include "ctrlCommon/GetSysCapMessage.h"

GetSysCapacityInfoTask::GetSysCapacityInfoTask()
{

	m_pMsg = NULL;
}


GetSysCapacityInfoTask::~GetSysCapacityInfoTask()
{

}

int GetSysCapacityInfoTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int GetSysCapacityInfoTask::goNext()
{
	GetSysCapMessage *pMsg = dynamic_cast<GetSysCapMessage *> (m_pMsg);
	if(!pMsg)
		return -1;

    GetSysCapInfoWorkItem *pWI = new GetSysCapInfoWorkItem(pMsg);
   
    pWI->setTaskID(getID());
    g_pDispatcher->postRequest(pWI);

    return 0;
}

void GetSysCapacityInfoTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)
{
	GetSysCapInfoWorkItem *p = dynamic_cast<GetSysCapInfoWorkItem *>(pWorkItem);
	// ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);
	if(p)
	{
		GetSysCapACKMessage* pAckMsg = p->GetACKMessage();
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

int GetSysCapacityInfoTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* GetSysCapacityInfoTask::createTask()
// {
//     return TaskManager::getInstance()->create<GetSysCapacityInfoTask>();
// }
