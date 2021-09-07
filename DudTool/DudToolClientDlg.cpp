
// MFCApplication2Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "DudToolClient.h"
#include "DudToolClientDlg.h"
#include "afxdialogex.h"
#include "CMyStaticWindow.h"
#include "CMyExpandWnd.h"
#include "CMyCheckWndGroup.h"
#include "CDataShowWnd.h"
#include <iostream>
#include <gdiplus.h>
#include "CommonDef.h"
#include "CLeftPannelWnd.h"
#include "CHomeChartWnd.h"
#include "CUserManagerWnd.h"
#include "CRoleManagerWnd.h"
#include "CUserInfoWnd.h"        
#include "HttpHelper.h"
#include "CComboxWndManger.h"
#include "CMyCalendar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

// CMFCApplication2Dlg dialog


//const int NavigationItemHeight = 60;
//const int IntervalBetweenNavigationAndDisplay = 20;
//const int MinDisplayRegion = 1626;
//const int MinDisplayHeight = 520 + 440 + 10;
//const int DisplayRegionTopOffset = 70;

const UINT LOGIN_WND_ID = 0;

DudToolClientDlg::DudToolClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

DudToolClientDlg::~DudToolClientDlg()
{
    for each (auto item in m_mapDirtyRect)
    {
        delete item.first;
    }
}

void DudToolClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DudToolClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_NCMBUTTONUP()
    ON_WM_NCMOUSEHOVER()
    ON_WM_NCMOUSELEAVE()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCRBUTTONDOWN()
    ON_WM_NCRBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFCApplication2Dlg message handlers

BOOL DudToolClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    this->SetWindowTextW(L"Arigin3D Cloud");

    CDialogEx::SetBackgroundColor(RGB(155, 170, 190));
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
    CRect rect;
    GetClientRect(&rect);     //取客户区大小
    CPoint old;
    old.x = rect.right - rect.left;
    old.y = rect.bottom - rect.top;
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);

    if (cx <= 1600)
    {
        m_bSmallFont = true;
    }
    CRect rt;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
    cy = rt.bottom;
    //MoveWindow(cx * 0.125, cy * 0.125, cx * 0.75, cy * 0.75);
    this->ShowWindow(SW_SHOWMAXIMIZED);
    auto refreshFunc = std::bind(&DudToolClientDlg::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    int iWidth = NavigationRegionWidth;
    int iOffsety = 0;
    CMyStaticWindow* pLogWind = new CMyStaticWindow(nullptr, this, refreshFunc);
    pLogWind->SetBkPic(IDB_PNG_LOG);
    auto logSize = pLogWind->GetBkPicSize();
    int iHeight = logSize.cy;
    pLogWind->MoveWindow(CPoint(0, iOffsety));
    pLogWind->SetDimention(iWidth, iHeight);
    m_mapWndList[WndID::LOGIN_WND_ID] = pLogWind;


    auto pQuitButton = new CMyButton(nullptr, this, refreshFunc);
    pQuitButton->SetBkPic(IDB_PNG_QUIT, IDB_PNG_QUIT_SELECT);
    pQuitButton->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnQuit(pWnd);
    });
    m_mapWndList[WndID::QUIT_BUTTON] = pQuitButton;
       
