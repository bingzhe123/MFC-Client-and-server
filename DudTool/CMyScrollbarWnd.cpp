#include "pch.h"
#include "CMyScrollbarWnd.h"

CMyScrollbarWnd::CMyScrollbarWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyContainerWnd(pParent, pRootWnd, fun)
{
    SetBkGroundColor(RGB(62, 62, 66));
    m_pSliderWnd = new CMyWindow(this, pRootWnd, fun);
    m_pSliderWnd->SetName(L"SliderWnd");
    m_pSliderWnd->SetBkGroundColor(RGB(104, 104, 104), RGB(158, 158, 158));
    m_pSliderWnd->SetMouseHoverSensitive(true);
    m_vecChildWnd.push_back(m_pSliderWnd);
}

CMyScrollbarWnd::~CMyScrollbarWnd()
{        
   /* if (m_pSliderWnd != nullptr)
    {
        delete m_pSliderWnd;
    }*/
}
void CMyScrollbarWnd::OnSize(int cx, int cy)
{
    //assert(m_iScrollHeight != 0);
    auto rect = GetClientRect();
    int h = rect.Height();
    int w = rect.Width();
    if (m_iScrollHeight == 0)
    {
        m_iSliderHeight = h;
    }
    else
    {
        m_iSliderHeight = ((float)h / m_iScrollHeight) * h;
        if (m_iSliderHeight > h)
        {
            m_iSliderHeight = h;
        }
    }
   
    int x = 0;
    int y = 0;
    int iSplitter = 5;
    m_pSliderWnd->MoveWindow(x + iSplitter, y);
    m_pSliderWnd->SetDimention(w- iSplitter*2, m_iSliderHeight);
}
 

bool CMyScrollbarWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    auto res =  __super::OnMouseMove(nFlags, point);
    OnSliderMouseMove(point);
    return res;
}

BOOL CMyScrollbarWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (m_iSliderHeight == m_rect.Height())
    {
        return true;
    }
    int iTemp = m_iCurrentPos;
    int iCanMoveDistance = m_rect.Height() - m_iSliderHeight;
    if (zDelta < 0)
    {//ÏòÏÂ¹ö¶¯
        iTemp += m_iPerScrollHeight;
        if (iTemp > iCanMoveDistance)
        {
            iTemp = iCanMoveDistance;
        }
    }
    else
    {
        iTemp -= m_iPerScrollHeight;
        if (iTemp < 0)
        {
            iTemp = 0;
        }
    }
    if (iTemp == m_iCurrentPos)
    {
        return true;
    }
    m_iCurrentPos = iTemp;
    int x = 0;
    int iSplitter = 5;
    m_pSliderWnd->MoveWindow(x + iSplitter, m_iCurrentPos);
    if (m_pScrollWndFunc != nullptr)
    {
        m_pScrollWndFunc((float)m_iCurrentPos / m_rect.Height());

    }
    return 0;
}

bool CMyScrollbarWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_pSliderWnd->HitTest(point))
    {
        m_bMouseDown = true;
        m_ptMousePos = point;
    }
    return false;
}

void CMyScrollbarWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_bMouseDown = false;
}

void CMyScrollbarWnd::SetScrollHeight(int iHeight)
{
    m_iScrollHeight = iHeight;
}

void CMyScrollbarWnd::RegisterScrollWndFun(ScrollWndFunc f)
{
    m_pScrollWndFunc = f;
}

void CMyScrollbarWnd::OnSliderMouseMove(CPoint pt)
{
    if (!m_bMouseDown)
    {
        return;
    }
    auto iOffset = pt.y - m_ptMousePos.y;

    if (m_iSliderHeight == m_rect.Height())
    {
        return;
    }
    int iTemp = m_iCurrentPos;
    int iCanMoveDistance = m_rect.Height() - m_iSliderHeight;
    iTemp += iOffset;
    if (iTemp > iCanMoveDistance)
    {
        iTemp = iCanMoveDistance;
    }
    if (iTemp < 0)
    {
        iTemp = 0;
    }
    if (iTemp == m_iCurrentPos)
    {
        return;
    }
    m_iCurrentPos = iTemp;
    int x = 0;
    int iSplitter = 5;
    m_pSliderWnd->MoveWindow(x + iSplitter, m_iCurrentPos);
    if (m_pScrollWndFunc != nullptr)
    {
        m_pScrollWndFunc((float)m_iCurrentPos / m_rect.Height());
    }
    m_ptMousePos = pt;
}

