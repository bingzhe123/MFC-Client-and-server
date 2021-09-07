// CFileOperatorDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CFileOperatorDlg.h"
#include "afxdialogex.h"
#include "CommonDef.h"
#include "resource.h"
#include <iostream>
// CFileOperatorDlg dialog

IMPLEMENT_DYNAMIC(CFileOperatorDlg, CDialogEx)

CFileOperatorDlg::CFileOperatorDlg(CDataShowrListWnd* pDataShowListWnd, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FILE_OPERATOR, pParent)
{
    m_pDataShowListWnd = pDataShowListWnd;
}

CFileOperatorDlg::~CFileOperatorDlg()
{
    if (nullptr != m_pFileOperatorWnd)
    {
        delete m_pFileOperatorWnd;
    }
    m_font.DeleteObject();
}

void CFileOperatorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileOperatorDlg, CDialogEx)
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_MESSAGE(WM_PROGRESS_UPDATE, OnCustomMsgHandler)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CFileOperatorDlg message handlers


BOOL CFileOperatorDlg::OnInitDialog()
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
    int iWidth = 1024;
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);


    m_pFileOperatorWnd = new CFileOperatorWnd(nullptr, this, [this](CMyWindow* pWnd, CRect rect)
    {
        RefreshFun(pWnd, rect);
    });         
    m_pFileOperatorWnd->Initialize(m_vecDataRegisterInfo, m_sFileList, m_pDataShowListWnd);
    CRect rect;
    GetClientRect(&rect);
    ResizeWnd();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CFileOperatorDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pFileOperatorWnd->OnMouseMove(nFlags, point);

    CDialogEx::OnMouseMove(nFlags, point);
}


void CFileOperatorDlg::OnPaint()
{

    std::cout << "____________________" << std::endl;
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CDialogEx::OnPaint() for painting messages
    CRect rect;
    GetClientRect(&rect);
    CDC* pDC = GetDC();
    CDC memDC;
    CBitmap memBitmap;
    auto pMemDc = memDC.CreateCompatibleDC(NULL);
    int nWidth = rect.Width();
    int nHeight = rect.Height();
    memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);
    memDC.FillSolidRect(rect, RGB(0, 128, 128));
    int x = 0, y = 0, r = 0, b = 0;
    bool bFirst = true;
    for each (auto dirtyItem in m_mapDirtyRect)
    {
        auto rect = dirtyItem.second;
        if (bFirst)
        {
            x = rect.left;
            y = rect.top;
            r = rect.right;
            b = rect.bottom;
            bFirst = false;
        }
        else
        {
            x = x > rect.left ? rect.left : x;
            y = y < rect.top ? y : rect.top;
            r = r > rect.right ? r : rect.right;
            b = b > rect.bottom ? b : rect.bottom;
        }
    }

    CRect dirtyRect(x, y, r, b);
    m_pFileOperatorWnd->Refresh(&memDC, dirtyRect);
    nWidth = r - x;
    nHeight = b - y;
    pDC->BitBlt(x, y, nWidth, nHeight, &memDC, x, y, SRCCOPY);
    m_mapDirtyRect.clear();
    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}

BOOL CFileOperatorDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        GetDlgItem(IDC_BUTTON_LOST_FOCUS)->SetFocus();
        return true;
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CFileOperatorDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return TRUE;
}


void CFileOperatorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pFileOperatorWnd->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CFileOperatorDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
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

void CFileOperatorDlg::ResizeWnd()
{
    CRect rect;
    GetClientRect(rect);

    m_pFileOperatorWnd->OnSize(rect.Width(), rect.Height());
}
void CFileOperatorDlg::SetDataRegisterInfo(const vector<CString>& vecFileInfo)
{
    m_vecDataRegisterInfo = vecFileInfo;
}
void CFileOperatorDlg::SetDataList(const CString& sFileList)
{
    m_sFileList = sFileList;
}

LRESULT CFileOperatorDlg::OnCustomMsgHandler(WPARAM wParam, LPARAM lParam)
{
    CMyWindow* pWnd = (CMyWindow*)wParam;
    CRect rect = pWnd->GetClientRect();
    RefreshFun(pWnd, rect); 
    return LRESULT();
}


void CFileOperatorDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    m_pFileOperatorWnd->OnDlgClose();
    CDialogEx::OnClose();
}
