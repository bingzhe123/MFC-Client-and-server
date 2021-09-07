#include "pch.h"
#include "CMyComboxWnd.h"
#include "CMyStaticWindow.h"
#include "CMyLable.h"
#include "CommonDef.h"

CMyComboxTitle::CMyComboxTitle(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, bool bCrossMarker)
    :CMyContainerWnd(pParent, pRootWnd, fun)
{
    m_bMouseHoverSensitive = true;
    m_iLayer = 2;
    SetBkPic(IDB_PNG_EXPAND, IDB_PNG_UNEXPAND);
    m_pCrossMarker = new CImage();
    LoadImageFromResource(m_pCrossMarker, IDB_PNG_CROSS_MARKER, _T("PNG"));
    m_bCrossMarker = bCrossMarker;
}

CMyComboxTitle::~CMyComboxTitle()
{
    if (m_pCrossMarker != nullptr)
    {
        m_pCrossMarker->Destroy();
        delete m_pCrossMarker;
    }
}

 
bool CMyComboxTitle::Refresh(CDC* pMemDC, CRect r)
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
    auto pOld = pMemDC->SelectObject(&m_font);
    //¾ØÐÎ¿ò
    pMemDC->FillSolidRect(rect, RGB(255, 255, 255));
    int rectanglew = 2;
    auto temp = rect;
    temp.left += rectanglew;
    temp.top += rectanglew;
    temp.right -= rectanglew;
    temp.bottom -= rectanglew;
    pMemDC->FillSolidRect(temp, RGB(63, 63, 70));
    int x = rect.left + 10;
    int y = rect.top;
    auto size = pMemDC->GetTextExtent(m_text);
    y += (rect.Height() - size.cy) / 2;
    pMemDC->SetTextColor(m_textHightLigtColor);
    pMemDC->TextOutW(x, y, m_text);

    if (m_bCrossMarker && m_isMouseIn && !m_text.IsEmpty())
    {
        int iconx = rect.right - m_pCrossMarker->GetWidth() - 8;
        int icony = rect.top + (rect.Height() - m_pCrossMarker->GetHeight()) / 2;
        m_pCrossMarker->Draw(pMemDC->m_hDC, iconx, icony);
    }
    else
    {
        if (m_bExpand && m_pBkSelectedImage != nullptr)
        {
            int iconx = rect.right - m_pBkSelectedImage->GetWidth() - 8;
            int icony = rect.top + (rect.Height() - m_pBkSelectedImage->GetHeight()) / 2;
            m_pBkSelectedImage->Draw(pMemDC->m_hDC, iconx, icony);
        }

        if (!m_bExpand && m_pBkImage != nullptr)
        {
            int iconx = rect.right - m_pBkImage->GetWidth() - 8;
            int icony = rect.top + (rect.Height() - m_pBkImage->GetHeight()) / 2;
            m_pBkImage->Draw(pMemDC->m_hDC, iconx, icony);
        }
    }
    pMemDC->SelectObject(pOld);
}

bool CMyComboxTitle::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_bVisible || !m_isMouseIn)
    {
        return false;
    }
    auto rect = GetClientRect();

    int l = rect.right - m_pCrossMarker->GetWidth() - 8;
    int r = l + m_pCrossMarker->GetWidth();
    int t = rect.top + (rect.Height() - m_pCrossMarker->GetHeight()) / 2;
    int b = t + m_pCrossMarker->GetHeight();
    CRect rTemp(l, t, r, b);
    if (m_bCrossMarker && rTemp.PtInRect(point) && !m_text.IsEmpty())
    {
        this->SetText(L"");
    }
    else
    {
        OnClick(this);
    }
}

void CMyComboxTitle::OnMouseIn(UINT nFlags, CPoint point)
{
    __super::OnMouseIn(nFlags, point);
}

void CMyComboxTitle::OnMouseLeave()
{
    __super::OnMouseLeave();
}


void CMyComboxTitle::OnClick(CMyWindow* pWnd)
{
    m_bExpand = !m_bExpand;
    if (m_pTitleClickFun != nullptr)
    {
        m_pTitleClickFun(m_bExpand);
    }
    UpdateMyWnd(GetClientRect());
}


void CMyComboxTitle::SetText(const CString& sText)
{
    m_text = sText;
    UpdateMyWnd(GetClientRect());
}

CString CMyComboxTitle::GetText()
{
    return m_text;
}

void CMyComboxTitle::RegisterTitleClickFun(TitleClickFun f)
{
    m_pTitleClickFun = f;
}

void CMyComboxTitle::SetExpand(bool bIsExapnd)
{
    m_bExpand = bIsExapnd;
}
bool CMyComboxTitle::IsExpand()
{
    return m_bExpand;
}


