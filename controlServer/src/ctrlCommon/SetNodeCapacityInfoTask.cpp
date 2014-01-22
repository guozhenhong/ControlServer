/*
 * =====================================================================================
 *
 *       Filename:  SetNodeCapacityInfoTask.cpp
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
#include "ctrlCommon/SetNodeCapacityInfoTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/SetNodeCapInfoWorkItem.h"
#include "ctrlCommon/NodeCapacityInfoMessage.h"

SetNodeCapacityInfoTask::SetNodeCapacityInfoTask()
{

	m_pMsg = NULL;
}


SetNodeCapacityInfoTask::~SetNodeCapacityInfoTask()
{

}

int SetNodeCapacityInfoTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int SetNodeCapacityInfoTask::goNext()
{
	NodeCapacityInfoMessage *pMsg = dynamic_cast<NodeCapacityInfoMessage *> (m_pMsg);
	if(!pMsg)
		return -1;

	string nodeName = m_pAgent->GetOppositeName();

    SetNodeCapInfoWorkItem *pWI = new SetNodeCapInfoWorkItem(nodeName, pMsg);
   
    pWI->setTaskID(getID());
    g_pDispatcher->postRequest(pWI);

    return 0;
}

void SetNodeCapacityInfoTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)
{
	SetNodeCapInfoWorkItem *p = dynamic_cast<SetNodeCapInfoWorkItem *>(pWorkItem);
	// ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);
	if(p)
	{
		//ACK
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

int SetNodeCapacityInfoTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* SetNodeCapacityInfoTask::createTask()
// {
//     return TaskManager::getInstance()->create<SetNodeCapacityInfoTask>();
// }
