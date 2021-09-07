#include "pch.h"
#include "CFileOperatorWnd.h"
#include "CommonDef.h"
#include "CMyButtonInList.h"
#include "CUploadFile.h"
#include "DudToolClient.h"
#include "DownLoadFile.h"
#include "CFileOperatorDlg.h"
#include "CMyProgressButton.h"
#include "CProgressManager.h"
#include "CConfirmDlg.h"

void CFileOperatorTileWnd::Initialize(bool bCanUpload)
{
    SetBkGroundColor(RGB(22, 22, 23));

    m_pBtnUploadFile = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnUploadFile->SetBkGroundColor(DataShowBkColor, RGB(69, 91, 178));
    m_pBtnUploadFile->SetText(CString(L"上传文件"));
    auto clickFun = std::bind(&CFileOperatorTileWnd::OnUpload, this, std::placeholders::_1);
    m_pBtnUploadFile->RegisterClickFun(clickFun);
    if (!bCanUpload)
    {
        m_pBtnUploadFile->SetMouseHoverSensitive(false);
    }
    m_vecChildWnd.push_back(m_pBtnUploadFile);
    m_pProgressBar = new CMyprogressBar(this, m_pRootWnd, m_refreshImpFun);
    //m_pProgressBar->SetPos(80);
    m_vecChildWnd.push_back(m_pProgressBar);
}

void CFileOperatorTileWnd::OnSize(int cx, int cy)
{
    int iOffsety = 5;
    int iOffsetx = 20;
    int iBtnWidth = 200;
    auto rect = GetClientRect();
    int x = iOffsetx;
    int iBtnHeight = 50;
    int y = (rect.Height() - iBtnHeight) / 2;
    m_pBtnUploadFile->MoveWindow(x, y);
    m_pBtnUploadFile->SetDimention(iBtnWidth, iBtnHeight);

    y = rect.Height() - m_iProgressHeight - iOffsety;
    m_pProgressBar->MoveWindow(0, y);
    m_pProgressBar->SetDimention(rect.Width(), m_iProgressHeight);
}

void CFileOperatorTileWnd::OnUpload(CMyWindow* pWnd)
{
    if (m_funAddNewItem != nullptr)
    {                              
        m_funAddNewItem(m_pProgressBar);
    }
}


void CFileOperatorTileWnd::RegisterAddNewItem(UploadFileFun f)
{
    m_funAddNewItem = f;
}
CMyprogressBar* CFileOperatorTileWnd::GetProgressBar()
{
    return m_pProgressBar;
}


void CFileOperatorListWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    int w = rect.Width();
    int iHeadHeight = 50;
    m_pHeadWnd->MoveWindow(0, 0);
    m_pHeadWnd->SetDimention(w, iHeadHeight);
    m_pHeadWnd->OnSize(cx, cy);

    int iSplitHeight = 2;//每行间距
    int iTemCount = 3;
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (iTemCount - 1)) / m_iPerPageItemDataCount;

    int iOffsety = iHeadHeight + iSplitHeight;
    for (int i = 1; i < m_vecChildWnd.size(); i++)
    {
        auto* pItemWnd = m_vecChildWnd[i];
        pItemWnd->MoveWindow(0, iOffsety);
        pItemWnd->SetDimention(w, iItemHeight);
        pItemWnd->OnSize(cx, cy);
        iOffsety += iItemHeight + iSplitHeight;
    }
}

void CFileOperatorListWnd::Initialize(const CString& sDataList, const vector<CString>& vecFileInfo)
{
    m_vecOwerFileInfo = vecFileInfo;
    auto rect = GetClientRect();
    m_pHeadWnd = new CMyListHeaderWnd(this, m_pRootWnd, m_refreshImpFun);
    int w = rect.Width();
    int iHeadHeight = 50;
    vector<CString> vecHeader;
    vecHeader.push_back(L"文件名");
    vecHeader.push_back(L"文件路径");
    vecHeader.push_back(L"文件大小");
    vecHeader.push_back(L"操作");

    float fRatio = 1.0 / vecHeader.size();
    vector<float> vecRatio;
    for (size_t i = 0; i < vecHeader.size(); i++)
    {
        vecRatio.push_back(fRatio);
    }

    m_pHeadWnd->Initialize(vecHeader, vecRatio);
    m_vecChildWnd.push_back(m_pHeadWnd);

    auto vecDataList = SplitString(sDataList, '|');
    for each (auto  item in vecDataList)
    {
        auto vecRow = SplitString(item, ',');
        assert(vecRow.size() == 4);
        AddItem(vecRow);
    }
}

