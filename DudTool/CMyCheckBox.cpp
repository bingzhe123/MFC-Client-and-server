#include "pch.h"
#include "CMyCheckBox.h"
#include "resource.h"

CMyCheckBox::CMyCheckBox(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyWindow(pParent, pRootWnd, fun)
{
    m_bMouseHoverSensitive = true;
    SetIconPic(IDB_PNG_CHECKBOX_NORMAL, IDB_PNG_CHECKBOX_CHECKED);
}

CMyCheckBox::~CMyCheckBox()
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

void CMyCheckBox::SetText(const CString& sText)
{
    m_text = sText;
}

void CMyCheckBox::SetIconPic(UINT nResNormalID, UINT nResSelectID)
{
    m_pNormalIcon = new CImage();
    m_pSelectIcon = new CImage();

    LoadImageFromResource(m_pNormalIcon, nResNormalID, _T("PNG"));
    LoadImageFromResource(m_pSelectIcon, nResSelectID, _T("PNG"));
}

bool CMyCheckBox::Refresh(CDC* pMemDC, CRect r)
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

    if (m_bMouseHoverSensitive)
    {
        if (m_isMouseIn)
        {
            if (m_bSetHigthLightBkGroundColor)
            {
                pMemDC->FillSolidRect(&rect, m_bkHigthLightColor);
            }
            else
            {
                if (IsSetBkGroundColor())
                {
                    pMemDC->FillSolidRect(&rect, m_bkColor);
                }
            }
        }
        else
        {
            if (IsSetBkGroundColor())
            {
                pMemDC->FillSolidRect(&rect, m_bkColor);
            }
        }

    }
    else
    {
        if (IsSetBkGroundColor())
        {
            pMemDC->FillSolidRect(&rect, m_bkColor);
        }
    }



    int iTextOffset = 10;
    int x = rect.left;
    int y = rect.top;
    int h = rect.Height();
    if (m_bSelected)
    {
        if (m_pSelectIcon != nullptr)
        {
            y = rect.top + (h - m_pSelectIcon->GetHeight()) / 2;
            m_ptCheckBoxPos.y = y;
            m_ptCheckBoxPos.x = x;

            m_pSelectIcon->Draw(pMemDC->m_hDC, x, y);
            x += m_pSelectIcon->GetWidth() + iTextOffset;
        }

    }
    else
    {
        if (m_pNormalIcon != nullptr)
        {
            y = rect.top + (h - m_pNormalIcon->GetHeight()) / 2;
            m_ptCheckBoxPos.y = y;
            m_ptCheckBoxPos.x = x;
            m_pNormalIcon->Draw(pMemDC->m_hDC, x, y);
            x += m_pNormalIcon->GetWidth() + iTextOffset;
            m_ptCheckBoxPos.y = y;
        }
    }
    //设置字体           
    auto pOld = pMemDC->SelectObject(&m_font);
    auto size = pMemDC->GetTextExtent(m_text);
    y = rect.top + (rect.Height() - size.cy) / 2;
    pMemDC->SetTextColor(m_textHightLigtColor);
    pMemDC->TextOutW(x, y, m_text);
    pMemDC->SelectObject(pOld);

}

bool CMyCheckBox::Refresh(CDC* pMemDC, CPoint ptOffset)
{
    if (!m_bVisible)
    {
        return false;
    }
    auto rect = GetClientRect() + ptOffset;

    if (IsSetBkGroundColor())
    {
        pMemDC->FillSolidRect(&rect, m_bkColor);
    }
    int iTextOffset = 10;
    int x = rect.left;
    int y = rect.top ;
    int h = rect.Height();
    if (m_bSelected)
    {
        if (m_pSelectIcon != nullptr)
        {
            y = rect.top + (h - m_pSelectIcon->GetHeight()) / 2;
            m_ptCheckBoxPos.y = y;
            m_ptCheckBoxPos.x = x;

            m_pSelectIcon->Draw(pMemDC->m_hDC, x, y);
            x += m_pSelectIcon->GetWidth() + iTextOffset;
        }

    }
    else
    {
        if (m_pNormalIcon != nullptr)
        {
            y = rect.top + (h - m_pNormalIcon->GetHeight()) / 2;
            m_ptCheckBoxPos.y = y;
            m_ptCheckBoxPos.x = x;
            m_pNormalIcon->Draw(pMemDC->m_hDC, x, y);
            x += m_pNormalIcon->GetWidth() + iTextOffset;
            m_ptCheckBoxPos.y = y;
        }
    }
    //设置字体           
    auto pOld = pMemDC->SelectObject(&m_font);
    auto size = pMemDC->GetTextExtent(m_text);
    y = rect.top + (rect.Height() - size.cy) / 2;
    pMemDC->SetTextColor(m_textHightLigtColor);
    pMemDC->TextOutW(x, y, m_text);

    pMemDC->SelectObject(pOld);
}


bool CMyCheckBox::OnLButtonDown(UINT nFlags, CPoint point)
{
    auto rect = GetClientRect();

    int iconx = m_pSelectIcon->GetWidth();
    int icony = m_pSelectIcon->GetHeight();
    int x = m_ptCheckBoxPos.x;
    int y = m_ptCheckBoxPos.y;

    CRect checkBoxRect(x, y, x + iconx, y + icony);
    if (checkBoxRect.PtInRect(point))
    {
        SetChecked(!m_bSelected);
    }
    return true;
}

void CMyCheckBox::SetChecked(bool bCheck)
{
    m_bSelected = bCheck;
    if (m_pChangedFunc != nullptr)
    {
        m_pChangedFunc(this);
    }
    UpdateMyWnd(GetClientRect());
}

void CMyCheckBox::RegisterCheckChangedFunc(CheckChangedFunc f)
{
    m_pChangedFunc = f;
}
