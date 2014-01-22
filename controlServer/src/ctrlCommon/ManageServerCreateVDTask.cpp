/*
 * =====================================================================================
 *
 *       Filename:  ManageServerCreateVDTask.cpp
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
#include <iostream>
#include "common/sys/ThreadPoolDispatcher.h"
#include "common/comm/BaseReq.h"
#include "common/comm/Agent.h"
#include "common/comm/SocketAddress.h"
#include "ctrlCommon/ManageServerCreateVDTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/ManageServerCreateVDMessage.h"
#include "ctrlCommon/ManageServerCreateOneVDTask.h"

ManageServerCreateVDTask::ManageServerCreateVDTask()
{
	m_pMsg = NULL;

	m_state = DO_TASK;
}


ManageServerCreateVDTask::~ManageServerCreateVDTask()
{

}

int ManageServerCreateVDTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int ManageServerCreateVDTask::goNext()
{
	switch(m_state)
	{
		case DO_TASK:
		{
			ManageServerCreateVDMessage *pCreatMsg = dynamic_cast<ManageServerCreateVDMessage *> (m_pMsg);
			if(!pCreatMsg)
				return -1;

			m_pOneTask = TaskManager::getInstance()->create<ManageServerCreateOneVDTask>();
		    if(!m_pOneTask)
				return -1;

			m_pOneTask->setSuperTaskID(getID());	
			m_pOneTask->recvMsg(pCreatMsg);
			
			m_pOneTask->goNext();

		    m_state = DO_FINISHED;
		    //goNext();
		    break;
		}
		case DO_FINISHED:
		{
			m_pAgent->SendMsg(m_pOneTask->getACKMessage());
			TaskManager::getInstance()->recycle(getID());
			TaskManager::getInstance()->recycle(m_pOneTask->getID());

			if(m_pMsg)
			{
				delete m_pMsg;
				m_pMsg = NULL;
			}
			break;
		}

	}
	
    return 0;
}

int ManageServerCreateVDTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* ManageServerCreateVDTask::createTask()
// {
//     return TaskManager::getInstance()->create<ManageServerCreateVDTask>();
// }
