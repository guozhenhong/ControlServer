#include "common/comm/SocketAddress.h"
#include "common/comm/TCPSocket.h"
#include "gtest/gtest.h"
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/RegisterServerInfoTask.h"
#include "common/sys/ThreadPool.h"  
#include "common/comm/Epoll.h" 
#include <string>

using namespace std;

Epoll *g_pEpoll = new Epoll;                                                          
ThreadPool *g_pThreadPool = new ThreadPool;                                                
ThreadPoolDispatcher *g_pDispatcher = new ThreadPoolDispatcher; 

string oppName("LogServer");
TCPSocket sock(20);
SocketAddress addr("192.168.9.65", 9876);

TEST(ControlAgent, Normal)
{
	ControlAgent *pAgent = new ControlAgent(sock, addr);
	EXPECT_EQ(pAgent->SetOppositeName(oppName), 0);
	EXPECT_EQ(pAgent->GetOppositeName(), oppName);
	EXPECT_EQ(pAgent->GetOppositeAddr(), addr);

	EXPECT_EQ(pAgent->RegisterTaskCreateFunc(100, RegisterServerInfoTask::createTask), 0);

	BaseTask *pTask = pAgent->GetTaskObject(100);
	EXPECT_TRUE(dynamic_cast<RegisterServerInfoTask*> (pTask));
}  
