#include "pch.h"
#include "CMylistCtrlWnd.h"
#include "CommonDef.h"


CMyListHeaderWnd::~CMyListHeaderWnd()
{
}



CMyLable* CMyListHeaderWnd::CreateLable(int x, int y, int w, int h, const CString& sContent)
{
    auto pLabel = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLabel->SetText(sContent);
    pLabel->MoveWindow(x, y);
    pLabel->SetDimention(w, h);
    return pLabel;
}
void CMyListHeaderWnd::Initialize(vector<CString> vecHeader, vector<float>& vecColumnRatio)
{
    m_vecRatio = vecColumnRatio;
    this->SetBkGroundColor(RGB(22, 22, 23));
    int iColumnCount = vecHeader.size();
    auto rect = GetClientRect();
    //需求描述和医院占比0.2,姓名0.15， 其他平分宽度
   /* float lengTextDelat = 0.25;
    float nameTextDelat = 0.1;
    float fdelat = (1 - lengTextDelat*2 - nameTextDelat) / (iColumnCount - 3);*/
    int w = rect.Width();
    int iHeight = rect.Height();
    int iOffsetx = 0;
    int iOffsety = 0;
    int iWidth = 0;
    for (int i = 0; i < iColumnCount; i++)
    {
        iWidth = w * vecColumnRatio[i];
        auto pLabel = CreateLable(iOffsetx, iOffsety, iWidth, iHeight, vecHeader[i]);
        pLabel->SetMouseHoverSensitive(false);
        m_vecChildWnd.push_back(pLabel);
        iOffsetx += iWidth;
    }
}

void CMyListHeaderWnd::OnSize(int cx, int cy)
{
    int iColumnCount = m_vecRatio.size();
    auto rect = GetClientRect();
    int w = rect.Width();
    int iHeight = rect.Height();
    int iOffsetx = 0;
    int iOffsety = 0;
    int iWidth = 0;
    for (int i = 0; i < iColumnCount; i++)
    {
        iWidth = w * m_vecRatio[i];
        auto pHeader = m_vecChildWnd[i];
        pHeader->MoveWindow(iOffsetx, iOffsety);
        pHeader->SetDimention(iWidth, iHeight);
        iOffsetx += iWidth;
    }
}

CMyListItemWnd::~CMyListItemWnd()
{
}

bool CMyListItemWnd::Refresh(CDC* pMemDC , CRect r)
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
            pMemDC->FillSolidRect(&rect, m_bkSelectColor);
        }
        else
        {
            pMemDC->FillSolidRect(&rect, m_bkColor);
        }
    }
    if (m_pBkImage != nullptr)
    {
        auto w = m_pBkImage->GetWidth();
        auto h = m_pBkImage->GetHeight();
        auto x = rect.left;
        auto y = (rect.Height() - h) / 2 + rect.top;
        m_pBkImage->Draw(pMemDC->m_hDC, x, y);
    }
    for each (auto item in m_vecChildWnd)
    {
        item->SetDirty(true);
        if (dynamic_cast<CMyLable*>(item) != nullptr)
        {
            item->SetSelectState(m_isMouseIn);
        }
        item->Refresh(pMemDC, r);
    }
    return true;
}
CMyLable* CMyListItemWnd::CreateLable(int x, int y, int w, int h, const CString& sContent)
{
    auto pLabel = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLabel->SetText(sContent);
    pLabel->MoveWindow(x, y);
    pLabel->SetDimention(w, h);
    pLabel->SetFontHeight(DISPLAY_LIST_FONT_SIZE);
    pLabel->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    return pLabel;
}

void CMyListItemWnd::SetBkGroundColor(COLORREF rgbNormal, COLORREF rgbSelect)
{
    m_bSetBkGroundColor = true;
    m_bkColor = rgbNormal;
    m_bkSelectColor = rgbSelect;
}

