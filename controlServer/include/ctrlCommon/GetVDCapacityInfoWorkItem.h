#ifndef GetVDCapacityInfoWorkItem_H
#define GetVDCapacityInfoWorkItem_H

#include "common/sys/ThreadPoolWorkItem.h"
#include "GetVDCapacityInfoMessage.h"

#include <string>
using std::string;


class GetVDCapacityInfoWorkItem : public ThreadPoolWorkItem
{
public:
	GetVDCapacityInfoWorkItem(GetVDCapacityInfoMessage *p);
	~GetVDCapacityInfoWorkItem();

	int process();

	GetVDCapacityInfoACKMessage *GetACKMessage();

private:
	GetVDCapacityInfoMessage *m_pMsg;
	GetVDCapacityInfoACKMessage *m_pACKMsg;
};

#endif 
