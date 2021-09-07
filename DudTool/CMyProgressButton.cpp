#include "pch.h"
#include "CMyProgressButton.h"

CMyProgressButton::CMyProgressButton(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyContainerWnd(pParent, pRootWnd, fun)
{
    m_pButton = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pButton->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_vecChildWnd.push_back(m_pButton);
    m_pProgressBar = new CMyprogressBar(this, m_pRootWnd, m_refreshImpFun);
    m_vecChildWnd.push_back(m_pProgressBar);
}

bool CMyProgressButton::Refresh(CDC* pMemDC, CRect r)
{
    return __super::Refresh(pMemDC, r);
}

void CMyProgressButton::OnSize(int cx, int cy)
{


}

void CMyProgressButton::SetText(const CString& sText)
{
    m_pButton->SetText(sText);
}
                 
void CMyProgressButton::RegisterClickFun(ClickFunc fun)
{
    m_pButton->RegisterClickFun(fun);
}

void CMyProgressButton::SetDimention(int w, int h)
{
    __super::SetDimention(w, h);
    int iOffsety = 5;
    auto rect = GetClientRect();
    int x = 0;
    int iBtnHeight = h - m_iProgressHeight * 2 - iOffsety*2;
    int y = (h - iBtnHeight) / 2;
    m_pButton->MoveWindow(x, y);
    m_pButton->SetDimention(w, iBtnHeight);

    y = rect.Height() - m_iProgressHeight - iOffsety;
    m_pProgressBar->MoveWindow(x, y);
    m_pProgressBar->SetDimention(w, m_iProgressHeight);
}
void CMyProgressButton::SetProgressPos(int iPos)
{
    m_pProgressBar->SetPos(iPos);
}

CMyprogressBar* CMyProgressButton::GetProgressBar()
{
    return m_pProgressBar;
}

void CMyProgressButton::SetMouseHoverSensitive(bool bMouseHoverSensitive)
{
    m_bMouseHoverSensitive = bMouseHoverSensitive;
    for each (auto item in m_vecChildWnd)
    {
        item->SetMouseHoverSensitive(bMouseHoverSensitive);
    }
}
