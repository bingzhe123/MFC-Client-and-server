#pragma once
#include "CMyContainerWnd.h"
#include "CMyButton.h"
#include "CEditEx.h"
#include "CMyLable.h"
#include "CMylistCtrlWnd.h"
#include "CBottomNavigateWnd.h"
#include <string>
using std::string;

using UploadFileFun = std::function<void(vector<CString>&)>;

class CRoleManagerTileWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    void Initialize();
    virtual void OnSize(int cx, int cy);
    void ShowEdit(bool bEdit);
    void OnNewRole(CMyWindow* pWnd);
    void RegisterAddNewItem(UploadFileFun f);
private:
    CMyButton * m_pBtnAddRole;
    CMyButton * m_pBtnSearch;
    CEditEx m_editUserName;
    CMyLable* m_pLabelUserName;
    UploadFileFun m_funAddNewItem = nullptr;
};


class CRoleManagerListWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual void OnSize(int cx, int cy);
    void Initialize();
    void OnAddNewItem(vector<CString>& vecContent);
    void AddRoleItem(vector<CString>& vecContent);
    void ClearPageItems();
    void SetBottomNavigatewnd(CBottomNavigateWnd* pWnd);
    CMyLable* CreateLable(int x, int y, int w, int h, const CString& sContent);
    void OnEdit(CMyWindow* pWnd);
    void OnEditRoleAuthority(CMyWindow* pWnd);
    void OnDelete(CMyWindow* pWnd);

private:
    CMyListHeaderWnd* m_pHeadWnd = nullptr;
    int m_iCurrentPageCount = 0;//��ǰҳ���м���

    //select id, data_authority,system_authority, role_short_name,
    //role_name, create_time from filemanager.role_table order by id
    //��ǰҳ����ʾ������:key ��Ӧ������itemWindow
    map<CMyWindow*, vector<CString>> m_mapContent;
    CBottomNavigateWnd* m_pBottom = nullptr;

};


class CRoleManagerWnd :
    public CMyContainerWnd
{
public:
    using   CMyContainerWnd::CMyContainerWnd;
    virtual ~CRoleManagerWnd();
    virtual void SetVisible(bool bVisible);
    void Initialize();
    virtual void OnSize(int cx, int cy);
    //ÿһ���Զ��ŷָ�
    void SetCurrentPageItems(const string &sContents);
    void SetTotalCount(int iTotalCount);
    //��������ҳ�浼����ť��iStart���µ�ҳ����ʾ�Ŀ�ʼ������iEnd:�µ�ҳ����ʾ�Ľ�������
    void OnPage(int iStart, int iCount);
private:
    CRoleManagerTileWnd* m_pTileWnd;
    CRoleManagerListWnd* m_pUserManagerListWnd;
    CBottomNavigateWnd* m_pBottom = nullptr;
};
 