// CLoginDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CLoginDlg.h"
#include "afxdialogex.h"
#include "CommonDef.h"
#include <string>
#include "httpHelper.h"
using std::string;

// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_Login, pParent)
{
    //this->text
}

CLoginDlg::~CLoginDlg()
{
    delete m_pBkWnd;
    delete m_pBottom;
    delete m_pBtnLogin;
    m_pBkImage->Destroy();
    delete m_pBkImage;
    m_pBottomImage->Destroy();
    delete m_pBottomImage;
    m_pLoginWndImage->Destroy();
    delete m_pLoginWndImage;
    m_font.DeleteObject();
    m_editUserName.DestroyWindow();
    m_editCode.DestroyWindow();

}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CLoginDlg message handlers


BOOL CLoginDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_font.CreateFont(30,                                    //   ◊÷ÃÂµƒ∏ﬂ∂»   
        0,                                          //   ◊÷ÃÂµƒøÌ∂»  
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
        _T("Œ¢»Ì—≈∫⁄"));


    m_pBkImage = new CImage();
    CMyWindow::LoadImageFromResource(m_pBkImage, IDB_PNG_LONGIN_BK, _T("PNG"));
    m_pBottomImage = new CImage();
    CMyWindow::LoadImageFromResource(m_pBottomImage, IDB_PNG_LOGIN_BOTTOM, _T("PNG"));
    m_pLoginWndImage = new CImage();
    CMyWindow::LoadImageFromResource(m_pLoginWndImage, IDB_PNG_LOGIN_DLG, _T("PNG"));
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    int iHeight = m_pBkImage->GetHeight() + m_pBottomImage->GetHeight() + 0;
    int iWidth = iHeight * 1.5;

    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);
    

    CRect rect;
    GetClientRect(&rect);

    int w = m_pLoginWndImage->GetWidth();
    x = rect.right - w + 50;
    y = 270;
    int iEditWidth = w - 100;
    int iEdithHeight = 30;
    CRect r(x, y, x + iEditWidth, y + iEdithHeight);
    m_editUserName.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER| ES_LEFT, r, this, UWM_IDD_USER_LOGIN_NAME);
    m_editUserName.ModifyStyleEx(WS_TABSTOP, 0, 0);
    //auto dwStyle = m_editUserName.GetStyle();
    //dwStyle |= ES_NUMBER;
    //SetWindowLong(m_editUserName.m_hWnd, GWL_STYLE, dwStyle);
    m_editUserName.SetWindowTextW(L"");
    m_editUserName.SetTextColor(RGB(255, 255, 255));
    m_editUserName.SetBackColor(RGB(64, 64, 64));
    m_editUserName.SetFont(&m_font);
    m_editUserName.ShowWindow(SW_SHOW);
    ::PostMessage(m_editUserName.m_hWnd, EM_SETSEL, 0, 0);
    m_vecEdit.push_back(&m_editUserName);
    y = 355;
    CRect rectCode(x, y, x + iEditWidth, y + iEdithHeight);
    m_editCode.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_PASSWORD, rectCode, this, UWM_IDD_USER_LOGIN_NAME);
    m_editCode.ModifyStyleEx(WS_TABSTOP, 0, 0);
    //auto dwStyle = m_editUserName.GetStyle();
    //dwStyle |= ES_NUMBER;
    //SetWindowLong(m_editUserName.m_hWnd, GWL_STYLE, dwStyle);
    m_editCode.SetWindowTextW(L"");
    m_editCode.SetTextColor(RGB(255, 255, 255));
    m_editCode.SetBackColor(RGB(64, 64, 64));
    m_editCode.SetFont(&m_font);
    m_editCode.ShowWindow(SW_SHOW);
   // ::PostMessage(m_editCode.m_hWnd, EM_SETSEL, 0, 0);
    m_vecEdit.push_back(&m_editCode);

    auto refreshFunc = std::bind(&CLoginDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    m_pBtnLogin = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnLogin->SetIconPic(IDB_PNG_BTN_LOGIN_NORMAL, IDB_PNG_LOGIN_BTN_CLICK);
    m_pBtnLogin->SetDimention(iEditWidth, iEdithHeight);
    y = 430;
    m_pBtnLogin->MoveWindow(CPoint(x, y));

    auto clickFun = std::bind(&CLoginDlg::Onlogin, this, std::placeholders::_1);

    m_pBtnLogin->RegisterClickFun(clickFun);
        
    m_bInit = true;
    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
void CLoginDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    SetDirtyRect(pWnd, rect);
    Invalidate();
}

void CLoginDlg::SetDirtyRect(CMyWindow* pWnd, CRect rect)
{

}

