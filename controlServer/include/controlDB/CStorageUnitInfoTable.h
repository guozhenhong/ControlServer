/*
 * =====================================================================================
 *
 *       Filename:  CStorageUnitInfoTable.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/26/13 11:02:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#ifndef CSUINFOTABLE_H
#define CSUINFOTABLE_H

#include <stdint.h>
#include <string>
#include "CTableHandler.h"

using namespace std;

/*  typedef struct capInfo
{
    uint64_t m_sumCap;
    uint64_t m_declaredCap;
    uint64_t m_usedCap;
}sCapacityInfo;
*/

class CStorageUnitInfoTable : public CTableHandler
{
public:
    CStorageUnitInfoTable();
    virtual ~CStorageUnitInfoTable();

    int CreateTable();

    int GetSysCapacityInfo(sCapacityInfo* pInfo);
    int GetNodeCapacityInfo(const string& nodeName, sCapacityInfo* pInfo);

    int SetNodeCapacityInfo(const string& nodeName, sCapacityInfo* pInfo);
    int SetNodeSumCapacity(const string& nodeName, uint64_t sumCap);
    int SetNodeDeclaredCapacity(const string& nodeName, uint64_t decCap);
    int SetNodeUsedCapacity(const string& nodeName, uint64_t usedCap);
    int SetNodeStatus(const string& nodeName, bool isOnline);
private:
    int GetCapacityInfo(const char* buf, sCapacityInfo *pInfo);
    int SetNodeInfoByOneField(const string& nodeName, const char* field, uint64_t value);

};

#endif
