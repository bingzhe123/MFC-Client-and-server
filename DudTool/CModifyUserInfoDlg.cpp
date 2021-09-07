// CModifyUserInfoDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CModifyUserInfoDlg.h"
#include "afxdialogex.h"


// CModifyUserInfoDlg dialog

IMPLEMENT_DYNAMIC(CModifyUserInfoDlg, CDialogEx)

CModifyUserInfoDlg::CModifyUserInfoDlg(vector<CString>& vecUserInfo, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MODIFY_USERINFO, pParent)
{
    m_vecOriUserInfo = vecUserInfo;
}

CModifyUserInfoDlg::~CModifyUserInfoDlg()
{
    delete m_pBtnOK;
    delete m_pBtnCancel;
    m_font.DeleteObject();

}

void CModifyUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_MODIFYUSERINFO_USERNAME, m_editUserName);
    DDX_Control(pDX, IDC_EDIT_MODIFYUSERINFO_lOGINNAME, m_editLoginName);
    DDX_Control(pDX, IDC_EDIT_MODIFYUSERINFO_PASSWORD, m_editPassword);
    DDX_Control(pDX, IDC_EDIT_MODIFYUSERINFO_CONFROM_PASSWORD, m_editConformPassword);
    DDX_Control(pDX, IDC_EDIT_MODIFYUSERINFO_EMAIL, m_editEmail);
}


BEGIN_MESSAGE_MAP(CModifyUserInfoDlg, CDialogEx)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_EN_CHANGE(IDC_EDIT_MODIFYUSERINFO_USERNAME, &CModifyUserInfoDlg::OnEnChangeEditModifyuserinfoUsername)
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CModifyUserInfoDlg message handlers


BOOL CModifyUserInfoDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    m_font.CreateFont(28,                                    //   字体的高度   
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


    x = 250;
    y = 50;
    int iOffset = 80;
    int iEdithHeight = 40;
    int iEditWidth = 350;
    CRect r(x, y, x + iEditWidth, y + iEdithHeight);

    m_editUserName.SetTextColor(RGB(255, 255, 255));
    m_editUserName.SetBackColor(RGB(64, 64, 64));
    m_editUserName.SetWindowTextW(m_vecOriUserInfo[1]);
    m_editUserName.SetFont(&m_font);
    m_editUserName.MoveWindow(r);   
    m_vecEdit.push_back(&m_editUserName);
    y += iOffset;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editLoginName.SetTextColor(RGB(255, 255, 255));
    m_editLoginName.SetBackColor(RGB(64, 64, 64));
    m_editLoginName.SetFont(&m_font);
    m_editLoginName.MoveWindow(r);
    m_editLoginName.SetWindowTextW(m_vecOriUserInfo[2]);
    m_vecEdit.push_back(&m_editLoginName);

    y += iOffset;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editPassword.SetTextColor(RGB(255, 255, 255));
    m_editPassword.SetBackColor(RGB(64, 64, 64));
    m_editPassword.SetFont(&m_font);
    m_editPassword.MoveWindow(r);
    m_vecEdit.push_back(&m_editPassword);


    y += iOffset;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editConformPassword.SetTextColor(RGB(255, 255, 255));
    m_editConformPassword.SetBackColor(RGB(64, 64, 64));
    m_editConformPassword.SetFont(&m_font);
    m_editConformPassword.MoveWindow(r);
    m_vecEdit.push_back(&m_editConformPassword);

    y += iOffset;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editEmail.SetTextColor(RGB(255, 255, 255));
    m_editEmail.SetBackColor(RGB(64, 64, 64));
    m_editEmail.SetFont(&m_font);
    m_editEmail.MoveWindow(r);
    m_editEmail.SetWindowTextW(m_vecOriUserInfo[4]);
    m_vecEdit.push_back(&m_editEmail);

    // TODO:  Add extra initialization here
    int iButtonWidth = 200;
    auto refreshFunc = std::bind(&CModifyUserInfoDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    m_pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnOK->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_pBtnOK->SetText(L"确定");

    int iSplitter = 100;
    x = (iWidth - iButtonWidth * 2 - iSplitter) / 2;
    y = iHeight - iEdithHeight - 100;
    m_pBtnOK->MoveWindow(CPoint(x, y));
    m_pBtnOK->SetDimention(iButtonWidth, iEdithHeight);
    auto clickFun = std::bind(&CModifyUserInfoDlg::OnClickOk, this, std::placeholders::_1);
    m_pBtnOK->RegisterClickFun(clickFun);

    m_pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnCancel->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_pBtnCancel->SetDimention(iButtonWidth, iEdithHeight);
    m_pBtnCancel->SetText(L"取消");
    x += iButtonWidth + iSplitter;
    m_pBtnCancel->MoveWindow(CPoint(x, y));
    clickFun = std::bind(&CModifyUserInfoDlg::OnClickCancel, this, std::placeholders::_1);
    m_pBtnCancel->RegisterClickFun(clickFun);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CModifyUserInfoDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CDialogEx::OnPaint() for painting messages

    // CEditRoleDlg dialog
    int iEditWidth = 250;
    CRect rect;
    GetClientRect(&rect);
    CDC* pDC = GetDC();
    pDC->SetBkMode(TRANSPARENT);
    CDC memDC;
    CBitmap memBitmap;
    memDC.CreateCompatibleDC(NULL);
    int nWidth = rect.Width();
    int nHeight = rect.Height();
    memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);
    memDC.FillSolidRect(rect, RGB(25, 31, 40));

    memDC.SetBkMode(TRANSPARENT);
   auto pOld =  memDC.SelectObject(&m_font);
    memDC.SetTextColor(RGB(255, 255, 255));

    int x = 150;
    int y = 50;
    int iOffset = 80;
    memDC.TextOutW(x, y, L"用户姓名：");
    y += iOffset;
    memDC.TextOutW(x, y, L"    登录名：");

    y += iOffset;
    memDC.TextOutW(x, y, L"       密码：");

    y += iOffset;
    memDC.TextOutW(x, y, L"确认密码：");
    y += iOffset;
    memDC.TextOutW(x, y, L"       邮箱：");

    m_pBtnCancel->Refresh(&memDC, rect);
    m_pBtnOK->Refresh(&memDC, rect);

    RefreshWithEdit(this, pDC, rect, m_vecEdit, memDC);

   // pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}


BOOL CModifyUserInfoDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    return true;
}

void CModifyUserInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pBtnCancel->OnLButtonDown(nFlags, point);
    m_pBtnOK->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CModifyUserInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
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

void CModifyUserInfoDlg::OnOK()
{
    CString sPws;
    m_editPassword.GetWindowTextW(sPws);
    CString sConfirmPws;
    m_editConformPassword.GetWindowTextW(sConfirmPws);
   /* if (sPws.IsEmpty())
    {
        AfxMessageBox(L"密码不能为空!");
        return ;
    }*/
    if (sPws != sConfirmPws)
    {
        AfxMessageBox(L"密码两次输入不相同!");
        return ;
    }
    CString sUserName;
    m_editUserName.GetWindowTextW(sUserName);

    if (sUserName.IsEmpty())
    {
        AfxMessageBox(L"用户名不能为空!");
        return ;
    }

    CString sLoginName;
    m_editLoginName.GetWindowTextW(sLoginName);
    if (sLoginName.IsEmpty())
    {
        AfxMessageBox(L"登录名不能为空!");
        return ;
    }

    CString sEmail;
    m_editEmail.GetWindowTextW(sEmail);
    m_newUserInfo.userName = sUserName;
    m_newUserInfo.loginName = sLoginName;
    m_newUserInfo.password = sPws;
    m_newUserInfo.confirmPassword = sConfirmPws;
    m_newUserInfo.email = sEmail;
    return __super::OnOK();
}

void CModifyUserInfoDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    if (m_pBtnCancel == nullptr)
    {
        return;
    }
    m_pBtnCancel->SetDirty(true);
    m_pBtnOK->SetDirty(true);

    Invalidate();
}

void CModifyUserInfoDlg::OnClickOk(CMyWindow* pWnd)
{
    this->OnOK();

}

void CModifyUserInfoDlg::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();

}

NewUserInfo CModifyUserInfoDlg::GetNewUserInfo()
{
    return m_newUserInfo;
}

void CModifyUserInfoDlg::OnEnChangeEditModifyuserinfoUsername()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialogEx::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}


void CModifyUserInfoDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
}
