#pragma once
#include "CMyExpandWnd.h"
#include "CMyCheckBox.h"
using HeaderCheckBoxClickFun = std::function<void(bool)>;
class CMyCheckedExapndHeaderWnd :
    public CMyExapndHeaderWnd
{
public:
    using   CMyExapndHeaderWnd::CMyExapndHeaderWnd;
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    void SetChecked(bool bChecked);
    void RegisterHeaderCheckFun(HeaderCheckBoxClickFun f);
private:
    HeaderCheckBoxClickFun m_pHeaderCheckBoxClickFun = nullptr;
};
class CMyCheckedExpandWnd :
    public CMyExpandWnd
{
public:
    CMyCheckedExpandWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, CGroupManager* pGroupManager);
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    virtual void AddChildWnd(CMyWindow* pChildWnd);
    virtual void OnSize(int cx, int cy);
    void SetChecked(bool bCheck);
    void ChildChecked(CMyCheckBox* pCheck);
    vector<CMyWindow*> GetAllCheckWnd();
    void OnHeaderChecked(bool bChecked);
private:
    CPoint m_ptCheckBoxPos;

};