void CLoginDlg::OnPaint()
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
    memDC.FillSolidRect(rect, RGB(10, 10, 10));

    auto w = m_pBkImage->GetWidth();
    auto h = m_pBkImage->GetHeight();
    auto iSorx = 400;
    auto iSory = 0;
    auto iDesx = 0;
    auto iDesy = 0;
    m_pBkImage->Draw(memDC.m_hDC, iDesx, iDesy, w,h, iSorx, iSory, w, h);
    
    w = m_pBottomImage->GetWidth();
    h = m_pBottomImage->GetHeight();
    iDesy = rect.Height() - h;

    m_pBottomImage->Draw(memDC.m_hDC, iDesx, iDesy, w, h, 0, iSory, w, h);
    w = m_pLoginWndImage->GetWidth();
    h = m_pLoginWndImage->GetHeight();
    iDesx = rect.right - w;
    iDesy = (rect.Height() - h)/2;
    iSorx = 0;
    iSory = 0;
    m_pLoginWndImage->Draw(memDC.m_hDC, iDesx, iDesy, w, h, iSorx, iSory, w, h);

    memDC.SetBkMode(TRANSPARENT);
    auto pOld = memDC.SelectObject(&m_font);
    iDesx = rect.right - w + 50;
    iDesy += 150;
    memDC.TextOutW(iDesx, iDesy, L"”√ªß√˚");

    iDesy = 270 + 50;

    memDC.TextOutW(iDesx, iDesy, L"√‹¬Î");
    m_pBtnLogin->Refresh(&memDC,rect); 
    
    RefreshWithEdit(this, pDC, rect, m_vecEdit, memDC);
    // pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOld);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}


BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return false;
}


void CLoginDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
 
    // TODO: Add your message handler code here
}
void CLoginDlg::Resize()
{
    auto bkSize = m_pBkWnd->GetBkPicSize();
    int iHeight = bkSize.cy;
    int iWidth = bkSize.cx;
    int iOffsetx = 0;
    int iOffsety = 0;
  /*  m_pBkWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pBkWnd->SetDimention(iWidth, iHeight);*/
    CRect rect(iOffsetx, iOffsety, iWidth, iHeight);
   // SetDirtyRect(m_pBkWnd, rect);

    iOffsety += bkSize.cy;
    bkSize = m_pBottom->GetBkPicSize();
    m_pBottom->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pBottom->SetDimention(iWidth, bkSize.cy);
    rect.SetRect(iOffsetx, iOffsety, iWidth, bkSize.cy);
    SetDirtyRect(m_pBottom, rect);
}




void CLoginDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (m_pBtnLogin != nullptr)
    {
        if (m_pBtnLogin->HitTest(point) != nullptr )
        {
            if (!m_pBtnLogin->IsMouseIn())
            {
                m_pBtnLogin->OnMouseIn(nFlags, point);
                Invalidate();
            }
        }
        else
        {
            if (m_pBtnLogin->IsMouseIn())
            {
                m_pBtnLogin->OnMouseLeave();
                Invalidate();
            }
        }

    }

    CDialogEx::OnMouseMove(nFlags, point);
}


void CLoginDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pBtnLogin->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}
void CLoginDlg::Onlogin(CMyWindow* pWnd)
{
    CString sUserName;
    m_editUserName.GetWindowTextW(sUserName);
    CString sPassWord;
    m_editCode.GetWindowTextW(sPassWord);
    if (VerifyUser(sUserName, sPassWord))
    {
        CDialogEx::OnOK();
    }
    else
    {
        MessageBox(L"’À∫≈ªÚ√‹¬Î¥ÌŒÛ", L"Arigin Medical");
    }
}

bool CLoginDlg::VerifyUser(const CString& sUser, const CString& sPassWord)
{

    std::string sStrPassword = CW2A(sPassWord.GetString());
    std::string sStrUserName = CW2A(sUser.GetString());
    if (sStrUserName.empty() || sStrPassword.empty())
    {
        return false;
    }
    //return true;
    HttpHelper verify(theApp.m_spClient);
    bool bIsManager = false;
    int iRole = 0;
    auto bSuccess = verify.IsValidUser(sStrUserName,
        sStrPassword,
        theApp.m_sUserID,
        theApp.m_bIsManager,
        theApp.m_userAuthority);
    theApp.m_sCurrentUserName = sUser;
    return bSuccess;
}


void CLoginDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    CString sUserName;
    m_editUserName.GetWindowTextW(sUserName);
    CString sPassWord;
    m_editCode.GetWindowTextW(sPassWord);
    if (VerifyUser(sUserName, sPassWord))
    {
        CDialogEx::OnOK();
    }
    else
    {
        MessageBox(L"’À∫≈ªÚ√‹¬Î¥ÌŒÛ");
    }
}
