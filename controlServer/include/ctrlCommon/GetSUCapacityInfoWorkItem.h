#ifndef GETSUCAPACITYINFOWORKITEM_H
#define GETSUCAPACITYINFOWORKITEM_H

#include "controlDB/CTableHandler.h"
#include "common/sys/ThreadPoolWorkItem.h"
#include "GetSUCapacityInfoMessage.h"

#include <string>
using std::string;


class GetSUCapacityInfoWorkItem : public ThreadPoolWorkItem
{
public:
	GetSUCapacityInfoWorkItem(GetSUCapacityInfoMessage *p);
	GetSUCapacityInfoWorkItem(string& strNodeName); 
	~GetSUCapacityInfoWorkItem();

	int process();

	uint64_t GetSumCapacity();
	uint64_t GetDeclareCapacity();
	uint64_t GetUsedCapacity();

	GetSUCapacityInfoACKMessage *GetACKMsg();

private:
	GetSUCapacityInfoMessage *m_pMsg;
	sCapacityInfo *m_pSUCapInfo;
	string m_strNodeName;
};

#endif 
