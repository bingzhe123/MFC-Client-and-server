#include "pch.h"
#include "CMyprogressBar.h"
#include "CommonDef.h"

bool CMyprogressBar::Refresh(CDC* pMemDC, CRect r)
{
    if (!m_bVisible)
    {
        return false;
    }
    auto rect = GetClientRect();
    if (!IsRectIntersect(rect, r))
    {
        return false;
    }
    if (m_iPos == 0 || m_iPos == 100)
    {
        return false;
    }

    int x = rect.left;
    int y = rect.top;

    //¹ö¶¯Ìõ±³¾°
    CRect rectProgress(x, y, x + rect.Width(), y + rect.Height());
    pMemDC->FillSolidRect(rectProgress, RGB(255, 255, 255));

    int rectanglew = 3;
    auto temp = rectProgress;
    temp.left += rectanglew;
    temp.top += rectanglew;
    int w = rect.Width() - rectanglew * 2;
    int currentWidth = w * ((float)m_iPos / 100.0);

    temp.right = temp.left + currentWidth;
    temp.bottom -= rectanglew;
    pMemDC->FillSolidRect(temp, RGB(0, 176, 145));
    return true;
}

void CMyprogressBar::SetPos(int iPos)
{
    if (m_iPos == iPos)
    {
        return;
    }
    m_iPos = iPos;
    auto rect = GetClientRect();
    ::PostMessage(m_pRootWnd->m_hWnd, 
        WM_PROGRESS_UPDATE,
        (WPARAM)this,
        0);
    //UpdateMyWnd(GetClientRect());
}
