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
    //���÷�ҳ�ص�����
    void RegisterOnpageFunction(OnpageFunc f);
    void SetMaxItemCount(int iMaxItemCount);
    CEditEx* GetEidt();
    int GetCurrentPage();
private:
    CMyLable* m_pLableTotalCount = nullptr;
    vector<CMyButton *> m_vecBtnPage;
    int m_iTotalCount = 0;
    int m_iPageBtnCount = 0;//ÿһҳ�м�����ť
    int m_iPageCount;//һ���м�ҳ
    int m_iCurrentPage = 1;//��ǰ���ڵ�ҳ��
    int m_iMaxPageCount = PerPageItemDataCount;//ÿҳ�����ʾ����
    CMyButton * m_pBtnLeft = nullptr;
    CMyButton * m_pBtnRight = nullptr;
    CMyLable* m_pAward = nullptr;
    CMyLable* m_pPage = nullptr;
    CEditEx  m_textBox;
    OnpageFunc m_funPage = nullptr;
};

