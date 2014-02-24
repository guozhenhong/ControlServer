/*
 * =====================================================================================
 *
 *       Filename:  MessageID.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/13 15:43:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#ifndef MESSAGEID_H
#define MESSAGEID_H

#define MSG_REGISTER_SERVER 100
#define MSG_ACK_REGISTER_SERVER 101

#define MSG_GET_SERVER_INFO 102
#define MSG_ACK_GET_SERVER_INFO 103

#define MSG_GET_SYS_CAP_INFO 108
#define MSG_ACK_SYS_CAP_INFO 109

#define MSG_GET_VD_CAP_INFO 110
#define MSG_ACK_VD_CAP_INFO 111

#define MSG_CHANGE_VD_CAP 122
#define MSG_ACK_CH_VD_CAP 125 

#define MSG_MANAGE_SERVER_CREATE_VD 118
#define MSG_ACK_MANAGE_SERVER_CREATE_VD 119

#define MSG_MANAGER_SERVER_BATCH_CREATE_VD 120
#define MSG_ACK_MANAGER_SERVER_BATCH_CREATE_VD 121

#define MSG_MANAGE_SERVER_DELETE_VD 114
#define MSG_ACK_MANAGE_SERVER_DELETE_VD 115

#define MSG_MANAGER_SERVER_BATCH_DELETE_VD 116
#define MSG_ACK_MANAGER_SERVER_BATCH_DELETE_VD 117

#define MSG_GET_SU_CAP_INFO 150
#define MSG_ACK_SU_CAP_INFO 151

#define MSG_UPDATE_NODE_CAP_INFO 161
#define MSG_ACK_NODE_CAP_INFO 162

#define MSG_SET_VD_STATUS 163

//io with locate server
#define MSG_GET_VD_LOCATE_NODE 123
#define MSG_ACK_VD_LOCATE_NODE 124

//io with node server
#define MSG_NODE_FORMAT_VD 167
#define MSG_ACK_FORMAT_VD 168

#define MSG_CHANGE_VD_FORMAT_CAP 158
#define MSG_ACK_CH_VD_FORMAT_CAP 159

#define MSG_NODE_DEL_VD 169
#define MSG_ACK_DEL_VD 170

#define MANAGE_SERVER_NAME "ManagerServer"
#define LOCATE_SERVER_NAME "LocateServer"
#define STORAGE_SERVER_ONE_NAME "SU1"

#endif
