#include "pch.h"
#include "CBottomNavigateWnd.h"
#include "CommonDef.h"
CBottomNavigateWnd::~CBottomNavigateWnd()
{
    m_textBox.DestroyWindow();
}

void CBottomNavigateWnd::Initialize(int iTotalCount)
{
    m_bSupportClick = true;
    m_iCurrentPage = 1;
    this->SetBkGroundColor(RGB(0, 0, 0));
    m_iTotalCount = iTotalCount;
    m_pLableTotalCount = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pLableTotalCount->SetMouseHoverSensitive(false);
    m_vecChildWnd.push_back(m_pLableTotalCount);

    m_pBtnLeft = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnLeft->SetText(CString(L"左"));
    m_pBtnLeft->SetBkGroundColor(RGB(0, 0, 0), RGB(255, 128, 255));
    
    m_pBtnLeft->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnFirst(pWnd);
    });

    m_vecChildWnd.push_back(m_pBtnLeft);
    //显示几个翻页按钮
    m_iPageBtnCount = m_iTotalCount / m_iMaxPageCount;
    if (m_iTotalCount % m_iMaxPageCount != 0)
    {
        m_iPageBtnCount++;
    }
    m_iPageBtnCount = m_iPageBtnCount > PerPageButtonCount ? PerPageButtonCount : m_iPageBtnCount;

    auto clickPageFun = std::bind(&CBottomNavigateWnd::OnClickPage, this, std::placeholders::_1);
    for (int  i = 0; i < m_iPageBtnCount; i++)
    {
        auto pBtnPage = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
        pBtnPage->SetBkGroundColor(RGB(0, 0, 0), RGB(255, 128, 255));
        pBtnPage->RegisterClickFun(clickPageFun);
        CString str;
        str.Format(L"%d", i + 1);
        pBtnPage->SetText(str);
        m_vecChildWnd.push_back(pBtnPage);
        m_vecBtnPage.push_back(pBtnPage);
    }
    m_pBtnRight = new CMyButton(this, m_pRootWnd, m_refreshImpFun);

    m_pBtnRight->SetText(CString(L"右"));
    m_pBtnRight->SetBkGroundColor(RGB(0, 0, 0), RGB(255, 128, 255));
    m_pBtnRight->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnLast(pWnd);
    });
    m_vecChildWnd.push_back(m_pBtnRight);

    m_pAward = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pAward->SetMouseHoverSensitive(false);
    m_pAward->SetText(L"前往");
    
    m_vecChildWnd.push_back(m_pAward);

    
    CRect r;
    m_textBox.Create(ES_CENTER|WS_CHILD|WS_VISIBLE|WS_BORDER,r, m_pRootWnd, UWM_IDD_PAGESELECT);
    m_textBox.ModifyStyleEx(WS_TABSTOP, 0, 0);
    auto dwStyle = m_textBox.GetStyle();
    dwStyle |= ES_NUMBER;
    SetWindowLong(m_textBox.m_hWnd, GWL_STYLE, dwStyle);
    m_textBox.SetWindowTextW(L"1");

    m_textBox.SetTextColor(RGB(255, 255, 255)); 
    m_textBox.SetBackColor(RGB(0, 0, 0));
    m_textBox.SetFont(&m_EditBoxfont);
    m_textBox.ShowWindow(SW_HIDE);
    ::PostMessage(m_textBox.m_hWnd, EM_SETSEL, 0, 0);
    m_textBox.PostMessageW(WM_KILLFOCUS, 0, 0);    //失去焦点
    auto finishEditFunc = std::bind(&CBottomNavigateWnd::OnEditSet, this, std::placeholders::_1);
    m_textBox.SetEditFinishFun(finishEditFunc);
    m_pRootWnd->SetFocus();
    m_pPage = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pPage->SetMouseHoverSensitive(false);
    m_pPage->SetText(L"页");
    m_vecChildWnd.push_back(m_pPage);;

}               
void CBottomNavigateWnd::OnSize(int cx, int cy)
{
    auto rect = m_rect;
    int h = rect.Height();
    int w = 100;
    int iPageBtnWidth = 50;
    int iBtnOffset = 3;
    int iControlRegionWidth = (iPageBtnWidth + iBtnOffset) * (5 + m_iPageBtnCount) + 100;
    int iOffsetx = rect.Width() - iControlRegionWidth;
    m_pLableTotalCount->MoveWindow(iOffsetx, 0);
    m_pLableTotalCount->SetDimention(w, h);
    iOffsetx += w + iBtnOffset;
    w = iPageBtnWidth;
    m_pBtnLeft->MoveWindow(iOffsetx, 0);
    m_pBtnLeft->SetDimention(w, h);
    for (int i = 0; i < m_iPageBtnCount; i++)
    {
        iOffsetx += w + iBtnOffset;
        auto pBtnPage = m_vecBtnPage[i];
        pBtnPage->MoveWindow(iOffsetx, 0);
        pBtnPage->SetDimention(w, h);
    }
    iOffsetx += w + iBtnOffset;
    m_pBtnRight->MoveWindow(iOffsetx, 0);
    m_pBtnRight->SetDimention(w, h);
    iOffsetx += w + iBtnOffset;
    m_pAward->MoveWindow(iOffsetx, 0);
    m_pAward->SetDimention(w, h);
   
    iOffsetx += w + iBtnOffset;
    rect = m_pAward->GetClientRect();
    CRect r;
    int iEditHeight = 25;
    int iTop = (h - iEditHeight) / 2;
    int x = rect.left + w + iBtnOffset;;
    int y = rect.top + iTop;
    r.SetRect(x, y, x + w, y + iEditHeight);
    m_textBox.MoveWindow(r);
    iOffsetx += w + iBtnOffset;
    m_pPage->MoveWindow(iOffsetx, 0);
    m_pPage->SetDimention(w, h);
}
void CBottomNavigateWnd::SetTotalCount(int iCount)
{
    if (m_iTotalCount == iCount)
    {
        return;
    }
    CString sTotalNum;
    sTotalNum.Format(L"共%d条", iCount);
    m_iTotalCount = iCount;
    m_pLableTotalCount->SetText(sTotalNum);

    //显示几个翻页按钮
    for each (auto item in m_vecBtnPage)
    {
        auto it = std::find_if(m_vecChildWnd.begin(), m_vecChildWnd.end(), [&item](auto _it)
        {
            return item == _it;
        });
        m_vecChildWnd.erase(it);
        delete item;
    }
    m_vecBtnPage.clear();

    m_iPageCount = m_iTotalCount / m_iMaxPageCount;
    if (m_iTotalCount % m_iMaxPageCount != 0)
    {
        m_iPageCount++;
    }
    m_iPageCount = m_iPageCount <= 0 ? 1 : m_iPageCount;
    if (m_iCurrentPage > m_iPageCount)
    {
        m_iCurrentPage = m_iPageCount;
        CString sPage;
        sPage.Format(L"%d", m_iCurrentPage);
        m_textBox.SetWindowTextW(sPage);

    }
    m_iPageBtnCount = m_iPageCount > PerPageButtonCount ? PerPageButtonCount : m_iPageCount;
    auto clickPageFun = std::bind(&CBottomNavigateWnd::OnClickPage, this, std::placeholders::_1);
    for (int i = 0; i < m_iPageBtnCount; i++)
    {
        auto pBtnPage = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
        pBtnPage->SetBkGroundColor(RGB(0, 0, 0), RGB(255, 128, 255));
        pBtnPage->RegisterClickFun(clickPageFun);
        CString str;
        str.Format(L"%d", i + 1);
        pBtnPage->SetText(str);
        m_vecChildWnd.push_back(pBtnPage);
        m_vecBtnPage.push_back(pBtnPage);
    }

    OnSize(0, 0);
}

