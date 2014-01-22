/*
 * =====================================================================================
 *
 *       Filename:  ManageServerDeleteBatchVDTask.cpp
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
#include "ctrlCommon/ManageServerDeleteBatchVDTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/ManageServerDeleteBatchVDMessage.h"
#include "ctrlCommon/ManageServerDeleteOneVDTask.h"

ManageServerDeleteBatchVDTask::ManageServerDeleteBatchVDTask()
{
	m_pMsg = NULL;
	m_pAgent = NULL;
	m_pACKMsgList = NULL;
	m_pOneTask = NULL;
	m_iCount = 0;
	m_state = BAT_DO_DEL_TASK;
	m_pOneTaskMessage = NULL;
}


ManageServerDeleteBatchVDTask::~ManageServerDeleteBatchVDTask()
{

}

int ManageServerDeleteBatchVDTask::recvMsg(Message *pMsg)
{
	m_pMsg = dynamic_cast<ManageServerDeleteBatchVDMessage *> (pMsg);
	if(!m_pMsg)
		return -1;
	m_iReqNum = m_pMsg->m_iDiskNum;

	// m_pACKMsg = new ManageServerDeleteBatchVDACKMessage();
    m_pACKMsgList = new ManageServerDeleteVDACKMessage[m_iReqNum];

	m_pOneTask = TaskManager::getInstance()->create<ManageServerDeleteOneVDTask>(); 
	m_state = BAT_DO_DEL_TASK;
	
	return 0;
}

int ManageServerDeleteBatchVDTask::goNext()
{
	switch(m_state)
	{
		case BAT_DO_DEL_TASK:
		{
			if(!m_pOneTask)
				return -1;

			m_pOneTask->setSuperTaskID(getID());
			m_pOneTaskMessage = new ManageServerDeleteVDMessage(m_pMsg->m_pDiskIDList[m_iCount]);	
			m_pOneTask->recvMsg(m_pOneTaskMessage);
			
			m_pOneTask->goNext();

			m_state = BAT_DO_DEL_ONE_FINISHED;

			break;
		}
		case BAT_DO_DEL_ONE_FINISHED:
		{	
			ManageServerDeleteVDACKMessage *pACKMsg = m_pOneTask->getACKMessage();
			memcpy(m_pACKMsgList + m_iCount, pACKMsg, sizeof(ManageServerDeleteVDACKMessage));

			delete pACKMsg;
			delete m_pOneTaskMessage;
			m_pOneTaskMessage = NULL;

			m_iCount++;
			if(m_iCount == m_iReqNum)
			{
				m_state = BAT_DO_DEL_FINISHED;
			}
			else
				m_state = BAT_DO_DEL_TASK;

			goNext();
			break;
		}
		case BAT_DO_DEL_FINISHED:
		{
			m_pAgent->SendMsg(new ManageServerDeleteBatchVDACKMessage(m_iReqNum, m_pACKMsgList));
			TaskManager::getInstance()->recycle(m_pOneTask->getID());
			TaskManager::getInstance()->recycle(getID());

			if(m_pMsg)
				delete m_pMsg;

			break;
		}
	}
	
    return 0;
}

int ManageServerDeleteBatchVDTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* ManageServerDeleteBatchVDTask::createTask()
// {
//     return TaskManager::getInstance()->create<ManageServerDeleteBatchVDTask>();
// }
