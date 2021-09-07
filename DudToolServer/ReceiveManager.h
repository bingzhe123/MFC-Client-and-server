#ifndef RECEIVE_MANAGER_H_
#define RECEIVE_MANAGER_H_

#include <map>
#include <string>
#include <share.h>
#include "ReceiveData.h"
using std::string;
using std::map;

class ReceiveManager
{
public:
    ReceiveManager();
    ~ReceiveManager();
    static ReceiveManager* Instance();
    std::shared_ptr<ReceiveData> GetReceiveData(const string& sID);
    void AddReceiveData(const string sID, std::shared_ptr<ReceiveData> spData);
    void DeleteReceiveData(const string sID, bool bFailed);
    void SaveFile(string sUID, string sDescription, char* pData, int64_t iLength);
    void SaveImp(string sUID, string sDescription, char* pData, int64_t iLength);
private:
    static ReceiveManager* m_pInstance;
    map<string, std::shared_ptr<ReceiveData>>      m_mapReceiveData;
    std::mutex  m_mutex;
    std::shared_ptr<MyThreadPool>  m_spThreadPool;
};

#endif // !RECEIVE_MANAGER_H_


