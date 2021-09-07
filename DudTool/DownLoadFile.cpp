#include "pch.h"
#include "DownLoadFile.h"
#include "CommonDef.h"
#include "url.hpp"
#include <iostream>
#include "CProgressManager.h"
using std::cout;
using std::endl;


DownLoadFile::DownLoadFile(std::shared_ptr<Client> spClient,
    std::shared_ptr<CStdioFile> spFile,
    const CString& sProgressID)
{
    m_spClient = spClient;
    m_progress = std::bind(&DownLoadFile::Process_file,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    //m_progress = progress;
    m_spStdioFile = spFile;
    m_sProgressID = sProgressID;
}

DownLoadFile::DownLoadFile(const string& sHostName, int iPort)
    :m_sHostName(sHostName), m_iPort(iPort)
{
}

bool DownLoadFile::DownLoad(const string& sUID)
{
    ContentReceiver receview = std::bind(&DownLoadFile::ContentReciver2File,
        this,
        std::placeholders::_1,
        std::placeholders::_2);

    Headers header;
    header.emplace("UID", sUID);
    auto res = m_spClient->Get("/DownLoad", header, receview, m_progress);
    if (res == nullptr || res->status != 200)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void DownLoadFile::DownLoadURL(const string& sUrl)
{
    Url u(sUrl);
    auto host = u.host();
    auto sPath = u.path();
    auto sPort = u.port();
    auto scheme = u.scheme();
    if (scheme != "http" && scheme != "https")
    {
        std::cout << "scheme错误，不是http，也不是https" << endl;
        return;
    }
    auto index = sPath.find_last_of("/");
    m_sFileName = sPath.substr(index + 1);
    if (m_sFileName.size() == 0)
    {
        std::cout << "没有文件" << endl;
        return;
    }
    int port = atol(sPort.data());
    ContentReceiver receview = std::bind(&DownLoadFile::ContentReciver2Buffer,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    Progress progress = std::bind(&DownLoadFile::Process_buffer,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    Headers header;
    sPath = "." + sPath;
    header.emplace("Path", sPath);
    auto res = m_spClient->Get("/DownLoad", header, receview, progress);
    if (res == nullptr || res->status != 200)
    {
        std::cout << "下载失败" << std::endl;
    }
    else
    {
        std::cout << "下载成功" << std::endl;   
    }

}

bool DownLoadFile::ContentReciver2Buffer(const char* data, size_t data_length)
{
    if (m_spData == nullptr)
    {
        memcpy(m_buffer, data, data_length);
    }
    else
    {
        auto pData = m_spData.get();
        memcpy(pData + m_iCurrent, data, data_length);
    }
    return true;
}
bool DownLoadFile::ContentReciver2File(const char* data, size_t data_length)
{
    m_spStdioFile->Write(data, data_length);
    return true;
}
bool DownLoadFile::Process_file(uint64_t current, uint64_t total)
{
    m_iCurrent = current;
    auto iPos = ((double)current / total) * 100;
    CDownloadProgressManager::Instance().UpdatePos(m_sProgressID, iPos);
    return true;
}
bool DownLoadFile::Process_buffer(uint64_t current, uint64_t total)
{
    std::cout << "Current:" << current << "  total:"
        << total << " finished:" << 100 * float(current) / total << "%" << std::endl;
    if (nullptr == m_spData)
    {
        m_itotal = total;
        m_spData = std::make_unique<char []>(total);
        auto pData = m_spData.get();
        memcpy(pData, m_buffer, current);
    }

    m_iCurrent = current;
    if (m_iCurrent == total)
    {
        FILE* pFile;
        fopen_s(&pFile, m_sFileName.data(), "wb");
        if (pFile == nullptr)
        {
            return false;
        }
        fwrite(m_spData.get(), sizeof(char), m_iCurrent, pFile);
        fclose(pFile);
        std::cout << "保存文件成功" << endl;  
    }

    return true;
}