void CFileOperatorListWnd::OnAddNewItem(vector<CString>& vecContent)
{
}

void CFileOperatorListWnd::AddItem(vector<CString>& vecContent)
{
    if (m_mapContent.size() >= m_iPerPageItemDataCount)
    {//如果当前这一页已经满了，就不用加了
        return;
    }
    //增加一条信息
    int iHeadHeight = 50;
    auto rect = GetClientRect();
    int iSplitHeight = 2;//每行间距
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (m_iPerPageItemDataCount - 1)) / m_iPerPageItemDataCount;
    int iOffsety = iHeadHeight + iSplitHeight + (iItemHeight + iSplitHeight) * m_iCurrentPageCount;
    CMyListItemWnd* pItemWnd = new CMyListItemWnd(this, m_pRootWnd, m_refreshImpFun);

    vector<CString> vecDisplayInfo;
    vecDisplayInfo.assign(vecContent.begin()+1, vecContent.end());
    auto sPath = vecDisplayInfo[1];
    sPath = sPath.Left(sPath.ReverseFind(L'/'));
    vecDisplayInfo[1] = sPath;

    auto iDataSize = _ttoi64(vecDisplayInfo[2].GetString());
    double fDataSize = iDataSize / 1024.0;//k
    if (fDataSize > 1024.0)
    {
        fDataSize = fDataSize / 1024.0;//M
        CString stemp;
        stemp.Format(L"%.2fM", fDataSize);
        vecDisplayInfo[2] = stemp;
    }
    else
    {
        CString stemp;
        stemp.Format(L"%.2fK", fDataSize);
        vecDisplayInfo[2] = stemp;
    }
    pItemWnd->AddItem(vecDisplayInfo, m_pHeadWnd, iOffsety, iItemHeight);


    auto iIndex = m_mapContent.size() - 1;
    vector<CMyWindow*> vecButton;
    auto pBtnDownLoad = new CMyProgressButton(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnDownLoad->SetText(CString(L"下载"));
    pBtnDownLoad->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnDownload(pWnd);
    });
    vecButton.push_back(pBtnDownLoad);
    m_vecDownloadButton.push_back(pBtnDownLoad);

    auto pBtnDelete = new CMyProgressButton(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnDelete->SetText(CString(L"删除"));
    pBtnDelete->SetName(L"数据删除");
    pBtnDelete->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnDelete(pWnd);
    });
    auto sCurrentUserID = CString(theApp.m_sUserID.c_str());
    if (sCurrentUserID != m_vecOwerFileInfo[9] && !theApp.m_bIsManager)
    {
        pBtnDelete->SetMouseHoverSensitive(false);
    }

    vecButton.push_back(pBtnDelete);

    pItemWnd->AddColumnWnd(3, vecButton);
    auto& progressManager = CDownloadProgressManager::Instance();
    progressManager.ResetProgress(vecContent[0], 
        pBtnDownLoad->GetProgressBar(),
        m_pRootWnd);

    iOffsety += iItemHeight + iSplitHeight;
    m_vecChildWnd.push_back(pItemWnd);
    if (m_iCurrentPageCount % 2 == 0)
    {
        pItemWnd->SetBkPic(IDB_PNG_DATA_ITEM_LAN_BK);
    }
    else
    {
        pItemWnd->SetBkPic(IDB_PNG_DATA_ITEM_HUANG_BK);
    }
    pItemWnd->UpdateMyWnd(pItemWnd->GetClientRect());
    m_mapContent[pItemWnd] = vecContent;
    m_iCurrentPageCount++;
}

