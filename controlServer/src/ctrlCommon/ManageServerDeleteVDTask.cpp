/*
 * =====================================================================================
 *
 *       Filename:  ManageServerDeleteVDTask.cpp
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
#include "ctrlCommon/ManageServerDeleteVDTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/ManageServerDeleteVDMessage.h"
#include "ctrlCommon/ManageServerDeleteOneVDTask.h"

ManageServerDeleteVDTask::ManageServerDeleteVDTask()
{
	m_pMsg = NULL;

	m_state = DO_DEL_TASK;
}


ManageServerDeleteVDTask::~ManageServerDeleteVDTask()
{

}

int ManageServerDeleteVDTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int ManageServerDeleteVDTask::goNext()
{
	switch(m_state)
	{
		case DO_DEL_TASK:
		{
			ManageServerDeleteVDMessage *pDeleteMsg = dynamic_cast<ManageServerDeleteVDMessage *> (m_pMsg);
			if(!pDeleteMsg)
				return -1;

			m_pOneTask = TaskManager::getInstance()->create<ManageServerDeleteOneVDTask>();
		    if(!m_pOneTask)
				return -1;

			m_pOneTask->setSuperTaskID(getID());	
			m_pOneTask->recvMsg(pDeleteMsg);
			
			m_pOneTask->goNext();

		    m_state = DO_DEL_FINISHED;
		    //goNext();
		    break;
		}
		case DO_DEL_FINISHED:
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

int ManageServerDeleteVDTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* ManageServerDeleteVDTask::createTask()
// {
//     return TaskManager::getInstance()->create<ManageServerDeleteVDTask>();
// }