#pragma region  左侧面板

    iHeight = cy - logSize.cy - iOffsety;
    iOffsety += logSize.cy + iOffsety;
    CLeftPannelWnd* pLeftPannelWnd = new CLeftPannelWnd(nullptr, this, refreshFunc);
    pLeftPannelWnd->MoveWindow(0, iOffsety + 20);
    pLeftPannelWnd->SetDimention(iWidth, iHeight);
    pLeftPannelWnd->Initialize();
    pLeftPannelWnd->SetName(L"LeftPannel");
    m_mapWndList[WndID::LEFT_PANNEL_ID] = pLeftPannelWnd; 

    auto pDataInfoFunc = std::bind(&DudToolClientDlg::OnDataInfo, this, std::placeholders::_1);
    pLeftPannelWnd->RegisterClickFun(LeftPannelChildWndID::DATA_MANAGER_ID, pDataInfoFunc);
    
    auto pHomeFunc = std::bind(&DudToolClientDlg::OnHome, this, std::placeholders::_1);
    pLeftPannelWnd->RegisterClickFun(LeftPannelChildWndID::HOME_ID, pHomeFunc);
        
    auto pUserManagerFunc = std::bind(&DudToolClientDlg::OnUserManager, this, std::placeholders::_1);
    pLeftPannelWnd->RegisterClickFun(LeftPannelChildWndID::USER_MANAGER_ID, pUserManagerFunc);

    auto pRoleManagerFunc = std::bind(&DudToolClientDlg::OnRoleManager, this, std::placeholders::_1);
    pLeftPannelWnd->RegisterClickFun(LeftPannelChildWndID::ROLE_MANAGER_ID, pRoleManagerFunc);
    
    auto pUserInfoFunc = std::bind(&DudToolClientDlg::OnUserInfo, this, std::placeholders::_1);
    pLeftPannelWnd->RegisterClickFun(LeftPannelChildWndID::USER_INFO_ID, pUserInfoFunc);

    auto p3DDataFunc = std::bind(&DudToolClientDlg::On3DDataInfo, this, std::placeholders::_1);
    pLeftPannelWnd->RegisterClickFun(LeftPannelChildWndID::DATA_3D_FILE, p3DDataFunc);
    
    auto pAriginDataFunc = std::bind(&DudToolClientDlg::OnAriginData, this, std::placeholders::_1);
    pLeftPannelWnd->RegisterClickFun(LeftPannelChildWndID::DATA_ARIGIN, pAriginDataFunc);

#pragma endregion

#pragma region 右侧显示区域 
    int iOffsetx = NavigationRegionWidth + IntervalBetweenNavigationAndDisplay;
    iWidth = rect.Width() - iOffsetx;
    CHomeChartWnd* pHomeChartWnd = new CHomeChartWnd(nullptr, this, refreshFunc);
    pHomeChartWnd->MoveWindow(iOffsetx, iOffsety);
    pHomeChartWnd->SetDimention(iWidth, iHeight);
    pHomeChartWnd->Initialize();
    pHomeChartWnd->SetName(L"HomeChartWnd");
    m_mapWndList[WndID::HOMTE_CHART_ID] = pHomeChartWnd;
    m_pCurrentDisplayWnd = pHomeChartWnd;

    CDataShowWnd* pDataShowWnd = new CDataShowWnd(nullptr, this, refreshFunc);
    pDataShowWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
    pDataShowWnd->SetDimention(iWidth, iHeight);
    pDataShowWnd->Initialize();
    pDataShowWnd->SetVisible(false);
    pDataShowWnd->SetName(L"DataShowWnd");
    m_mapWndList[WndID::DATA_SHOW_WND_ID] = pDataShowWnd;

    
    CUserManagerWnd* pUserManagerWnd = new CUserManagerWnd(nullptr, this, refreshFunc);
    pUserManagerWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
    pUserManagerWnd->SetDimention(iWidth, iHeight);
    pUserManagerWnd->Initialize();
    pUserManagerWnd->SetVisible(false);
    pUserManagerWnd->SetName(L"UserManagerWnd");
    m_mapWndList[WndID::USER_MANAGER_WND_ID] = pUserManagerWnd;

    CRoleManagerWnd* pRoleManagerWnd = new CRoleManagerWnd(nullptr, this, refreshFunc);
    pRoleManagerWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
    pRoleManagerWnd->SetDimention(iWidth, iHeight);
    pRoleManagerWnd->Initialize();
    pRoleManagerWnd->SetVisible(false);
    pRoleManagerWnd->SetName(L"CRoleManagerWnd");
    m_mapWndList[WndID::ROLE_MANAGER_WND_ID] = pRoleManagerWnd;


    CUserInfoWnd* pUserInfoWnd = new CUserInfoWnd(nullptr, this, refreshFunc);
    pUserInfoWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
    pUserInfoWnd->SetDimention(iWidth, iHeight);
    pUserInfoWnd->Initialize();
    pUserInfoWnd->SetVisible(false);
    pUserInfoWnd->SetName(L"pUserInfoWnd");
    m_mapWndList[WndID::USER_INFO_WND_ID] = pUserInfoWnd;
    
