#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include "../include/ctrlCommon/ManageServerDeleteBatchVDMessage.h"

using namespace std;

TEST(ManageServerDeleteBatchVDACKMessage, SeriaAndDeserial)
{
    int num = 5;
    ManageServerDeleteVDACKMessage *pCreateList = new ManageServerDeleteVDACKMessage [5];
    for(int i = 0; i < 5; i++)
    {
        pCreateList[i].m_iVDID = 4 + i;
        pCreateList[i].m_bIsSuccess = 1;
    }

    ManageServerDeleteBatchVDACKMessage *pBatchACKMsg = new ManageServerDeleteBatchVDACKMessage(num, pCreateList);
    ManageServerDeleteBatchVDACKMsgSerializer ser;
    ManageServerDeleteBatchVDACKMsgDeserializer des;

    int bufLength = 0;

    char *pBuffer = ser.Serialize(pBatchACKMsg, &bufLength);

    EXPECT_TRUE(pBuffer);
    EXPECT_EQ(bufLength, 44);

    Message *pD = des.Deserialize(pBuffer);
    ManageServerDeleteBatchVDACKMessage *pDesMsg = dynamic_cast<ManageServerDeleteBatchVDACKMessage*> (pD);

    EXPECT_TRUE(pDesMsg);
    delete [] pBuffer;

    EXPECT_EQ(pBatchACKMsg->m_iDiskNum, pDesMsg->m_iDiskNum);
    
    ManageServerDeleteVDACKMessage *pSrcList = pBatchACKMsg->m_pResList;
    EXPECT_TRUE(pSrcList);

    ManageServerDeleteVDACKMessage *pDesList = pDesMsg->m_pResList;
    EXPECT_TRUE(pDesList);

    for(int i = 0; i < num; i++)
    {
        EXPECT_EQ(pSrcList[i].m_iVDID, pDesList[i].m_iVDID);
        EXPECT_EQ(pSrcList[i].m_bIsSuccess, pDesList[i].m_bIsSuccess);
    }

    delete pBatchACKMsg;
    delete pDesMsg;
}  

// TEST(ManageServerDeleteBatchVDACKMessage, OneMemSeriaAndDeserial)
// {
//     int num = 1;
//     ManageServerDeleteVDACKMessage *pCreateList = new ManageServerDeleteVDACKMessage [1];
//     for(int i = 0; i < num; i++)
//     {
//         pCreateList[i].m_iVDID = 4 + i;
//         pCreateList[i].m_bIsSuccess = 1;
//     }

//     ManageServerDeleteBatchVDACKMessage *pBatchACKMsg = new ManageServerDeleteBatchVDACKMessage(num, pCreateList);

//     delete pBatchACKMsg; 
// }

TEST(ManageServerDeleteBatchVDACKMessage, OneMemSeriaAndDeserial)
{
    int num = 1;
    ManageServerDeleteVDACKMessage *pCreateList = new ManageServerDeleteVDACKMessage [1];
    for(int i = 0; i < num; i++)
    {
        pCreateList[i].m_iVDID = 4 + i;
        pCreateList[i].m_bIsSuccess = 1;
    }

    ManageServerDeleteBatchVDACKMessage *pBatchACKMsg = new ManageServerDeleteBatchVDACKMessage(num, pCreateList);
    ManageServerDeleteBatchVDACKMsgSerializer ser;
    ManageServerDeleteBatchVDACKMsgDeserializer des;

    int bufLength = 0;

    char *pBuffer = ser.Serialize(pBatchACKMsg, &bufLength);

    EXPECT_TRUE(pBuffer);
    EXPECT_EQ(bufLength, 12);

    Message *pD = des.Deserialize(pBuffer);
    ManageServerDeleteBatchVDACKMessage *pDesMsg = dynamic_cast<ManageServerDeleteBatchVDACKMessage*> (pD);

    EXPECT_TRUE(pDesMsg);
    delete [] pBuffer;

    EXPECT_EQ(num, pDesMsg->m_iDiskNum);
    EXPECT_EQ(pBatchACKMsg->m_iDiskNum, pDesMsg->m_iDiskNum);
    
    ManageServerDeleteVDACKMessage *pSrcList = pBatchACKMsg->m_pResList;
    EXPECT_TRUE(pSrcList);

    ManageServerDeleteVDACKMessage *pDesList = pDesMsg->m_pResList;
    EXPECT_TRUE(pDesList);

    for(int i = 0; i < num; i++)
    {
        EXPECT_EQ(pSrcList[i].m_iVDID, pDesList[i].m_iVDID);
        EXPECT_EQ(pSrcList[i].m_bIsSuccess, pDesList[i].m_bIsSuccess);
    }

    delete pBatchACKMsg;
    delete pDesMsg;
}  