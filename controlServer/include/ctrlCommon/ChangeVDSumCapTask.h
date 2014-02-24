/*
 * =====================================================================================
 *
 *       Filename:  ChangeVDSumCapTask.h
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
#ifndef ChangeVDSumCapTask_H
#define ChangeVDSumCapTask_H

#include "common/comm/BaseTask.h"
#include "common/comm/TaskManager.h"

class Message;
class ControlAgent;
class ThreadPoolWorkItem;
class ChangeVDSumCapACKMessage;

const uint32_t DO_CHECK_VD_CAP = 1;
const uint32_t DO_GET_VD_LOCATE = 2;
const uint32_t DO_CHECK_NODE_CAP = 3;
const uint32_t DO_CHANGE_FORMAT_CAP = 4;
const uint32_t DO_CHANGE_DB_INFO = 5;
const uint32_t FINISHED = 6;

class ChangeVDSumCapTask : public BaseTask
{
public:
    ChangeVDSumCapTask();
    virtual ~ChangeVDSumCapTask();

    virtual int goNext();
   
    virtual int recvMsg(Message *pMsg);

    virtual void recvWorkItem( ThreadPoolWorkItem* pWorkItem);

    int setAgent(ControlAgent * agent);

    // static BaseTask* createTask();
private:
    ControlAgent *m_pAgent;
    Message *m_pMsg;
    ChangeVDSumCapACKMessage *m_pACKMsg;

    uint32_t m_state;
    uint32_t m_iRes;

    uint32_t m_iVDID;
    uint64_t m_vdNewSumCap;
    uint64_t m_vdOldSumCap;
    uint64_t m_nodeRestCap;

    string m_strVDLocateNodeName;

private:
    ChangeVDSumCapTask(const ChangeVDSumCapTask& );
    ChangeVDSumCapTask& operator=(const ChangeVDSumCapTask& );
};                               


#endif
