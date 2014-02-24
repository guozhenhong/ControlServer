#ifndef GetVDCapacityInfoWorkItem_H
#define GetVDCapacityInfoWorkItem_H

#include <stdint.h>
#include "common/sys/ThreadPoolWorkItem.h"
#include "GetVDCapacityInfoMessage.h"
#include "controlDB/CTableHandler.h"

#include <string>
using std::string;

class GetVDCapacityInfoWorkItem : public ThreadPoolWorkItem
{
public:
	GetVDCapacityInfoWorkItem(uint32_t vdID);
	~GetVDCapacityInfoWorkItem();

	int process();

	GetVDCapacityInfoACKMessage *GetACKMessage();
	
	uint64_t GetDeclaredCapacity();
	uint64_t GetUsedCapacity();
	uint64_t GetRealUsedCapacity();

private:
	uint32_t m_iVDID;
	sCapacityInfo *m_pCapInfo;
};

#endif 