#pragma endregion*/
    ResizeWnd();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

 
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void DudToolClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
        //return __super::OnPaint();
        CPaintDC dc(this);
        CRect rect;
        GetClientRect(&rect);
        CDC* pDC = GetDC();
        if (pDC == nullptr)
        {
            return;
        }
        CDC memDC;
        CBitmap memBitmap;
        memDC.CreateCompatibleDC(NULL);
        int nWidth = rect.Width();
        int nHeight = rect.Height();

        nWidth = nWidth < MinDisplayWidth ? MinDisplayWidth : nWidth;
        nHeight = nHeight < MinDisplayHeight ? MinDisplayHeight : nHeight;
        rect.right = rect.left + nWidth;
        rect.bottom = rect.top + nHeight;
        memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
        CBitmap* pOldbit = (CBitmap * )memDC.SelectObject(&memBitmap);
        memDC.FillSolidRect(rect, RGB(0, 0, 0));
        
        int x = 0, y = 0, r = 0, b = 0;
        bool bFirst = true;
        for each (auto dirtyItem in m_mapDirtyRect)
        {
            auto pWnd = dirtyItem.first;
            auto rect = dirtyItem.second;
            auto pStaticWnd = dynamic_cast<CMyStaticWindow*>(pWnd);
            if (pStaticWnd != nullptr)
            {
                std::cout << CW2A(pStaticWnd->m_text) << " " << std::endl;
            }
            else
            {
                auto pStaticWnd = dynamic_cast<CMyExpandWnd*>(pWnd);
                if (pStaticWnd != nullptr)
                {
                    std::cout << CW2A(pStaticWnd->GetText()) << " " << std::endl;

                }
                else
                {
                    std::cout << "other" << std::endl;
                }
            }

            std::cout << rect.left << " " ;
            std::cout << rect.top << " " ;
            std::cout << rect.right << " " ;
            std::cout << rect.bottom << " " << std::endl;

            //if (pWnd->Refresh(&memDC))
            {
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
               
               /* nWidth = rect.Width();
                nHeight = rect.Height();
                if (!m_bForceRefresh)
                {
                    pDC->BitBlt(x, y, nWidth, nHeight, &memDC, x, y, SRCCOPY);
                }*/
                std::cout << "***************Refresh*************" << std::endl;
            }
        }

       /* if (m_bForceRefresh)                s
        {
            pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
            m_bForceRefresh = false;
        }
        else*/
        vector<CEdit*> vecTemp;
        {
            CRect dirtyRect(x, y, r, b);
            auto vecWnd = SortByLayer(m_mapWndList);
            for each (auto item in vecWnd)
            {
                if (!item->IsVisible())
                {
                    continue;
                }
                auto pWndWithEdit = dynamic_cast<IWidthEditWnd*>(item);
                auto pWndWithEdit2 = dynamic_cast<CMyContainerWnd*>(item);
                if (pWndWithEdit != nullptr)                   
                {
                    auto vecEditCtrl = pWndWithEdit->GetEditCtrl();
                    if (vecEditCtrl.size() != 0)
                    {
                        vecTemp.insert(vecTemp.end(), vecEditCtrl.begin(), vecEditCtrl.end());
                    }
                }
                item->Refresh(&memDC, dirtyRect);
            }
            /*for each (auto item in m_mapWndList)
            {
                item.second->Refresh(&memDC, dirtyRect);
            }*/
            nWidth = r - x;
            nHeight = b - y;
           // pDC->BitBlt(x, y, nWidth, nHeight, &memDC, x, y, SRCCOPY);
        }
        if (vecTemp.size() == 0)
        {
            pDC->BitBlt(x, y, nWidth, nHeight, &memDC, x, y, SRCCOPY);
        }
        else
        {
            CRect rTemp;
            rTemp.left = x;
            rTemp.top = y;
            rTemp.right = x + nWidth;
            rTemp.bottom = y + nHeight;
            RefreshWithEdit_Horizontal(this, pDC, rTemp, vecTemp, memDC);
        }

        memDC.SelectObject(pOldbit);
        m_mapDirtyRect.clear();
        memBitmap.DeleteObject();
        memDC.DeleteDC();
        ReleaseDC(pDC);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR DudToolClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void DudToolClientDlg::OnNcMButtonUp(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnNcMButtonUp(nHitTest, point);
}


