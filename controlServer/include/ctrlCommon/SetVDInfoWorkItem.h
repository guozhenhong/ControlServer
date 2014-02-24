#ifndef SetVDInfoWorkItem_H
#define SetVDInfoWorkItem_H

#include <stdint.h>
#include "common/sys/ThreadPoolWorkItem.h"
#include "ManageServerCreateVDMessage.h"
#include "SetVDStatusMessage.h"


#include <string>
using std::string;

#define CREATE_VD 1
#define CH_DEC_CAP 2
#define CH_USED_CAP 3
#define CH_FS_USED_CAP 4
#define CH_STATUS 5
#define CH_FS_Type 6
#define DEL_VD 7

class SetVDInfoWorkItem : public ThreadPoolWorkItem
{
public:
    SetVDInfoWorkItem(ManageServerCreateVDMessage *p);
    SetVDInfoWorkItem(SetVDStatusMessage *p);
    // SetVDInfoWorkItem(uint32_t vdID, uint32_t cmd, uint32_t value);
    SetVDInfoWorkItem(uint32_t cmd, uint32_t vdID, uint64_t value);
    // SetVDInfoWorkItem(uint32_t vdID, uint32_t cmd, const string& value);

    ~SetVDInfoWorkItem();

    uint32_t getVDID();
    uint32_t getRes();

    int process();

private:
    Message *m_pMsg;
    uint32_t m_iOPRes;
    uint32_t m_iCmd;
    uint32_t m_iVDID;
    uint64_t m_iValue;
};

#endif 
