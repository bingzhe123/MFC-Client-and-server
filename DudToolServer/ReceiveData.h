#ifndef RECEIVE_DATA_H_
#define RECEIVE_DATA_H_
#include "httplib/httplib.h"
//#include <atomic>
#include <string>
#include "ThreadPool.h"
#include "DBOperator.h"
using std::string;
using namespace httplib;
using namespace std;

enum class StatusCode
{
    Finished,
    WatingData,
    Failed
};

class ReceiveData
{
public:
    using WaitingTimeOut = std::function<void(const string & sUID)>;
    ReceiveData(int64_t iTotalSize, 
        const string& sFileName, 
        const string& sUID, 
        const string &sUserName,
        std::shared_ptr<DBOperator> spDataOperator);
    ~ReceiveData();
public:
    StatusCode Recevie(const Request& req, Response& res);
    bool AddNewFileInfoToDB(const Request& req, Response& res, int64_t &iNewID);
    void StartTimeOutThread();
    void EndReceive();
    string GetFilePath() const;
    string GetUID();
    static string GetRealFilePath(const string sPath);
private:
    void WaitingThread();
public:
    WaitingTimeOut m_funWatingTimeOut = nullptr;

    std::shared_ptr<DBOperator>      m_spDBOperator;

private:
    int64_t m_iReceivedData = 0;
    int64_t m_iTotalLength;
    string  m_sFileName;
    string  m_sFilePath;
    string m_sUID;
    string m_sUserName;
    std::mutex  m_mutex;

    //计时线程
    std::condition_variable m_condition;
    std::condition_variable m_condition_close;
    std::mutex m_mutex_close;

    bool  m_bProcessing = false;
    bool m_bWaiting = true;
    bool m_bFinshed = false;
    FILE* m_pFile = nullptr;
};
#endif
