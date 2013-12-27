#ifndef REGISTERWORKITEM_H
#define REGISTERWORKITEM_H

#include "common/sys/ThreadPoolWorkItem.h"
#include "RegisterServerMessage.h"

#include <string>
using std::string;


class RegisterWorkItem : public ThreadPoolWorkItem
{
public:
	RegisterWorkItem(RegisterServerMessage *p, string& ip);
	~RegisterWorkItem();

	int process();

	bool IsRegisterSuccess() const;

private:
	RegisterServerMessage *m_pMsg;
	bool m_bRegisterResult;
	string m_strIP;
};

#endif 
