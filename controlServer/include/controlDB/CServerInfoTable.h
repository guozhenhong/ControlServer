#ifndef CSERVERINFOTABLE_H
#define CSERVERINFOTABLE_H

#include "CTableHandler.h"

using namespace std;

class CServerInfoTable : public CTableHandler
{
public:
	CServerInfoTable();
	virtual ~CServerInfoTable();

	virtual int CreateTable();

	int SetServerInfo(const string name, const string ip, const int port);
	int GetServerInfo(string& name, string& ip, int& port);

private:
//	bool IsServerInfoExist(string name);

private:
	CServerInfoTable(const CServerInfoTable&);
	CServerInfoTable& operator=(const CServerInfoTable&);
};

#endif
