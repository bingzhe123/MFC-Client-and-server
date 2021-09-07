#include "ReceiveData.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include "Directory Help.h"
#include "ReceiveManager.h"
#include <algorithm>
const int Wait_Time_Out = 1000*60;

ReceiveData::ReceiveData(int64_t iTotalSize, 
    const string& sFileName,
    const string &sUID,
    const string &sUserName,
    std::shared_ptr<DBOperator> spDataOperator)
{
    m_iTotalLength = iTotalSize;
    m_sFileName = sFileName;
   // m_spBuffer.reset(new char[iTotalSize], std::default_delete<char[]>());
    m_sUID = sUID;
    m_sUserName = sUserName;
    m_spDBOperator = spDataOperator;
}
ReceiveData::~ReceiveData()
{
    try
    {
        std::cout << __FUNCTION__ << " begin" << std::endl;
        std::unique_lock<std::mutex> lk(m_mutex_close);
        m_condition_close.wait(lk, [&]() {return !m_bWaiting; });
        if (nullptr != m_pFile)
        {
            fclose(m_pFile);
        }
    }
    catch (...)
    {

    }
    std::cout << __FUNCTION__ << " end" << std::endl;

}

void ReceiveData::StartTimeOutThread()
{
    m_bWaiting = true;
    m_bFinshed = false;
    thread t(std::bind(&ReceiveData::WaitingThread, this));
    t.detach();
}

void ReceiveData::EndReceive()
{
    std::lock_guard<std::mutex> lk(m_mutex);
    std::cout << __FUNCTION__ << " begin" << std::endl;
    m_bProcessing = false;
    m_bFinshed = true;
    m_condition.notify_all();
    std::cout << __FUNCTION__ << " end" << std::endl;

}
string ReceiveData::GetFilePath() const
{
    return m_sFilePath;
}

string ReceiveData::GetUID()
{
    return m_sUID;
}

StatusCode ReceiveData::Recevie(const Request& req, Response& res)
{
    m_bProcessing = true;
   
    auto &header = req.headers;

    string sOffset;
    auto it = header.find("Offset");
    if (it != header.end())
    {
        sOffset = it->second;
    }
    assert(!sOffset.empty());
    int64_t iOffset = _atoi64(sOffset.data());

    string sCurrentSize;
    it = header.find("CurentSize");
    if (it != header.end())
    {
        sCurrentSize = it->second;
    }
    assert(!sCurrentSize.empty());
    int64_t iCurrentSize = _atoi64(sCurrentSize.data());

    if (m_pFile == nullptr)
    {
        string s = R"(/)";
        auto sPath = "UserData" + s + m_sUserName;
        std::replace(sPath.begin(), sPath.end(), '\\', '/');

        if (!MkDir(sPath))
        {
            std::cout << __FUNCTION__ << " MkDir failed" << std::endl;
            return StatusCode::Failed;
        }               
        auto sFile = sPath + s + m_sFileName;
        //解决重名的问题
        m_sFilePath = GetRealFilePath(sFile);

        fopen_s(&m_pFile, m_sFilePath.data(), "wb");
        if (m_pFile == nullptr)
        {
            std::cout << __FUNCTION__ << " foen file failed" << std::endl;
            return StatusCode::Failed;
        }
    }
    fwrite(req.body.data(), sizeof(char), iCurrentSize, m_pFile);

    int64_t iTemp;
    {
        std::lock_guard< std::mutex> lock(m_mutex);
        m_iReceivedData += iCurrentSize;
        iTemp = m_iReceivedData;
    }
    
    std::cout << "-------------TotalReceive = " << m_iReceivedData
        <<" m_iTotalLength = "<< m_iTotalLength<<std::endl;
    auto sConent = std::to_string(m_iReceivedData);
    res.set_content(sConent, "ReceivetData");
    if (iTemp == m_iTotalLength)
    {
        int64_t iNewID;
        if (!AddNewFileInfoToDB(req, res, iNewID))
        {
            std::cout << __FUNCTION__ << " add to db failed" << std::endl;
            return StatusCode::Failed;
        }
        m_bProcessing = false;
        m_condition.notify_all();

        return StatusCode::Finished;

    }          
    std::unique_lock<std::mutex> lk(m_mutex);
    m_bProcessing = false;
    m_condition.notify_all();
    return StatusCode::WatingData;
}

bool ReceiveData::AddNewFileInfoToDB(const Request& req, Response& res, int64_t& iNewID)
{
    auto& header = req.headers;
    string sNewInfo;
    auto bRes = m_spDBOperator->AddUserData(m_sUserName,
        header,
        m_sFilePath, iNewID, sNewInfo);

    string sDataOwerID;
    auto it = header.find("FileOwerID");
    if (it != header.end())
    {
        sDataOwerID = it->second;
    }
    assert(!sDataOwerID.empty());
    m_spDBOperator->AppendDataId2DataInfo(sDataOwerID, std::to_string(iNewID));

    auto& resHeader = res.headers;
    res.headers.emplace("NewDataInfo", std::move(sNewInfo));
    return bRes;
}

void ReceiveData::WaitingThread()
{
    std::cout << __FUNCTION__ << "begin" << std::endl;
    while (m_bWaiting)
    {
        bool bTimeOut = true;
        std::unique_lock<std::mutex> lk(m_mutex);

        auto t0 = chrono::system_clock::now();
        auto t1 = t0 + std::chrono::milliseconds(Wait_Time_Out);
        std::cout << __FUNCTION__ << " wait_until begin" << std::endl;
        if (m_bFinshed)
        {
            break;
        }
        auto status = m_condition.wait_until(lk, t1);
        bTimeOut = status == cv_status::timeout;
        std::cout << __FUNCTION__ << " wait_until end m_bProcessing = "
            << m_bProcessing << "bTimeOut = "<< bTimeOut 
            <<"m_bFinshed = "<< m_bFinshed <<std::endl;
        if (m_bFinshed)
        {
            break;
        }
        if (bTimeOut && !m_bProcessing)
        {
            //必须要放到线程中，否则会和EndRecive函数 产生死锁
            std::thread t([this]()
            {
                if (m_funWatingTimeOut)
                {
                    m_funWatingTimeOut(m_sUID);
                }
            });
            t.detach();
            break;
        }

    }
    std::cout << __FUNCTION__ << " while end " << std::endl;

    std::unique_lock<std::mutex> lk(m_mutex_close);
    m_bWaiting = false;
    std::cout << __FUNCTION__ << "end" << std::endl;
    m_condition_close.notify_all();
}

string ReceiveData::GetRealFilePath(const string sPath)
{
    string sTemp = sPath;
    auto iRtn = _access(sTemp.c_str(), 0);
    while (iRtn == 0)
    {
        auto iPos = sTemp.rfind('.');
        sTemp = sTemp.substr(0, iPos) + "_copy" + sTemp.substr(iPos);
        iRtn = _access(sTemp.c_str(), 0);
    }
    return sTemp;
}
