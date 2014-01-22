/*
 * =====================================================================================
 *
 *       Filename:  ManageServerCreateVDTask.h
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
#ifndef ManageServerCreateVDTask_H
#define ManageServerCreateVDTask_H

#include <string>
#include "common/comm/BaseTask.h"

const uint32_t DO_TASK = 1;
const uint32_t DO_FINISHED = 2;

class Message;
class ControlAgent;
class ThreadPoolWorkItem;
class ManageServerCreateOneVDTask;

class ManageServerCreateVDTask : public BaseTask
{
public:
    ManageServerCreateVDTask();
    virtual ~ManageServerCreateVDTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    int setAgent(ControlAgent * agent);
    // static BaseTask* createTask();

private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;

    ManageServerCreateOneVDTask *m_pOneTask;
    uint32_t m_state;

private:
    ManageServerCreateVDTask(const ManageServerCreateVDTask& );
    ManageServerCreateVDTask& operator=(const ManageServerCreateVDTask& );
};                               


#endif
