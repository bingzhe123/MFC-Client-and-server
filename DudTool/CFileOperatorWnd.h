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
    int m_iCurrentPageCount = 0;//��ǰҳ���м���
    int m_iPerPageItemDataCount = 3;//��ǰҳ�������ʾ����                                             
    //��ǰҳ����ʾ������:key ��Ӧ������itemWindow
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
    //ÿһ���Զ��ŷָ�
    void SetCurrentPageItems(const string& sContents);
    void SetTotalCount(int iTotalCount);
    //��������ҳ�浼����ť��iStart���µ�ҳ����ʾ�Ŀ�ʼ������iEnd:�µ�ҳ����ʾ�Ľ�������
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
    //�����ڵ���ʾ������Ϣ�б���Ҫ����ͼ��
    CDataShowrListWnd* m_pDataShowListWnd = nullptr;
};
