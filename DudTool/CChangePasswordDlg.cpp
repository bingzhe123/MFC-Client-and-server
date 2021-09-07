// CChangePasswordDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CChangePasswordDlg.h"
#include "afxdialogex.h"


// CChangePasswordDlg dialog

IMPLEMENT_DYNAMIC(CChangePasswordDlg, CDialogEx)

CChangePasswordDlg::CChangePasswordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CHANGE_PASSWORD, pParent)
{

}

CChangePasswordDlg::~CChangePasswordDlg()
{
    m_font.DeleteObject();
    delete m_pBtnOK;
    delete m_pBtnCancel;
}

void CChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_OLD_PASSWORD, m_editOld);
    DDX_Control(pDX, IDC_EDIT_NEW_PASSWORD, m_editNew);
    DDX_Control(pDX, IDC_EDIT_CONFIRM_PASSWORD, m_editConfirm);
}


BEGIN_MESSAGE_MAP(CChangePasswordDlg, CDialogEx)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
END_MESSAGE_MAP()

// CChangePasswordDlg message handlers


BOOL CChangePasswordDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here

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


    int iHeight = 400;
    int iWidth = 600;
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);

    x = 150;
    y = 50;
    int iOffset = 80;
    int iEdithHeight = 40;
    int iEditWidth = 350;
    CRect r(x, y, x + iEditWidth, y + iEdithHeight);

    m_editOld.SetTextColor(RGB(255, 255, 255));
    m_editOld.SetBackColor(RGB(64, 64, 64));
    m_editOld.SetFont(&m_font);
    m_editOld.MoveWindow(r);
    m_vecEdit.push_back(&m_editOld);

    y += iOffset;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editNew.SetTextColor(RGB(255, 255, 255));
    m_editNew.SetBackColor(RGB(64, 64, 64));
    m_editNew.SetFont(&m_font);
    m_editNew.MoveWindow(r);
    m_vecEdit.push_back(&m_editNew);

    y += iOffset;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editConfirm.SetTextColor(RGB(255, 255, 255));
    m_editConfirm.SetBackColor(RGB(64, 64, 64));
    m_editConfirm.SetFont(&m_font);
    m_editConfirm.MoveWindow(r);
    m_vecEdit.push_back(&m_editConfirm);

    int iButtonWidth = 200;
    auto refreshFunc = std::bind(&CChangePasswordDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    m_pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnOK->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_pBtnOK->SetText(L"确定");

    int iSplitter = 100;
    x = (iWidth - iButtonWidth * 2 - iSplitter) / 2;
    y = iHeight - iEdithHeight - 60;
    m_pBtnOK->MoveWindow(CPoint(x, y));
    m_pBtnOK->SetDimention(iButtonWidth, iEdithHeight);
    auto clickFun = std::bind(&CChangePasswordDlg::OnClickOk, this, std::placeholders::_1);
    m_pBtnOK->RegisterClickFun(clickFun);

    m_pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnCancel->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_pBtnCancel->SetDimention(iButtonWidth, iEdithHeight);
    m_pBtnCancel->SetText(L"取消");
    x += iButtonWidth + iSplitter;
    m_pBtnCancel->MoveWindow(CPoint(x, y));
    clickFun = std::bind(&CChangePasswordDlg::OnClickCancel, this, std::placeholders::_1);
    m_pBtnCancel->RegisterClickFun(clickFun);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CChangePasswordDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CDialogEx::OnPaint() for painting messages

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
    memDC.SelectObject(&m_font);
    memDC.SetTextColor(RGB(255, 255, 255));

    int x = 50;
    int y = 50;
    int iOffset = 80;
    memDC.TextOutW(x, y, L"   原密码：");
    y += iOffset;
    memDC.TextOutW(x, y, L"   新密码：");
    y += iOffset;
    memDC.TextOutW(x, y, L"确认密码：");

    m_pBtnCancel->Refresh(&memDC, rect);
    m_pBtnOK->Refresh(&memDC, rect);

    RefreshWithEdit(this, pDC, rect, m_vecEdit, memDC);
    memDC.SelectObject(pOldbit);

    //pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}

void CChangePasswordDlg::OnOK()
{
    m_editOld.GetWindowTextW(m_oldPassword);
    m_editNew.GetWindowTextW(m_newPassword);
    m_editConfirm.GetWindowTextW(m_confrimPassword);
    if (m_oldPassword.IsEmpty() || m_newPassword.IsEmpty() || m_confrimPassword.IsEmpty())
    {
        AfxMessageBox(L"输入不能为空!");
        return;
    }
    if (m_newPassword != m_confrimPassword)
    {
        AfxMessageBox(L"两次输入不一致!");
        return;
    }
    __super::OnOK();
}


void CChangePasswordDlg::OnMouseMove(UINT nFlags, CPoint point)
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
    CDialogEx::OnMouseMove(nFlags, point);
}


void CChangePasswordDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pBtnCancel->OnLButtonDown(nFlags, point);
    m_pBtnOK->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CChangePasswordDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return true;
}


void CChangePasswordDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    if (m_pBtnCancel == nullptr)
    {
        return;
    }
    m_pBtnCancel->SetDirty(true);
    m_pBtnOK->SetDirty(true);

    Invalidate();
}

void CChangePasswordDlg::OnClickOk(CMyWindow* pWnd)
{
    this->OnOK();

}

void CChangePasswordDlg::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();
}

void CChangePasswordDlg::GetNewPasswordInfo(CString& sOld, CString& sNew )
{
    sOld = m_oldPassword;
    sNew = m_newPassword;
}
