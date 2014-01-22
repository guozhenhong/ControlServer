/*
 * =====================================================================================
 *
 *       Filename:  RecvVDFormatResTask.h
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
#ifndef RecvVDFormatResTask_H
#define RecvVDFormatResTask_H

#include "common/comm/BaseTask.h"
#include "common/comm/TaskManager.h"

class Message;
class ControlAgent;
class ThreadPoolWorkItem;

class RecvVDFormatResTask : public BaseTask
{
public:
    RecvVDFormatResTask();
    virtual ~RecvVDFormatResTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    int setAgent(ControlAgent * agent);

    // static BaseTask* createTask();
private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;

private:
    RecvVDFormatResTask(const RecvVDFormatResTask& );
    RecvVDFormatResTask& operator=(const RecvVDFormatResTask& );
};                               


#endif
