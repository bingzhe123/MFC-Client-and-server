#include "pch.h"
#include "CMyCheckedExpandWnd.h"
#include "CommonDef.h"
const int MarkOffset = 20;

bool CMyCheckedExapndHeaderWnd::Refresh(CDC* pMemDC, CRect r)
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
    auto ibkx = rect.left;
    auto ibky = rect.top;
    if (IsSetBkGroundColor())
    {
        pMemDC->FillSolidRect(&rect, m_bkColor);
    }
    //ÉèÖÃ×ÖÌå           
    auto pOld = pMemDC->SelectObject(&m_font);

    auto size = pMemDC->GetTextExtent(m_text);

    int iTextOffset = 10;

    auto bShowHightLing = m_bSelected;
    if (bShowHightLing)
    {
        if (m_pBkSelectedImage != nullptr)
        {
            m_pBkSelectedImage->Draw(pMemDC->m_hDC, ibkx, ibky);
        }

        int x = rect.left;
        int y = 0;
        int iconx = 0;
        int icony = 0;
        if (m_pSelectIcon != nullptr)
        {
            iconx = 20 + rect.left;
            icony = m_pSelectIcon->GetHeight();
            y = (m_rect.Height() - icony) / 2 + rect.top;
            m_pSelectIcon->Draw(pMemDC->m_hDC, iconx, y);
            x = iconx + m_pSelectIcon->GetWidth() + iTextOffset;
        }
        y = rect.top + (rect.Height() - size.cy) / 2;
        pMemDC->SetTextColor(m_textHightLigtColor);
        pMemDC->TextOutW(x, y, m_text);
    }
    else
    {
        if (m_pBkImage != nullptr)
        {
            m_pBkImage->Draw(pMemDC->m_hDC, ibkx, ibky);
        }
        int x = rect.left;
        int y = 0;
        int iconx = 0;
        int icony = 0;
        if (m_pNormalIcon != nullptr)
        {
            iconx = 20 + rect.left;
            icony = m_pNormalIcon->GetHeight();
            y = (m_rect.Height() - icony) / 2 + rect.top;
            m_pNormalIcon->Draw(pMemDC->m_hDC, iconx, y);
            x = iconx + m_pNormalIcon->GetWidth() + iTextOffset;
        }
        y = rect.top + (rect.Height() - size.cy) / 2;
        pMemDC->SetTextColor(m_textNormalColor);
        pMemDC->TextOutW(x, y, m_text);
    }

    int iExpandx = 0;
    int iExpandy = 0;
    int iExpandOffset = 20;
    auto myRect = GetMyRect();
    auto w = myRect.Width();
    auto h = myRect.Height();
    if (!m_bExpand)
    {
        if (m_pExpandIcon != nullptr)
        {
            iExpandx = ibkx + w - m_pExpandIcon->GetWidth() - iExpandOffset;
            iExpandy = ibky + (h - m_pExpandIcon->GetHeight()) / 2;
            m_pExpandIcon->Draw(pMemDC->m_hDC, iExpandx, iExpandy);
        }
    }
    else
    {
        if (m_pUnexpandIcon != nullptr)
        {
            iExpandx = ibkx + w - m_pUnexpandIcon->GetWidth() - iExpandOffset;
            iExpandy = ibky + (h - m_pUnexpandIcon->GetHeight()) / 2;
            m_pUnexpandIcon->Draw(pMemDC->m_hDC, iExpandx, iExpandy);
        }
    }
    pMemDC->SelectObject(pOld);
}

bool CMyCheckedExapndHeaderWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_bVisible)
    {
        return false;
    }


    auto rect = GetClientRect();
    int x = 20 + rect.left;
    int icony = m_pNormalIcon->GetHeight();
    int iconx = m_pNormalIcon->GetWidth();

    int y = (m_rect.Height() - icony) / 2 + rect.top;
    CRect checkBoxRect(x, y, x + iconx, y + icony);
    bool bClickCheckBox = false;
    if (checkBoxRect.PtInRect(point))
    {
        bClickCheckBox = true;
        SetChecked(!m_bSelected);
        return true;
    }
    else
    {
        return __super::OnLButtonDown(nFlags, point);
    }
    return false;
}

