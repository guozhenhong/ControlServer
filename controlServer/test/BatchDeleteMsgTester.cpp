#include "gtest/gtest.h"
#include <iostream>
#include <string>
#include "../include/ctrlCommon/ManageServerDeleteBatchVDMessage.h"

using namespace std;

TEST(ManageServerDeleteBatchVDMessage, SeriaAndDeserial)
{
    int num = 4;
    uint32_t *pDelIDList = new uint32_t[4];
    for(int i = 0; i < 4; i++)
    {
        pDelIDList[i] = i + 2;
    }

    ManageServerDeleteBatchVDMessage *pBatchDelMsg = new ManageServerDeleteBatchVDMessage(num, pDelIDList);
    ManageServerDeleteBatchVDMsgSerializer ser;
    ManageServerDeleteBatchVDMsgDeserializer des;

    int bufLength = 0;

    char *pBuffer = ser.Serialize(pBatchDelMsg, &bufLength);
    Message *pD = des.Deserialize(pBuffer);
    ManageServerDeleteBatchVDMessage *pDesMsg = dynamic_cast<ManageServerDeleteBatchVDMessage*> (pD);
    

    EXPECT_TRUE(pBuffer);
    EXPECT_EQ(bufLength, 20);
    EXPECT_TRUE(pDesMsg);
    
    delete [] pBuffer;

    EXPECT_EQ(pBatchDelMsg->m_iDiskNum, pDesMsg->m_iDiskNum);
    
    uint32_t *pSrcList = pBatchDelMsg->m_pDiskIDList;
    EXPECT_TRUE(pSrcList);

    uint32_t *pDesIDList = pDesMsg->m_pDiskIDList;
    EXPECT_TRUE(pDesIDList);

    for(int i = 0; i < num; i++)
    {
        EXPECT_EQ(pSrcList[i], pDesIDList[i]);
    }

    delete pBatchDelMsg;
    delete pDesMsg;
}  
