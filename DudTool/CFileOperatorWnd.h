#pragma once
#include "httphelper.h"
#include "CMyContainerWnd.h"
#include "CMyButton.h"
#include "CEditEx.h"
#include "CMyLable.h"
#include "CMylistCtrlWnd.h"
#include "CBottomNavigateWnd.h"
#include "CMyprogressBar.h"
#include "CMyProgressButton.h"
#include "CMyprogressBar.h"
#include "CDataShowListWnd.h"
#include <string>
using std::string;

using UploadFileFun = std::function<void(CMyprogressBar * pWnd) >;
using DownLoadItemFun = std::function<void(const vector<CString> & vecItem, CMyprogressBar *pProgressBar) >;
using DeleteItemFun = std::function<void(CMyWindow *pWnd, const CString & sUID) >;

class CFileOperatorTileWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    void Initialize(bool bUpload);
    virtual void OnSize(int cx, int cy);
    void OnUpload(CMyWindow* pWnd);
    void RegisterAddNewItem(UploadFileFun f);
    CMyprogressBar* GetProgressBar();
private:
    CMyButton * m_pBtnUploadFile;
    UploadFileFun m_funAddNewItem = nullptr;
    CMyprogressBar* m_pProgressBar = nullptr;
    int m_iProgressHeight = 20;
};


class CFileOperatorListWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual void OnSize(int cx, int cy);
    void Initialize(const CString &sDataList, const vector<CString>& vecFileInfo);
    void OnAddNewItem(vector<CString>& vecContent);
    void AddItem(vector<CString>& vecContent);
    void ClearPageItems();
    void SetBottomNavigatewnd(CBottomNavigateWnd* pWnd);
    CMyLable* CreateLable(int x, int y, int w, int h, const CString& sContent);
    void OnDownload(CMyWindow* pWnd);
    void RegisterDownloadFunc(DownLoadItemFun f);
    void RegisterDeleteFunc(DeleteItemFun f);

    void OnDelete(CMyWindow* pWnd);

private:
    CMyListHeaderWnd* m_pHeadWnd = nullptr;
    int m_iCurrentPageCount = 0;//当前页面有几行
    int m_iPerPageItemDataCount = 3;//当前页面最大显示几行                                             
    //当前页面显示的内容:key 对应这条的itemWindow
    map<CMyWindow*, vector<CString>> m_mapContent;
    CBottomNavigateWnd* m_pBottom = nullptr;
    DownLoadItemFun m_pDownLoadFun = nullptr;
    DeleteItemFun m_pDeleteFun = nullptr;
    vector<CMyProgressButton*> m_vecDownloadButton;
    vector<CString> m_vecOwerFileInfo;

};


class CFileOperatorWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    virtual ~CFileOperatorWnd();
    void Initialize(const vector<CString>& vecFileInfo, 
        const CString& sDataList, CDataShowrListWnd* pDataShowListWnd);
    virtual void OnSize(int cx, int cy);
    //每一行以逗号分隔
    void SetCurrentPageItems(const string& sContents);
    void SetTotalCount(int iTotalCount);
    //点击下面的页面导航按钮，iStart：新的页面显示的开始索引，iEnd:新的页面显示的结束索引
    void OnPage(int iStart, int iCount);
    void OnUpload(CMyprogressBar* pWnd);
    void OnDownload(const vector<CString> &vecItem, CMyprogressBar *pProgressBar);
    void OnDlgClose();
    void OnDeleteItem(CMyWindow* pWnd, const CString& sUID);
private:
    CFileOperatorTileWnd* m_pTileWnd;
    CFileOperatorListWnd* m_pDataListWnd;
    CBottomNavigateWnd* m_pBottom = nullptr;
    vector<CString> m_vecFileInfo;
    CString m_sDataList;
    //父窗口的显示数据信息列表，需要更新图标
    CDataShowrListWnd* m_pDataShowListWnd = nullptr;
};