int CBottomNavigateWnd::GetTotalCount()
{
    return m_iTotalCount;
}

bool CBottomNavigateWnd::Refresh(CDC* pMemDC , CRect r)
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
    for each (auto item in m_vecChildWnd)
    {
        item->SetDirty(true);
        item->Refresh(pMemDC, r);
    }  
    return true;
}

void CBottomNavigateWnd::ShowEdit(bool bEdit)
{
    if (bEdit)
    {
        m_textBox.ShowWindow(SW_SHOW);
    }
    else
    {
        m_textBox.ShowWindow(SW_HIDE);
    }
}

void CBottomNavigateWnd::OnClickPage(CMyWindow* pWnd)
{
    auto pButton = (CMyButton *)pWnd;
    auto sPage = pButton->GetText();
    int iPage = _ttoi(sPage);
    int iIndex = 0;
    for each (auto item in m_vecBtnPage)
    {
        iIndex++;
        if (pButton == item)
        {
            break;
        }
    }
  /*  if (iIndex == m_iCurrentPage)
    {
        return;
    }*/
   
    //点了最后一个按钮，这一页移动到第一个按钮
    int iLastPageValue = 0;
    if (iIndex == m_iPageBtnCount)
    {
        iLastPageValue = iPage + m_iPageBtnCount - 1;
        iLastPageValue = iLastPageValue > m_iPageCount ? m_iPageCount : iLastPageValue;
    }
    if (iPage == m_iCurrentPage)
    {//当前页面
        return;
    }
    //点了第一个按钮，向前移动一页
    if (iIndex == 1)
    {
        int iFirstPageValue = iPage - (m_iPageBtnCount -1);
        iFirstPageValue = iFirstPageValue < 1 ? 1 : iFirstPageValue;
        iLastPageValue = iFirstPageValue + m_iPageBtnCount - 1;
    }
    if (iIndex == m_iPageBtnCount || iIndex == 1)
    {
        if (iLastPageValue <= m_iPageCount)
        {
            for (auto it = m_vecBtnPage.rbegin(); it != m_vecBtnPage.rend(); it++)
            {
                CString str;
                str.Format(L"%d", iLastPageValue--);
                (*it)->SetText(str);
            }
        }
    }
    m_textBox.SetWindowTextW(sPage);
    m_iCurrentPage = iPage;
    ForceUpdateThisPage();
}