void DudToolClientDlg::OnNcMouseHover(UINT nFlags, CPoint point)
{
    // This feature requires Windows 2000 or greater.
    // The symbols _WIN32_WINNT and WINVER must be >= 0x0500.
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnNcMouseHover(nFlags, point);
}


void DudToolClientDlg::OnNcMouseLeave()
{
    // This feature requires Windows 2000 or greater.
    // The symbols _WIN32_WINNT and WINVER must be >= 0x0500.
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnNcMouseLeave();
}


void DudToolClientDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnNcMouseMove(nHitTest, point);
}


void DudToolClientDlg::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnNcRButtonDown(nHitTest, point);
}


void DudToolClientDlg::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnNcRButtonUp(nHitTest, point);
}


void DudToolClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    //auto pWnd = GetCurrentMouseWnd(point);
    //if (pWnd != nullptr)
    //{ 
    //    pWnd->OnLButtonDown(nFlags, point);
    //}

    if (nullptr == m_pCurrentMouseInWnd)
        return;
    auto pLeftPannelWnd = (CLeftPannelWnd*)m_mapWndList[WndID::LEFT_PANNEL_ID];
    auto pHome = pLeftPannelWnd->GetWindow(LeftPannelChildWndID::HOME_ID);
    if (pHome->HitTest(point))
    {
        OnHome(pHome);
    }
    else
    {
        auto vecWnd = CComboxWndManger::Instance().GetWnd();
        for each (auto item in vecWnd)
        {
            if (m_pCurrentMouseInWnd != item)
            {
                ((CMyComboxWnd*)item)->OnOpen(false);
            }
        }
        auto vecCrossWnd = CCrosWndManger::Instance().GetWnd();
        for each (auto item in vecCrossWnd)
        {
            if (item != nullptr && !item->HitTest(point))
            {
                item->SetVisible(false);
            }
        }
        m_pCurrentMouseInWnd->OnLButtonDown(nFlags, point);
    }

    CDialogEx::OnLButtonDown(nFlags, point);
}


void DudToolClientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    auto vecCrossWnd = CComboxWndManger::Instance().GetWnd();
    for each (auto item in vecCrossWnd)
    {
        item->OnLButtonUp(nFlags, point);
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}


void DudToolClientDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnLButtonDblClk(nFlags, point);
}


void DudToolClientDlg::OnMouseHover(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnMouseHover(nFlags, point);
}


void DudToolClientDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // This feature requires Windows Vista or greater.
    // The symbol _WIN32_WINNT must be >= 0x0600.
    // TODO: Add your message handler code here and/or call default

    CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


void DudToolClientDlg::OnMouseLeave()
{                                           
}


void DudToolClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    bool bMouseMoveInWnd = false;
    auto vecCrossWnd = CCrosWndManger::Instance().GetWnd();
    for each (auto item in vecCrossWnd)
    {
        if (item->OnMouseMove(nFlags, point))
        {
            m_pCurrentMouseInWnd = item;
            return;
        }
    }
    auto vecCombox = CComboxWndManger::Instance().GetWnd();
    for each (auto item in vecCombox)
    {
        if (item->OnMouseMove(nFlags, point))
        {
            m_pCurrentMouseInWnd = item;
            return;
        }
    }



    auto vecWnd = SortByLayer(m_mapWndList);
    for (auto reIt=vecWnd.rbegin(); reIt != vecWnd.rend(); reIt++)
    {
        auto pWnd = *reIt;
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
        m_pCurrentMouseInWnd = nullptr;
    }
    CDialogEx::OnMouseMove(nFlags, point);
}


