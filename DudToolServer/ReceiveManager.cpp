#include "ReceiveManager.h"
 
ReceiveManager* ReceiveManager::m_pInstance = nullptr;

ReceiveManager::ReceiveManager()
{
    m_spThreadPool = std::make_shared<MyThreadPool>(200);
}

ReceiveManager* ReceiveManager::Instance()
{
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [&] {m_pInstance = new  ReceiveManager; });
    return m_pInstance;
}

ReceiveManager::~ReceiveManager()
{
    if (m_pInstance != nullptr)
    {
        delete m_pInstance;
    }
}

std::shared_ptr<ReceiveData> ReceiveManager::GetReceiveData(const string& sID)
{
    std::lock_guard< std::mutex> lock(m_mutex);
    if (m_mapReceiveData.find(sID) != m_mapReceiveData.end())
    {
        return m_mapReceiveData[sID];
    }     
    return nullptr;
}

void ReceiveManager::AddReceiveData(const string sID, std::shared_ptr<ReceiveData> spReceiveData)
{
    std::lock_guard< std::mutex> lock(m_mutex);
    m_mapReceiveData[sID] = spReceiveData;
}

void ReceiveManager::DeleteReceiveData(const string sID, bool bFailed)
{
    {
        std::lock_guard< std::mutex> lock(m_mutex);
        if (m_mapReceiveData.find(sID) == m_mapReceiveData.end())
        {
            return;
        }
        if (bFailed)
        {
            auto sPath = m_mapReceiveData[sID]->GetFilePath();
            remove(sPath.c_str());
        }
        m_mapReceiveData[sID]->EndReceive();
        m_mapReceiveData.erase(sID);
    }
    
}
void ReceiveManager::SaveFile(string sUID, string sDescription, char*pData, int64_t iLength)
{
    m_spThreadPool->enqueue(&ReceiveManager::SaveImp, this, sUID, sDescription, pData, iLength);
}

void ReceiveManager::SaveImp(string sUID, string sDescription, char* pData, int64_t iLength)
{
    string s = R"(\)";
    auto sPath = "UserData" + s + sDescription;
    FILE* pFile;
    fopen_s(&pFile, sPath.data(), "wb");
    if (pFile == nullptr)
    {
        std::cout << "打开文件失败" << std::endl;
        return;
    }
    fwrite(pData, sizeof(char), iLength, pFile);
    fclose(pFile);
    DeleteReceiveData(sUID, false);

    std::cout << "删除：" << sUID << endl;


}