void CMyListItemWnd::AddItem(const vector<CString>& vecItemContent,
    CMyListHeaderWnd* pHeadWnd, int y, int h)
{
    m_pHeaderWnd = pHeadWnd;
    this->SetMouseHoverSensitive(true);
    this->SetBkGroundColor(DataShowBkColor, RGB(69, 91, 178));
    int width = pHeadWnd->GetMyRect().Width();
    this->MoveWindow(0, y);
    this->SetDimention(width, h);
    auto vecChilds = pHeadWnd->GetChildWnds();
    auto iCountSize = vecItemContent.size();
    for (size_t i = 0; i < iCountSize; i++)
    {
        auto pHeader = vecChilds[i];
        auto rect = pHeader->GetMyRect();
        int x = rect.left;
        int w = rect.Width();
        auto pLable = CreateLable(x, 0, w, h, vecItemContent[i]);
        m_vecChildWnd.push_back(pLable);
        m_mapItem[i] = pLable;
    }
}

void CMyListItemWnd::AddColumnWnd(int iColumn, vector<CMyWindow*> vecButton)
{
    auto vecChilds = m_pHeaderWnd->GetChildWnds();
    auto pHeader = vecChilds[iColumn];
    auto rect = pHeader->GetMyRect();
    int x = rect.left;
    int w = rect.Width();
    int iBtnOffset = 3;
    int iButtonCount = vecButton.size();

    int iRealWidht = w - (iButtonCount - 1) * iBtnOffset;
    int iBtnWidth = iRealWidht / iButtonCount;
    int iBtnHeight = m_rect.Height();
    int y = (m_rect.Height() - iBtnHeight) / 2;
    for each (auto item in vecButton)
    {
        item->MoveWindow(x, y);
        item->SetDimention(iBtnWidth, iBtnHeight);
        m_vecChildWnd.push_back(item);
        x += iBtnWidth + iBtnOffset;
    }
    m_mapClolumnWnd[iColumn] = vecButton;
    m_mapItem[iColumn] = nullptr;
}

vector<CMyWindow*> CMyListItemWnd::GetColumnWnd(int iColunm)
{
    if (m_mapClolumnWnd.find(iColunm)!= m_mapClolumnWnd.end())
    {
        return m_mapClolumnWnd[iColunm];
    }
    return vector<CMyWindow*>();
}

void CMyListItemWnd::RemoveColumnWnd(int iColum)
{
    if (m_mapClolumnWnd.find(iColum) == m_mapClolumnWnd.end())
    {
        return;
    }
    for each (auto item in m_mapClolumnWnd[iColum])
    {
        auto it = std::find_if(m_vecChildWnd.begin(), m_vecChildWnd.end(), [item]( CMyWindow* pWnd)
        {
            return pWnd == item;
        });
        if (it != m_vecChildWnd.end())
        {
            m_vecChildWnd.erase(it);
        }
    }
    UpdateMyWnd(GetClientRect());
}

void CMyListItemWnd::ModifyColumnText(int iColumn, const CString& sText)
{
    if (m_mapItem.find(iColumn) == m_mapItem.end())
    {
        return;
    }
    auto *pLabel = m_mapItem[iColumn];
    pLabel->SetText(sText);
    pLabel->UpdateMyWnd(pLabel->GetClientRect());
}

void CMyListItemWnd::OnSize(int cx, int cy)
{
    if (nullptr == m_pHeaderWnd)
    {
        return;
    }
    int h = GetMyRect().Height();

    auto vecChilds = m_pHeaderWnd->GetChildWnds();
    auto iCountSize = vecChilds.size() - m_mapClolumnWnd.size();
    for (size_t i = 0; i < iCountSize; i++)
    {
        auto pHeader = vecChilds[i];
        auto rect = pHeader->GetMyRect();
        int x = rect.left;
        int w = rect.Width();
        auto pItem = m_vecChildWnd[i];
        pItem->MoveWindow(x, 0);
        pItem->SetDimention(w, h);
    }
    for each (auto item in m_mapClolumnWnd)
    {
        auto vecButtons = item.second;
        auto iColumn = item.first;

        auto pHeader = vecChilds[iColumn];
        auto rect = pHeader->GetMyRect();
        int x = rect.left;
        int w = rect.Width();
        int iBtnOffset = 3;
        int iButtonCount = vecButtons.size();
        int iRealWidht = w - (iButtonCount - 1) * iBtnOffset;
        int iBtnWidth = iRealWidht / iButtonCount;
        for each (auto item in vecButtons)
        {
            item->MoveWindow(x, 0);
            item->SetDimention(iBtnWidth, h);
            x += iBtnWidth + iBtnOffset;
        }
    }
}
