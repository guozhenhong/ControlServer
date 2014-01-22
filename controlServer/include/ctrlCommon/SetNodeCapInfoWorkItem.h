#ifndef SetNodeCapInfoWorkItem_H
#define SetNodeCapInfoWorkItem_H

#include "common/sys/ThreadPoolWorkItem.h"
#include "NodeCapacityInfoMessage.h"

#include <string>
using std::string;


class SetNodeCapInfoWorkItem : public ThreadPoolWorkItem
{
public:
	SetNodeCapInfoWorkItem(string& name, NodeCapacityInfoMessage *p);
	~SetNodeCapInfoWorkItem();

	int process();

private:
	NodeCapacityInfoMessage *m_pMsg;
	string m_strNodeName;
};

#endif 
