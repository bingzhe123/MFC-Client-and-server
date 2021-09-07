#include "pch.h"
#include "CProgressManager.h"
#include <cassert>
CProgressManager::CProgressManager()
{
}

void CProgressManager::AddProgress(const CString& sID, CMyprogressBar* pProgress, CWnd* pOwerWnd)
{     
    std::lock_guard<std::mutex> lk(m_mutex);
    if (m_mapProgress.find(sID) != m_mapProgress.end())
    {
        return;
    }
    ProgressInfo info;
    info.bVisible = true;
    info.iCurrentPos = 0;
    info.pOwerWnd = pOwerWnd;
    info.pProgress = pProgress;
    m_mapProgress.emplace(sID, info);
}

void CProgressManager::RemoveProgress(CString id)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_mapProgress.erase(id);       
}

void CProgressManager::RemoveProgress(CMyprogressBar* pProgress)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = std::find_if(m_mapProgress.begin(), m_mapProgress.end(), [pProgress](auto item)
    {
        return item.second.pProgress = pProgress;
    });
    if (it != m_mapProgress.end())
    {
        m_mapProgress.erase(it);
    }
}

void CProgressManager::SetProgressVisible(CMyprogressBar* pProgress, bool bVisible)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = std::find_if(m_mapProgress.begin(), m_mapProgress.end(), [pProgress](auto item)
    {
        return item.second.pProgress == pProgress;
    });
    if (it != m_mapProgress.end())
    {
        auto& info = it->second;
        info.bVisible = bVisible;
    }
}
void CProgressManager::SetProgressVisible(CString id, bool bVisible)
{
    std::lock_guard<std::mutex> lk(m_mutex);

    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end() && it->second.bVisible)
    {
        it->second.bVisible = bVisible;
    }
}
CMyprogressBar* CProgressManager::GetProgress(CString id)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end() && it->second.bVisible)
    {
        return it->second.pProgress;
    }
    return nullptr;
}
void CProgressManager::UpdatePos(CString id, int iPos)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end())
    {
        it->second.iCurrentPos = iPos;
        if (it->second.bVisible)
        {
            it->second.pProgress->SetPos(iPos);
        }
    }
}

void CProgressManager::AddNewFileInfo(CString id, CString& sFileName, int64_t iSize)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_mapProgress.find(id);  
    assert(it != m_mapProgress.end());
    if (it != m_mapProgress.end())
    {
        it->second.mapFileInThisProgress[sFileName].bFinished = false;
        it->second.mapFileInThisProgress[sFileName].SetCurrentSize(0);
        it->second.mapFileInThisProgress[sFileName].iTotalDataSize = iSize;
    }
    return;
}

void CProgressManager::UpdateDataSize(CString id, CString &sFileName, int64_t iSize)
{
    std::lock_guard<std::mutex> lk(m_mutex);

    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end())
    {
        it->second.mapFileInThisProgress[sFileName].SetCurrentSize( iSize);
        auto iCurrentSize = it->second.GetCurrentSize();
        auto iTotalSize = it->second.GetTotalSize();
        int iPos = double(iCurrentSize) / iTotalSize * 100;
        it->second.iCurrentPos = iPos;
        if (it->second.bVisible)
        {
            it->second.pProgress->SetPos(iPos);
        }
    }
}

void CProgressManager::ResetProgress(CString id, CMyprogressBar* pProgress, CWnd* pOwerWnd)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end())
    {
        it->second.pProgress = pProgress;
        it->second.bVisible = true;
        it->second.pOwerWnd = pOwerWnd;
        pProgress->SetPos(it->second.iCurrentPos);
    }
}

CWnd* CProgressManager::GetOwerWnd(CString id)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end())
    {
        return it->second.pOwerWnd;
    }
    return nullptr;
}

void CProgressManager::SetOWerWnd(CString id, CWnd* pWnd)
{
    std::lock_guard<std::mutex> lk(m_mutex);
 
    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end())
    {
        auto& info = it->second;
        info.pOwerWnd = pWnd;
    }
}

bool CProgressManager::IsFinished(CString id)
{
    std::lock_guard<std::mutex> lk(m_mutex);

    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end())
    {                             
        return it->second.IsFinished();
    }
    return false;
}

void CProgressManager::SetFinished(CString id, const CString& sFileName)
{
    std::lock_guard<std::mutex> lk(m_mutex);

    auto it = m_mapProgress.find(id);
    if (it != m_mapProgress.end())
    {
        it->second.mapFileInThisProgress[sFileName].bFinished = true;
    }
}

CDownloadProgressManager& CDownloadProgressManager::Instance()
{
    static CDownloadProgressManager progressManager;
    return progressManager;
}


CUploadProgressManager& CUploadProgressManager::Instance()
{
    static CUploadProgressManager progressManager;
    return progressManager;
}
