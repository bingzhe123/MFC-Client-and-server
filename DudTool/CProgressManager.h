#pragma once
#include "CMyprogressBar.h"
#include <mutex>
#include <map>
class ProgressFileInfo
{
public:
    void SetCurrentSize(int64_t iSize)
    {
        iCurrentDataSize = iSize;
    }
    int64_t GetCurrentSize()
    {
        return iCurrentDataSize;
    }
private:
    int64_t iCurrentDataSize = 0;
public:
    int64_t iTotalDataSize = 0;
    bool bFinished = true;
};
class ProgressInfo
{
public:
    CMyprogressBar* pProgress = nullptr;
    int iCurrentPos = 0;
    bool bVisible = false;
    CWnd* pOwerWnd = nullptr; //对应的dlg窗口
    std::map<CString, ProgressFileInfo> mapFileInThisProgress; //当前progress控制几个文件

    int64_t GetTotalSize()
    {
        int64_t iSize = 0;
        for each (auto item in mapFileInThisProgress)
        {
            iSize += item.second.iTotalDataSize;
        }
        return iSize;
    }
    int64_t GetCurrentSize()
    {
        int64_t iSize = 0;
        for each (auto item in mapFileInThisProgress)
        {
            iSize += item.second.GetCurrentSize();
        }
        return iSize;
    }
    bool IsFinished()
    {
        for each (auto item in mapFileInThisProgress)
        {
            if (!item.second.bFinished)
            {
                return false;
            };
        }
        return true;
    }
};
class CProgressManager
{
public:
    void AddProgress(const CString &sID,  CMyprogressBar* pProgress, CWnd *pOwerWnd);
    void RemoveProgress(CString id);
    void RemoveProgress(CMyprogressBar* pProgress);
    void SetProgressVisible(CString id, bool bVisible);
    void SetProgressVisible(CMyprogressBar* pProgress, bool bVisible);
    CMyprogressBar* GetProgress(CString id);
    void UpdatePos(CString id, int iPos);
    void UpdateDataSize(CString id, CString& sFileName, int64_t iSize);
    void AddNewFileInfo(CString id, CString &sFileName, int64_t iSize);

    //切换页面的时候当前正在下载的进度条被删除，当切回来的时候，要显示出来
    void ResetProgress(CString id, CMyprogressBar* pProgress, CWnd *pOwerWnd);
    CWnd* GetOwerWnd(CString id);
    void SetOWerWnd(CString id, CWnd* pWnd);
    bool IsFinished(CString id);
    void SetFinished(CString id, const CString& sFileName);
protected:
    CProgressManager();
    CProgressManager& operator=(const CProgressManager&) = delete;
    std::mutex m_mutex;
    std::map<CString, ProgressInfo> m_mapProgress;
};

class CDownloadProgressManager :public CProgressManager
{
public:
    static CDownloadProgressManager& Instance();
};

class CUploadProgressManager :public CProgressManager
{
public:
    static CUploadProgressManager& Instance();
};