CMyComboxWnd::CMyComboxWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, bool bCrossMarker)
    :CMyContainerWnd(pParent, pRootWnd, fun)
{
    m_pComboxTitle = new CMyComboxTitle(this, m_pRootWnd, m_refreshImpFun, bCrossMarker);
    m_pComboxTitle->RegisterTitleClickFun([this](bool bOpen)
    {
        this->OnOpen(bOpen);
    });
    m_vecChildWnd.push_back(m_pComboxTitle);

    m_pBoderWnd = new CMyBoderWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pBoderWnd->SetBkGroundColor(RGB(200, 200, 200));
    m_pBoderWnd->SetBoderWeight(2);

    m_pExpandWnd = new CMyTextDisplayWnd(m_pBoderWnd, m_pRootWnd, m_refreshImpFun);
    m_pExpandWnd->SetBkGroundColor(RGB(0, 0, 0));
   // m_pExpandWnd->SetVisible(false);
    m_pExpandWnd->RegisterClickItemFun([this](CMyWindow* pWnd)
    {
        OnSelectItem(pWnd);
    });
    m_pBoderWnd->AddChild(m_pExpandWnd);
    m_pBoderWnd->SetVisible(false);
    m_vecChildWnd.push_back(m_pBoderWnd);

    //m_vecChildWnd.push_back(m_pExpandWnd);
}

void CMyComboxWnd::OnSize(int cx, int cy)
{
    CRect rect = GetClientRect();
    m_pComboxTitle->MoveWindow(0,0);
    m_pComboxTitle->SetDimention(rect.Width(), rect.Height());
    m_pBoderWnd->MoveWindow(0, rect.Height());
    m_pBoderWnd->SetDimention(rect.Width(), m_iExpandHeight);
    m_pBoderWnd->OnSize(cx, cy);
    UpdateMyWnd(GetClientRect());

}
bool CMyComboxWnd::OnLButtonDown(UINT nFlags, CPoint point)
{         
    if (!m_bVisible || !m_bSupportClick || !m_isMouseIn)
    {
        return false;
    }
    auto rect = GetClientRect();
    if (!rect.PtInRect(point))
    {
        OnOpen(false);
    }
    return __super::OnLButtonDown(nFlags, point);
}
void CMyComboxWnd::AddItems(const vector<CString>& vecItems, bool bAutoSelectFirst)
{
    m_vecItemsContent = vecItems;

    if (bAutoSelectFirst && vecItems.size() != 0)
    {
        m_pComboxTitle->SetText(vecItems[0]);
    }
    m_pExpandWnd->SetTextItems(vecItems);
    OnSize(0, 0);

}
vector<CString> CMyComboxWnd::GetItems()
{
    return m_vecItemsContent;
}
void CMyComboxWnd::SetCurrentSelectIndex(int iCurrentSelect)
{
    m_iCurrentSelect = iCurrentSelect;
    m_pComboxTitle->SetText(m_vecItemsContent[iCurrentSelect]);
}

void CMyComboxWnd::OnOpen(bool bOpen)
{
    CRect refreshRect;
    if (bOpen)
    {
        auto rect = GetClientRect();
        auto expandWndRect = m_pBoderWnd->GetClientRect();
        SetDimention(rect.Width(), rect.Height() + expandWndRect.Height());
        refreshRect = GetClientRect();
    }
    else
    {
        refreshRect = GetClientRect();
        auto rect = m_pComboxTitle->GetClientRect();
        SetDimention(rect.Width(), rect.Height());
    }
    m_pBoderWnd->SetVisible(bOpen);
    m_pComboxTitle->SetExpand(bOpen);
    m_pBoderWnd->UpdateMyWnd(m_pBoderWnd->GetClientRect());
}

void CMyComboxWnd::OnSelectItem(CMyWindow* pWnd)
{
    OnOpen(false);
    m_pComboxTitle->SetText(((CComboxItem*)pWnd)->GetText());
    m_iCurrentSelect = ((CComboxItem*)pWnd)->m_iIndex;
}

void CMyComboxWnd::SetExpandHeight(int iHeight)
{
    m_iExpandHeight = iHeight;
}

CString CMyComboxWnd::GetSelectItem()
{
    return m_pComboxTitle->GetText();
}
int CMyComboxWnd::GetSelectIndex()
{
    return m_iCurrentSelect;
}


void CMyComboxWnd::SetScrollbarVisible(bool bVisible)
{
    m_pExpandWnd->SetScrollBarVisible(bVisible);
}
void CMyComboxWnd::ClearSelectedItem()
{
    m_pComboxTitle->SetText(L"");
}
