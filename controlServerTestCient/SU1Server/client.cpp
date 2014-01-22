/*
 * =====================================================================================
 *
 *       Filename:  client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/26/2013 10:17:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#include "MasterAgent.h"
#include "RegisterServerMessage.h"
#include "GetServerInfoMessage.h"
#include "GetSUCapacityInfoMessage.h"
#include "GetSysCapMessage.h"
#include "NodeCapacityInfoMessage.h"
#include "GetVDLocateNodeInfoMessage.h"
#include "ManageServerCreateVDMessage.h"
#include "MessageID.h"
#include "common/comm/Epoll.h"
#include "common/comm/SocketAddress.h"
#include "common/comm/AgentManager.h"
#include "VirtualDiskFormatMessage.h"
#include "VirtualDiskDeleteMessage.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

Epoll *g_pEpoll = NULL;

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        cout<<"./main serverAddr serverPort"<<endl;

        return -1;
    } 

    g_pEpoll = new Epoll;

    g_pEpoll->initialize(1000);

    SocketAddress addr(argv[1], atoi(argv[2]));
    MasterAgent* master = (AgentManager::getInstance())->createAgent<MasterAgent>(addr);
    master->init();

    master->RegisterSerializer(MSG_REGISTER_SERVER, new RegisterServerMsgSerializer());
    master->RegisterDeserializer(MSG_ACK_REGISTER_SERVER, new RegisterServerACKMsgDeserializer());

    master->RegisterSerializer(MSG_GET_SERVER_INFO, new GetServerInfoMsgSerializer());
    master->RegisterDeserializer(MSG_ACK_GET_SERVER_INFO, new GetServerInfoACKMsgDeserializer());
    
    master->RegisterSerializer(MSG_GET_SYS_CAP_INFO, new GetSysCapMsgSerializer());
    master->RegisterDeserializer(MSG_ACK_SYS_CAP_INFO, new GetSysCapACKMsgDeserializer());

    master->RegisterSerializer(MSG_UPDATE_NODE_CAP_INFO, new NodeCapacityInfoMsgSerializer());

    master->RegisterSerializer(MSG_GET_SU_CAP_INFO, new GetSUCapacityInfoMsgSerializer());
    master->RegisterDeserializer(MSG_ACK_SU_CAP_INFO, new GetSUCapacityInfoACKMsgDeserializer());

    master->RegisterSerializer(MSG_MANAGE_SERVER_CREATE_VD, new ManageServerCreateVDMsgSerializer());

    master->RegisterDeserializer(MSG_GET_VD_LOCATE_NODE, new GetVDLocateNodeInfoMsgDeserializer());

    master->RegisterDeserializer(MSG_NODE_FORMAT_VD, new VirtualDiskFormatMsgDeserializer());
    master->RegisterSerializer(MSG_ACK_FORMAT_VD, new VirtualDiskFormatACKMsgSerializer());

    master->RegisterDeserializer(MSG_NODE_DEL_VD, new VirtualDiskDeleteMsgDeserializer());
    master->RegisterSerializer(MSG_ACK_DEL_VD, new VirtualDiskDeleteACKMsgSerializer());

    master->connect();

    //master->SendMsg(new GetServerInfoMessage("SU"));

    g_pEpoll->run();

    return 0;
}
