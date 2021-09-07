#include "pch.h"
#include "CMyTextDisplayWnd.h"
#include "CMyLable.h"
CMyTextDisplayWnd::CMyTextDisplayWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyScrollWnd(pParent, pRootWnd, fun)
{
    m_iRectanglew = 0;

}

void CMyTextDisplayWnd::SetTextItems(const vector<CString>& vecContent)
{
    for each (auto item in m_vecChildWnd)
    {
        if (item == m_pScrollBar)
        {
            continue;
        }
        delete item;
    }
    m_vecChildWnd.clear();
 
    int iIndex = 0;
    for each (auto item in vecContent)
    {
        auto pLable = new CComboxItem(this, m_pRootWnd, m_refreshImpFun);
        pLable->SetAlignMode(Align::left);
        pLable->SetText(item);
        pLable->SetBkGroundColor(RGB(27, 27, 28), RGB(63, 63, 70));
        pLable->SetMouseHoverSensitive(true);
        pLable->SetSupportClick(true);
        pLable->RegisterClickFun([this](CMyWindow* pWnd)
        {
            OnClickTextItem(pWnd);
        });
        pLable->m_iIndex = iIndex++;
        m_vecChildWnd.push_back(pLable);
    }
    m_vecChildWnd.push_back(m_pScrollBar);
}
//
//bool CMyTextDisplayWnd::Refresh(CDC* pMemDC, CRect r)
//{
//    if (m_bVisible == false)
//    {
//        return false;
//    }
//    auto rect = GetClientRect();
//    if (!IsRectIntersect(rect, r))
//    {
//        return false;
//    }
//    CBitmap memBitmap;
//    CDC memDC;
//    memDC.CreateCompatibleDC(NULL);
//    int nWidth = rect.Width();
//    int nHeight = rect.Height();
//    CRect rootWndRect;
//    m_pRootWnd->GetClientRect(rootWndRect);
//    memBitmap.CreateCompatibleBitmap(pMemDC, rootWndRect.Width(), rootWndRect.Height());
//    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);
//    if (IsSetBkGroundColor())
//    {
//        memDC.FillSolidRect(rect, m_bkColor);
//    }
//    for each (auto item in m_vecChildWnd)
//    {
//        item->Refresh(&memDC, r);
//    }
//    pMemDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, rect.left, rect.top, SRCCOPY);
//    memBitmap.DeleteObject();
//    memDC.DeleteDC();
//    return true;
//}
//
//void CMyTextDisplayWnd::OnSize(int cx, int cy)
//{
//    auto rect = GetClientRect();
//    int x = 0;
//    int y = 0;
//    for each (auto item in m_vecChildWnd)
//    {
//        item->MoveWindow(x, y);
//        item->SetDimention(rect.Width(), m_iItemHeight);
//        y += m_iItemHeight + m_iOffsety;
//    }
//}

void CMyTextDisplayWnd::OnClickTextItem(CMyWindow* pWnd)
{
    if (m_pFunClickTextItem != nullptr)
    {
        m_pFunClickTextItem(pWnd);
    }
}

void CMyTextDisplayWnd::RegisterClickItemFun(ClickFunc f)
{
    m_pFunClickTextItem = f;
}
