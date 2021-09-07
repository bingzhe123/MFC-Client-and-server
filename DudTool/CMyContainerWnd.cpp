#include "pch.h"
#include "CMyContainerWnd.h"
#include "CommonDef.h"



CMyContainerWnd::~CMyContainerWnd()
{
    for each (auto item in m_vecChildWnd)
    {
        delete item;
    }
    m_pParent = nullptr;
}

bool CMyContainerWnd::Refresh(CDC* pMemDC , CRect r)
{
    if (m_pRootWnd == nullptr || m_bVisible == false)
    {
        return false;
    }
    /*if (!__super::Refresh(pMemDC, r))
    {
        return false;
    }*/
    __super::Refresh(pMemDC, r);
    auto vecTemp = SortByLayer(m_vecChildWnd);
    for each (auto item in vecTemp)
    {
        item->Refresh(pMemDC, r);
    }
   /* for each (auto item in m_vecChildWnd)
    {
        item->Refresh(pMemDC, r);
    }*/
    return true;
}

void CMyContainerWnd::OnMouseLeave()
{
    if (!m_isMouseIn)
    {
        return;
    }
    m_isMouseIn = false;
    if (m_bMouseHoverSensitive)
    {
        UpdateMyWnd(GetDirtyRect());
    }
    for each (auto item in m_vecChildWnd)
    {
        item->OnMouseLeave();
    }
    m_pCurrentMouseInWnd = nullptr;
}
CMyWindow* CMyContainerWnd::GetTopMouseInWnd(CPoint point)
{
    int iLayer = m_iLayer;
    CMyWindow* pWnd = nullptr;
    for each (auto item in m_vecChildWnd)
    {
        auto iTemp = item->GetMaxlayer(point);
        if (iTemp > iLayer)
        {
            iLayer = iTemp;
            pWnd = item;
        }
    }
    return pWnd;
}

int CMyContainerWnd::GetMaxlayerIncludePt(CPoint pt)
{
    if (!m_bVisible || !GetClientRect().PtInRect(pt))
    {
        return INT_MIN;
    }
    int iLayer = m_iLayer;
    for each (auto item in m_vecChildWnd)
    {
        int iTemp = item->GetMaxlayer(pt);
        if (iTemp > iLayer)
        {
            iLayer = iTemp;
        }
    }
    return iLayer;
}

bool CMyContainerWnd::OnMouseMove(UINT nFlags, CPoint point)
{     
    if (!m_bVisible)
    {    
        return false;
    }
    auto it = std::find_if(m_vecChildWnd.begin(), m_vecChildWnd.end(), [&](auto _cur)
    {
        return  _cur == m_pCurrentMouseInWnd;
    });
    if (it == m_vecChildWnd.end())
    {
        m_pCurrentMouseInWnd = nullptr;
    }
    auto rect = GetClientRect();
    if (!rect.PtInRect(point))
    {
        if (m_pCurrentMouseInWnd != nullptr)
        {
            m_pCurrentMouseInWnd->OnMouseLeave();
            m_pCurrentMouseInWnd = nullptr;
        }
        return false;
    }
    bool bMouseInChild = false;
    auto pPreviousMouseInWnd = m_pCurrentMouseInWnd;
    for each (auto item in m_vecChildWnd)
    {
        if (item->OnMouseMove(nFlags, point))
        {
            if (m_pCurrentMouseInWnd == item)
            {
                bMouseInChild = true;
                continue;
            }
            if (m_pCurrentMouseInWnd != nullptr && m_pCurrentMouseInWnd != this)
            {
                m_pCurrentMouseInWnd->OnMouseLeave();
            }
            m_pCurrentMouseInWnd = item;
            item->OnMouseIn(nFlags, point);
        }
        if (item->HitTest(point) != nullptr && item->IsMouseIn())
        {
            this->OnMouseIn(nFlags, point);
            return true;
        }
    }
    this->OnMouseIn(nFlags, point);
    if (m_pCurrentMouseInWnd == nullptr)
    {
        m_pCurrentMouseInWnd = this;
        m_pCurrentMouseInWnd->OnMouseIn(nFlags, point);
    }
    else
    {
        if (!bMouseInChild && m_pCurrentMouseInWnd != this)
        {
            m_pCurrentMouseInWnd->OnMouseLeave();
            m_pCurrentMouseInWnd = this;
        }
    }
    return true;
}
BOOL CMyContainerWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (m_pCurrentMouseInWnd != nullptr && m_pCurrentMouseInWnd != this)
    {
        m_pCurrentMouseInWnd->OnMouseWheel(nFlags, zDelta, pt);
    }
    return true;
}


void CMyContainerWnd::SetDirty(bool isDirty)
{
    //for each (auto item in m_vecChildWnd)
    //{
    //    item->SetDirty(isDirty);
    //}
    //m_bIsDirty = true;
}

bool CMyContainerWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_bVisible || !m_bSupportClick || !m_isMouseIn)
    {
        return false;
    }
    if (m_pCurrentMouseInWnd == nullptr || m_pCurrentMouseInWnd == this)
    {
        return false;
    }
    return m_pCurrentMouseInWnd->OnLButtonDown(nFlags, point);
}

void CMyContainerWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    for each (auto item in m_vecChildWnd)
    {
        item->OnLButtonUp(nFlags, point);
    }
}

void CMyContainerWnd::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
    for each (auto item in m_vecChildWnd)
    {
        item->SetVisible(bVisible);
    }
    UpdateMyWnd(GetClientRect());
}

void CMyContainerWnd::OnSize(int cx, int cy)
{
    SetDimention(cx, cy);
}

void CMyContainerWnd::AddChildWnd(CMyWindow* pWnd, const CPoint& ptOffset)
{
    m_vecChildWnd.push_back(pWnd);
    pWnd->MoveWindow(ptOffset);
}
vector<CMyWindow*> CMyContainerWnd::GetChildWnds()
{
    return m_vecChildWnd;
}

vector<CEdit*> CMyContainerWnd::GetEditCtrl()
{
    vector<CEdit*> vecTemp;
    for each (auto item in m_vecChildWnd)
    {
        auto pWndWithEdit = dynamic_cast<IWidthEditWnd*>(item);
        if (pWndWithEdit != nullptr)
        {
            auto vecEdtiCtrl = pWndWithEdit->GetEditCtrl();
            if (vecEdtiCtrl.size() != 0)
            {
                vecTemp.insert(vecTemp.end(), vecEdtiCtrl.begin(), vecEdtiCtrl.end());
            }
        }
    }
    return vecTemp;
}
