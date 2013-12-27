#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "controlDB/CServerInfoTable.h"
#include "controlDB/CppSQLite3.h"

CServerInfoTable::CServerInfoTable()
{

}

CServerInfoTable::~CServerInfoTable()
{

}

int CServerInfoTable::CreateTable()
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
		sprintf(buf, "create table %s(ID integer primary key autoincrement, name nvarchar(100), ip nvarchar(20), port integer)", m_strTableName.c_str());
		m_pDB->execDML(buf);
	}

	catch(CppSQLite3Exception e)
	{
		cerr<<"CServerInfoTable::Create:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return 0;
}

int CServerInfoTable::SetServerInfo(const string name, const string ip, const int port)
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
		if(IsServerInfoExist("name", name))
		{
			sprintf(buf, "update %s set ip = '%s', port = %d where name = '%s'", m_strTableName.c_str(), ip.c_str(), port, name.c_str());
		}
		else
		{
			sprintf(buf, "insert into %s (name, ip, port) values ('%s', '%s', %d)", 
				m_strTableName.c_str(), name.c_str(), ip.c_str(), port);
		}	
		m_pDB->execDML(buf);
	}

	catch(CppSQLite3Exception e)
	{
		cerr<<"CServerInfoTable::SetServerInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return 0;
}

int CServerInfoTable::GetServerInfo(string& name, string& ip, int& port)
{
	if(!m_pDB)
	{
		cout<<"Invalid Sqlite"<<endl;
		return -1;
	}
	try
	{
		char buf[MAX_BUF_LEN];
		memset(buf, 0 ,MAX_BUF_LEN);
		sprintf(buf, "select * from %s where name = '%s'", 
			m_strTableName.c_str(), name.c_str());

		CppSQLite3Query q = m_pDB->execQuery(buf);
		if (!q.eof())
		{
			ip = q.fieldValue("ip");
			port = q.getIntField("port");
		}
		else
		{
			return -1;
		}
	}

	catch(CppSQLite3Exception e)
	{
		cerr<<"CServerInfoTable::GetServerInfo:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return 0;
}
/* 
bool CServerInfoTable::IsServerInfoExist(string name)
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
		sprintf(buf, "select count(*) from %s where name = '%s'", m_strTableName.c_str(), name.c_str());
		int count = m_pDB->execScalar(buf);
		if(count > 0)
			return true;
	}

	catch(CppSQLite3Exception e)
	{
		cerr<<"CServerInfoTable::IsServerInfoExist:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return false;
}
*/
