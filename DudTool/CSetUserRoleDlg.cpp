// CSetUserRoleDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CSetUserRoleDlg.h"
#include "afxdialogex.h"


// CSetUserRoleDlg dialog

IMPLEMENT_DYNAMIC(CSetUserRoleDlg, CDialogEx)

CSetUserRoleDlg::CSetUserRoleDlg(vector<RoleInfo>& vecRoleInfo, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ASIGN_ROLE, pParent)
{
    m_vecRoleInfo = vecRoleInfo;
}

CSetUserRoleDlg::~CSetUserRoleDlg()
{
    for each (auto item in m_vecWnd)
    { 
        delete item;
    }                         
}

void CSetUserRoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetUserRoleDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CSetUserRoleDlg message handlers


BOOL CSetUserRoleDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

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



    int iButtonWidth = 150;
    auto refreshFunc = std::bind(&CSetUserRoleDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);

    int iOffsetx = 130;
    m_pMutiComboxWnd = new CMyMutiSelectItemWnd(nullptr, this, refreshFunc);
    m_pMutiComboxWnd->MoveWindow(iOffsetx, 20);
    m_pMutiComboxWnd->SetDimention(iWidth- iOffsetx - 50, 400);
 
    m_pMutiComboxWnd->Initialize(m_vecRoleInfo);
    m_vecWnd.push_back(m_pMutiComboxWnd);
    m_pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnOK->SetBkGroundColor(RGB(64, 64, 64), RGB(69, 91, 178));
    m_pBtnOK->SetText(L"确定");

    x = 150;
    int iEditWidth = 400;
    int iEdithHeight = 40;
    y = iHeight - iEdithHeight - 60;

    m_pBtnOK->MoveWindow(CPoint(x, y));
    m_pBtnOK->SetDimention(iButtonWidth, iEdithHeight);
    auto clickFun = std::bind(&CSetUserRoleDlg::OnClickOk, this, std::placeholders::_1);
    m_pBtnOK->RegisterClickFun(clickFun);
    m_vecWnd.push_back(m_pBtnOK);

    m_pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnCancel->SetBkGroundColor(RGB(64, 64, 64), RGB(69, 91, 178));
    m_pBtnCancel->SetDimention(iButtonWidth, iEdithHeight);
    m_pBtnCancel->SetText(L"取消");
    x += 300;
    m_pBtnCancel->MoveWindow(CPoint(x, y));
    clickFun = std::bind(&CSetUserRoleDlg::OnClickCancel, this, std::placeholders::_1);
    m_pBtnCancel->RegisterClickFun(clickFun);
    m_vecWnd.push_back(m_pBtnCancel);

    m_pMutiComboxWnd->OnSize(0, 0);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CSetUserRoleDlg::OnPaint()
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

    memDC.TextOutW(50, 50, L"角色:");

    m_pMutiComboxWnd->Refresh(&memDC, rect);
    m_pBtnCancel->Refresh(&memDC, rect);
    m_pBtnOK->Refresh(&memDC, rect);

    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOld);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}


BOOL CSetUserRoleDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return TRUE;
}


void CSetUserRoleDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    bool bMouseMoveInWnd = false;
    for each (auto item in m_vecWnd)
    {
        auto pWnd = item;
        if (pWnd->OnMouseMove(nFlags, point))
        {
            if (m_pCurrentMouseInWnd != nullptr && pWnd != m_pCurrentMouseInWnd)
            {
                m_pCurrentMouseInWnd->OnMouseLeave();
            }
            pWnd->OnMouseIn(nFlags, point);
            m_pCurrentMouseInWnd = pWnd;
            bMouseMoveInWnd = true;
            break;
        }
        if (pWnd->HitTest(point))
        {
            bMouseMoveInWnd = true;
        }
    }
    if (!bMouseMoveInWnd && m_pCurrentMouseInWnd != nullptr)
    {
        m_pCurrentMouseInWnd->OnMouseLeave();
    }
    CDialogEx::OnMouseMove(nFlags, point);
}


void CSetUserRoleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
   // m_pBtnCancel->OnLButtonDown(nFlags, point);
   // m_pBtnOK->OnLButtonDown(nFlags, point);
    if (nullptr == m_pCurrentMouseInWnd)
    {
        return;
    }
    m_pCurrentMouseInWnd->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CSetUserRoleDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    if (m_pBtnCancel == nullptr)
    {
        return;
    }
    m_pBtnCancel->SetDirty(true);
    m_pBtnOK->SetDirty(true);

    Invalidate();
}

void CSetUserRoleDlg::OnClickOk(CMyWindow* pWnd)
{

    this->OnOK();
}

void CSetUserRoleDlg::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();
}

vector<RoleInfo> CSetUserRoleDlg::GetAllSelectRole()
{
    return m_pMutiComboxWnd->GetAllSelectRole();
}

void CSetUserRoleDlg::OnOK()
{

    __super::OnOK();
}


void CSetUserRoleDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // This feature requires Windows Vista or greater.
    // The symbol _WIN32_WINNT must be >= 0x0600.
    // TODO: Add your message handler code here and/or call default
    CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CSetUserRoleDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default

    m_pMutiComboxWnd->OnMouseWheel(nFlags, zDelta, pt);
    return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CSetUserRoleDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (nullptr == m_pCurrentMouseInWnd)
    {
        return;
    }
    m_pCurrentMouseInWnd->OnLButtonUp(nFlags, point);
    CDialogEx::OnLButtonUp(nFlags, point);
}
