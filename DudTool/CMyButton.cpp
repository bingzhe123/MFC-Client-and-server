#include "pch.h"
#include "CMyButton.h"

CMyButton::CMyButton(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyWindow(pParent, pRootWnd, fun)
{
    m_bMouseHoverSensitive = true;
}

CMyButton::~CMyButton()
{
    if (m_pNormalIcon != nullptr)
    {
        m_pNormalIcon->Destroy();
        delete m_pNormalIcon;
    }
    if (m_pSelectIcon != nullptr)
    {
        m_pSelectIcon->Destroy();
        delete m_pSelectIcon;
    }
}       

bool CMyButton::Refresh(CDC* pMemDC , CRect r)
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
    auto bShowHightLing = m_isMouseIn | m_bSelected;
    if (IsSetBkGroundColor())
    {
        if (bShowHightLing)
        {
            pMemDC->FillSolidRect(&rect, m_bkHigthLightColor);
        }
        else
        {
            pMemDC->FillSolidRect(&rect, m_bkColor);
        }
    }
    auto ibkx = rect.left;
    auto ibky = rect.top;
    int x = rect.left;
    int y = rect.top;
    auto pOld = pMemDC->SelectObject(&m_font);
    auto size = pMemDC->GetTextExtent(m_text);
    if (!m_bMouseHoverSensitive)
    {
        pMemDC->SetTextColor(RGB(80, 80, 80));
    }
    else if (bShowHightLing)
    {
        pMemDC->SetTextColor(m_textHightLigtColor);
    }
    else
    {
        pMemDC->SetTextColor(m_textNormalColor);
    }
    if (bShowHightLing)
    {
        if (m_pBkSelectedImage != nullptr)
        {
            m_pBkSelectedImage->Draw(pMemDC->m_hDC, ibkx, ibky);
        }
        int iconx = 0;
        int icony = 0;
        int iTextOffset = 5;
        if (m_pSelectIcon != nullptr)
        {
            auto w = m_pSelectIcon->GetWidth();
            auto h = m_pSelectIcon->GetHeight();
            iconx = (rect.Width() - w - size.cx) / 2;
            icony = (rect.Height() - h - size.cy) / 2;
            iconx = iconx + rect.left;
            icony = icony + rect.top;
            m_pSelectIcon->Draw(pMemDC->m_hDC, iconx, icony);
            x = iconx + w + iTextOffset;
        }
        else
        {
            x += (rect.Width() - size.cx) / 2;
        }
        if (!m_text.IsEmpty())
        {
            x = x < rect.left ? rect.left : x;
            y += (rect.Height() - size.cy) / 2;
            pMemDC->TextOutW(x, y, m_text);
        }
    }
    else
    {
        if (m_pBkImage != nullptr)
        {
            m_pBkImage->Draw(pMemDC->m_hDC, ibkx, ibky);
        }
        int iconx = 0;
        int icony = 0;
        int iTextOffset = 5;
        if (m_pNormalIcon != nullptr)
        {
            auto w = m_pNormalIcon->GetWidth();
            auto h = m_pNormalIcon->GetHeight();
            iconx = (rect.Width() - w - size.cx) / 2;
            icony = (rect.Height() - h - size.cy) / 2;
            iconx = iconx + rect.left;
            icony = icony + rect.top;
            m_pNormalIcon->Draw(pMemDC->m_hDC, iconx, icony);
            x = iconx + w + iTextOffset;
        }
        else
        {
            x += (rect.Width() - size.cx) / 2;
        }
        if (!m_text.IsEmpty())
        {
            x = x < rect.left ? rect.left : x;
            y += (rect.Height() - size.cy) / 2;
            pMemDC->TextOutW(x, y, m_text);
        }

    }
    pMemDC->SelectObject(pOld);
    m_bIsDirty = false;
    return true;
}

bool CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_isMouseIn)
    {
        return false;
    }
    if (__super::OnMouseMove(nFlags, point))
    {
        this->OnMouseIn(nFlags, point);
        return true;
    }
    return false;
}
void CMyButton::SetText(const CString& sText)
{
    m_text = sText;
    SetName(sText);
    UpdateMyWnd(GetDirtyRect());
}

CString CMyButton::GetText()
{
    return m_text;
}

void CMyButton::SetIconPic(UINT nResNormalID, UINT nResSelectID)
{
    m_pNormalIcon = new CImage();
    m_pSelectIcon = new CImage();

    LoadImageFromResource(m_pNormalIcon, nResNormalID, _T("PNG"));
    LoadImageFromResource(m_pSelectIcon, nResSelectID, _T("PNG"));
}


