#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include "../include/ctrlCommon/ManageServerCreateBatchVDMessage.h"

using namespace std;

TEST(ManageServerCreateBatchVDACKMessage, SeriaAndDeserial)
{
    int num = 5;
    ManageServerCreateVDACKMessage *pCreateList = new ManageServerCreateVDACKMessage [5];
    for(int i = 0; i < 5; i++)
    {
        pCreateList[i].m_iEchoID = i + 1;
        pCreateList[i].m_bIsSuccess = 1;
        pCreateList[i].m_iDiskID = 4 + i;

    }

    ManageServerCreateBatchVDACKMessage *pBatchACKMsg = new ManageServerCreateBatchVDACKMessage(num, pCreateList);
    ManageServerCreateBatchVDACKMsgSerializer ser;
    ManageServerCreateBatchVDACKMsgDeserializer des;

    int bufLength = 0;

    char *pBuffer = ser.Serialize(pBatchACKMsg, &bufLength);

    EXPECT_TRUE(pBuffer);
    EXPECT_EQ(bufLength, 64);

    Message *pD = des.Deserialize(pBuffer);
    ManageServerCreateBatchVDACKMessage *pDesMsg = dynamic_cast<ManageServerCreateBatchVDACKMessage*> (pD);

    EXPECT_TRUE(pDesMsg);
    delete [] pBuffer;

    EXPECT_EQ(pBatchACKMsg->m_iDiskNum, pDesMsg->m_iDiskNum);
    
    ManageServerCreateVDACKMessage *pSrcList = pBatchACKMsg->m_pResList;
    EXPECT_TRUE(pSrcList);

    ManageServerCreateVDACKMessage *pDesList = pDesMsg->m_pResList;
    EXPECT_TRUE(pDesList);

    for(int i = 0; i < num; i++)
    {
        EXPECT_EQ(pSrcList[i].m_iEchoID, pDesList[i].m_iEchoID);
        EXPECT_EQ(pSrcList[i].m_bIsSuccess, pDesList[i].m_bIsSuccess);
        EXPECT_EQ(pSrcList[i].m_iDiskID, pDesList[i].m_iDiskID);
    }

    delete pBatchACKMsg;
    delete pDesMsg;
}  