void CFileOperatorListWnd::ClearPageItems()
{
    auto& progressManager = CDownloadProgressManager::Instance();
    for each (auto item in m_vecDownloadButton)
    {
        progressManager.SetProgressVisible(item->GetProgressBar(), false);
    }
    m_iCurrentPageCount = 0;
    for each (auto item in m_vecChildWnd)
    {
        if (item != m_pHeadWnd)
        {
            delete item;
        }
        if (item == m_pCurrentMouseInWnd)
        {
            m_pCurrentMouseInWnd = nullptr;
        }
    }
    
    m_vecDownloadButton.clear();
    m_vecChildWnd.clear();
    m_vecChildWnd.push_back(m_pHeadWnd);
    m_mapContent.clear();
    UpdateMyWnd(GetClientRect());
}

void CFileOperatorListWnd::SetBottomNavigatewnd(CBottomNavigateWnd* pWnd)
{
    m_pBottom = pWnd;
}

CMyLable* CFileOperatorListWnd::CreateLable(int x, int y, int w, int h, const CString& sContent)
{
    return nullptr;
}
                       
void CFileOperatorListWnd::OnDownload(CMyWindow* pWnd)
{
    auto pParent = pWnd->GetParent()->GetParent();
    if (m_pDownLoadFun != nullptr)
    {
        auto &vecItem = m_mapContent[pParent];
        auto pProgressBar = ((CMyProgressButton*)pWnd->GetParent())->GetProgressBar();
        m_pDownLoadFun(vecItem, pProgressBar);
    }
}

void CFileOperatorListWnd::RegisterDownloadFunc(DownLoadItemFun f)
{
    m_pDownLoadFun = f;
}

void CFileOperatorListWnd::RegisterDeleteFunc(DeleteItemFun f)
{
    m_pDeleteFun = f;
}


void CFileOperatorListWnd::OnDelete(CMyWindow* pWnd)
{
    if (!theApp.m_bIsManager)
    {
        AfxMessageBox(L"没有权限删除！");
        return;
    }

    CConfirmDlg dlg;
    if (IDCANCEL == dlg.DoModal())
    {
        return;
    }
    auto pListButton = (CMyButtonInList*)pWnd;
    auto pParent = pWnd->GetParent()->GetParent();
    auto& vecItem = m_mapContent[pParent];

    CString sUID = vecItem[0];
    if (m_pDeleteFun != nullptr)
    {
        m_pDeleteFun(pWnd, sUID);
    }
    
}

CFileOperatorWnd::~CFileOperatorWnd()
{
    m_pDataShowListWnd = nullptr;
    CUploadProgressManager::Instance().SetProgressVisible(m_vecFileInfo[0], false);
    CDownloadProgressManager::Instance().SetProgressVisible(m_vecFileInfo[0], false);
}

void CFileOperatorWnd::Initialize(const vector<CString>& vecFileInfo,
    const CString &sDataList, CDataShowrListWnd* pDataShowListWnd)
{
    m_pDataShowListWnd = pDataShowListWnd;
    m_sDataList = sDataList;
    m_vecFileInfo = vecFileInfo;
    auto sCurrentUserID = CString(theApp.m_sUserID.c_str());
    bool bCanUpload = true;
    if (sCurrentUserID != m_vecFileInfo[9] && !theApp.m_bIsManager)
    {
        bCanUpload = false;
    }
    m_pTileWnd = new CFileOperatorTileWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pTileWnd->Initialize(bCanUpload);
    m_pTileWnd->RegisterAddNewItem([this](CMyprogressBar* pProgressBar)
    {
        this->OnUpload(pProgressBar);
    });
    m_vecChildWnd.push_back(m_pTileWnd);

    m_pDataListWnd = new CFileOperatorListWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pDataListWnd->Initialize(m_sDataList, vecFileInfo);
    m_pDataListWnd->RegisterDownloadFunc([this](const vector<CString>& vecItem, CMyprogressBar *pProgressBar)
    {
        this->OnDownload(vecItem, pProgressBar);
    });
    m_pDataListWnd->RegisterDeleteFunc([this](CMyWindow* pWnd, const CString& sUID)
    {
        this->OnDeleteItem(pWnd, sUID);
    });
    m_vecChildWnd.push_back(m_pDataListWnd);

    m_pBottom = new CBottomNavigateWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pBottom->Initialize(3);
    m_pBottom->ShowEdit(true);
    m_pBottom->SetMaxItemCount(3);
    m_pBottom->RegisterOnpageFunction([this](int iStart, int iCount)
    {
        this->OnPage(iStart, iCount);
    });
    m_vecChildWnd.push_back(m_pBottom);

    m_pDataListWnd->SetBottomNavigatewnd(m_pBottom);  

    auto vecDataList = SplitString(sDataList, '|');
    SetTotalCount(vecDataList.size());
    CUploadProgressManager::Instance().ResetProgress(vecFileInfo[0],
        m_pTileWnd->GetProgressBar(),
        m_pRootWnd);

}

