#include "pch.h"
//#include "commdef.h"
#include "CUploadFile.h"
#include <map>
#include <iostream>
#include <iomanip>
#include "CProgressManager.h"

//#include <afxwin.h>

typedef std::map<std::string, int> map_int_t;
typedef std::map<std::string, std::string> map_str_t;
//UploadFile::UploadFile(string sHostName, int iPort)
//{ 
//    m_sHostName = sHostName;
//    m_iPort = iPort;
//#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
//    m_spClient = std::make_shared< httplib::SSLClient>(m_sHostName, m_iPort);
//    auto spClient = std::dynamic_pointer_cast<httplib::SSLClient>(m_spClient);
//
//    spClient->set_ca_cert_path(CA_CERT_FILE);
//    spClient->enable_server_certificate_verification(true);
//#else
//    m_spClient = std::make_shared<Client>(m_sHostName, m_iPort);
//#endif
//}
UploadFile::UploadFile(std::shared_ptr<Client> spClient,
    const CString& sProgressID,
    const CString& sFilePath)
{
    m_spClient = spClient;
    m_sProgressID = sProgressID;
    m_sFilePath = sFilePath;
}
bool UploadFile::SendData(FILE* pFile, UploadFileInfo &info)
{
    m_uploadFileInfo = info;
    bool bSuccess =  SplitTask(pFile, info.iFileLength);
    info.sServerResponds = m_uploadFileInfo.sServerResponds;
    //CUploadProgressManager::Instance().UpdateDataSize(m_sProgressID, m_sFilePath,0);
    return bSuccess;
}


bool UploadFile::SplitTask(FILE* pFile, int64_t iLength)
{ 
#ifdef WIN32
    GUID guid;
    CoCreateGuid(&guid);
#else
    TmUUID_t    guid;
    TmMsg_CreateUUID(&guid);
#endif                                
    char guidbuf[1024] = { 0 }; 
    sprintf_s(guidbuf, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1],
        guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5],
        guid.Data4[6], 
        guid.Data4[7]);

    int64_t iSubSize = SUB_TASK_DATA_SIZE;
    auto iTempSize = iLength / 100;
    //int iMinSize = 1024 * 1024;
    //if (iTempSize < iMinSize)
    //{

    //}
    auto iSubDataSize = iSubSize > iTempSize ? iSubSize : iTempSize;
    //auto  iSubDataSize = SUB_TASK_DATA_SIZE;
    auto iBlockSize = iLength / iSubDataSize;
    iBlockSize = iBlockSize == 0 ? 1:iBlockSize;
    bool bSuccess;
    for (size_t i = 0; i < iBlockSize-1; i++)
    {
        auto iOffset = i * iSubDataSize;
        auto blockInifo = InitializeBlockInfo(m_spClient,
            iOffset,
            iSubDataSize,
            iLength,
            guidbuf);
        bSuccess = TaskFunc(pFile, blockInifo, iLength);
        if (!bSuccess)
        {
            return bSuccess;
        }
    }
    auto iLeftSize = iLength - (iBlockSize - 1) * iSubDataSize;
    auto iOffset = (iBlockSize - 1) * iSubDataSize;
    auto blockInifo = InitializeBlockInfo(m_spClient,
        iOffset,
        iLeftSize,
        iLength,
        guidbuf);
    return TaskFunc(pFile, blockInifo, iLength);

}

bool UploadFile::TaskFunc(FILE* pFile, const BlockInfo &info, int64_t iLength )
{
     auto client = info.m_spClient;
    auto res = client->Post("/ClientUploadFile",
        info.m_header,
        size_t(info.m_iDataSize),
        [&](size_t offset, size_t length, httplib::DataSink& sink)->bool
    {
        const size_t DATA_CHUNK_SIZE = 4096;
        char pData[DATA_CHUNK_SIZE];
        auto iLength = (std::min)(length, DATA_CHUNK_SIZE);
        auto iReadSize = fread(pData, sizeof(char), iLength, pFile);//   file.Read(pData, iLength);
        sink.write(pData, iLength);
        return true;
        //std::cout << "Write Data:" << offset << std::endl;
    },
        "application/Dicom");
    
   // std::cout << "Response Data:" << res->body << std::endl;
    bool bSuccess = false;
    if (res == nullptr || res->body == "Disconnect")
    {
        MessageBox(NULL, L"已经失去连接", L"错误", MB_OK);
        std::cout << "-----------Has Disconnected " << std::endl;
    }
    else
    {
        string sReceived = res->body;
        char* pStopString;
        auto iReceivedLength = _strtoi64(sReceived.c_str(), &pStopString, 10);

        std::cout << "TotalData:" << iLength << "  This time Send Data:" <<
            info.m_iDataSize << "  Total SendData" << iReceivedLength << std::endl;

        std::cout << "-----------Has Send " << setprecision(3) << 100 * (float)iReceivedLength / iLength << "%" << std::endl;
        auto iPos = ((double)iReceivedLength / iLength) * 100;
        CUploadProgressManager::Instance().UpdateDataSize(m_sProgressID, m_sFilePath, iReceivedLength);

        auto it = res->headers.find("NewDataInfo");
        if (it != res->headers.end())
        {
            m_uploadFileInfo.sServerResponds = it->second;
        }
        bSuccess = true;

    }
    return bSuccess;
}
BlockInfo UploadFile::InitializeBlockInfo(std::shared_ptr<Client> spClient,
    int64_t iDataOffset,
    int64_t iCurrentDataSize,
    int64_t iTotalLength,
    const string& sGUID)
{
    BlockInfo info;
    info.m_iDataOffset = iDataOffset;
    info.m_iDataSize = iCurrentDataSize;
    //info.m_pData = pData + info.m_iDataOffset;
    info.m_spClient = spClient;
   
    info.m_header.emplace("ID", sGUID);
    auto sOffet = std::to_string(info.m_iDataOffset);
    info.m_header.emplace("OffSet", sOffet);
    auto sTotalLength = std::to_string(iTotalLength);
    info.m_header.emplace("TotalLength", sTotalLength);
    auto sCurentSize = std::to_string(iCurrentDataSize);
    info.m_header.emplace("CurentSize", sCurentSize);

    info.m_header.emplace("FileOwerID", m_uploadFileInfo.sFileOwerID);
    info.m_header.emplace("UserOwerID", m_uploadFileInfo.sUserOwerID);
    info.m_header.emplace("FileName", m_uploadFileInfo.sFileName);
    return info;
}