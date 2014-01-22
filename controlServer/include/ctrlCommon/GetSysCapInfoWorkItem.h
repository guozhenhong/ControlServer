#ifndef GETSYSCAPINFOWORKITEM_H
#define GETSYSCAPINFOWORKITEM_H

#include "common/sys/ThreadPoolWorkItem.h"
#include "GetSysCapMessage.h"

#include <string>
using std::string;


class GetSysCapInfoWorkItem : public ThreadPoolWorkItem
{
public:
	GetSysCapInfoWorkItem(GetSysCapMessage *p);
	~GetSysCapInfoWorkItem();

	int process();

	GetSysCapACKMessage* GetACKMessage();

private:
	GetSysCapMessage *m_pMsg;
	GetSysCapACKMessage *m_pACKMsg;
};

#endif 
