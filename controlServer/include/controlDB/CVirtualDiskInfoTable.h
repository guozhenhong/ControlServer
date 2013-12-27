/*
 * =====================================================================================
 *
 *       Filename:  CVirtualDiskInfoTable.h
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

#define VD_FORMATING_STATUS "formating"
#define VD_FORMATED_STATUS "formated"

class CVirtualDiskInfoTable : public CTableHandler
{
public:
    CVirtualDiskInfoTable();
    virtual ~CVirtualDiskInfoTable();

    int CreateTable();

    int GetVDCapacityInfo(uint32_t& vdID, sCapacityInfo* pInfo);

    int CreateVirtualDiskInfo(sCapacityInfo* pInfo, const string& fsType, const string& vdStatus, uint32_t& vdID);

    int SetVDCapacityInfo(uint32_t& vdID, sCapacityInfo* pInfo);
    
    int SetVDDeclaredCapacity(uint32_t& vdID, uint64_t decCap);
    int SetVDUsedCapacity(uint32_t& vdID, uint64_t usedCap);
    int SetVDUsedByFSCapacity(uint32_t& vdID, uint64_t usedCapByFS);

    int SetVDStatus(uint32_t& vdID, const string& vdStatus);
    int SetVDFSType(uint32_t& vdID, const string& vdFSType);

    int DeleteVirtualDiskInfo(uint32_t& vdID);

private:
    int GetCapacityInfo(const char* buf, sCapacityInfo *pInfo);
    int SetVDCapInfoByOneField(uint32_t& vdID, const char* field, uint64_t& value);
    int GetCurrentLastID();
};

#endif
