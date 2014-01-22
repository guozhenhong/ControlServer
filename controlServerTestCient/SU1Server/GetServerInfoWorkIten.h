#ifndef GETSERVERINFOWORKITEM_H                                                       
#define GETSERVERINFOWORKITEM_H                                                       
                                                                                 
#include "common/sys/ThreadPoolWorkItem.h"                                       
#include "GetServerInfoMessage.h"                                               
                                                                                 
#include <string>                                                                
using std::string;                                                               
                                                                                 
                                                                                 
class GetServerInfoWorkItem : public ThreadPoolWorkItem                               
{                                                                                
public:                                                                          
    GetServerInfoWorkItem(GetServerInfoMessage *p);                      
    ~GetServerInfoWorkItem();                                                         
                                                                                 
    int process();    

    const int& ID() const;
    const int& Port() const;
    const string& IP() const;                                                           
                                                                                 
    bool IsRegisterSuccess() const;                                              
                                                                                 
private:                                                                         
    GetServerInfoMessage *m_pMsg;                                               
                                                          
    string m_strIP;
    int m_iEchoID;
    int m_iServerPort;                                                              
};                                                                               
                                                                                 
#endif     