/*
 * =====================================================================================
 *
 *       Filename:  SetNodeCapacityInfoTask.h
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
#ifndef SETNODECAPACITYINFOTASK_H
#define SETNODECAPACITYINFOTASK_H

#include "common/comm/BaseTask.h"
#include "common/comm/TaskManager.h"

class Message;
class ControlAgent;
class ThreadPoolWorkItem;

class SetNodeCapacityInfoTask : public BaseTask
{
public:
    SetNodeCapacityInfoTask();
    virtual ~SetNodeCapacityInfoTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    virtual void recvWorkItem( ThreadPoolWorkItem* pWorkItem);

    int setAgent(ControlAgent * agent);

    // static BaseTask* createTask();
private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;

private:
    SetNodeCapacityInfoTask(const SetNodeCapacityInfoTask& );
    SetNodeCapacityInfoTask& operator=(const SetNodeCapacityInfoTask& );
};                               


#endif
