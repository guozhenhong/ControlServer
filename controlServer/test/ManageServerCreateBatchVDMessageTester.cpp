#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include "../include/ctrlCommon/ManageServerCreateBatchVDMessage.h"

using namespace std;

TEST(ManageServerCreateBatchVDMessage, SeriaAndDeserial)
{
    int num = 5;
    ManageServerCreateVDMessage *pCreateList = new ManageServerCreateVDMessage [5];
    for(int i = 0; i < 5; i++)
    {
        pCreateList[i].m_iEchoID = i + 1;
        pCreateList[i].m_FileSysType = 3;
        pCreateList[i].m_VDCapacity = 10000000000 + i;

    }

    ManageServerCreateBatchVDMessage *pBatchCreateMsg = new ManageServerCreateBatchVDMessage(num, pCreateList);
    ManageServerCreateBatchVDMsgSerializer ser;
    ManageServerCreateBatchVDMsgDeserializer des;

    int bufLength = 0;

    char *pBuffer = ser.Serialize(pBatchCreateMsg, &bufLength);
    Message *pD = des.Deserialize(pBuffer);
    ManageServerCreateBatchVDMessage *pDesMsg = dynamic_cast<ManageServerCreateBatchVDMessage*> (pD);
    

    EXPECT_TRUE(pBuffer);
    EXPECT_EQ(bufLength, 84);
    EXPECT_TRUE(pDesMsg);
    
    delete [] pBuffer;

    EXPECT_EQ(pBatchCreateMsg->m_iDiskNum, pDesMsg->m_iDiskNum);
    
    ManageServerCreateVDMessage *pSrcList = pBatchCreateMsg->m_diskInfoList;
    EXPECT_TRUE(pSrcList);

    ManageServerCreateVDMessage *pDesList = pDesMsg->m_diskInfoList;
    EXPECT_TRUE(pDesList);

    for(int i = 0; i < num; i++)
    {
        EXPECT_EQ(pSrcList[i].m_iEchoID, pDesList[i].m_iEchoID);
        EXPECT_EQ(pSrcList[i].m_FileSysType, pDesList[i].m_FileSysType);
        EXPECT_EQ(pSrcList[i].m_VDCapacity, pDesList[i].m_VDCapacity);
    }

    delete pBatchCreateMsg;
    delete pDesMsg;
}  
