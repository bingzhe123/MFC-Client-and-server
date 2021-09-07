// CConfirmDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CConfirmDlg.h"
#include "afxdialogex.h"


// CConfirmDlg dialog

IMPLEMENT_DYNAMIC(CConfirmDlg, CDialogEx)

CConfirmDlg::CConfirmDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONFIRM, pParent)
{

}

CConfirmDlg::~CConfirmDlg()
{
    m_font.DeleteObject();
    if (m_pImageQuestion != nullptr)
    {
        m_pImageQuestion->Destroy();
        delete m_pImageQuestion;
    }
    delete m_pBtnOK;
    delete m_pBtnCancel;

}

void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CDialogEx)
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CConfirmDlg message handlers


BOOL CConfirmDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // TODO:  Add extra initialization here
    m_pImageQuestion = new CImage();
    CMyWindow::LoadImageFromResource(m_pImageQuestion, IDB_PNG_QUESTION, _T("PNG"));
    m_font.CreateFont(30,                                    //   字体的高度   
        0,                                          //   字体的宽度  
        0,                                          //  nEscapement 
        0,                                          //  nOrientation   
        FW_NORMAL,                                  //   nWeight   
        FALSE,                                      //   bItalic   
        FALSE,                                      //   bUnderline   
        0,                                                   //   cStrikeOut   
        ANSI_CHARSET,                             //   nCharSet   
        OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
        CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
        DEFAULT_QUALITY,                       //   nQuality   
        DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
        _T("微软雅黑"));
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);

    int iHeight = 300;
    int iWidth = 600;
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);

    auto refreshFunc = std::bind(&CConfirmDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    m_pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnOK->SetBkGroundColor(RGB(64, 64, 64), RGB(69, 91, 178));
    m_pBtnOK->SetText(L"确定");

    int iButtonWidth = 150;   
    int iButtonHeight = 30;

    x = (iWidth - iButtonWidth * 2 - 50)/2;
    y  = iHeight - 100;
    m_pBtnOK->MoveWindow(CPoint(x, y));
    m_pBtnOK->SetDimention(iButtonWidth, iButtonHeight);
    auto clickFun = std::bind(&CConfirmDlg::OnClickOk, this, std::placeholders::_1);
    m_pBtnOK->RegisterClickFun(clickFun);

    m_pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnCancel->SetBkGroundColor(RGB(64, 64, 64), RGB(69, 91, 178));
    m_pBtnCancel->SetDimention(iButtonWidth, iButtonHeight);
    m_pBtnCancel->SetText(L"取消");
    x += iButtonWidth + 50;
    m_pBtnCancel->MoveWindow(CPoint(x, y));
    clickFun = std::bind(&CConfirmDlg::OnClickCancel, this, std::placeholders::_1);
    m_pBtnCancel->RegisterClickFun(clickFun);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CConfirmDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    bool bRefresh = false;
    if (m_pBtnOK != nullptr)
    {
        if (m_pBtnOK->HitTest(point) != nullptr)
        {
            if (!m_pBtnOK->IsMouseIn())
            {
                m_pBtnOK->OnMouseIn(nFlags, point);
                bRefresh = true;
            }
        }
        else
        {
            if (m_pBtnOK->IsMouseIn())
            {
                m_pBtnOK->OnMouseLeave();
                bRefresh = true;
            }
        }
    }

    if (m_pBtnCancel != nullptr)
    {
        if (m_pBtnCancel->HitTest(point) != nullptr)
        {
            if (!m_pBtnCancel->IsMouseIn())
            {
                m_pBtnCancel->OnMouseIn(nFlags, point);
                bRefresh = true;
            }
        }
        else
        {
            if (m_pBtnCancel->IsMouseIn())
            {
                m_pBtnCancel->OnMouseLeave();
                bRefresh = true;
            }
        }
    }
    if (bRefresh)
    {
        Invalidate();
    }
    CDialogEx::OnMouseMove(nFlags, point);

}


BOOL CConfirmDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return true;
}


void CConfirmDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pBtnCancel->OnLButtonDown(nFlags, point);
    m_pBtnOK->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CConfirmDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CDialogEx::OnPaint() for painting messages


    CRect rect;
    GetClientRect(&rect);
    CDC* pDC = GetDC();
    CDC memDC;
    CBitmap memBitmap;
    memDC.CreateCompatibleDC(NULL);
    int nWidth = rect.Width();
    int nHeight = rect.Height();
    memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);
    memDC.FillSolidRect(rect, RGB(25, 31, 40));

    memDC.SetBkMode(TRANSPARENT);
    memDC.SelectObject(&m_font);
    memDC.SetTextColor(RGB(255, 255, 255));
    int w = m_pImageQuestion->GetWidth();
    int h = m_pImageQuestion->GetHeight();
    int x = 100;
    int y = (rect.Height() - h)/2 - 30;
   

    m_pImageQuestion->Draw(memDC.m_hDC, x, y, w, h);

    x += w + 50;
    y += 15;
    memDC.TextOutW(x, y, L"确认删除?");

    m_pBtnCancel->Refresh(&memDC, rect);
    m_pBtnOK->Refresh(&memDC, rect);

    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}





void CConfirmDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    if (m_pBtnCancel == nullptr)
    {
        return;
    }
    m_pBtnCancel->SetDirty(true);
    m_pBtnOK->SetDirty(true);

    Invalidate();
}

void CConfirmDlg::OnClickOk(CMyWindow* pWnd)
{

    this->OnOK();
}

void CConfirmDlg::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();
}
 
void CConfirmDlg::OnOK()
{
   
    __super::OnOK();
}