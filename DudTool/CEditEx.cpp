#include "pch.h"
#include "CEditEx.h"
 

CEditEx::CEditEx()
{
}

CEditEx::~CEditEx()
{
    if (m_brBackGnd.GetSafeHandle())
        m_brBackGnd.DeleteObject();
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)

    //{{AFX_MSG_MAP(CEditEx)
    ON_WM_CTLCOLOR_REFLECT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CEditEx message handlers

HBRUSH CEditEx::CtlColor(CDC * pDC, UINT nCtlColor)
{
    // TODO: Change any attributes of the DC here

    // TODO: Return a non-NULL brush if the parent's handler should not be called
    // TODO: Return a non-NULL brush if the parent's handler should not be called

    //set text color
    pDC->SetTextColor(m_crText);
    //set the text's background color
    pDC->SetBkColor(m_crBackGnd);
    //return the brush used for background this sets control background
    return m_brBackGnd;
}


void CEditEx::SetBackColor(COLORREF rgb)
{
    //set background color ref (used for text's background)
    m_crBackGnd = rgb;

    //free brush
    if (m_brBackGnd.GetSafeHandle())
        m_brBackGnd.DeleteObject();
    //set brush to new color
    m_brBackGnd.CreateSolidBrush(rgb);

    //redraw
    Invalidate(TRUE);
}


void CEditEx::SetTextColor(COLORREF rgb)
{
    //set text color ref
    m_crText = rgb;

    //redraw
    Invalidate(TRUE);
}

BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
    if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
    {
        if (pMsg->wParam == VK_RETURN)
        {
            CString sText;
            GetWindowTextW(sText);
            if (m_endEidtFuc != nullptr)
            {
                m_endEidtFuc(sText);
            }
            //AfxMessageBox(sText);
            //this->PostMessageW(WM_KILLFOCUS, 0, 0);    //Ê§È¥½¹µã
            UpdateData(TRUE);
        }
    }      
    if (WM_LBUTTONDOWN == pMsg->message)
    {
        if (m_clickFun != nullptr)
        {
            CString sText;
            GetWindowTextW(sText);
            m_clickFun(sText);
        }
       
    }
    return CEdit::PreTranslateMessage(pMsg);
}

void CEditEx::SetEditFinishFun(EndEditFuc fun)
{
    m_endEidtFuc = fun;
}

void CEditEx::SetClickFun(ClickFuc fun)
{
    m_clickFun = fun;
}
