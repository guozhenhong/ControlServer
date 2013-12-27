#include <stdint.h>
#include <iostream>
#include <string>
#include <inttypes.h>
#include "controlDB/CVirtualDiskInfoTable.h"
#include "controlDB/CppSQLite3.h"

using namespace std;

CVirtualDiskInfoTable::CVirtualDiskInfoTable()
{

}

CVirtualDiskInfoTable::~CVirtualDiskInfoTable()
{

}

int CVirtualDiskInfoTable::CreateTable()
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
        sprintf(buf, "create table %s(ID integer primary key autoincrement, declaredCapacity bigint, usedCapacity bigint, usedCapacityByFS bigint, fsType nvarchar(10), vdStatus nvarchar(20))", m_strTableName.c_str());

         m_pDB->execDML(buf);
    }

    catch(CppSQLite3Exception e)
    {
        cerr<<"CVirtualDiskInfoTable::Create:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;

    }

    return 0;
}

int CVirtualDiskInfoTable::GetVDCapacityInfo(uint32_t& vdID, sCapacityInfo* pInfo)
{
    char buf[MAX_BUF_LEN];
    memset(buf, 0, MAX_BUF_LEN);
    sprintf(buf, "select * from %s where ID = %"PRIu32"", m_strTableName.c_str(), vdID);

    return GetCapacityInfo(buf, pInfo);
}

int CVirtualDiskInfoTable::CreateVirtualDiskInfo(sCapacityInfo* pInfo, const string& fsType, const string& vdStatus, uint32_t& vdID)
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
        // if(IsServerInfoExist("ID", (int)vdID))
        // {
        //     sprintf(buf, "update %s set declaredCapacity = %"PRIu64", 
        //     	usedCapacity = %"PRIu64", usedCapacityByFS = %"PRIu64", 
        //     	fsType = %s, vdStatus = %s where ID = %"PRIi32"", 
        //     	m_strTableName.c_str(), pInfo->m_sumCap, pInfo->m_declaredCap, 
        //     	pInfo->m_usedCap, fsType.c_str(), vdStatus.c_str(), vdID);
        // }
        // else
        // {
        sprintf(buf, "insert into %s (declaredCapacity, usedCapacity, usedCapacityByFS, fsType, vdStatus) values (%"PRIu64", %"PRIu64", %"PRIu64", %s, %s)", m_strTableName.c_str(), pInfo->m_sumCap, pInfo->m_declaredCap, pInfo->m_usedCap, fsType.c_str(), vdStatus.c_str());
       // }
        m_pDB->execDML(buf);
        int  i = GetCurrentLastID();
        if(0 < i)
        	vdID = i;
    }
    catch(CppSQLite3Exception e)
    {
        cerr<<"CVirtualDiskInfoTable::SetNodeCapacityInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }


    return 0;
}

int CVirtualDiskInfoTable::GetCurrentLastID()
{
	char buf[MAX_BUF_LEN];
    memset(buf, 0, MAX_BUF_LEN);
    sprintf(buf, "select last_insert_rowid() from %s", m_strTableName.c_str());
	CppSQLite3Query q = m_pDB->execQuery(buf);
	if(!q.eof())
	{
		int i = q.getIntField(0);
		return i;
	}
	else
		return -1;
}

int CVirtualDiskInfoTable::SetVDCapacityInfo(uint32_t& vdID, sCapacityInfo* pInfo)
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
        if(IsServerInfoExist("ID", (int)vdID))
        {
            sprintf(buf, "update %s set sumCapacity = %"PRIu64", declaredCapacity = %"PRIu64", usedCapacity = %"PRIu64" where ID = %d", m_strTableName.c_str(), pInfo->m_sumCap, pInfo->m_declaredCap, pInfo->m_usedCap, vdID);
        }
        else
        {
        	cerr<<"CVirtualDiskInfoTable::SetVDCapacityInfo: the vdID is not exist"<<endl;
        	return -1;
        }
        m_pDB->execDML(buf);
    }
    catch(CppSQLite3Exception e)
    {
        cerr<<"CVirtualDiskInfoTable::SetVDCapacityInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }


    return 0;
}

int CVirtualDiskInfoTable::GetCapacityInfo(const char* buf, sCapacityInfo *pInfo)
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
            pInfo->m_sumCap = q.getInt64Field("declaredCapacity");
            pInfo->m_declaredCap = q.getInt64Field("usedCapacity");
            pInfo->m_usedCap = q.getInt64Field("usedCapacityByFS");
        }
        else
           return -1; 

    }

    catch(CppSQLite3Exception e)
    {
        cerr<<"CVirtualDiskInfoTable::GetCapacityInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }

    return 0;
}

int CVirtualDiskInfoTable::SetVDDeclaredCapacity(uint32_t& vdID, uint64_t decCap)
{
	return SetVDCapInfoByOneField(vdID, "declaredCapacity", decCap);
}

int CVirtualDiskInfoTable::SetVDUsedCapacity(uint32_t& vdID, uint64_t usedCap)
{
	return SetVDCapInfoByOneField(vdID, "usedCapacity", usedCap);
}

int CVirtualDiskInfoTable::SetVDUsedByFSCapacity(uint32_t& vdID, uint64_t usedCapByFS)
{
	return SetVDCapInfoByOneField(vdID, "usedCapacityByFS", usedCapByFS);
}

int CVirtualDiskInfoTable::SetVDCapInfoByOneField(uint32_t& vdID, const char* field, uint64_t& value)
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
        int vID = (int)(vdID);
        if(IsServerInfoExist("ID", vID))
        {
            sprintf(buf, 
                "update %s set %s = %"PRIu64" where ID = %d", 
                    m_strTableName.c_str(), field, value, vID);
        }
        else
        {
            cerr<<"CVirtualDiskInfoTable::SetVDCapInfoByOneField: the vdID is not exist"<<endl;
        	return -1;
       	}
        m_pDB->execDML(buf);
    }
    catch(CppSQLite3Exception e)
    {
        cerr<<"CVirtualDiskInfoTable::SetVDCapInfoByOneField:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }

    return 0;
}

int CVirtualDiskInfoTable::SetVDStatus(uint32_t& vdID, const string& vdStatus)
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
        int vID = (int)(vdID);
        if(IsServerInfoExist("ID", vID))
        {
            sprintf(buf, 
                "update %s set vdStatus = '%s' where ID = %d", 
                    m_strTableName.c_str(), vdStatus.c_str(), vID);
        }
        else
        {
            cerr<<"CVirtualDiskInfoTable::SetVDStatus: the vdID is not exist"<<endl;
        	return -1;
       	}
        m_pDB->execDML(buf);
    }
    catch(CppSQLite3Exception e)
    {
        cerr<<"CVirtualDiskInfoTable::SetVDStatus:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }

    return 0;
}

int CVirtualDiskInfoTable::SetVDFSType(uint32_t& vdID, const string& vdFSType)
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
        int vID = (int)(vdID);
        if(IsServerInfoExist("ID", vID))
        {
            sprintf(buf, 
                "update %s set fsType = '%s' where ID = %d", 
                    m_strTableName.c_str(), vdFSType.c_str(), vID);
        }
        else
        {
            cerr<<"CVirtualDiskInfoTable::SetVDFSType: the vdID is not exist"<<endl;
        	return -1;
       	}
        m_pDB->execDML(buf);
    }
    catch(CppSQLite3Exception e)
    {
        cerr<<"CVirtualDiskInfoTable::SetVDFSType:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
    }

    return 0;
}
