#include "pch.h"
#include "CMyExpandWnd.h"

#include "CommonDef.h"


CMyExapndHeaderWnd::CMyExapndHeaderWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyWindow(pParent,pRootWnd, fun)
{
    this->SetMouseHoverSensitive(true);
}

CMyExapndHeaderWnd::~CMyExapndHeaderWnd()
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

    if (m_pExpandIcon != nullptr)
    {
        m_pExpandIcon->Destroy();
        delete m_pExpandIcon;
    }

    if (m_pUnexpandIcon != nullptr)
    {
        m_pUnexpandIcon->Destroy();
        delete m_pUnexpandIcon;
    }
}

bool CMyExapndHeaderWnd::Refresh(CDC* pMemDC, CRect r)
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
    //设置字体           
    auto pOld = pMemDC->SelectObject(&m_font);

    auto size = pMemDC->GetTextExtent(m_text);

    int iTextOffset = 10;

    auto bShowHightLing = m_isMouseIn | m_bSelected;
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

//check,uncheck的图片
void CMyExapndHeaderWnd::SetIconPic(UINT nResNormalID, UINT nResSelectID)
{
    m_pNormalIcon = new CImage();
    m_pSelectIcon = new CImage();

    LoadImageFromResource(m_pNormalIcon, nResNormalID, _T("PNG"));
    LoadImageFromResource(m_pSelectIcon, nResSelectID, _T("PNG"));
}
//展开收缩的箭头标记
void CMyExapndHeaderWnd::SetExpandMark(UINT nResExpandID, UINT nResUnexpandID)
{
    m_pExpandIcon = new CImage();
    m_pUnexpandIcon = new CImage();

    LoadImageFromResource(m_pExpandIcon, nResExpandID, _T("PNG"));
    LoadImageFromResource(m_pUnexpandIcon, nResUnexpandID, _T("PNG"));
}
void CMyExapndHeaderWnd::SetText(const CString& sText)
{
    m_text = sText;
}
CString CMyExapndHeaderWnd::GetText()
{
    return m_text;
}
void CMyExapndHeaderWnd::SetExpand(bool bExpand)
{
    m_bExpand = bExpand;
}

CSize CMyExapndHeaderWnd::GetBkPicSize()
{
    if (m_pNormalIcon != nullptr)
    {
        return CSize(m_pNormalIcon->GetWidth(), m_pNormalIcon->GetHeight());
    }
    return CSize();
}

CMyExpandDisplayWnd::~CMyExpandDisplayWnd()
{
    if (m_pScrollBar != nullptr)
    {
        delete m_pScrollBar;
    }
}

void CMyExpandDisplayWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    int x = 0;
    int iOffsety = 2;
    int y = iOffsety;
    for each (auto item in m_vecChildWnd)
    {
        item->MoveWindow(x, y);
        item->SetDimention(rect.Width(), m_iItemHeight);
        y += m_iItemHeight + iOffsety;
    }
}

void CMyExpandDisplayWnd::AddChildWnd(CMyWindow* pChildWnd, int iHeight, int iSpaceY)
{
    pChildWnd->SetParent(this);
    m_vecChildWnd.push_back(pChildWnd);
    m_iContentHeight += iSpaceY;
    m_iContentHeight += iHeight;
    m_iSpaceY = iSpaceY;
    m_iItemHeight = iHeight;
}
void CMyExpandDisplayWnd::SetSelectStateFromChild(bool bSelect)
{
    ((CMyExpandWnd*)m_pParent)->SetSelectStateFromChild(bSelect);
}

void CMyExpandDisplayWnd::SetFixedHeight(int iHeight)
{
    m_iFixedHeight = iHeight;
}

CSize CMyExpandDisplayWnd::GetDisplayWndSize()
{
    if (m_iFixedHeight == 0)
    {
        return CSize(m_rect.Width(), m_iContentHeight);
    }
    else
    {
        return CSize(m_rect.Width(), m_iFixedHeight);
    }
}

bool CMyExpandDisplayWnd::HasDisplayItem()
{
    return m_vecChildWnd.size() != 0;
}


CMyExpandWnd::CMyExpandWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, CGroupManager* pGroupManager)
    :CMyContainerWnd(pParent, pRootWnd, fun)
{
    m_isMouseIn = false;
    m_bMouseHoverSensitive = true;
    m_pChildGroupManager = new CGroupManager();
    if (nullptr != pGroupManager)
    {
        m_pGroupManager = pGroupManager;
        m_pGroupManager->AddWnd(this);
    }
    m_pHeaderWnd = new CMyExapndHeaderWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pHeaderWnd->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnClickTilte();
    });
    m_vecChildWnd.push_back(m_pHeaderWnd);

    m_pDisplayWnd = new CMyExpandDisplayWnd(this, m_pRootWnd, m_refreshImpFun);
    m_vecChildWnd.push_back(m_pDisplayWnd);
}

CMyExpandWnd::~CMyExpandWnd()
{
    
    if (m_pChildGroupManager != nullptr)
    {
        delete m_pChildGroupManager;
    }                          
}

bool CMyExpandWnd::Refresh(CDC* pMemDC , CRect r)
{
    return __super::Refresh(pMemDC, r);
}
void CMyExpandWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    m_pHeaderWnd->MoveWindow(0, 0);
    m_pHeaderWnd->SetDimention(rect.Width(), NavigationItemHeight);

    auto size = m_pDisplayWnd->GetDisplayWndSize();
    m_pDisplayWnd->MoveWindow(0, NavigationItemHeight);
    m_pDisplayWnd->SetDimention(rect.Width(), size.cy);
    m_pDisplayWnd->OnSize(rect.Width(), size.cy);
}
               

