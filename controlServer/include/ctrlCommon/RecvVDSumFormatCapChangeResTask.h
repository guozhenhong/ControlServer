/*
 * =====================================================================================
 *
 *       Filename:  RecvVDSumFormatCapChangeResTask.h
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
#ifndef RecvVDSumFormatCapChangeResTask_H
#define RecvVDSumFormatCapChangeResTask_H

#include "common/comm/BaseTask.h"
#include "common/comm/TaskManager.h"

class Message;
class ControlAgent;
class ThreadPoolWorkItem;

class RecvVDSumFormatCapChangeResTask : public BaseTask
{
public:
    RecvVDSumFormatCapChangeResTask();
    virtual ~RecvVDSumFormatCapChangeResTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    int setAgent(ControlAgent * agent);

    // static BaseTask* createTask();
private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;

private:
    RecvVDSumFormatCapChangeResTask(const RecvVDSumFormatCapChangeResTask& );
    RecvVDSumFormatCapChangeResTask& operator=(const RecvVDSumFormatCapChangeResTask& );
};                               


#endif
