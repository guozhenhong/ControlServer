/*
 * =====================================================================================
 *
 *       Filename:  BaseTask.h
 *
 *    Description:  the virtual base class of all task class
 *
 *        Version:  1.0
 *        Created:  2010年03月26日 14时56分50秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  ptz, ptz_007@uestc.edu.cn
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */
#ifndef _BASETASK_H_
#define _BASETASK_H_

#include <stdint.h>

#include "common/comm/Error.h"
#include "common/comm/TaskManager.h"

struct InReq;
class ThreadPoolWorkItem;
class Message;
class ControlAgent;

class BaseTask
{
public:
    BaseTask() {}
    virtual ~BaseTask() {}

	virtual int goNext() = 0;
	
    virtual int recvMsg(Message *pMsg)
	{
        return SUCCESSFUL;
    }

    virtual void readBack( InReq& ) {}
    virtual void writeBack( bool ) {}

    //add for TaskManager
    virtual void setID( uint64_t id )
    {
        m_ID = id;
    }
    virtual uint64_t getID() const
	{
        return m_ID;
	}

    //add for threadPool
    virtual void recvWorkItem( ThreadPoolWorkItem* ) {}

    virtual int setAgent(ControlAgent* agent) = 0;
    
    template <typename T> 
    static BaseTask *createTask()
    {
        return TaskManager::getInstance()->create<T>();
    }

private:
    uint64_t m_ID;
};

#endif