bool CMyExpandWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
   auto bRes = __super::OnLButtonDown(nFlags, point);
    return bRes;
}

void CMyExpandWnd::SetText(const CString& sText)
{
    m_pHeaderWnd->SetText(sText);
}
CString CMyExpandWnd::GetText()
{
    return m_pHeaderWnd->GetText();
}
void CMyExpandWnd::SetIconPic(UINT nResNormalID, UINT nResSelectID)
{
    m_pHeaderWnd->SetIconPic(nResNormalID, nResSelectID);
}

void CMyExpandWnd::SetExpandMark(UINT nResExpandID, UINT nResUnexpandID)
{
    m_pHeaderWnd->SetExpandMark(nResExpandID, nResUnexpandID);
}
void CMyExpandWnd::SetBkPic(UINT nResNormalID, UINT nResSelectID)
{
    m_pHeaderWnd->SetBkPic(nResNormalID, nResSelectID);
}
void CMyExpandWnd::SetBkGroundColor(COLORREF rgb)
{
    m_pHeaderWnd->SetBkGroundColor(rgb);
    CMyContainerWnd::SetBkGroundColor(rgb);
}

void CMyExpandWnd::SetIsExpand(bool bIsExpand)
{
    if (bIsExpand)
    {
        ExpandChild();
    }
    else
    {
        UnexpandChild();
    }
}

bool CMyExpandWnd::IsExpand()
{
    return m_bExpand;
}

void CMyExpandWnd::OnClickTilte()
{                  
    if (m_bExpand)
    {
        UnexpandChild();
    }
    else
    {
        ExpandChild();
    }
}              

void CMyExpandWnd::AddChildWnd(CMyWindow* pChildWnd, int iHeight, int iSpaceY)
{
    pChildWnd->SetVisible(false);
    pChildWnd->SetFontHeight(23);
    pChildWnd->SetGroupManager(m_pChildGroupManager);
    m_pChildGroupManager->AddWnd(pChildWnd);
    m_pDisplayWnd->AddChildWnd(pChildWnd, iHeight, iSpaceY);
}

void CMyExpandWnd::SetSelectState(bool bSelect)
{
    __super::SetSelectState(bSelect);
    if (!bSelect)
    {
        auto vecDisplayChild = m_pDisplayWnd->GetChildWnds();
        for each (auto it in vecDisplayChild)
        {
            it->SetSelectState(false);
        }
    }
    m_pHeaderWnd->SetSelectState(bSelect);
}

void CMyExpandWnd::SetSelectStateFromChild(bool bSelect)
{
    SetSelectState(bSelect);
    if (nullptr != m_pGroupManager)
    {
        m_pGroupManager->SelectWnd(this);
    }
    m_pHeaderWnd->SetSelectState(bSelect);

}

CRect CMyExpandWnd::GetDirtyRect()
{
    //子窗口是相对于当前窗口左上角的偏移量
    //if (m_vecChildWnd.empty())
    if (m_vecChildWnd.empty() || m_dirtyRect.IsRectEmpty())
    {
        return __super::GetDirtyRect();
    }
    auto tempRect = m_dirtyRect;
    m_dirtyRect.SetRectEmpty();
    return tempRect;

}
CMyWindow* CMyExpandWnd::HitTest(CPoint pt)
{
    return __super::HitTest(pt);
}

void CMyExpandWnd::OnMouseLeave()
{       
    __super::OnMouseLeave();
}

void CMyExpandWnd::OnMouseIn(UINT nFlags, CPoint point)
{
   
    if (m_isMouseIn)
    {
        return;
    }
    auto rect = GetClientRect();
    if (rect.PtInRect(point) == false)
    {
        return;
    }
    m_isMouseIn = true;
    UpdateMyWnd(GetDirtyRect());
}

bool CMyExpandWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    return __super::OnMouseMove(nFlags, point);
}

 

void CMyExpandWnd::ForceUpdate()
{
    for each (auto item in m_vecChildWnd)
    {
        //item->SetVisible(true);
        item->SetDirty(true);
        item->UpdateMyWnd(item->GetClientRect());
    }
    __super::ForceUpdate();
}

void CMyExpandWnd::ExpandChild()
{
    m_bExpand = true;
    if (!m_pDisplayWnd->HasDisplayItem())
    {
        return;
    }
    m_pHeaderWnd->SetExpand(m_bExpand);
    m_pDisplayWnd->SetVisible(true);
    auto size = m_pDisplayWnd->GetDisplayWndSize();
    auto rect = GetClientRect();
    SetDimention(rect.Width(), rect.Height() + size.cy);

    if (m_pGroupManager != nullptr)
    {
        auto pGroupManager = (CExpandGroup*)m_pGroupManager;
        pGroupManager->ExpandWnd(this);
    }
}
void CMyExpandWnd::UnexpandChild()
{
    m_bExpand = false;
    if (!m_pDisplayWnd->HasDisplayItem())
    {
        return;
    }
    m_pHeaderWnd->SetExpand(m_bExpand);
    m_pDisplayWnd->SetVisible(false);
    auto rect = m_pHeaderWnd->GetMyRect();
    SetDimention(rect.Width(), rect.Height());
    if (m_pGroupManager != nullptr)
    {
        auto pGroupManager = (CExpandGroup*)m_pGroupManager;
        pGroupManager->UnExpandWnd(this);
    }
}
CSize CMyExpandWnd::GetDisplayWndSize()
{
    return m_pDisplayWnd->GetDisplayWndSize();
}
CRect CMyExpandWnd::GetDisplayRect()
{
    return m_pDisplayWnd->GetClientRect();
}
vector<CMyWindow*> CMyExpandWnd::GetChildWnds()
{
    return m_vecChildWnd;
}