void CFileOperatorWnd::OnSize(int cx, int cy)
{
    this->SetDimention(cx, cy);
    m_pTileWnd->MoveWindow(0, 5);
    m_pTileWnd->SetDimention(cx, DispalyRegionToolBarHeight);
    m_pTileWnd->OnSize(cx, cy);

    int iToolBarHeight = DispalyRegionToolBarHeight;
    int w = m_rect.Width();
    int h = 50;
    int iOffsetx = 0;
    int iOffsety = iToolBarHeight + 10;
    int iBottomHeight = 50;
    int iBottomOffset = 5;
    int iContentHeight = m_rect.Height() - iToolBarHeight - iBottomHeight - iBottomOffset * 2;
    m_pDataListWnd->MoveWindow(iOffsetx, iOffsety);
    m_pDataListWnd->SetDimention(w, iContentHeight);
    m_pDataListWnd->OnSize(cx, cy);
    iOffsety += iContentHeight;
    m_pBottom->MoveWindow(iOffsetx, iOffsety);
    m_pBottom->SetDimention(w, iBottomHeight - iBottomOffset);
    m_pBottom->OnSize(cx, cy);
}

void CFileOperatorWnd::SetCurrentPageItems(const string& sContents)
{
    
}

void CFileOperatorWnd::SetTotalCount(int iTotalCount)
{
    m_pBottom->SetTotalCount(iTotalCount);
}

