/*
 * =====================================================================================
 *
 *       Filename:  ManageServerDeleteBatchVDTask.h
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
#ifndef ManageServerDeleteBatchVDTask_H
#define ManageServerDeleteBatchVDTask_H

#include <string>
#include "common/comm/BaseTask.h"

const uint32_t BAT_DO_DEL_TASK = 1;
const uint32_t BAT_DO_DEL_ONE_FINISHED = 2;
const uint32_t BAT_DO_DEL_FINISHED = 3;

class Message;
class ControlAgent;
class ThreadPoolWorkItem;
class ManageServerDeleteBatchVDMessage;
class ManageServerDeleteOneVDTask;
class ManageServerDeleteVDACKMessage;
class ManageServerDeleteVDMessage;

class ManageServerDeleteBatchVDTask : public BaseTask
{
public:
    ManageServerDeleteBatchVDTask();
    virtual ~ManageServerDeleteBatchVDTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    int setAgent(ControlAgent * agent);

private:
    ControlAgent *m_pAgent;
    ManageServerDeleteBatchVDMessage *m_pMsg;
    // ManageServerCreateBatchVDACKMessage *m_pACKMsg;
    ManageServerDeleteVDACKMessage *m_pACKMsgList;
    ManageServerDeleteVDMessage *m_pOneTaskMessage;
    ManageServerDeleteOneVDTask *m_pOneTask;
    uint32_t m_state;

    int m_iReqNum;
    int m_iCount;

private:
    ManageServerDeleteBatchVDTask(const ManageServerDeleteBatchVDTask& );
    ManageServerDeleteBatchVDTask& operator=(const ManageServerDeleteBatchVDTask& );
};                               


#endif
