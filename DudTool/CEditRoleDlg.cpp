// CEditRoleDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CEditRoleDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "CommonDef.h"
#include "httpHelper.h"

static const int iTextx = 60;
static const int iTesty = 50;
static const int iOffsety = 40;
// CEditRoleDlg dialog
static int iEditWidth = 250;

IMPLEMENT_DYNAMIC(CEditRoleDlg, CDialogEx)

CEditRoleDlg::CEditRoleDlg(const vector<CString>& vecRoleInfo, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ROLE_EDIT, pParent)
{
    m_vecRoleInfo = vecRoleInfo;
    //m_sRoleUID = sRoleId;
}

CEditRoleDlg::~CEditRoleDlg()
{
    m_font.DeleteObject();
    delete m_pBtnOK;
    delete m_pBtnCancel;
    m_editRoleDescription.DestroyWindow();
    m_editRoleDescription.DestroyWindow();

}

void CEditRoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditRoleDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CEditRoleDlg message handlers


BOOL CEditRoleDlg::OnInitDialog()
{
    m_font.CreateFont(26,                                    //   字体的高度   
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
    int iWidth = 500;
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);

    CRect rect;
    GetClientRect(&rect);
    x = iTextx + 100;
    y = iTesty;
    int iEdithHeight = 30;
    CRect r(x, y, x + iEditWidth, y + iEdithHeight);
    m_editRoleDescription.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, r, this, UWM_IDD_EDIT_ROLE_DESCRIPTION);
    m_editRoleDescription.ModifyStyleEx(WS_TABSTOP, 0, 0);
    //auto dwStyle = m_editUserName.GetStyle();
    //dwStyle |= ES_NUMBER;
    //SetWindowLong(m_editUserName.m_hWnd, GWL_STYLE, dwStyle);
    m_editRoleDescription.SetWindowTextW(m_vecRoleInfo[5]);
    m_editRoleDescription.SetTextColor(RGB(255, 255, 255));
    m_editRoleDescription.SetBackColor(RGB(64, 64, 64));
    m_editRoleDescription.SetFont(&m_font);
    m_editRoleDescription.ShowWindow(SW_SHOW);
    ::PostMessage(m_editRoleDescription.m_hWnd, EM_SETSEL, 0, 0);
    m_vecEdit.push_back(&m_editRoleDescription);

    //x += iEditWidth + 150;
    y += iEdithHeight + iOffsety + 10;
    CRect rectCode(x, y, x + iEditWidth, y + iEdithHeight);
    m_editRoleName.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, rectCode, this, UWM_IDD_EDIT_ROLE_NAME);
    m_editRoleName.ModifyStyleEx(WS_TABSTOP, 0, 0);
    //auto dwStyle = m_editUserName.GetStyle();
    //dwStyle |= ES_NUMBER;
    //SetWindowLong(m_editUserName.m_hWnd, GWL_STYLE, dwStyle);
    m_editRoleName.SetWindowTextW(m_vecRoleInfo[6]);
    m_editRoleName.SetTextColor(RGB(255, 255, 255));
    m_editRoleName.SetBackColor(RGB(64, 64, 64));
    m_editRoleName.SetFont(&m_font);
    m_editRoleName.ShowWindow(SW_SHOW);
    // ::PostMessage(m_editCode.m_hWnd, EM_SETSEL, 0, 0);
    m_vecEdit.push_back(&m_editRoleName);


    int iButtonWidth = 150;
    auto refreshFunc = std::bind(&CEditRoleDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    m_pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnOK->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_pBtnOK->SetText(L"确定");

    int iSplitter = 50;
    x = (rect.Width() - iButtonWidth*2 - iSplitter)/2;
    y  = 200;
    m_pBtnOK->MoveWindow(CPoint(x, y));
    m_pBtnOK->SetDimention(iButtonWidth, iEdithHeight);
    auto clickFun = std::bind(&CEditRoleDlg::OnClickOk, this, std::placeholders::_1);
    m_pBtnOK->RegisterClickFun(clickFun);

    m_pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    m_pBtnCancel->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_pBtnCancel->SetDimention(iButtonWidth, iEdithHeight);
    m_pBtnCancel->SetText(L"取消");
    x += iButtonWidth + iSplitter;
    m_pBtnCancel->MoveWindow(CPoint(x, y));
    clickFun = std::bind(&CEditRoleDlg::OnClickCancel, this, std::placeholders::_1);
    m_pBtnCancel->RegisterClickFun(clickFun);
    return 0;
}
void CEditRoleDlg::OnPaint()
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

    int y = iTesty;
    memDC.TextOutW(iTextx, y, L"角色简称");
   // int x = 450;
    y += 40 + iOffsety;
    memDC.TextOutW(iTextx, y, L"角色名称");
    m_pBtnCancel->Refresh(&memDC, rect);
    m_pBtnOK->Refresh(&memDC, rect);

    RefreshWithEdit(this, pDC, rect, m_vecEdit, memDC);

   // pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}


BOOL CEditRoleDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return TRUE;
}


void CEditRoleDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pBtnCancel->OnLButtonDown(nFlags, point);
    m_pBtnOK->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CEditRoleDlg::OnMouseMove(UINT nFlags, CPoint point)
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


void CEditRoleDlg::OnOK()
{
    m_editRoleDescription.GetWindowTextW(m_sRoleDes);
    m_editRoleName.GetWindowTextW(m_sRoleName);
    if (m_sRoleDes.IsEmpty() || m_sRoleName.IsEmpty())
    {
        MessageBox(L"请输入正确信息!");
        return;
    }
    HttpHelper httpHelper(theApp.m_spClient);
    vector<string> vecNewInfo;
    string _sDes = CW2A(m_sRoleDes.GetString());
    string _sName = CW2A(m_sRoleName.GetString());
    string _sUID = CW2A(m_vecRoleInfo[0].GetString());
    auto eResult = httpHelper.ModifyRoleInfo(_sDes, _sName, _sUID);
    
    if (eResult == ErrorCode::Failed)
    {
        MessageBox(L"修改失败!");
        return;
    }
    if (eResult == ErrorCode::Repeat)
    {
        MessageBox(L"角色信息重复!");
        return;
    }
    __super::OnOK();
}

void CEditRoleDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    if (m_pBtnCancel == nullptr)
    {
        return;
    }
    m_pBtnCancel->SetDirty(true);
    m_pBtnOK->SetDirty(true);

    Invalidate();
}

void CEditRoleDlg::OnClickOk(CMyWindow* pWnd)
{
    this->OnOK();

}          

void CEditRoleDlg::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();

}

void CEditRoleDlg::GetNewInfo(CString& sRoleDes, CString& sRoleName)
{
    sRoleDes = m_sRoleDes;
    sRoleName = m_sRoleName;
}
