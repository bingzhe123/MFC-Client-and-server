#include "pch.h"
#include "CMyLable.h"
#include <iostream> 
#include "resource.h"
CMyLable::CMyLable(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyWindow(pParent, pRootWnd, fun)
{
    m_pImageStar = new CImage();
    CMyWindow::LoadImageFromResource(m_pImageStar, IDB_PNG_START, _T("PNG"));

}
CMyLable::~CMyLable()
{
    if (m_pImageStar != nullptr)
    {
        m_pImageStar->Destroy();
        delete m_pImageStar;
    }
}
void CMyLable::SetText(const CString& text, bool showStarMarker)
{
    m_bShowStarMarker = showStarMarker;
    m_text = text;
    m_bMouseHoverSensitive = false;
}
CString CMyLable::GetText()
{
    return m_text;
}

void CMyLable::SetAlignMode(Align align)
{
    m_alignMode = align;
}

bool CMyLable::OnMouseMove(UINT nFlags, CPoint point)
{
    return __super::OnMouseMove(nFlags, point);
}

bool CMyLable::Refresh(CDC* pMemDC , CRect r)
{
    //³¬¹ý·¶Î§¾Í»»ÐÐ
    if (!m_bVisible)
    {
        return false;
    }
    auto rect = GetClientRect();
    if (!IsRectIntersect(rect, r))
    {
        return false;
    }
    int iStarMarkerW = 0;
    int iStarMarkerH = 0;
    if (m_bShowStarMarker)
    {
        iStarMarkerW = m_pImageStar->GetWidth();
        iStarMarkerH = m_pImageStar->GetHeight();

    }
    if (!m_text.IsEmpty())
    {
        auto pOld = pMemDC->SelectObject(&m_font);
        auto bShowHightLing = m_isMouseIn | m_bSelected;
        auto size = pMemDC->GetTextExtent(m_text);
        
        int iLine = size.cx / rect.Width() + 1;
        int x = 0, y = 0;
        int iLineOffset = 1;
        if (iLine > 1)
        {
            y = rect.top + (rect.Height() - (size.cy + iLineOffset) * iLine) / 2;
            y += iLineOffset;
            y = y < rect.top ? rect.top : y;
        }
        else
        {
            y = rect.top + (rect.Height() - size.cy) / 2;
        }

        int iLineTextCount = 0;
        int iTempLength = 0;
        auto tempSize = pMemDC->GetTextExtent(m_text.Left(iLineTextCount));
        int iTextLength = m_text.GetLength();
        while (iLineTextCount < iTextLength && tempSize.cx < rect.Width()-8)
        {
            iLineTextCount++;
            tempSize = pMemDC->GetTextExtent(m_text.Left(iLineTextCount));
        }
        iLineTextCount <= 0 ? iTextLength : iLineTextCount;
        if (iLineTextCount == iTextLength)
        {
            y = rect.top + (rect.Height() - size.cy) / 2;
        }

        int iPos = 0;
        if (m_alignMode == Align::left)
        {
            x = rect.left;
        }
        else if (m_alignMode == Align::center)
        {
            x = rect.left + (rect.Width() - size.cx) / 2;
        }
        else
        {
            x = rect.Width() - size.cx + rect.left;
        }
        x = x < rect.left + 3 ? rect.left + 3 : x;
        pMemDC->SetBkMode(TRANSPARENT);
        if (bShowHightLing)
        {
            if (m_bSetHigthLightBkGroundColor)
            {
                pMemDC->FillSolidRect(&rect, m_bkHigthLightColor);
            }
            std::cout << "____________HigtLight" << CW2A(m_text) << std::endl;
            pMemDC->SetTextColor(m_textHightLigtColor);           
        }
        else
        {
            if (m_bSetBkGroundColor)
            {
                pMemDC->FillSolidRect(&rect, m_bkColor);
            }
            std::cout << "____________NOT HigtLight" << CW2A(m_text) << std::endl;
            pMemDC->SetTextColor(m_textNormalColor);
        }

        if (m_bShowStarMarker)
        {
            m_pImageStar->Draw(pMemDC->m_hDC, x - iStarMarkerW, y + iStarMarkerH/2, iStarMarkerW, iStarMarkerH);
        }
        for (int i = 0; i < iLine; i++)
        {
            CString text;
            if (iPos + iLineTextCount > iTextLength)
            {
                iLineTextCount = iTextLength - iPos;
            }
            text = m_text.Mid(iPos, iLineTextCount);
            pMemDC->TextOutW(x, y, text);
            iPos += iLineTextCount;
            y += iLineOffset + size.cy;
            if (y > rect.bottom)
            {
                break;
            }
        }
        pMemDC->SelectObject(pOld);

    }

    m_bIsDirty = false;
    return true;
}

