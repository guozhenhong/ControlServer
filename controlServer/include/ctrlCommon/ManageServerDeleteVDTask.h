/*
 * =====================================================================================
 *
 *       Filename:  ManageServerDeleteVDTask.h
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
#ifndef ManageServerDeleteVDTask_H
#define ManageServerDeleteVDTask_H

#include <string>
#include "common/comm/BaseTask.h"

const uint32_t DO_DEL_TASK = 1;
const uint32_t DO_DEL_FINISHED = 2;

class Message;
class ControlAgent;
class ThreadPoolWorkItem;
class ManageServerDeleteOneVDTask;

class ManageServerDeleteVDTask : public BaseTask
{
public:
    ManageServerDeleteVDTask();
    virtual ~ManageServerDeleteVDTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    int setAgent(ControlAgent * agent);
    // static BaseTask* createTask();

private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;

    ManageServerDeleteOneVDTask *m_pOneTask;
    uint32_t m_state;

private:
    ManageServerDeleteVDTask(const ManageServerDeleteVDTask& );
    ManageServerDeleteVDTask& operator=(const ManageServerDeleteVDTask& );
};                               


#endif
