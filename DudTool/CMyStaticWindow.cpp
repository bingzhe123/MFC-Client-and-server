#include "pch.h"
#include "CMyStaticWindow.h"
#include <iostream>
CMyStaticWindow::CMyStaticWindow(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun) 
    :CMyWindow(pParent, pRootWnd, fun)
{
    m_pGroupManager = false;
}

CMyStaticWindow::~CMyStaticWindow()
{
}

bool  CMyStaticWindow::Refresh(CDC* pMemDC , CRect r)
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
    if (IsSetBkGroundColor())
    {
        pMemDC->FillSolidRect(&rect, m_bkColor);
    }
   
    if (m_pBkImage != nullptr)
    {
        auto w = m_pBkImage->GetWidth();
        auto h = m_pBkImage->GetHeight();
        auto x = (rect.Width() - w) / 2 + rect.left;
        x = x < rect.left ? rect.left : x;
        auto y = (rect.Height() - h) / 2 + rect.top;
        y = y < rect.top ? rect.top : y;
       /* if (m_bBKStretch)
        {
            RECT desRect;
            desRect.left = rect.left;
            desRect.top = rect.top;
            desRect.right = desRect.left + rect.Width();
            desRect.bottom = desRect.top + rect.Height();
            pMemDC->SetStretchBltMode(COLORONCOLOR);
            m_pBkImage->StretchBlt(pMemDC->m_hDC, desRect, SRCCOPY);
        }
        else*/
        {
            m_pBkImage->Draw(pMemDC->m_hDC, x, y);
        }
    }
    if (!m_text.IsEmpty())
    {
        pMemDC->SelectObject(&m_font);
        auto bShowHightLing = m_isMouseIn | m_bSelected;
        int x = rect.left + m_ptTextOffset.x;
        int y = rect.top + m_ptTextOffset.y;
        if (bShowHightLing)
        {
            std::cout << "____________HigtLight" << CW2A(m_text) << std::endl;
            pMemDC->SetTextColor(m_textHightLigtColor);
            pMemDC->TextOutW(x, y, m_text);
        }
        else
        {
            std::cout << "____________NOT HigtLight" << CW2A(m_text) << std::endl;
            pMemDC->SetTextColor(m_textNormalColor);
            pMemDC->TextOutW(x, y, m_text);
        }
    }

    m_bIsDirty = false;
    return true;
}

bool CMyStaticWindow::OnMouseMove(UINT nFlags, CPoint point)
{
    return __super::OnMouseMove(nFlags, point);
}

void CMyStaticWindow::SetText(CString& sText, CPoint ptTextOffset)
{
    m_text = sText;
    m_ptTextOffset = ptTextOffset;
}

void CMyStaticWindow::SetBkPicStretch(bool bStretch)
{
    m_bBKStretch = bStretch;
}

//
//void CMyStaticWindow::OnLButtonDown(UINT nFlags, CPoint point)
//{
//    if (!m_bSupportClick)
//    {
//        return;
//    }
//    SetSelectState(!m_bSelected);
//    if (m_pGroupManager != nullptr)
//    {
//        m_pGroupManager->SelectWnd(this);
//    }
//}
//
//void CMyStaticWindow::SetSupportClick(bool bCanClick)
//{
//    m_bSupportClick = bCanClick;
//}