BOOL DudToolClientDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pCurrentMouseInWnd == nullptr)
    {
        return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);

    }
    m_pCurrentMouseInWnd->OnMouseWheel(nFlags, zDelta, pt);
    return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL DudToolClientDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return true;
}

BOOL DudToolClientDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
        return   TRUE;
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        GetDlgItem(IDC_BUTTON_APP_LOST_FOCUS)->SetFocus();
        return   TRUE;
    }
    else
        return   CDialog::PreTranslateMessage(pMsg);
}

void DudToolClientDlg::SetDirtyRect(CMyWindow* pWnd, CRect rect)
{
    int x, y, r, b;
    if (m_mapDirtyRect.find(pWnd) != m_mapDirtyRect.end())
    {
        auto oriRect = m_mapDirtyRect[pWnd];  
        x = min(oriRect.left, rect.left);
        y = min(oriRect.top, rect.top);
        r = max(oriRect.right, rect.right);
        b = max(oriRect.bottom, rect.bottom);
        m_mapDirtyRect[pWnd] = CRect(x,y,r,b);
    }
    else
    {
        m_mapDirtyRect[pWnd] = rect;
    }
}

void DudToolClientDlg::RemoveDirtyRect(CMyWindow* pWnd)
{
    m_mapDirtyRect.erase(pWnd);
}

void DudToolClientDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    SetDirtyRect(pWnd, rect);
    Invalidate();
}
void DudToolClientDlg::ShowDataWindow(DataCategory eDataCategory)
{

    auto pDataDisplayWnd = (CDataShowWnd*)m_mapWndList[WndID::DATA_SHOW_WND_ID];
    /*if (pDataDisplayWnd == m_pCurrentDisplayWnd)
    {
        return;
    }*/
    pDataDisplayWnd->SetDataCategory(eDataCategory);
#pragma region 获取数据
    //获取数据的所有医院
    HttpHelper httpHelper(theApp.m_spClient);
    string sHospital;
    if (ErrorCode::Success == httpHelper.GetHospitalName(theApp.m_sUserID, sHospital, eDataCategory))
    {
        pDataDisplayWnd->SetHospital(sHospital);
    }
    else
    {
        pDataDisplayWnd->SetHospital("");
    }
    string sPatientType;
    if (ErrorCode::Success == httpHelper.GetPatientType(sPatientType))
    {
        pDataDisplayWnd->SetPatientType(sPatientType);
    }

    //每个用户有多个角色
    int   iTotalCount = 0;
    auto s = pDataDisplayWnd->GetSearchCondition();
    auto sDataInfo = httpHelper.GetUserDataInfo(theApp.m_sUserID,
        s,
        0, PerPageItemDataCount,
        iTotalCount, eDataCategory);
    pDataDisplayWnd->SetTotalCount(iTotalCount);
    pDataDisplayWnd->SetCurrentPageItems(sDataInfo, 0, iTotalCount);
    //return;
#pragma endregion

    if (m_pCurrentDisplayWnd != nullptr)
    {
        m_pCurrentDisplayWnd->SetVisible(false);
    }
    pDataDisplayWnd->SetVisible(true);
    m_pCurrentDisplayWnd = pDataDisplayWnd;
    auto vecWnd = CComboxWndManger::Instance().GetWnd();
    for each (auto item in vecWnd)
    {
        if (m_pCurrentMouseInWnd != item)
        {
            ((CMyComboxWnd*)item)->OnOpen(false);
        }
    }
    ForceUpdate();
}

void DudToolClientDlg::OnDataInfo(CMyWindow* pWnd)
{
    ShowDataWindow(DataCategory::Data_COMMON);
}

void DudToolClientDlg::On3DDataInfo(CMyWindow* pWnd)
{
    ShowDataWindow(DataCategory::Data_3D);

}

void DudToolClientDlg::OnAriginData(CMyWindow* pWnd)
{
    ShowDataWindow(DataCategory::Data_Arigin);
}


