#include <iostream>
#include "controlDB/CCtrlDBHandler.h"
#include "controlDB/CppSQLite3.h"
#include "controlDB/CTableHandler.h"

using namespace std;

CCtrlDBHandler::CCtrlDBHandler()
{
	m_pDB = new CppSQLite3DB();
}

CCtrlDBHandler::~CCtrlDBHandler()
{

}

int CCtrlDBHandler::OpenDBFile(const char* filename)
{
	try
	{
		m_pDB->open(filename);
	}
	catch(CppSQLite3Exception e)
	{
		cerr<<"CCtrlDBHandler::OpenDBFile:"<<e.errorCode()<<" "<<e.errorMessage()<<endl;
	}

	return 0;
}

int CCtrlDBHandler::RegisterTable(string tableName, CTableHandler* pTableHandler)
{
	map<string, CTableHandler*>::iterator it = m_TableMap.find(tableName);
	if(it != m_TableMap.end())
		return -1;

	m_TableMap[tableName] = pTableHandler;
	
	pTableHandler->SetSQLite3DBHandler(m_pDB);
	pTableHandler->Create(tableName.c_str());

	return 0;
}

CTableHandler* CCtrlDBHandler::GetTableHandler(string tableName)
{
	map<string, CTableHandler*>::iterator it = m_TableMap.find(tableName);
	if(it == m_TableMap.end())
		return NULL;

	return it->second;
}