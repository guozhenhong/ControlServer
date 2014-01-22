#ifndef GETSUCAPACITYINFOWORKITEM_H
#define GETSUCAPACITYINFOWORKITEM_H

#include "common/sys/ThreadPoolWorkItem.h"
#include "GetSUCapacityInfoMessage.h"

#include <string>
using std::string;


class GetSUCapacityInfoWorkItem : public ThreadPoolWorkItem
{
public:
	GetSUCapacityInfoWorkItem(GetSUCapacityInfoMessage *p);
	~GetSUCapacityInfoWorkItem();

	int process();

	GetSUCapacityInfoACKMessage *GetACKMsg();

private:
	GetSUCapacityInfoMessage *m_pMsg;
	GetSUCapacityInfoACKMessage *m_pACKMsg;
};

#endif 
