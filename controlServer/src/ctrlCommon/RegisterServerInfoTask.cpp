/*
 * =====================================================================================
 *
 *       Filename:  RegisterServerInfoTask.cpp
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
#include "ctrlCommon/RegisterServerInfoTask.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/RegisterWorkItem.h"
#include "ctrlCommon/RegisterServerMessage.h"

RegisterServerInfoTask::RegisterServerInfoTask()
{

	m_pMsg = NULL;
}


RegisterServerInfoTask::~RegisterServerInfoTask()
{

}

int RegisterServerInfoTask::recvMsg(Message *pMsg)
{
	m_pMsg = pMsg;

	return 0;
}

int RegisterServerInfoTask::goNext()
{
	RegisterServerMessage *pMsg = dynamic_cast<RegisterServerMessage *> (m_pMsg);
	if(!pMsg)
		return -1;

	string strIP = (m_pAgent->GetOppositeAddr()).getIP();
    m_pAgent->SetOppositeName(pMsg->m_strServerName);

    RegisterWorkItem *pWI = new RegisterWorkItem(pMsg, strIP);
   
    pWI->setTaskID(getID());
    g_pDispatcher->postRequest(pWI);

    return 0;
}

void RegisterServerInfoTask::recvWorkItem( ThreadPoolWorkItem* pWorkItem)
{
	RegisterWorkItem *p = dynamic_cast<RegisterWorkItem *>(pWorkItem);
	// ControlAgent *pAgent = dynamic_cast<ControlAgent *>(m_pAgent);
	if(p)
	{
		m_pAgent->SendMsg(new RegisterServerACKMSG(p->IsRegisterSuccess()));
	}
	else
	{
		m_pAgent->SendMsg(new RegisterServerACKMSG(false));
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

	return;
}

int RegisterServerInfoTask::setAgent(ControlAgent *agent)
{
    m_pAgent = agent;

    return 0;
}
