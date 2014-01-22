/*
 * =====================================================================================
 *
 *       Filename:  MasterAgent.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/26/2013 09:41:44 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#ifndef MASTERAGENT_H
#define MASTERAGENT_H

#include <map>
#include "common/comm/TCPAgent.h"
#include "common/comm/BaseReq.h"
#include "common/comm/BaseTask.h"

class SocketAddrss;
class MessageSerializer;
class MessageDeserializer;
class MultiMsgDeserializer;
class MultiMsgSerializer;

class MasterAgent : public TCPAgent
{
public:
    MasterAgent(const SocketAddress& addr);
    virtual ~MasterAgent();
        
public:
    virtual void readBack(InReq& req);
    virtual void writeBack(bool result);
    virtual int connectAfter(bool result);

    int RegisterSerializer(unsigned long msgID, MessageSerializer *pSerializer);
    int RegisterDeserializer(unsigned long msgID, MessageDeserializer *pDeserializer);
    int RegisterWorkTask(unsigned long msgID, BaseTask *pTask);
    void SendMsg(Message *pMsg);

private:
	MultiMsgDeserializer *m_pDeserializer;
	MultiMsgSerializer *m_pSerializer;
    
    std::map<unsigned long, BaseTask *> m_WorkTaskTable;
};


#endif
