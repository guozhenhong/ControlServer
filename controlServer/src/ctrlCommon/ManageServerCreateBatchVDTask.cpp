/*
 * =====================================================================================
 *
 *       Filename:  ManageServerCreateBatchVDTask.cpp
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
#include "ctrlCommon/ManageServerCreateBatchVDTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/ManageServerCreateBatchVDMessage.h"
#include "ctrlCommon/ManageServerCreateOneVDTask.h"

ManageServerCreateBatchVDTask::ManageServerCreateBatchVDTask()
{
	m_pMsg = NULL;
	m_pAgent = NULL;
	m_pACKMsgList = NULL;
	m_pOneTask = NULL;
	m_iCount = 0;
	m_state = BAT_DO_TASK;
}


ManageServerCreateBatchVDTask::~ManageServerCreateBatchVDTask()
{

}

int ManageServerCreateBatchVDTask::recvMsg(Message *pMsg)
{
	m_pMsg = dynamic_cast<ManageServerCreateBatchVDMessage *> (pMsg);
	if(!m_pMsg)
		return -1;
	m_iReqNum = m_pMsg->m_iDiskNum;

	// m_pACKMsg = new ManageServerCreateBatchVDACKMessage();
    m_pACKMsgList = new ManageServerCreateVDACKMessage[m_iReqNum];

	m_pOneTask = TaskManager::getInstance()->create<ManageServerCreateOneVDTask>(); 
	m_state = BAT_DO_TASK;
	
	return 0;
}

int ManageServerCreateBatchVDTask::goNext()
{
	switch(m_state)
	{
		case BAT_DO_TASK:
		{
			if(!m_pOneTask)
				return -1;

			m_pOneTask->setSuperTaskID(getID());	
			m_pOneTask->recvMsg(&(m_pMsg->m_diskInfoList[m_iCount]));
			
			m_pOneTask->goNext();

			m_state = BAT_DO_ONE_FINISHED;

			break;
		}
		case BAT_DO_ONE_FINISHED:
		{	
			ManageServerCreateVDACKMessage *pACKMsg = m_pOneTask->getACKMessage();
			memcpy(m_pACKMsgList + m_iCount, pACKMsg, sizeof(ManageServerCreateVDACKMessage));

			delete pACKMsg;

			m_iCount++;
			if(m_iCount == m_iReqNum)
			{
				m_state = BAT_DO_FINISHED;
			}
			else
				m_state = BAT_DO_TASK;

			goNext();
			break;
		}
		case BAT_DO_FINISHED:
		{
			m_pAgent->SendMsg(new ManageServerCreateBatchVDACKMessage(m_iReqNum, m_pACKMsgList));
			TaskManager::getInstance()->recycle(m_pOneTask->getID());
			TaskManager::getInstance()->recycle(getID());

			if(m_pMsg)
				delete m_pMsg;

			break;
		}
	}
	
    return 0;
}

int ManageServerCreateBatchVDTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* ManageServerCreateBatchVDTask::createTask()
// {
//     return TaskManager::getInstance()->create<ManageServerCreateBatchVDTask>();
// }
