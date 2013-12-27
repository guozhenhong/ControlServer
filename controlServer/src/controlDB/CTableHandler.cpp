#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "controlDB/CTableHandler.h"
#include "controlDB/CppSQLite3.h"

using namespace std;

CTableHandler::CTableHandler()
{

}

CTableHandler::~CTableHandler()
{

}

int CTableHandler::Create(const char* pName)
{
	m_strTableName = pName;

	bool isTableExist = m_pDB->tableExists(pName);
	if(true == isTableExist)
		return 0;

	return CreateTable();
}

int CTableHandler::Delete()
{
	if(!m_pDB)
	{
		cout<<"Invalid Sqlite"<<endl;
		return -1;
	}
	try
	{
		char buf[200];
		memset(buf, 0, 200);
		sprintf(buf, "drop table %s", m_strTableName.c_str());
		m_pDB->execDML(buf);
	}

	catch(CppSQLite3Exception e)
	{
		cerr<<"CTableHandler::Delete:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return 0;	
}

int CTableHandler::SetSQLite3DBHandler(CppSQLite3DB* pDB)
{
	m_pDB = pDB;

	return 0;
}

bool CTableHandler::IsServerInfoExist(string field, string name)
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
		sprintf(buf, "select count(*) from %s where %s = '%s'", m_strTableName.c_str(), field.c_str(), name.c_str());
		int count = m_pDB->execScalar(buf);
		if(count > 0)
			return true;
	}

	catch(CppSQLite3Exception e)
	{
		cerr<<"CTableHandler::IsServerInfoExist:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return false;
}

bool CTableHandler::IsServerInfoExist(string field, int ID)
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
		sprintf(buf, "select count(*) from %s where %s = %d", m_strTableName.c_str(), field.c_str(), ID);
		int count = m_pDB->execScalar(buf);
		if(count > 0)
			return true;
	}

	catch(CppSQLite3Exception e)
	{
		cerr<<"CTableHandler::IsServerInfoExist:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return false;
}
