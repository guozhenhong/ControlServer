/*
 * =====================================================================================
 *
 *       Filename:  RegisterServerInfoTask.h
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
#ifndef REGISTERSERVERINFOTASK_H
#define REGISTERSERVERINFOTASK_H

#include "common/comm/BaseTask.h"

class Message;
class ControlAgent;
class ThreadPoolWorkItem;

class RegisterServerInfoTask : public BaseTask
{
public:
    RegisterServerInfoTask();
    virtual ~RegisterServerInfoTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    virtual void recvWorkItem( ThreadPoolWorkItem* pWorkItem);

    int setAgent(ControlAgent * agent);

private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;

private:
    RegisterServerInfoTask(const RegisterServerInfoTask& );
    RegisterServerInfoTask& operator=(const RegisterServerInfoTask& );
};                               


#endif
