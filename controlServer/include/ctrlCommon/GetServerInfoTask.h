/*
 * =====================================================================================
 *
 *       Filename:  GetServerInfoTask.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/13 16:27:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#ifndef GETSERVERINFOTASK_H
#define GETSERVERINFOTASK_H

#include "common/comm/BaseTask.h"
#include "common/comm/TaskManager.h"

class Message;
class ControlAgent;
class ThreadPoolWorkItem;

class GetServerInfoTask : public BaseTask
{
public:
    GetServerInfoTask();
    virtual ~GetServerInfoTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    virtual void recvWorkItem( ThreadPoolWorkItem* pWorkItem);

    int setAgent(ControlAgent * agent);

    // static BaseTask* createTask();
private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;

private:
    GetServerInfoTask(const GetServerInfoTask& );
    GetServerInfoTask& operator=(const GetServerInfoTask& );
};                               


#endif
