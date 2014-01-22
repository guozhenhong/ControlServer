#ifndef CTABLEHANDLER_H
#define CTABLEHANDLER_H

#define MAX_BUF_LEN 300

#include <string>
class CppSQLite3DB;

typedef struct capInfo
{
    uint64_t m_sumCap;
    uint64_t m_declaredCap;
    uint64_t m_usedCap;
    capInfo() : m_sumCap(0), m_declaredCap(0), m_usedCap(0)
    {}
    capInfo(uint64_t s, uint64_t d, uint64_t u) : m_sumCap(s), m_declaredCap(d), m_usedCap(u)
    {}

}sCapacityInfo;


class CTableHandler
{
public:
	CTableHandler();
	virtual ~CTableHandler();

	virtual int Create(const char* pName);
	virtual int Delete();
	virtual int SetSQLite3DBHandler(CppSQLite3DB* pDB);

    virtual bool IsServerInfoExist(std::string field, std::string name);
    virtual bool IsServerInfoExist(std::string field, int ID);
protected:
	virtual int CreateTable() = 0;

protected:
	CppSQLite3DB *m_pDB;
	std::string m_strTableName;

private:
	CTableHandler(const CTableHandler&);
	CTableHandler& operator=(const CTableHandler&);
};

#endif
