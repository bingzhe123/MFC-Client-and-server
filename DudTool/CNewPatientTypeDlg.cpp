// CNewPatientTypeDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CNewPatientTypeDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "HttpHelper.h"


// CNewPatientTypeDlg dialog

IMPLEMENT_DYNAMIC(CNewPatientTypeDlg, CDialogEx)

CNewPatientTypeDlg::CNewPatientTypeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD_PATIENT_TYPE, pParent)
{

}

CNewPatientTypeDlg::~CNewPatientTypeDlg()
{
    m_font.DeleteObject();
}

void CNewPatientTypeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewPatientTypeDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CNewPatientTypeDlg message handlers

BOOL CNewPatientTypeDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
   
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);

    int iHeight = 600;
    int iWidth = 1024;
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);


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

 
    m_pPatientTypeWnd = new CPatientTypeWnd(nullptr, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    m_pPatientTypeWnd->SetDimention(iWidth, iHeight);
    m_pPatientTypeWnd->Initialize();
    m_pPatientTypeWnd->SetName(L"CNewPatientTypeDlg");

    CRect r;
    m_editType.Create(ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, r, this, UWM_IDD_USER_SEARCH);
    m_editType.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editType.SetWindowTextW(L"");
    m_editType.SetTextColor(RGB(255, 255, 255));
    m_editType.SetBackColor(RGB(0, 50, 20));
    m_editType.SetFont(&m_font);
    m_editType.ShowWindow(SW_SHOW);
    m_editType.SetFocus();
    m_pPatientTypeWnd->SetEdit(&m_editType);

    CRect rc;
    GetClientRect(&rc);
    m_pPatientTypeWnd->OnSize(rc.Width(), rc.Height());
                                   
    HttpHelper httpHelper(theApp.m_spClient);
    string sType;
    auto errorCode = httpHelper.GetPatientType(sType);    
    auto vecInfos = SplitString(sType, '|');                 

    m_pPatientTypeWnd->SetTotalCount(vecInfos.size());
    m_pPatientTypeWnd->SetAllItems(sType);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNewPatientTypeDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CDialogEx::OnPaint() for painting messages

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
    m_pPatientTypeWnd->Refresh(&memDC, rect);      
 
   // pDC->BitBlt(0, 0, nWidth, nHeight, &memDC, 0, 0, SRCCOPY);

    vector<CEdit*> vecEdit;
    vecEdit.push_back(&m_editType);
    auto pEdit  = m_pPatientTypeWnd->GetBottomEidt();
    vecEdit.push_back(pEdit);
    RefreshWithEdit(this, pDC, rect, vecEdit, memDC);

    m_mapDirtyRect.clear();

    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}


void CNewPatientTypeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    m_pPatientTypeWnd->OnMouseMove(nFlags, point);
    Invalidate();
    CDialogEx::OnMouseMove(nFlags, point);
}


void CNewPatientTypeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pPatientTypeWnd->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CNewPatientTypeDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return true;
}

void CNewPatientTypeDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    int x, y, r, b;
    if (m_mapDirtyRect.find(pWnd) != m_mapDirtyRect.end())
    {
        auto oriRect = m_mapDirtyRect[pWnd];
        x = min(oriRect.left, rect.left);
        y = min(oriRect.top, rect.top);
        r = max(oriRect.right, rect.right);
        b = max(oriRect.bottom, rect.bottom);
        m_mapDirtyRect[pWnd] = CRect(x, y, r, b);
    }
    else
    {
        m_mapDirtyRect[pWnd] = rect;
    }
    Invalidate();
}


void CNewPatientTypeDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if (m_pPatientTypeWnd == nullptr)
    {
        return;
    }
    m_pPatientTypeWnd->OnSize(cx, cy);
    // TODO: Add your message handler code here
}


void CNewPatientTypeDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    //CDialogEx::OnOK();
}
