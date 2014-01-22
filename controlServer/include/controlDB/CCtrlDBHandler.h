#ifndef CCTRLDBHANDLER_H
#define CCTRLDBHANDLER_H

#include <map>
#include <string>
#include "common/sys/Singleton.h"

#define DEFAULT_SERVER_INFO_TABLE "Server_Info_Table"
#define DEFAULT_DISK_INFO_TABLE "Disk_Info_Table" 
#define DEFAULT_STORAGE_INFO_TABLE "SU_Info_Table"

using namespace std;

class CTableHandler;
class CppSQLite3DB;

class CCtrlDBHandler : public Singleton<CCtrlDBHandler>
{
	friend class Singleton<CCtrlDBHandler>;
public:
	int OpenDBFile(const char* filename);
	int RegisterTable(string tableName, CTableHandler* pTableHandler);
	CTableHandler* GetTableHandler(string tableName);

private:
	CCtrlDBHandler();
	~CCtrlDBHandler();

private:
	CCtrlDBHandler(const CCtrlDBHandler &);
	CCtrlDBHandler& operator=(const CCtrlDBHandler &);

private:
	CppSQLite3DB *m_pDB;
	map<string, CTableHandler*> m_TableMap;
};


#endif
