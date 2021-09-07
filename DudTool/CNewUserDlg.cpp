// CNewUserDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CNewUserDlg.h"
#include "afxdialogex.h"
#include "CommonDef.h"


// CNewUserDlg dialog
const int iTextx = 130;
const int iTesty = 50;
const int iOffsety = 80;
IMPLEMENT_DYNAMIC(CNewUserDlg, CDialogEx)

CNewUserDlg::CNewUserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD_NEWUSER, pParent)
{

}

CNewUserDlg::CNewUserDlg(const NewUserInfo& newInfo, CWnd* pParent ) : CDialogEx(IDD_DIALOG_ADD_NEWUSER, pParent)
{
    m_newUserInfo = newInfo;
}

CNewUserDlg::~CNewUserDlg()
{
    m_font.DeleteObject();
    delete m_pBtnOK;
    delete m_pBtnCancel;
    if (m_pImageStar != nullptr)
    {
        m_pImageStar->Destroy();
        delete m_pImageStar;
    }
}

void CNewUserDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewUserDlg, CDialogEx)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CNewUserDlg message handlers


BOOL CNewUserDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();


    m_pImageStar = new CImage();
    CMyWindow::LoadImageFromResource(m_pImageStar, IDB_PNG_START, _T("PNG"));


    // TODO:  Add extra initialization here
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

    int iHeight = 600;
    int iWidth = 800;
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);


    x = iTextx + 120;
    y = iTesty;
    int iEditWidth = 400;
    int iEdithHeight = 40;
    
    CRect r(x, y, x + iEditWidth, y + iEdithHeight);
    m_editUserName.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, r, this, UWM_IDD_EDIT_ADDUSER_USERNAME);
    m_editUserName.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editUserName.SetWindowTextW(L"");
    m_editUserName.SetTextColor(RGB(255, 255, 255));
    m_editUserName.SetBackColor(RGB(64, 64, 64));
    m_editUserName.SetFont(&m_font);
    m_editUserName.ShowWindow(SW_SHOW);
    ::PostMessage(m_editUserName.m_hWnd, EM_SETSEL, 0, 0);
    m_vecEdit.push_back(&m_editUserName);

    y += iOffsety;
    CRect rectCode(x, y, x + iEditWidth, y + iEdithHeight);
    m_editLoginName.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, rectCode, this, UWM_IDD_EDIT_ADDUSER_LOGINNAME);
    m_editLoginName.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editLoginName.SetWindowTextW(L"");
    m_editLoginName.SetTextColor(RGB(255, 255, 255));
    m_editLoginName.SetBackColor(RGB(64, 64, 64));
    m_editLoginName.SetFont(&m_font);
    m_editLoginName.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editLoginName);

    y += iOffsety;
    rectCode.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editPassword.Create(ES_PASSWORD|WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, rectCode, this, UWM_IDD_EDIT_ADDUSER_PASSWORD);
    m_editPassword.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editPassword.SetWindowTextW(L"");
    m_editPassword.SetTextColor(RGB(255, 255, 255));
    m_editPassword.SetBackColor(RGB(64, 64, 64));
    m_editPassword.SetFont(&m_font);
    m_editPassword.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editPassword);


    y += iOffsety;
    rectCode.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editConfirmPassword.Create(ES_PASSWORD|WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, rectCode, this, UWM_IDD_EDIT_ADDUSER_CONFIRM_PASSWORD);
    m_editConfirmPassword.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editConfirmPassword.SetWindowTextW(L"");
    m_editConfirmPassword.SetTextColor(RGB(255, 255, 255));
    m_editConfirmPassword.SetBackColor(RGB(64, 64, 64));
    m_editConfirmPassword.SetFont(&m_font);
    m_editConfirmPassword.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editConfirmPassword);


    y += iOffsety;
    rectCode.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editEmail.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, rectCode, this, UWM_IDD_EDIT_ADDUSER_EMAIL);
    m_editEmail.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editEmail.SetWindowTextW(L"");
    m_editEmail.SetTextColor(RGB(255, 255, 255));
    m_editEmail.SetBackColor(RGB(64, 64, 64));
    m_editEmail.SetFont(&m_font);
    m_editEmail.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editEmail);


    int iButtonWidth = 150;
    auto refreshFunc = std::bind(&CNewUserDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    m_pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnOK->SetBkGroundColor(RGB(64, 64, 64), RGB(69, 91, 178));
    m_pBtnOK->SetText(L"确定");
     
    x = iTextx+50;
    y += iOffsety+20;
    m_pBtnOK->MoveWindow(CPoint(x, y));
    m_pBtnOK->SetDimention(iButtonWidth, iEdithHeight);
    auto clickFun = std::bind(&CNewUserDlg::OnClickOk, this, std::placeholders::_1);
    m_pBtnOK->RegisterClickFun(clickFun);

    m_pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnCancel->SetBkGroundColor(RGB(64, 64, 64), RGB(69, 91, 178));
    m_pBtnCancel->SetDimention(iButtonWidth, iEdithHeight);
    m_pBtnCancel->SetText(L"取消");
    x += 300;
    m_pBtnCancel->MoveWindow(CPoint(x, y));
    clickFun = std::bind(&CNewUserDlg::OnClickCancel, this, std::placeholders::_1);
    m_pBtnCancel->RegisterClickFun(clickFun);

    if (!m_newUserInfo.userName.IsEmpty())
    {
        m_editUserName.SetWindowTextW(m_newUserInfo.userName);
        m_editLoginName.SetWindowTextW(m_newUserInfo.loginName);
        m_editPassword.SetWindowTextW(m_newUserInfo.password);
        m_editConfirmPassword.SetWindowTextW(m_newUserInfo.confirmPassword);
        m_editEmail.SetWindowTextW(m_newUserInfo.email);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNewUserDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pBtnCancel->OnLButtonDown(nFlags, point);
    m_pBtnOK->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CNewUserDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnMouseMove(nFlags, point);
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
}


BOOL CNewUserDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return true;
}