void CFileOperatorWnd::OnPage(int iStart, int iCount)
{
   // 使用rolemanage的方式，否则新增的数据不能获取/删除的数据也不能获取。
    string sUID = CW2A( m_vecFileInfo[0].GetString());
    HttpHelper httpHelper(theApp.m_spClient);
    auto slist = httpHelper.GetDataList(sUID);
    m_sDataList = CString(slist.c_str());

    m_pDataListWnd->ClearPageItems();
    auto vecDataList = SplitString(m_sDataList, '|');
    for (int i = iStart; i < iCount + iStart; i++)
    {
        if (i >= vecDataList.size())
        {
            break;
        }
        auto vecRow = SplitString(vecDataList[i], ',');
        assert(vecRow.size() == 4);
        m_pDataListWnd->AddItem(vecRow);
    }                    
}
void CFileOperatorWnd::OnUpload(CMyprogressBar* pProgressBar)
{
    //同一条注册数据同时只能上传一个数据，不同的注册数据可以同时上传
    auto pCurrentProgress = CUploadProgressManager::Instance().GetProgress(m_vecFileInfo[0]);
    if (pCurrentProgress != nullptr)
    {
        AfxMessageBox(L"当前正在上传文件!");
        return;
    }
    BOOL isOpen = TRUE;		//是否打开(否则为保存)
    CString  sPath, strTemp;
    GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
    sPath.ReleaseBuffer();
    int nPos = sPath.ReverseFind('\\');
    strTemp = sPath.Left(nPos);

    CString defaultDir = strTemp;	//默认打开的文件路径
    CString fileName = L"";			//默认打开的文件名
    CString filter = L"文件 ( *.*; |*.*;||)";	//文件过虑的类型
    int nFileNumbers = 200;
    CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY| OFN_ALLOWMULTISELECT, filter, NULL);
    openFileDlg.m_ofn.lpstrFile = new TCHAR[nFileNumbers * MAX_PATH];
    ZeroMemory(openFileDlg.m_ofn.lpstrFile, sizeof(TCHAR) * nFileNumbers * MAX_PATH);
    openFileDlg.m_ofn.nMaxFile = nFileNumbers * MAX_PATH;//此最大数,按MSDN解释为fileDlg.m_ofn.lpstrFile指向的缓冲的最大字符数,//默认是260    
    INT_PTR result = openFileDlg.DoModal();
    if (result != IDOK)
    {
        return;
    }   
    CArray<CString, CString> allFilename;
    POSITION posFile = openFileDlg.GetStartPosition();
    while (posFile != NULL)
    {
        allFilename.Add(openFileDlg.GetNextPathName(posFile));
    }
    int fileNum = allFilename.GetSize();

    fileName = allFilename.GetAt(0);

    string sFileOwerID = CW2A(m_vecFileInfo[0].GetString());
    CString wsFileOwerID = CString(sFileOwerID.c_str());
    CUploadProgressManager::Instance().AddProgress(wsFileOwerID,
        (CMyprogressBar*)pProgressBar,
        m_pRootWnd);
    vector<CString> vecNeedUploadFiles;
    for (int i = 0; i < fileNum; i++)
    {
        auto sFilePath = allFilename.GetAt(i);
        //auto sFilePath = openFileDlg.GetPathName();
        FILE* pFile = nullptr;
        string _sPath = CW2A(sFilePath.GetString());
        fopen_s(&pFile, _sPath.c_str(), "rb");
        if (pFile == nullptr)
        {
            AfxMessageBox(L"打开文件失败!");
            continue;
        }
        _fseeki64(pFile, 0, SEEK_END);
        auto iFileLength = _ftelli64(pFile);
        _fseeki64(pFile, 0, SEEK_SET);
        CUploadProgressManager::Instance().AddNewFileInfo(wsFileOwerID,sFilePath, iFileLength);
        vecNeedUploadFiles.push_back(sFilePath);
        fclose(pFile);
    }

    if (vecNeedUploadFiles.size() == 0)
    {
        CUploadProgressManager::Instance().RemoveProgress(wsFileOwerID);
        return;
    }
    string sUserOwerID = CW2A(m_vecFileInfo[9].GetString());
        std::thread t([this, vecNeedUploadFiles, wsFileOwerID, sFileOwerID, sUserOwerID, pProgressBar]()
        {
            bool bSuccess = false;
            bool bHasFailed = false;
            auto fileNum = vecNeedUploadFiles.size();
            for (int i = 0; i < fileNum; i++)
            {
                auto sFilePath = vecNeedUploadFiles[i];
                auto sFileName = sFilePath.Right(sFilePath.GetLength() - sFilePath.ReverseFind(L'\\') - 1);
                //pDataShowListWnd：保存dataShowListWnd指针,因为如果对话框被关闭后 m_pDataShowListWnd就为空了
                CDataShowrListWnd* pDataShowListWnd = m_pDataShowListWnd;
                FILE* pFile = nullptr;
                string _sPath = CW2A(sFilePath.GetString());
                fopen_s(&pFile, _sPath.c_str(), "rb");
                if (pFile == nullptr)
                {
                    CUploadProgressManager::Instance().SetFinished(wsFileOwerID, sFilePath);
                    AfxMessageBox(L"打开文件失败!");
                    continue;;
                }
                _fseeki64(pFile, 0, SEEK_END);
                auto iFileLength = _ftelli64(pFile);
                _fseeki64(pFile, 0, SEEK_SET);

                CString wsFileOwerID = CString(sFileOwerID.c_str());
                UploadFile upload(theApp.m_spClient, wsFileOwerID, sFilePath);
                UploadFileInfo fileInfo;
                fileInfo.sFileName = CW2A(sFileName.GetString());
                fileInfo.iFileLength = iFileLength;
                fileInfo.sFileOwerID = sFileOwerID;
                fileInfo.sUserOwerID = sUserOwerID;
                // CUploadProgressManager::Instance().AddProgress(wsFileOwerID,
                  //   (CMyprogressBar*)pProgressBar,
                  //   m_pRootWnd);

                bool bSuccess = upload.SendData(pFile, fileInfo);
                if (bSuccess)
                {
                    auto pWnd = CUploadProgressManager::Instance().GetOwerWnd(wsFileOwerID);
                    if (pWnd != nullptr)
                    {//如果对话框没有关闭
                        auto vecRow = SplitString(fileInfo.sServerResponds, ',');
                        assert(vecRow.size() == 4);
                        auto vecstrRow = String2CString(vecRow);
                        m_pDataListWnd->AddItem(vecstrRow);
                        auto iTotal = m_pBottom->GetTotalCount();
                        m_pBottom->SetTotalCount(iTotal + 1);
                    }
                    bSuccess = true;
                    // AfxMessageBox(L"上传成功");
                }
                else
                {
                    bHasFailed = true;
                    // AfxMessageBox(L"上传失败");
                }
                fclose(pFile);
                if (bSuccess)
                {//更新附件图标
                          
                    if (pDataShowListWnd != nullptr )
                    {
                        pDataShowListWnd->SetAttachmentMark(wsFileOwerID, true);
                    }
                }
                CUploadProgressManager::Instance().SetFinished(wsFileOwerID, sFilePath);
                if (CUploadProgressManager::Instance().IsFinished(wsFileOwerID))
                {
                    CUploadProgressManager::Instance().RemoveProgress(wsFileOwerID);
                }
            }

            if (bSuccess)
            {
                AfxMessageBox(L"上传成功");
            }
        });
        t.detach();
}

