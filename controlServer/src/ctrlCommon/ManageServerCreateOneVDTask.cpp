/*
 * =====================================================================================
 *
 *       Filename:  ManageServerCreateOneVDTask.cpp
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
#include "ctrlCommon/ManageServerCreateOneVDTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/ManageServerCreateVDMessage.h"
#include "ctrlCommon/GetVDLocateNodeInfoMessage.h"
#include "ctrlCommon/VirtualDiskFormatMessage.h"
#include "ctrlCommon/SetVDStatusMessage.h"
#include "ctrlCommon/SetVDInfoWorkItem.h"

ManageServerCreateOneVDTask::ManageServerCreateOneVDTask()
{
	m_pAgent = NULL;
	m_pMsg = NULL;
	m_pSetStatusMsg = NULL;
	m_pACKMsg = NULL;

	m_iRes = 0;
	m_state = DO_INIT_DB;
}


ManageServerCreateOneVDTask::~ManageServerCreateOneVDTask()
{

}

int ManageServerCreateOneVDTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int ManageServerCreateOneVDTask::goNext()
{
	switch(m_state)
	{
		case DO_INIT_DB:
		{
			ManageServerCreateVDMessage *pCreatMsg = dynamic_cast<ManageServerCreateVDMessage *> (m_pMsg);
			if(!pCreatMsg)
				return -1;

		    SetVDInfoWorkItem *pWI = new SetVDInfoWorkItem(pCreatMsg);
		   
		    pWI->setTaskID(getID());
		    g_pDispatcher->postRequest(pWI);

		    m_iEchoID = pCreatMsg->m_iEchoID;
		    m_iCap = pCreatMsg->m_VDCapacity;
		    m_iFSType = pCreatMsg->m_FileSysType;

		    m_state = DO_GET_LOCATION;
		    //goNext();
		    break;
		}
		case DO_GET_LOCATION:
		{
			//get Agent* for location server.and send msg to it to get a node info where to locate the vd
			//and the msg should have a word to sign the task ID
			// cause of the heart connection, it is supporst to be a connection with every server
			// but if the connect is break? that means the heart is breaK, so i must new one connect to locateserver!!!!
			m_pAgent->SendMsgToServer(LOCATE_SERVER_NAME, new GetVDLocateNodeInfoMessage(getID(), m_iVDID));

			m_state = DO_FORMAT_VD;
			break;
		}
		case DO_FORMAT_VD:
		{
			if(!m_pMsg)
				return -1;
			GetVDLocateNodeInfoACKMessage *pMsg = dynamic_cast<GetVDLocateNodeInfoACKMessage *> (m_pMsg);
			if(!pMsg)
				return -1;

			m_pAgent->SendMsgToServer(pMsg->m_strLocateNodeName, new VirtualDiskFormatMessage(getID(), m_iVDID, m_iCap, m_iFSType/*, fsType*/));
			m_state = DO_SET_DB;

			delete m_pMsg;
			m_pMsg = NULL;

			break;
		}
		case DO_SET_DB:
		{
			if(!m_pMsg)
				return -1;
			VirtualDiskFormatACKMessage *pMsg = dynamic_cast<VirtualDiskFormatACKMessage *> (m_pMsg);
			if(!pMsg)
				return -1;

			m_iRes = pMsg->m_iRes;

			delete m_pMsg;
			m_pMsg = NULL;

			if(m_iRes == 0)
			{
				std::cout<<"In ManageServerCreateOneVDTask::goNext(), format ACK recv, but res is failed!"<<std::endl;
				m_state = FINISHED;
				goNext();
			}
			m_pSetStatusMsg = new SetVDStatusMessage(m_iVDID, FORMAT_END);
			SetVDInfoWorkItem *pWI = new SetVDInfoWorkItem(m_pSetStatusMsg);
		   
		    pWI->setTaskID(getID());
		    g_pDispatcher->postRequest(pWI);

		    m_state = FINISHED;
			break;
		}
		case FINISHED:
		{
			m_pACKMsg = new ManageServerCreateVDACKMessage(m_iEchoID, m_iRes, m_iVDID);
			m_state = DO_INIT_DB;
			(TaskManager::getInstance()->get(m_iSuperTaskID))->goNext();

			break;
		}

	}
	
    return 0;
}

void ManageServerCreateOneVDTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)
{
	SetVDInfoWorkItem *p = dynamic_cast<SetVDInfoWorkItem *>(pWorkItem);

	// ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);
	if(p)
	{
		m_iRes = p->getRes();
		if(DO_GET_LOCATION == m_state)
		{
			m_iVDID = p->getVDID();
			if(m_iRes == 0)
				m_state = FINISHED;
		}
		else if(FINISHED == m_state)
		{
			
		}
		if(m_pSetStatusMsg)
		{
			delete m_pSetStatusMsg;
			m_pSetStatusMsg = NULL;
		}
		
		if (pWorkItem != NULL)
	    {
	        delete pWorkItem;
	        pWorkItem = NULL;
	    }		
	    
		goNext();
	}
	else
	{
		//error
	}

	
	return;
}

int ManageServerCreateOneVDTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}

// BaseTask* ManageServerCreateOneVDTask::createTask()
// {
//     return TaskManager::getInstance()->create<ManageServerCreateOneVDTask>();
// }

int ManageServerCreateOneVDTask::setSuperTaskID(uint64_t ID)
{
	m_iSuperTaskID = ID;
	return 0;
}

ManageServerCreateVDACKMessage* ManageServerCreateOneVDTask::getACKMessage()
{
	return m_pACKMsg;
}