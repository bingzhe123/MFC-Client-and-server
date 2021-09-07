#pragma once
#include "CMyContainerWnd.h"
#include "CMyButton.h"
#include "CEditEx.h"
#include "CMyLable.h"
#include "CMylistCtrlWnd.h"
#include "CNewUserDlg.h"
#include "CBottomNavigateWnd.h"

using AddNewInfoFun = std::function<void(CString &)>;
using DeleteUserFun = std::function<void(const CString&)>;
using SearchUserFun = std::function<void(const CString&)>;
using ModifyPatientTypeFun = std::function<void(const CString&,const CString&)>;

const int PatientListCount = 5;

class CPatientTypeTileWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    ~CPatientTypeTileWnd();
    void Initialize();
    virtual void OnSize(int cx, int cy);
    void ShowEdit(bool bEdit);
    void OnSave(CMyWindow* pWnd);
    void RegisterSave(AddNewInfoFun f);
    void SetEdit(CEditEx* pEdit);
private:
    CMyButton* m_pBtnNewType;
    CEditEx *m_pEdit;
    CMyLable* m_pLabelPatientName;
    AddNewInfoFun m_funAddNewPatientType = nullptr;
};


class CPatientTypeListWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual void OnSize(int cx, int cy);
    void Initialize();
    CMyLable* CreateLable(int x, int y, int w, int h, const CString& sContent);
    void AddNewItem(vector<CString>& vecContent);
    void ModifyItem(CMyWindow* pWnd, int iIndex, const CString& sNewContent);
    void ClearPageItems();
    void OnSetRole(CMyWindow* pWnd);
    void OnEdit(CMyWindow* pWnd);
    void OnDelete(CMyWindow* pWnd);
    void RegisterDeleteUserFun(DeleteUserFun f);
    void SetBottom(CBottomNavigateWnd* pBottom);
    void RegisterModifyPatientType(ModifyPatientTypeFun f);
private:
    CMyListHeaderWnd* m_pHeadWnd = nullptr;
    int m_iCurrentPageCount = 0;//当前页面有几行
    map<CMyWindow*, vector<CString>> m_mapContent;
    CBottomNavigateWnd* m_pBottom = nullptr;
    DeleteUserFun m_pDeleteFun = nullptr;
    ModifyPatientTypeFun m_pModifyPatientType = nullptr;
};


class CPatientTypeWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    virtual ~CPatientTypeWnd();
    virtual void SetVisible(bool bVisible);
    void Initialize();
    virtual void OnSize(int cx, int cy);
    void OnAddNewPatientType(CString &sNewType);

    void OnDelete(const CString& sUID);
    //每一行以逗号分隔
    void SetAllItems(const string& sContents);
    void SetTotalCount(int iTotalCount);
    //点击下面的页面导航按钮，iStart：新的页面显示的开始索引，iEnd:新的页面显示的结束索引
    void OnPage(int iStart, int iCount);
    void SetEdit(CEditEx* pEdit);
    CEditEx* GetBottomEidt();
    void MoidfyPatientType(const CString& sID, const CString& sNewType);

private:
    CPatientTypeTileWnd* m_pTileWnd;
    CPatientTypeListWnd* m_pPatientTypeListWnd;
    CBottomNavigateWnd* m_pBottom = nullptr;
    vector<vector<CString>> m_vecInfo;
};

