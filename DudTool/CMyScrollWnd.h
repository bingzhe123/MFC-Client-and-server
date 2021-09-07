#pragma once
#include "CMyContainerWnd.h"
#include "CMyScrollbarWnd.h"

class CMyScrollWnd :
    public CMyContainerWnd
{
public:
    CMyScrollWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    virtual ~CMyScrollWnd();
    virtual void OnSize(int cx, int cy);
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    //fCurrentPosDelat:当前窗口所在的比例
    void OnScrollWnd(float fCurrentPosDelat);
    void SetRectanglew(int w);
    void SetScrollBarVisible(bool bVisible);

protected:
    CMyScrollbarWnd* m_pScrollBar = nullptr;
    int m_iScrollContentHeight = 0;
    int m_iRectanglew = 5;
    bool m_bScrollBarVisible = true;
};

