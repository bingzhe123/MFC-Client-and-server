#pragma once
#include "CMyContainerWnd.h"
#include "CMyLable.h"
#include "CMyButton.h"
#include "CEditEx.h"
#include "CommonDef.h"

using OnpageFunc = std::function<void(int,int)>;
class CBottomNavigateWnd      
    :public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    ~CBottomNavigateWnd();
    void Initialize(int iTotalCount);
    virtual void OnSize(int cx, int cy);
    void SetTotalCount(int iCount);
    int GetTotalCount();
    virtual bool Refresh(CDC* pMemDC , CRect rect);
    void ShowEdit(bool bEdit);
    void OnClickPage(CMyWindow* pWnd);
    void OnFirst(CMyWindow* pWnd);
    void OnForceFirst(CMyWindow* pWnd);

    void OnLast(CMyWindow* pWnd);
    void OnEditSet(const CString& sText);
    void ForceUpdateThisPage();
    //设置翻页回调函数
    void RegisterOnpageFunction(OnpageFunc f);
    void SetMaxItemCount(int iMaxItemCount);
    CEditEx* GetEidt();
    int GetCurrentPage();
private:
    CMyLable* m_pLableTotalCount = nullptr;
    vector<CMyButton *> m_vecBtnPage;
    int m_iTotalCount = 0;
    int m_iPageBtnCount = 0;//每一页有几个按钮
    int m_iPageCount;//一共有几页
    int m_iCurrentPage = 1;//当前所在的页面
    int m_iMaxPageCount = PerPageItemDataCount;//每页最多显示几行
    CMyButton * m_pBtnLeft = nullptr;
    CMyButton * m_pBtnRight = nullptr;
    CMyLable* m_pAward = nullptr;
    CMyLable* m_pPage = nullptr;
    CEditEx  m_textBox;
    OnpageFunc m_funPage = nullptr;
};

