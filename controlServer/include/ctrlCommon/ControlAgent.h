/*
 * =====================================================================================
 *
 *       Filename:  ControlAgent.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/22/2013 05:54:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#ifndef CONTROLAGENT_H
#define CONTROLAGENT_H

#include "common/comm/TCPAgent.h"
#include "common/comm/BaseTask.h"

#include <map>
#include <string>
#include <stdint.h>

typedef BaseTask* (*pTaskCreateFunction)();

class MessageSerializer;
class MessageDeserializer;
class MultiMsgDeserializer;
class MultiMsgSerializer;

class ControlAgent : public TCPAgent
{
public:
	ControlAgent();
	ControlAgent(const SocketAddress &addr);
    ControlAgent(const TCPSocket& socket, const SocketAddress& addr);
    virtual ~ControlAgent();

    virtual int init();
    
public:
    virtual void readBack(InReq& req);
    virtual void writeBack(bool result);

    const SocketAddress& GetOppositeAddr() const;
    const string& GetOppositeName() const;
    int SetOppositeName(string& name);

    int RegisterSerializer(unsigned long msgID, MessageSerializer *pSerializer);
    int RegisterDeserializer(unsigned long msgID, MessageDeserializer *pDeserializer);
    int RegisterTaskCreateFunc(unsigned long msgID, pTaskCreateFunction pFunc);
    void SendMsg(Message *pMsg);

    void SendMsgToServer(const std::string& name, Message *pMsg);
    void Dispatch(uint32_t cmd, Message *pMsg);

//private:
    BaseTask *GetTaskObject(uint32_t cmd);

private:
	MultiMsgDeserializer *m_pDeserializer;
	MultiMsgSerializer *m_pSerializer;
        
    std::map<unsigned long, pTaskCreateFunction> m_WorkTaskCreaterTable;
    std::string m_strOppositeName;
};

#endif