void CBottomNavigateWnd::OnFirst(CMyWindow* pWnd)
{
    if (m_iCurrentPage == 1)
    {                       
        return;
    }
    OnForceFirst(pWnd);
}

void CBottomNavigateWnd::OnForceFirst(CMyWindow* pWnd)
{
    m_iCurrentPage = 1;

    CString sPage;
    sPage.Format(L"%d", m_iCurrentPage);
    m_textBox.SetWindowTextW(sPage);
    auto iLastPageValue = m_iCurrentPage + m_iPageBtnCount - 1;
    for (auto it = m_vecBtnPage.rbegin(); it != m_vecBtnPage.rend(); it++)
    {
        CString str;
        str.Format(L"%d", iLastPageValue--);
        (*it)->SetText(str);
    }
    ForceUpdateThisPage();
}



void CBottomNavigateWnd::OnLast(CMyWindow* pWnd)
{
    m_iCurrentPage = m_iPageCount;

    CString sPage;
    sPage.Format(L"%d", m_iCurrentPage);
    m_textBox.SetWindowTextW(sPage);
    auto iLastPageValue = m_iPageCount;
    for (auto it = m_vecBtnPage.rbegin(); it != m_vecBtnPage.rend(); it++)
    {
        CString str;
        str.Format(L"%d", iLastPageValue--);
        (*it)->SetText(str);
    }
    ForceUpdateThisPage();
}

void CBottomNavigateWnd::OnEditSet(const CString& sText)
{
    //跳到指定的页
    //范围是 (行号*(m_iPageBtnCount-1) + 1) 到 (行号+1）* (m_iPageBtnCount-1) + 1；
    //行号= （iPage - 1)/    (m_iPageBtnCount-1 )
    auto iPage = _ttoi(sText);
    if (m_iCurrentPage == iPage)
    {
        return;
    }
    else if (iPage > m_iPageCount)
    {
        CString sPage;
        sPage.Format(L"%d", m_iPageCount);
        m_textBox.SetWindowTextW(sPage);
        m_iCurrentPage = m_iPageCount;
        iPage = m_iPageCount;
    }
    else if (iPage < 1)
    {
        CString sPage;
        sPage.Format(L"%d", 1);
        m_textBox.SetWindowTextW(sPage);
        m_iCurrentPage = 1;
        iPage = m_iCurrentPage;
    }
    else
    {
        m_iCurrentPage = iPage;
    }
    ForceUpdateThisPage();
    m_pRootWnd->SetFocus();
    m_textBox.PostMessageW(WM_KILLFOCUS, 0, 0);    //失去焦点

}

void CBottomNavigateWnd::ForceUpdateThisPage()
{
    if (m_funPage != nullptr)
    {
        //索引从0开始
        int iStart = (m_iCurrentPage - 1) * m_iMaxPageCount;
        m_funPage(iStart, m_iMaxPageCount);
    }
}

void CBottomNavigateWnd::RegisterOnpageFunction(OnpageFunc f)
{
    m_funPage = f;
}

void CBottomNavigateWnd::SetMaxItemCount(int iMaxItemCount)
{
    m_iMaxPageCount = iMaxItemCount;
}

CEditEx* CBottomNavigateWnd::GetEidt()
{
    return &m_textBox;
}

int CBottomNavigateWnd::GetCurrentPage()
{
    return m_iCurrentPage;
}


