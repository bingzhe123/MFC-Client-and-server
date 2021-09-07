#pragma once
#include "CMyContainerWnd.h"
#include "CMyTextDisplayWnd.h"
#include "CMyBoderWnd.h"
#include <memory>

using TitleClickFun = std::function<void(bool)>;

class CMyComboxTitle :
    public CMyContainerWnd
{
public:
    CMyComboxTitle(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, bool bCrossMarker = false);
    ~CMyComboxTitle();
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnMouseIn(UINT nFlags, CPoint point);
    virtual void OnMouseLeave();
    void OnClick(CMyWindow *pWnd);
    void SetText(const CString& sText);
    CString GetText();
    void RegisterTitleClickFun(TitleClickFun f);
    void SetExpand(bool bIsExapnd);
    bool IsExpand();
private:
    TitleClickFun m_pTitleClickFun = nullptr;
    bool m_bExpand = false;
    CString m_text;
    CImage* m_pCrossMarker = nullptr; //²æºÅ
    bool m_bCrossMarker = false;
};


class CMyComboxWnd :
    public CMyContainerWnd
{
public:
    CMyComboxWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, bool bCrossMarker = false);
    virtual void OnSize(int cx, int cy);
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    void AddItems(const vector<CString>& vecItems, bool bAutoSelectFirst = true);
    vector<CString> GetItems();
    void SetCurrentSelectIndex(int iCurrentSelect);
    void OnOpen(bool bOpen);
    void OnSelectItem(CMyWindow* pWnd);
    void SetExpandHeight(int iHeight);
    CString GetSelectItem();
    int GetSelectIndex();
    void SetScrollbarVisible(bool bVisible);
    void ClearSelectedItem();
private:
    int m_iCurrentSelect = 0;
    int m_iExpandHeight = 200;
    CMyComboxTitle* m_pComboxTitle = nullptr;
    CMyTextDisplayWnd* m_pExpandWnd = nullptr;
    CMyBoderWnd* m_pBoderWnd = nullptr;
    vector<CString> m_vecItemsContent;
};

using CMyComboxWndPtr = std::shared_ptr<CMyComboxWnd>;
using CMyComboxTitlePtr = std::shared_ptr<CMyComboxTitle>;