void CNewUserDlg::OnPaint()
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
    auto pOld = memDC.SelectObject(&m_font);
    memDC.SetTextColor(RGB(255, 255, 255));

                         
    int y = iTesty;
    memDC.TextOutW(iTextx, y, L"用户姓名:");

    int w = m_pImageStar->GetWidth();
    int h = m_pImageStar->GetHeight();
    int iOffsetStar = 6;
    int x = iTextx - w - iOffsetStar;
    m_pImageStar->Draw(memDC.m_hDC, x, y + iOffsetStar, w, h);


    y += iOffsety;
    memDC.TextOutW(iTextx, y, L"   登录名:");
    m_pImageStar->Draw(memDC.m_hDC, x, y + iOffsetStar, w, h);
    y += iOffsety;
    memDC.TextOutW(iTextx, y, L"      密码:");
    m_pImageStar->Draw(memDC.m_hDC, x, y + iOffsetStar, w, h);
    y += iOffsety;
    memDC.TextOutW(iTextx, y, L"确认密码:");
    m_pImageStar->Draw(memDC.m_hDC, x, y + iOffsetStar, w, h);
    y += iOffsety;
    memDC.TextOutW(iTextx, y, L"      邮箱:");

    m_pBtnCancel->Refresh(&memDC, rect);
    m_pBtnOK->Refresh(&memDC, rect);

    RefreshWithEdit(this, pDC, rect, m_vecEdit, memDC);
    //pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOld);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}

void CNewUserDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    if (m_pBtnCancel == nullptr)
    {
        return;
    }
    m_pBtnCancel->SetDirty(true);
    m_pBtnOK->SetDirty(true);

    Invalidate();
}

void CNewUserDlg::OnClickOk(CMyWindow* pWnd)
{

    this->OnOK();
}

void CNewUserDlg::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();
}
NewUserInfo CNewUserDlg::GetNewUserInfo()
{
    return m_newUserInfo;
}
void CNewUserDlg::OnOK()
{
    m_editUserName.GetWindowTextW(m_newUserInfo.userName);

    m_editLoginName.GetWindowTextW(m_newUserInfo.loginName);
    m_editPassword.GetWindowTextW(m_newUserInfo.password);
    m_editConfirmPassword.GetWindowTextW(m_newUserInfo.confirmPassword);
    m_editEmail.GetWindowTextW(m_newUserInfo.email);
    if (m_newUserInfo.password != m_newUserInfo.confirmPassword)
    {
        AfxMessageBox(L"密码不一致！");
        return;
    }
    if (m_newUserInfo.userName.IsEmpty() ||
        m_newUserInfo.loginName.IsEmpty() ||
        m_newUserInfo.password.IsEmpty())
    {
        AfxMessageBox(L"输入信息不能为空！");
        return;
    }
    __super::OnOK();
}

 
