#pragma once
#include "CMyContainerWnd.h"
#include "CMyButton.h"
#include "CEditEx.h"
#include "CMyLable.h"
#include "CMylistCtrlWnd.h"
#include "CNewUserDlg.h"
#include "CBottomNavigateWnd.h"

using AddNewInfoFun = std::function<void()>;
using DeleteUserFun = std::function<void(const CString&)>;
using SearchUserFun = std::function<void(const CString&)>;

class CUserManagerTileWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    void Initialize();
    virtual void OnSize(int cx, int cy);
    void ShowEdit(bool bEdit);
    void OnNewUser(CMyWindow* pWnd);
    void OnSearch(CMyWindow* pWnd);
    void RegisterAddNewUser(AddNewInfoFun f);
    void RegisterSearchUser(SearchUserFun f);
    string GetSearchName();

private:
    CMyButton * m_pBtnUserRegister;
    CMyButton * m_pBtnSearch;
    CEditEx m_editUserName;
    CMyLable* m_pLabelUserName;
    AddNewInfoFun m_funAddNewInfo = nullptr;
    SearchUserFun m_funSearch = nullptr;
};


class CUserManagerListWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual void OnSize(int cx, int cy);
    void Initialize();
    CMyLable* CreateLable(int x, int y, int w, int h, const CString& sContent);
    void AddNewItem(vector<CString>& vecContent);
    void ModifyItem(CMyWindow *pWnd, int iIndex, const CString& sNewContent);
    void ClearPageItems();
    void OnSetRole(CMyWindow* pWnd);
    void OnEdit(CMyWindow* pWnd);
    void OnDelete(CMyWindow* pWnd);
    void RegisterDeleteUserFun(DeleteUserFun f);
private:
    CMyListHeaderWnd* m_pHeadWnd = nullptr;
    int m_iCurrentPageCount = 0;//当前页面有几行
    map<CMyWindow*, vector<CString>> m_mapContent;
    CBottomNavigateWnd* m_pBottom = nullptr;
    DeleteUserFun m_pDeleteFun = nullptr;
};


class CUserManagerWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    virtual ~CUserManagerWnd();
    virtual void SetVisible(bool bVisible);
    void Initialize();
    virtual void OnSize(int cx, int cy);
    void OnAddNewUser(NewUserInfo newUserInfo);

    void OnDeleteUser(const CString& sUID);
    //每一行以逗号分隔
    void SetCurrentPageItems(const string& sContents);
    void SetTotalCount(int iTotalCount);
    //点击下面的页面导航按钮，iStart：新的页面显示的开始索引，iEnd:新的页面显示的结束索引
    void OnPage(int iStart, int iCount);
    void OnSearch(const CString &sUserName);
private:
    CUserManagerTileWnd* m_pTileWnd;
    CUserManagerListWnd* m_pUserManagerListWnd;
    CBottomNavigateWnd* m_pBottom = nullptr;
};