void CFileOperatorWnd::OnDownload(const vector<CString>& vecItem,
    CMyprogressBar* pProgressBar)
{
    if (vecItem.size() == 0)
    {                      
        return;
    }
    BOOL isOpen = FALSE;		//是否打开(否则为保存)
    CString  sPath, strTemp;
    GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
    sPath.ReleaseBuffer();
    CString sFileName = vecItem[1];			//默认打开的文件名
    if (sFileName.IsEmpty())
    {
        AfxMessageBox(L"请选择一个文件！");
        return;
    }

    int nPos = sPath.ReverseFind('\\');
    strTemp = sPath.Left(nPos);

    CString defaultDir = strTemp;	//默认打开的文件路径
    CString filter = L"文件 ( *.*; |*.*;||)";	//文件过虑的类型
    CFileDialog openFileDlg(isOpen, L"", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
    openFileDlg.m_ofn.lpstrInitialDir = defaultDir;
    INT_PTR result = openFileDlg.DoModal();
    if (result != IDOK)
    {
        return;
    }
    sPath = openFileDlg.GetPathName();
    CString wsID = vecItem[0];

    std::thread t([this, sPath, pProgressBar, wsID]()
    {        
        string sID = CW2A(wsID.GetString());
        auto spFile = std::make_shared< CStdioFile>();
        bool bRes = spFile->Open(sPath, CStdioFile::modeWrite | CFile::modeCreate | CStdioFile::typeBinary);
        if (!bRes)
        {
            AfxMessageBox(L"创建文件失败！");
            return;
        }
        CDownloadProgressManager::Instance().AddProgress(wsID, 
            pProgressBar, m_pRootWnd);
        DownLoadFile downLoadFile(theApp.m_spClient, spFile, wsID);
        //string sID = CW2A(vecItem[0].GetString());
        auto bSuccess = downLoadFile.DownLoad(sID);
        CDownloadProgressManager::Instance().RemoveProgress(wsID);
        if (!bSuccess)
        {
            AfxMessageBox(L"下载失败");
            //CStdioFile::Remove(sPath);
        }
        else
        {
            AfxMessageBox(L"下载成功");
        }
        spFile->Close();
    });
    t.detach();
}

void CFileOperatorWnd::OnDlgClose()
{
    CUploadProgressManager::Instance().SetOWerWnd(m_vecFileInfo[0], nullptr);
}

void CFileOperatorWnd::OnDeleteItem(CMyWindow* pWnd, const CString& sUID)
{
    HttpHelper httpHelper(theApp.m_spClient);
    string _sUID = CW2A(sUID);
    auto eErrorCode = httpHelper.DeleteData(_sUID);
    if (eErrorCode == ErrorCode::Failed)
    {
        AfxMessageBox(L"删除失败！");
        return;
    }
    if (m_pBottom != nullptr)
    {
        auto iCount = m_pBottom->GetTotalCount();
        m_pBottom->SetTotalCount(--iCount);
        m_pBottom->ForceUpdateThisPage();
        if (iCount == 0)
        {
            //更新附件图标
            m_pDataShowListWnd->SetAttachmentMark(m_vecFileInfo[0], false);
        }
    }
}
