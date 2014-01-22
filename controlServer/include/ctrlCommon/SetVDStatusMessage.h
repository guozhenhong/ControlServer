#ifndef SetVDStatusMessage_H
#define SetVDStatusMessage_H

#include <string>
#include <string.h>
#include "Message.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "MessageID.h"
#include <stdint.h>

#define FORMAT_END "formated"
#define FORMAT_ING "formating"
#define DELETE_ING "deleting"
#define DELETE_END "deleted"

using namespace std;

class SetVDStatusMessage : public Message
{
public:	
	SetVDStatusMessage() : Message(MSG_SET_VD_STATUS)
	{

	}
	SetVDStatusMessage(uint32_t id, string vdStatus) : 
		Message(MSG_SET_VD_STATUS), m_iVDID(id), m_strStatus(vdStatus)
	{

	}

	~SetVDStatusMessage()
	{

	}

public:
	uint32_t  m_iVDID;
	string    m_strStatus; 
};

#endif