void DudToolClientDlg::OnHome(CMyWindow* pWnd)
{
    auto pCharWnd = m_mapWndList[WndID::HOMTE_CHART_ID];
    if (pCharWnd == m_pCurrentDisplayWnd)
    {
        return;
    }
    if (m_pCurrentDisplayWnd != nullptr)
    {
        m_pCurrentDisplayWnd->SetVisible(false);
    }
    pCharWnd->SetVisible(true);
    m_pCurrentDisplayWnd = pCharWnd;
    ForceUpdate();
}
void DudToolClientDlg::OnUserManager(CMyWindow* pWnd)
{
    HttpHelper httpHelper(theApp.m_spClient);
    UserAuthoriy userAuthority;
    httpHelper.GetUserAuthority(theApp.m_sUserID, userAuthority);
    if (!(userAuthority.uiSystemAuthoriy & (UINT64) SystemAuthorityDef::userManager))
    {
        AfxMessageBox(L"没有操作权限!");
        return;
    }
    auto pUserManager = (CUserManagerWnd*)m_mapWndList[WndID::USER_MANAGER_WND_ID];
    if (pUserManager == m_pCurrentDisplayWnd)
    {
        return;
    }
    if (m_pCurrentDisplayWnd != nullptr)
    {
        m_pCurrentDisplayWnd->SetVisible(false);
    }

#pragma region 获取数据 
    pUserManager->OnPage(0, PerPageItemDataCount);
#pragma endregion


    pUserManager->SetVisible(true);
    m_pCurrentDisplayWnd = pUserManager;
    ForceUpdate();
}

void DudToolClientDlg::OnRoleManager(CMyWindow* pWnd)
{
    HttpHelper httpHelper(theApp.m_spClient);
    UserAuthoriy userAuthority;
    httpHelper.GetUserAuthority(theApp.m_sUserID, userAuthority);
    if (!(userAuthority.uiSystemAuthoriy & (UINT64)SystemAuthorityDef::roleManager))
    {
        AfxMessageBox(L"没有操作权限!");
        return;
    }
    auto pRoleManager = m_mapWndList[WndID::ROLE_MANAGER_WND_ID];
    if (pRoleManager == m_pCurrentDisplayWnd)
    {
        return;
    }

    int iCTotalCount = 0;
    auto sRoleInfos = httpHelper.GetAllRoleInfo(0, PerPageItemDataCount, iCTotalCount);

    CRoleManagerWnd* pRoleWnd = (CRoleManagerWnd*)pRoleManager;
    pRoleWnd->SetTotalCount(iCTotalCount);
    pRoleWnd->SetCurrentPageItems(sRoleInfos);
    if (m_pCurrentDisplayWnd != nullptr)
    {
        m_pCurrentDisplayWnd->SetVisible(false);
    }


    pRoleManager->SetVisible(true);
    m_pCurrentDisplayWnd = pRoleManager;
    ForceUpdate();
}


void DudToolClientDlg::OnUserInfo(CMyWindow* pWnd)
{
    HttpHelper httpHelper(theApp.m_spClient);
    UserAuthoriy userAuthority;
    httpHelper.GetUserAuthority(theApp.m_sUserID, userAuthority);
    if (!(userAuthority.uiUserCenterAuthoriy & (UINT64)UserCenterAuthorityDef::userInfo))
    {
        AfxMessageBox(L"没有操作权限!");
        return;
    }

    auto pUserInfoWnd = m_mapWndList[WndID::USER_INFO_WND_ID];
    if (pUserInfoWnd == m_pCurrentDisplayWnd)
    {
        return;
    }
    if (m_pCurrentDisplayWnd != nullptr)
    {
        m_pCurrentDisplayWnd->SetVisible(false);
    }
    string sUserInfo;
    auto errorCode = httpHelper.GetUserInfo(theApp.m_sUserID, sUserInfo);
    if (errorCode == ErrorCode::Failed)
    {
        return;
    }
    CString wsInfo = CString(sUserInfo.c_str());
    auto vecInfo = SplitString(wsInfo, ',');

    assert(vecInfo.size() == 8);

    ((CUserInfoWnd*)pUserInfoWnd)->SetUserInfo(vecInfo[1], vecInfo[2], vecInfo[7]);

    pUserInfoWnd->SetVisible(true);
    m_pCurrentDisplayWnd = pUserInfoWnd;
    ForceUpdate();
}

