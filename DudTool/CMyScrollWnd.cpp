#include "pch.h"
#include "CMyScrollWnd.h"
#include "CommonDef.h"
CMyScrollWnd::CMyScrollWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyContainerWnd(pParent, pRootWnd, fun)
{
    m_pScrollBar = new CMyScrollbarWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pScrollBar->RegisterScrollWndFun([this](float fDelat)
    {
        this->OnScrollWnd(fDelat);
    });
    m_vecChildWnd.push_back(m_pScrollBar);
}
CMyScrollWnd::~CMyScrollWnd()
{
}
void CMyScrollWnd::OnSize(int cx, int cy)
{
    m_iScrollContentHeight = 0;
    int iOffset = 3;
    int x = m_iRectanglew;
    int y = 0;
    auto rect = GetClientRect();
    int w = rect.Width();
    int h = NavigationItSubTextHeight;
    int iScrollBarWidth = 0;
    if (m_bScrollBarVisible)
    {
        iScrollBarWidth = 20;
    }
    int iCheckBoxWidth = w - iScrollBarWidth - m_iRectanglew * 2 - m_iRectanglew;
    for each (auto item in m_vecChildWnd)
    {
        if (item == m_pScrollBar)
        {
            continue;
        }
        item->MoveWindow(x, y);
        item->SetDimention(iCheckBoxWidth, h);
        y += iOffset + h;
        m_iScrollContentHeight += iOffset + h;
    }
    if (m_bScrollBarVisible)
    {
        x = w - m_iRectanglew - iScrollBarWidth;
        y = 0;
        m_pScrollBar->MoveWindow(x, y);
        m_pScrollBar->SetDimention(iScrollBarWidth, rect.Height());
        m_pScrollBar->SetScrollHeight(m_iScrollContentHeight);
        m_pScrollBar->OnSize(cx, cy);
    }
    
}

bool CMyScrollWnd::Refresh(CDC* pMemDC, CRect r)
{
    if (m_pRootWnd == nullptr || m_bVisible == false)
    {
        return false;
    }

    auto rect = GetClientRect();
    CBitmap memBitmap;
    CDC memDC;
    memDC.CreateCompatibleDC(NULL);
    CRect rootRect;
    m_pRootWnd->GetClientRect(rootRect);
    int nWidth = rootRect.Width() - m_iRectanglew * 2;
    int nHeight = rootRect.Height() - m_iRectanglew * 2;

    memBitmap.CreateCompatibleBitmap(pMemDC, nWidth, nHeight);
    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);

    if (IsSetBkGroundColor())
    {
        memDC.FillSolidRect(rect, m_bkColor);
    }
    CPoint pt = rect.TopLeft();
    for each (auto item in m_vecChildWnd)
    {
        item->Refresh(&memDC, r);
    }
    pMemDC->BitBlt(rect.left, rect.top,
        rect.Width(), rect.Height(),
        &memDC, rect.left, rect.top, SRCCOPY);
    m_pScrollBar->Refresh(pMemDC, r);
    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    return true;
}

BOOL CMyScrollWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    return m_pScrollBar->OnMouseWheel(nFlags, zDelta, pt);
}

//向上移动的比例
void CMyScrollWnd::OnScrollWnd(float fCurrentPosDelat)
{
    int x = m_iRectanglew;
    int y = -fCurrentPosDelat * m_iScrollContentHeight;
    auto rect = GetClientRect();
    int w = rect.Width();
    int h = NavigationItSubTextHeight;
    int iScrollBarWidth = 20;
    int iOffset = 3;
    int iCheckBoxWidth = w - iScrollBarWidth - m_iRectanglew * 2 - m_iRectanglew;
    for each (auto item in m_vecChildWnd)
    {
        if (item == m_pScrollBar)
        {
            continue;
        }
        item->MoveWindow(x, y);
        y += iOffset + h;
    }
}
void CMyScrollWnd::SetRectanglew(int w)
{
    m_iRectanglew = w;
}

void CMyScrollWnd::SetScrollBarVisible(bool bVisible)
{
    m_bScrollBarVisible = bVisible;
    m_pScrollBar->SetVisible(bVisible);
}
