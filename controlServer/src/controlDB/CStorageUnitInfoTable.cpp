/*
 * =====================================================================================
 *
 *       Filename:  CStorageUnitInfoTable.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/26/13 11:33:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL
 *
 * =====================================================================================
 */
#include <stdint.h>
#include <iostream>
#include <string>
#include <inttypes.h>
#include "controlDB/CStorageUnitInfoTable.h"
#include "controlDB/CppSQLite3.h"

using namespace std;

CStorageUnitInfoTable::CStorageUnitInfoTable()
{

}

CStorageUnitInfoTable::~CStorageUnitInfoTable()
{

}

int CStorageUnitInfoTable::CreateTable()
{
    if(!m_pDB)
    {
        cout<<"Invalid Sqlite"<<endl;
        return -1;
    }    

    try
    {
        char buf[MAX_BUF_LEN];
        memset(buf, 0, MAX_BUF_LEN);
        sprintf(buf, "create table %s(ID integer primary key autoincrement, name nvarchar(100), sumCapacity bigint, declaredCapacity bigint, usedCapacity bigint, isOnline bool)", m_strTableName.c_str());

         m_pDB->execDML(buf);
    }

    catch(CppSQLite3Exception e)
    {
        cerr<<"CStorageUnitInfoTable::Create:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;

    }

    return 0;
}

int CStorageUnitInfoTable::GetSysCapacityInfo(sCapacityInfo *pInfo)
{
    char buf[MAX_BUF_LEN];
    memset(buf, 0, MAX_BUF_LEN);
    sprintf(buf, "select sum(sumCapacity), sum(declaredCapacity), sum(usedCapacity) from %s", m_strTableName.c_str());
    if(!m_pDB) 
    {
        cout<<"Invalid Sqlite"<<endl;
        return -1;
    }    

    try
    {
        CppSQLite3Query q = m_pDB->execQuery(buf);
        if(!q.eof())
        {
            pInfo->m_sumCap = q.getInt64Field(0);
            pInfo->m_declaredCap = q.getInt64Field(1);
            pInfo->m_usedCap = q.getInt64Field(2);
        }
        else
           return -1; 

    }

    catch(CppSQLite3Exception e)
    {
        cerr<<"CStorageUnitInfoTable::GetSysCapacityInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }


    return 0;
}

int CStorageUnitInfoTable::GetNodeCapacityInfo(const string& nodeName, sCapacityInfo *pInfo)
{  
    char buf[MAX_BUF_LEN];
    memset(buf, 0, MAX_BUF_LEN);
    sprintf(buf, "select * from %s where name = '%s'", m_strTableName.c_str(), nodeName.c_str());

    return GetCapacityInfo(buf, pInfo);
}

int CStorageUnitInfoTable::SetNodeCapacityInfo(const string& nodeName, sCapacityInfo* pInfo)
{
    if(!m_pDB) 
    {
        cout<<"Invalid Sqlite"<<endl;
        return -1;
    }    

    try
    {
        char buf[MAX_BUF_LEN];
        memset(buf, 0, MAX_BUF_LEN);
        if(IsServerInfoExist("name", nodeName))
        {
            sprintf(buf, "update %s set sumCapacity = %"PRIu64", declaredCapacity = %"PRIu64", usedCapacity = %"PRIu64", isOnline = 1 where name = '%s'", m_strTableName.c_str(), pInfo->m_sumCap, pInfo->m_declaredCap, pInfo->m_usedCap, nodeName.c_str());
        }
        else
        {
            sprintf(buf, "insert into %s (name, sumCapacity, declaredCapacity, usedCapacity, isOnline) values ('%s', %"PRIu64", %"PRIu64", %"PRIu64", 1)", m_strTableName.c_str(), nodeName.c_str(), pInfo->m_sumCap, pInfo->m_declaredCap, pInfo->m_usedCap);
        }
        m_pDB->execDML(buf);
    }
    catch(CppSQLite3Exception e)
    {
        cerr<<"CStorageUnitInfoTable::SetNodeCapacityInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }


    return 0;
}

int CStorageUnitInfoTable::SetNodeSumCapacity(const string& nodeName, uint64_t sumCap)
{
    return SetNodeInfoByOneField(nodeName, "sumCapacity", sumCap);
}

int CStorageUnitInfoTable::SetNodeDeclaredCapacity(const string& nodeName, uint64_t decCap)
{
    return SetNodeInfoByOneField(nodeName, "declaredCapacity", decCap);
}

int CStorageUnitInfoTable::SetNodeUsedCapacity(const string& nodeName, uint64_t usedCap)
{
    return SetNodeInfoByOneField(nodeName, "usedCapacity", usedCap);
}

int CStorageUnitInfoTable::SetNodeStatus(const string& nodeName, bool isOnline)
{
    if(isOnline)
        return SetNodeInfoByOneField(nodeName, "isOnline", 1);   
    else
        return SetNodeInfoByOneField(nodeName, "isOnline", 0);
}

int CStorageUnitInfoTable::SetNodeInfoByOneField(const string& nodeName, const char* field, uint64_t value)
{
    if(!m_pDB) 
    {
        cout<<"Invalid Sqlite"<<endl;
        return -1;
    }    

    try
    {
        char buf[MAX_BUF_LEN];
        memset(buf, 0, MAX_BUF_LEN);
        if(IsServerInfoExist("name", nodeName))
        {
            sprintf(buf, 
                "update %s set %s = %"PRIu64" where name = '%s'", 
                    m_strTableName.c_str(), field, value, nodeName.c_str());
        }
        else
        {
            sprintf(buf, "insert into %s (name, %s) values ('%s', %"PRIu64")", m_strTableName.c_str(), field, nodeName.c_str(), value);
        }
        m_pDB->execDML(buf);
    }
    catch(CppSQLite3Exception e)
    {
        cerr<<"CStorageUnitInfoTable::SetNodeSumCapacity:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }

    return 0;
}

int CStorageUnitInfoTable::GetCapacityInfo(const char* buf, sCapacityInfo *pInfo)
{
    if(!m_pDB) 
    {
        cout<<"Invalid Sqlite"<<endl;
        return -1;
    }    

    try
    {
        CppSQLite3Query q = m_pDB->execQuery(buf);
        if(!q.eof())
        {
            pInfo->m_sumCap = q.getInt64Field("sumCapacity");
            pInfo->m_declaredCap = q.getInt64Field("declaredCapacity");
            pInfo->m_usedCap = q.getInt64Field("usedCapacity");
        }
        else
           return -1; 

    }

    catch(CppSQLite3Exception e)
    {
        cerr<<"CStorageUnitInfoTable::GetCapacityInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }

    return 0;
}
