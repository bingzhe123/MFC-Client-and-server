#include "pch.h"
#include "CMyMutiSelectItemWnd.h"
#include "CommonDef.h"
bool CMyMutiSelectItemTitle::Refresh(CDC* pMemDC, CRect r)
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
    //矩形框
    pMemDC->FillSolidRect(rect, RGB(255, 255, 255));
    int rectanglew = 2;
    auto temp = rect;
    temp.left += rectanglew;
    temp.top += rectanglew;
    temp.right -= rectanglew;
    temp.bottom -= rectanglew;
    pMemDC->FillSolidRect(temp, RGB(63, 63, 70));
    pMemDC->SetTextColor(m_textHightLigtColor);
    if (!m_sText.IsEmpty())
    {
        auto pOld = pMemDC->SelectObject(&m_font);
        auto size = pMemDC->GetTextExtent(m_sText);

        int iLine = size.cx / rect.Width() + 1;
        int x = 0, y = 0;
        int iLineOffset = 1;
        if (iLine > 1)
        {
            y = rect.top + (rect.Height() - (size.cy + iLineOffset) * iLine) / 2;
            y = y < rect.top ? rect.top : y;
            y += iLineOffset;
        }
        else
        {
            y = rect.top + (rect.Height() - size.cy) / 2;
        }
        int iLineTextCount = 0;
        int iTempLength = 0;
        auto tempSize = pMemDC->GetTextExtent(m_sText.Left(iLineTextCount));
        int iTextLength = m_sText.GetLength();
        while (iLineTextCount < iTextLength && tempSize.cx <rect.Width()-8)
        {
            iLineTextCount++;
            tempSize = pMemDC->GetTextExtent(m_sText.Left(iLineTextCount));
        }
        iLineTextCount <= 0 ? iTextLength : iLineTextCount;

        int iLineTextLength = iLineTextCount;
        int iPos = 0;
        x = rect.left;
        x = x < rect.left + 3 ? rect.left + 3 : x;
        pMemDC->SetBkMode(TRANSPARENT);
       
        for (int i = 0; i < iLine; i++)
        {
            CString text;
            if (iPos + iLineTextLength > iTextLength)
            {
                iLineTextLength = iTextLength - iPos;
            }
            text = m_sText.Mid(iPos, iLineTextLength);
            pMemDC->TextOutW(x, y, text);
            iPos += iLineTextLength;
            y += iLineOffset + size.cy;
            if (y + size.cy > rect.bottom)
            {
                break;
            }
        }
        pMemDC->SelectObject(pOld);
    }
    return false;
}

void CMyMutiSelectItemTitle::SetText(const CString& sText)
{
    m_sText = sText;
}

void CMyCheckedScrollWnd::Initialze(vector<RoleInfo>& vecRoleInfo)
{
    SetFontHeight(30);
    for each (auto item in vecRoleInfo)
    {
        auto pCheckBox = new CMyCheckBox(this, m_pRootWnd, m_refreshImpFun);
        pCheckBox->SetText(item.roleName);
        pCheckBox->SetBkGroundColor(RGB(19, 44, 73), RGB(128, 128, 50));
        pCheckBox->RegisterCheckChangedFunc([this](CMyWindow* pWnd)
        {
            if (this->m_pItemSelectFun != nullptr)
            {
                this->m_pItemSelectFun(m_vecRoleInfoItems);
            }
        });
       
        m_vecChildWnd.push_back(pCheckBox);
        m_vecRoleInfoItems.emplace_back(std::pair<CMyCheckBox*, RoleInfo>( pCheckBox ,item ));
        if (item.isSelected)
        {
            pCheckBox->SetChecked(true);
        }
    } 
}
//
//void CMyCheckedScrollWnd::OnSize(int cx, int cy)
//{
//    m_iScrollContentHeight = 0;
//    int iOffset = 3;
//    int x = m_iRectanglew;
//    int y = 0;
//    auto rect = GetClientRect();
//    int w = rect.Width();
//    int h = NavigationItSubTextHeight;
//    int iScrollBarWidth = 20;
//    int iCheckBoxWidth = w - iScrollBarWidth - m_iRectanglew * 2 - m_iRectanglew;
//    for each (auto item in m_vecRoleInfoItems)
//    {
//        item.first->MoveWindow(x, y);
//        item.first->SetDimention(iCheckBoxWidth, h);
//        y += iOffset + h;
//        m_iScrollContentHeight += iOffset + h;
//    }
//    x = w - m_iRectanglew - iScrollBarWidth;
//    y = 0;
//    m_pScrollBar->MoveWindow(x, y);
//    m_pScrollBar->SetDimention(iScrollBarWidth, rect.Height());
//    m_pScrollBar->SetScrollHeight(m_iScrollContentHeight);
//    m_pScrollBar->OnSize(cx, cy);
//}
//
//bool CMyCheckedScrollWnd::Refresh(CDC* pMemDC, CRect r)
//{
//    if (m_pRootWnd == nullptr || m_bVisible == false)
//    {
//        return false;
//    }
//
//    auto rect = GetClientRect();
//    CBitmap memBitmap;
//    CDC memDC;
//    memDC.CreateCompatibleDC(NULL);
//    CRect rootRect;
//    m_pRootWnd->GetClientRect(rootRect);
//    int nWidth = rootRect.Width() - m_iRectanglew *2;
//    int nHeight = rootRect.Height() - m_iRectanglew * 2;
// 
//    memBitmap.CreateCompatibleBitmap(pMemDC, nWidth, nHeight);
//    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);
//
//    if (IsSetBkGroundColor())
//    {
//        memDC.FillSolidRect(rect, m_bkColor);
//    }
//    CPoint pt = rect.TopLeft();
//    for each (auto item in m_vecRoleInfoItems)
//    {
//        ((CMyCheckBox*)item.first)->Refresh(&memDC, r);
//    }
//    pMemDC->BitBlt(rect.left , rect.top,
//        rect.Width(), rect.Height(),
//        &memDC, rect.left, rect.top, SRCCOPY);
//    m_pScrollBar->Refresh(pMemDC, r);
//    memBitmap.DeleteObject();
//    memDC.DeleteDC();
//    return true;
//}
//
//BOOL CMyCheckedScrollWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//    return m_pScrollBar->OnMouseWheel(nFlags, zDelta, pt);
//}
//
////向上移动的比例
//void CMyCheckedScrollWnd::OnScrollWnd(float fCurrentPosDelat)
//{
//    int x = m_iRectanglew;
//    int y = -fCurrentPosDelat * m_iScrollContentHeight;
//    auto rect = GetClientRect();
//    int w = rect.Width();
//    int h = NavigationItSubTextHeight;
//    int iScrollBarWidth = 20;
//    int iOffset = 3;
//    int iCheckBoxWidth = w - iScrollBarWidth - m_iRectanglew * 2 - m_iRectanglew;
//    for each (auto item in m_vecRoleInfoItems)
//    {
//        item.first->MoveWindow(x, y);
//        y += iOffset + h;
//    }
//}
     

