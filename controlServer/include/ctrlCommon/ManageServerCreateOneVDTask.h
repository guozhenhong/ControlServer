/*
 * =====================================================================================
 *
 *       Filename:  ManageServerCreateOneVDTask.h
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
#ifndef ManageServerCreateOneVDTask_H
#define ManageServerCreateOneVDTask_H

#include <string>
#include "common/comm/BaseTask.h"

const uint32_t DO_INIT_DB = 1;
const uint32_t DO_GET_LOCATION = 2;
const uint32_t DO_FORMAT_VD = 3;
const uint32_t DO_SET_DB = 4;
const uint32_t FINISHED = 5;

class Message;
class ControlAgent;
class ThreadPoolWorkItem;
class ManageServerCreateVDACKMessage;
class SetVDStatusMessage;

class ManageServerCreateOneVDTask : public BaseTask
{
public:
    ManageServerCreateOneVDTask();
    virtual ~ManageServerCreateOneVDTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    virtual void recvWorkItem( ThreadPoolWorkItem* pWorkItem);

    int setAgent(ControlAgent * agent);
    int setSuperTaskID(uint64_t ID);

    ManageServerCreateVDACKMessage* getACKMessage();
    // static BaseTask* createTask();

private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;
    SetVDStatusMessage *m_pSetStatusMsg;
    ManageServerCreateVDACKMessage *m_pACKMsg;

    uint64_t m_iSuperTaskID;
    uint32_t m_iVDID;
    uint32_t m_iEchoID;
    uint64_t m_iCap;
    uint32_t m_iFSType;
    uint32_t m_iRes;
    uint32_t m_state;


private:
    ManageServerCreateOneVDTask(const ManageServerCreateOneVDTask& );
    ManageServerCreateOneVDTask& operator=(const ManageServerCreateOneVDTask& );
};                               


#endif