void CMyCheckedExapndHeaderWnd::SetChecked(bool bChecked)
{
    m_bSelected = bChecked;
    if (m_pHeaderCheckBoxClickFun != nullptr)
    {
        m_pHeaderCheckBoxClickFun(bChecked);
    }
}

void CMyCheckedExapndHeaderWnd::RegisterHeaderCheckFun(HeaderCheckBoxClickFun f)
{
    m_pHeaderCheckBoxClickFun = f;
}

CMyCheckedExpandWnd::CMyCheckedExpandWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, CGroupManager* pGroupManager)
    :CMyExpandWnd(pParent, pRootWnd, fun, pGroupManager)
{
    auto it = std::find(m_vecChildWnd.begin(), m_vecChildWnd.end(), m_pHeaderWnd);
    if (it != m_vecChildWnd.end())
    {
        delete* it;
        m_vecChildWnd.erase(it);
    }
    m_pHeaderWnd = new CMyCheckedExapndHeaderWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pHeaderWnd->SetBkGroundColor(RGB(50, 50, 50));
    m_pHeaderWnd->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnClickTilte();
    });
    ((CMyCheckedExapndHeaderWnd*)m_pHeaderWnd)->RegisterHeaderCheckFun([this](bool bChecked)
    {
        this->OnHeaderChecked(bChecked);
    });
    m_vecChildWnd.insert(m_vecChildWnd.begin(), m_pHeaderWnd);
}
bool CMyCheckedExpandWnd::Refresh(CDC* pMemDC, CRect r)
{
    __super::Refresh(pMemDC, r);
    return true;
}

bool CMyCheckedExpandWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    __super::OnLButtonDown(nFlags, point);
    return true;
}

void CMyCheckedExpandWnd::AddChildWnd(CMyWindow* pChildWnd)
{
    __super::AddChildWnd(pChildWnd, NavigationItSubTextHeight);
    auto pCheckBox = dynamic_cast<CMyCheckBox*>(pChildWnd);
    pCheckBox->RegisterCheckChangedFunc([this](CMyWindow*pWnd)
    {
        ChildChecked((CMyCheckBox * )pWnd);
    });
}

void CMyCheckedExpandWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    m_pHeaderWnd->MoveWindow(0, 0);
    m_pHeaderWnd->SetDimention(rect.Width(), NavigationItemHeight);

    auto size = m_pDisplayWnd->GetDisplayWndSize();
    m_pDisplayWnd->MoveWindow(50, NavigationItemHeight);
    m_pDisplayWnd->SetDimention(rect.Width()-50, size.cy);
    m_pDisplayWnd->OnSize(rect.Width(), size.cy);
}

void CMyCheckedExpandWnd::SetChecked(bool bCheck)
{
    m_bSelected = bCheck;
    for each (auto item in m_vecChildWnd)
    {
        auto pCheckBox = dynamic_cast<CMyCheckBox*>(item);
        pCheckBox->SetChecked(bCheck);
    }

    auto displayRect = GetDisplayRect();
    UpdateMyWnd(displayRect);
}

void CMyCheckedExpandWnd::ChildChecked(CMyCheckBox* pCheck)
{
    bool isChecked = false;
    auto vecCheckBox = m_pDisplayWnd->GetChildWnds();
    for each (auto item in vecCheckBox)
    {
        auto pCheckBox = dynamic_cast<CMyCheckBox*>(item);
        isChecked |= pCheckBox->IsSelect();
    }
    m_pHeaderWnd->SetSelectState(isChecked);
    auto displayRect = GetDisplayRect();
    UpdateMyWnd(displayRect);
}

vector<CMyWindow*> CMyCheckedExpandWnd::GetAllCheckWnd()
{
    return m_pDisplayWnd->GetChildWnds();
}

void CMyCheckedExpandWnd::OnHeaderChecked(bool bChecked)
{
    auto vecCheckWnd = GetAllCheckWnd();
    for each (auto item in vecCheckWnd)
    {
        ((CMyCheckBox*)item)->SetChecked(bChecked);
    }
}

