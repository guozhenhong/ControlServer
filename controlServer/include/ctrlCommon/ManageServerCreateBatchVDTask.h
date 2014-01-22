/*
 * =====================================================================================
 *
 *       Filename:  ManageServerCreateBatchVDTask.h
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
#ifndef ManageServerCreateBatchVDTask_H
#define ManageServerCreateBatchVDTask_H

#include <string>
#include "common/comm/BaseTask.h"

const uint32_t BAT_DO_TASK = 1;
const uint32_t BAT_DO_ONE_FINISHED = 2;
const uint32_t BAT_DO_FINISHED = 3;

class Message;
class ControlAgent;
class ThreadPoolWorkItem;
class ManageServerCreateBatchVDMessage;
class ManageServerCreateOneVDTask;
class ManageServerCreateVDACKMessage;

class ManageServerCreateBatchVDTask : public BaseTask
{
public:
    ManageServerCreateBatchVDTask();
    virtual ~ManageServerCreateBatchVDTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    int setAgent(ControlAgent * agent);

private:
    ControlAgent *m_pAgent;
    ManageServerCreateBatchVDMessage *m_pMsg;
    // ManageServerCreateBatchVDACKMessage *m_pACKMsg;
    ManageServerCreateVDACKMessage *m_pACKMsgList;

    ManageServerCreateOneVDTask *m_pOneTask;
    uint32_t m_state;

    int m_iReqNum;
    int m_iCount;

private:
    ManageServerCreateBatchVDTask(const ManageServerCreateBatchVDTask& );
    ManageServerCreateBatchVDTask& operator=(const ManageServerCreateBatchVDTask& );
};                               


#endif