void DudToolClientDlg::OnQuit(CMyWindow* pWnd)
{
    WCHAR pBuf[MAX_PATH];
    //获取应用程序完全路径，比 GetCurrentDirectory 好用多了
    GetModuleFileName(NULL, pBuf, MAX_PATH);

    STARTUPINFO startupinfo;
    PROCESS_INFORMATION proc_info;
    memset(&startupinfo, 0, sizeof(STARTUPINFO));
    startupinfo.cb = sizeof(STARTUPINFO);
    // 最重要的地方
    ::CreateProcess(pBuf, NULL, NULL, NULL, FALSE,
        NORMAL_PRIORITY_CLASS, NULL, NULL, &startupinfo, &proc_info);

    PostMessage(WM_QUIT);
}

void DudToolClientDlg::ForceUpdate()
{
    for each (auto  item in m_mapWndList)
    {
        item.second->ForceUpdate();
    }
    m_bForceRefresh = true;
}

void DudToolClientDlg::ShowHomeWnds(bool bShow)
{
    //m_mapWndList[WndID::HOME_TABLE_WND_ID]->SetVisible(false);
    //m_mapWndList[WndID::HOME_SOFTWARE_WND_ID] ->SetVisible(false);
    //m_mapWndList[WndID::HOME_USERINFO_WND_ID] ->SetVisible(false);
    //m_mapWndList[WndID::HOME_HOSPITAL_WND_ID] ->SetVisible(false);
}
void DudToolClientDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if (m_mapWndList.size() != 0)
    {
        ResizeWnd();
    }
    ForceUpdate();
    // TODO: Add your message handler code here
}
void DudToolClientDlg::ResizeWnd()
{
    CRect rect;
    GetClientRect(rect);
    auto logSize = m_mapWndList[WndID::LOGIN_WND_ID]->GetBkPicSize();
    int iOffsety = 5;
    int iHeight = rect.Height() - logSize.cy - iOffsety;
    int iOffsetx = NavigationRegionWidth + IntervalBetweenNavigationAndDisplay;
    int iWidth = rect.Width() - iOffsetx - IntervalRightDisplay;
    iWidth = iWidth < MinDisplayWidth ? MinDisplayWidth : iWidth;
    iHeight = iHeight < MinDisplayHeight ? MinDisplayHeight : iHeight;
    m_mapWndList[WndID::HOMTE_CHART_ID]->OnSize(iWidth, iHeight);
    m_mapWndList[WndID::DATA_SHOW_WND_ID]->OnSize(iWidth, iHeight);
    m_mapWndList[WndID::USER_MANAGER_WND_ID]->OnSize(iWidth, iHeight);
    m_mapWndList[WndID::ROLE_MANAGER_WND_ID]->OnSize(iWidth, iHeight);
    m_mapWndList[WndID::USER_INFO_WND_ID]->OnSize(iWidth, iHeight);
    m_mapWndList[WndID::LEFT_PANNEL_ID]->OnSize(iWidth, iHeight);
    auto size = m_mapWndList[WndID::QUIT_BUTTON]->GetBkPicSize();
    m_mapWndList[WndID::QUIT_BUTTON]->MoveWindow(iOffsetx, 5);
    m_mapWndList[WndID::QUIT_BUTTON]->SetDimention(size.cx, size.cy);
}                                       

HBRUSH DudToolClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
 
    // TODO:  Change any attributes of the DC here
   /* UINT id = pWnd->GetDlgCtrlID();
    if (id == UWM_IDD_PAGESELECT)
    {
        pDC->SetBkMode(TRANSPARENT);
        CBrush brush;
        brush.CreateStockObject(NULL_BRUSH);
        return brush;
    }*/
    // TODO:  Return a different brush if the default is not desired
    return hbr;
}
