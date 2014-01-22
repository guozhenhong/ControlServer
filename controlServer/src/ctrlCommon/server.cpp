/*
 * =====================================================================================
 *
 *       Filename:  server.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/25/2013 04:09:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#include "ctrlCommon/ControlAgent.h"
#include "ctrlCommon/RegisterServerMessage.h"
#include "ctrlCommon/RegisterServerInfoTask.h"
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CServerInfoTable.h"
#include "controlDB/CStorageUnitInfoTable.h"
#include "controlDB/CVirtualDiskInfoTable.h"
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include "common/comm/Epoll.h"
#include "common/comm/TCPListenAgent.h"
#include "common/comm/AgentManager.h"
#include "common/comm/TaskManager.h"
#include "common/sys/ThreadPool.h"
#include "common/comm/SocketAddress.h"


using namespace std;

Epoll *g_pEpoll = NULL;
ThreadPool *g_pThreadPool = NULL;
ThreadPoolDispatcher *g_pDispatcher = NULL;

void doExit(int sig)
{
    if(sig == SIGINT)
    {
        cout<<"EXIT!"<<endl;
        if(g_pEpoll)
            g_pEpoll->finish();

    }
}

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cout<<"./main addrss port!"<<endl;

        return -1;
    }
    g_pEpoll = new Epoll;

    g_pEpoll->initialize(10000);

    g_pThreadPool = new ThreadPool();
    g_pThreadPool->start();

    //must init epoll and thread pool before
    g_pDispatcher = (AgentManager::getInstance())->createAgent<ThreadPoolDispatcher>();
    g_pDispatcher->init();

    SocketAddress addr(argv[1], atoi(argv[2])); 

    TCPListenAgent<ControlAgent> *pListenAgent= 
        (AgentManager::getInstance())->createAgent
            < TCPListenAgent<ControlAgent> >(addr);
    
    pListenAgent->init();
    CCtrlDBHandler::getInstance()->OpenDBFile("CtrlServerDB.db");
    CCtrlDBHandler::getInstance()->RegisterTable(DEFAULT_SERVER_INFO_TABLE, new CServerInfoTable());
    CCtrlDBHandler::getInstance()->RegisterTable(DEFAULT_STORAGE_INFO_TABLE, new CStorageUnitInfoTable());
    CCtrlDBHandler::getInstance()->RegisterTable(DEFAULT_DISK_INFO_TABLE, new CVirtualDiskInfoTable());
    
   /*pControlAgent->RegisterSerializer(MSG_REGISTER_SERVER, new RegisterServerMsgSerializer());
    pControlAgent->RegisterDeserializer(MSG_REGISTER_SERVER, new RegisterServerMsgDeserializer());
    pControlAgent->RegisterWorkTask(MSG_REGISTER_SERVER, new RegisterServerInfoTask(pControlAgent));
*/
    if(signal(SIGINT, doExit) == SIG_ERR)
    {
        cout<<"signal() ERROR"<<endl;
        return -1;
    }
    g_pEpoll->run();

    AgentManager::destroyInstance();
    TaskManager::destroyInstance();    
    CCtrlDBHandler::destroyInstance();

    g_pThreadPool->stop();
    if ( g_pThreadPool != NULL)
    {
        delete g_pThreadPool;
        g_pThreadPool = NULL;
    }
    
    if ( g_pEpoll != NULL)
    {
        delete g_pEpoll;
        g_pEpoll = NULL;
    }

    return 0;
}