void CMyCheckedScrollWnd::RegisterItemSelectFunc(ItemSelectFunc f)
{
    m_pItemSelectFun = f;
}

vector<RoleInfo> CMyCheckedScrollWnd::GetAllSelectRole()
{
    vector<RoleInfo> vecRoleInfo;
    for each (auto item in m_vecRoleInfoItems)
    {
        if (item.first->IsSelect())
        {
            vecRoleInfo.push_back(item.second);
        }
    }
    return vecRoleInfo;
}


CMyMutiSelectItemWnd::~CMyMutiSelectItemWnd()
{
}

void CMyMutiSelectItemWnd::Initialize(vector< RoleInfo>& vecRoleInfo)
{
    m_pTitle = new CMyMutiSelectItemTitle(this, m_pRootWnd, m_refreshImpFun);
    m_vecChildWnd.push_back(m_pTitle);

    m_pBoderWnd = new CMyBoderWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pBoderWnd->SetBkGroundColor(RGB(200, 200, 200));
    m_vecChildWnd.push_back(m_pBoderWnd);

    m_pScrollWnd = new CMyCheckedScrollWnd(m_pBoderWnd, m_pRootWnd, m_refreshImpFun);
    m_pScrollWnd->RegisterItemSelectFunc([this](vector<std::pair<CMyCheckBox*, RoleInfo>> vecRoleInfo)
    {
        this->OnItemSelect(vecRoleInfo);
    });
    m_pBoderWnd->AddChild(m_pScrollWnd);
    m_pScrollWnd->Initialze(vecRoleInfo);
}

void CMyMutiSelectItemWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    int iTitleHeight = 70;
    int iOffset = 2;
    m_pTitle->MoveWindow(0, 0);
    m_pTitle->SetDimention(rect.Width(), iTitleHeight);
    m_pBoderWnd->MoveWindow(0, iTitleHeight + iOffset);
    m_pBoderWnd->SetDimention(rect.Width(), rect.Height() - iTitleHeight - iOffset);
    m_pBoderWnd->OnSize(cx, cy);
}

bool CMyMutiSelectItemWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    return __super::OnMouseMove(nFlags, point);
}

BOOL CMyMutiSelectItemWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_pScrollWnd->OnMouseWheel(nFlags, zDelta, pt);
    return 0;
}

void CMyMutiSelectItemWnd::OnItemSelect(vector<std::pair<CMyCheckBox*, RoleInfo>> vecRoleInfo)
{
    CString sContent; 
    for each (auto item in vecRoleInfo)
    {
        if (item.first->IsSelect())
        {
            sContent += item.second.roleName + L"/";
        }
    }
    sContent = sContent.Left(sContent.GetLength() - 1);
    m_pTitle->SetText(sContent);
}

vector<RoleInfo> CMyMutiSelectItemWnd::GetAllSelectRole()
{
    return m_pScrollWnd->GetAllSelectRole();
}
