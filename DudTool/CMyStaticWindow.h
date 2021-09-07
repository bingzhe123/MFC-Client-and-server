#pragma once
#include "CMyWindow.h"
class CMyStaticWindow : public CMyWindow
{
public:
    CMyStaticWindow(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    virtual ~CMyStaticWindow();
    virtual bool Refresh(CDC* pMemDC , CRect rect);
    virtual bool OnMouseMove(UINT nFlags, CPoint point);
    //virtual void OnLButtonDown(UINT nFlags, CPoint point);
    void SetText(CString& sText, CPoint ptTextOffset);
    void SetBkPicStretch(bool bStretch);
    //void SetSupportClick(bool bCanClick);
public:
    CString m_text;
    CPoint m_ptTextOffset;
    bool m_bBKStretch = false;

    //bool m_bSupportClick = false;
};

