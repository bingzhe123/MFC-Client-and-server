// CSetRoleAuthority.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CSetRoleAuthority.h"
#include "afxdialogex.h"
#include "CMyCheckBox.h"
#include "CMyButton.h"


// CSetRoleAuthority dialog

IMPLEMENT_DYNAMIC(CSetRoleAuthority, CDialogEx)

CSetRoleAuthority::CSetRoleAuthority(CString& sRoleName, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SET_ROLE_AUTHORITY, pParent)
{
    m_sCaption = sRoleName;
}

CSetRoleAuthority::~CSetRoleAuthority()
{
    for each (auto item in m_mapWndList)
    {
        delete item.second;
    }
    delete m_pExpandGroup;
}

void CSetRoleAuthority::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetRoleAuthority, CDialogEx)
    ON_WM_PAINT()
    ON_WM_MOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSEMOVE()
    ON_WM_SIZE()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CSetRoleAuthority message handlers


BOOL CSetRoleAuthority::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    int iHeight = 700;
    int iWidth = 500;
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);


    int iWindIndex = 0;
    CString sText = L"设置" + m_sCaption + L"权限";
    SetWindowText(sText);
    m_pExpandGroup = (CExpandGroup*) CExpandGroup::Instance();
    m_pExpandGroup->SetSelectExclusive(false);
    auto pDataManager = new CMyCheckedExpandWnd(nullptr, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    }, m_pExpandGroup);

    pDataManager->SetText(CString(L"存储管理"));
   // pDataManager->SetBkGroundColor(RGB(50, 50, 50));
    pDataManager->SetIconPic(IDB_PNG_CHECKBOX_NORMAL, IDB_PNG_CHECKBOX_CHECKED);
    pDataManager->SetExpandMark(IDB_PNG_ARROW_RIGHT, IDB_PNG_ARROW_DOWN);
    m_mapWndList[iWindIndex++] = pDataManager;

    auto pAllMRDataVisible = new CMyCheckBox(pDataManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pAllMRDataVisible->SetText(L"全部CT/MRI数据可见");
    pAllMRDataVisible->SetBkGroundColor(RGB(69,91,178), RGB(128, 128, 50));
    pDataManager->AddChildWnd(pAllMRDataVisible);

    auto pAll3DDataVisible = new CMyCheckBox(pDataManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pAll3DDataVisible->SetText(L"全部术前模型数据可见");
    pAll3DDataVisible->SetBkGroundColor(RGB(69, 91, 178), RGB(128, 128, 50));
    pDataManager->AddChildWnd(pAll3DDataVisible);

    auto pAllAriginDataVisible = new CMyCheckBox(pDataManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pAllAriginDataVisible->SetText(L"全部导板数据可见");
    pAllAriginDataVisible->SetBkGroundColor(RGB(69, 91, 178), RGB(128, 128, 50));
    pDataManager->AddChildWnd(pAllAriginDataVisible);


    COLORREF selfdataColor = RGB(89, 138, 134);

    auto pPartialMRDataVisible = new CMyCheckBox(pDataManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pPartialMRDataVisible->SetText(L"自建CT/MRI数据可见");
    pPartialMRDataVisible->SetBkGroundColor(selfdataColor, RGB(128, 128, 50));
    pDataManager->AddChildWnd(pPartialMRDataVisible);

    auto pPartial3DDataVisible = new CMyCheckBox(pDataManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pPartial3DDataVisible->SetText(L"自建术前模型数据可见");
    pPartial3DDataVisible->SetBkGroundColor(selfdataColor, RGB(128, 128, 50));
    pDataManager->AddChildWnd(pPartial3DDataVisible);

    auto pPartialAriginDataVisible = new CMyCheckBox(pDataManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pPartialAriginDataVisible->SetText(L"自建导板数据可见");
    pPartialAriginDataVisible->SetBkGroundColor(selfdataColor, RGB(128, 128, 50));
    pDataManager->AddChildWnd(pPartialAriginDataVisible);

    if (IsMaskValid(m_dataAuthority, DataAuthorityDef::allMRDataVisible))
    {
        pAllMRDataVisible->SetChecked(true);
    }

    if (IsMaskValid(m_dataAuthority, DataAuthorityDef::all3DDataVisible))
    {
        pAll3DDataVisible->SetChecked(true);
    }

    if (IsMaskValid(m_dataAuthority, DataAuthorityDef::allAriginDataVisible))
    {
        pAllAriginDataVisible->SetChecked(true);
    }

    if (IsMaskValid(m_dataAuthority, DataAuthorityDef::selfMRDataVisible))
    {
        pPartialMRDataVisible->SetChecked(true);
    }
    if (IsMaskValid(m_dataAuthority, DataAuthorityDef::self3DDataVisible))
    {
        pPartial3DDataVisible->SetChecked(true);
    }
    if (IsMaskValid(m_dataAuthority, DataAuthorityDef::selfAriginDataVisible))
    {
        pPartialAriginDataVisible->SetChecked(true);
    }

    pDataManager->SetIsExpand(true);

    auto pSystemManager = new CMyCheckedExpandWnd(nullptr, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    }, m_pExpandGroup);

    pSystemManager->SetText(CString(L"系统管理"));
    //pSystemManager->SetBkGroundColor(RGB(50, 50, 50));
    pSystemManager->SetIconPic(IDB_PNG_CHECKBOX_NORMAL, IDB_PNG_CHECKBOX_CHECKED);
    pSystemManager->SetExpandMark(IDB_PNG_ARROW_RIGHT, IDB_PNG_ARROW_DOWN);
    m_mapWndList[iWindIndex++] = pSystemManager;
    auto pUserManager = new CMyCheckBox(pSystemManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pUserManager->SetText(L"用户管理");
    pUserManager->SetBkGroundColor(RGB(69, 91, 178), RGB(128, 128, 50));
    pSystemManager->AddChildWnd(pUserManager);

    auto pRoleManager = new CMyCheckBox(pSystemManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pRoleManager->SetText(L"角色管理");
    pRoleManager->SetBkGroundColor(RGB(69, 91, 178), RGB(128, 128, 50));
    pSystemManager->AddChildWnd(pRoleManager);
    pSystemManager->SetIsExpand(true);

    if (IsMaskValid(m_systemAuthority, SystemAuthorityDef::userManager))
    {
        pUserManager->SetChecked(true);
    }

    if (IsMaskValid(m_systemAuthority, SystemAuthorityDef::roleManager))
    {
        pRoleManager->SetChecked(true);
    }

    auto pUserCenterManager = new CMyCheckedExpandWnd(nullptr, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    }, m_pExpandGroup);

    pUserCenterManager->SetText(CString(L"个人中心"));
    //pUserCenterManager->SetBkGroundColor(RGB(50, 50, 50));
    pUserCenterManager->SetIconPic(IDB_PNG_CHECKBOX_NORMAL, IDB_PNG_CHECKBOX_CHECKED);
    pUserCenterManager->SetExpandMark(IDB_PNG_ARROW_RIGHT, IDB_PNG_ARROW_DOWN);
    m_mapWndList[iWindIndex++] = pUserCenterManager;
    auto pUserInfo = new CMyCheckBox(pUserCenterManager, this, [this](CMyWindow* pWnd, CRect rect)
    {
        this->RefreshFun(pWnd, rect);
    });
    pUserInfo->SetText(L"用户信息");
    pUserInfo->SetBkGroundColor(RGB(69, 91, 178), RGB(128, 128, 50));
    pUserCenterManager->AddChildWnd(pUserInfo);
    if (IsMaskValid(m_userCenterAuthority, UserCenterAuthorityDef::userInfo))
    {
        pUserInfo->SetChecked(true);
    }
    pUserCenterManager->SetIsExpand(true);

    //auto pStatisticManager = new CMyCheckedExpandWnd(nullptr, this, [this](CMyWindow* pWnd, CRect rect)
    //{
    //    this->RefreshFun(pWnd, rect);
    //}, m_pExpandGroup);

    //pStatisticManager->SetText(CString(L"统计分析"));
    ////pStatisticManager->SetBkGroundColor(RGB(50, 50, 50));
    //pStatisticManager->SetIconPic(IDB_PNG_CHECKBOX_NORMAL, IDB_PNG_CHECKBOX_CHECKED);
    //pStatisticManager->SetExpandMark(IDB_PNG_ARROW_RIGHT, IDB_PNG_ARROW_DOWN);
    ////m_mapWndList[iWindIndex++] = pStatisticManager;


    auto refreshFunc = std::bind(&CSetRoleAuthority::RefreshFun,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    auto pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    pBtnOK->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    pBtnOK->SetText(L"确定");                          
    pBtnOK->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnClickOk(pWnd);
    });
    m_mapWndList[iWindIndex++] = pBtnOK;

    auto pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    pBtnCancel->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    pBtnCancel->SetText(L"取消");
    
    pBtnCancel->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnClickCancel(pWnd);
    });
    m_mapWndList[iWindIndex++] = pBtnCancel;

    Resize();


    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CSetRoleAuthority::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CDialogEx::OnPaint() for painting messages
    CRect rect;
    GetClientRect(&rect);
    CDC* pDC = GetDC();
    CDC memDC;
    CBitmap memBitmap;
    if (!memDC.CreateCompatibleDC(NULL) || memDC.m_hDC == nullptr)
    {
        return;
    }
    int nWidth = rect.Width();
    int nHeight = rect.Height();
    memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);
    memDC.FillSolidRect(rect, RGB(25, 31, 40));

    for each (auto item in m_mapWndList)
    {
        auto pWnd = item.second;
        //auto childRect = pWnd->GetClientRect();
        pWnd->Refresh(&memDC, rect);
    }
    pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);
}


void CSetRoleAuthority::OnMove(int x, int y)
{

    // TODO: Add your message handler code here
}


void CSetRoleAuthority::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pCurrentMouseInWnd == nullptr)
    {
        return;
    }
    m_pCurrentMouseInWnd->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CSetRoleAuthority::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return true;
}

void CSetRoleAuthority::OnOK()
{
    int iIndex = 0;
    auto pDataManager = (CMyCheckedExpandWnd*)m_mapWndList[iIndex++];
    auto vecChilds = pDataManager->GetAllCheckWnd();
    m_dataAuthority = 0;
    if (vecChilds[0]->IsSelect())
    {
        m_dataAuthority |= (UINT)DataAuthorityDef::allMRDataVisible;
    }
    if (vecChilds[1]->IsSelect())
    {
        m_dataAuthority |= (UINT)DataAuthorityDef::all3DDataVisible;
    }
    if (vecChilds[2]->IsSelect())
    {
        m_dataAuthority |= (UINT)DataAuthorityDef::allAriginDataVisible;
    }
    if (vecChilds[3]->IsSelect())
    {
        m_dataAuthority |= (UINT)DataAuthorityDef::selfMRDataVisible;
    }
    if (vecChilds[4]->IsSelect())
    {
        m_dataAuthority |= (UINT)DataAuthorityDef::self3DDataVisible;
    }
    if (vecChilds[5]->IsSelect())
    {
        m_dataAuthority |= (UINT)DataAuthorityDef::selfAriginDataVisible;
    }

    auto pSystemManager = (CMyCheckedExpandWnd*)m_mapWndList[iIndex++];
    vecChilds = pSystemManager->GetAllCheckWnd();
    m_systemAuthority = 0;
    if (vecChilds[0]->IsSelect())
    {
        m_systemAuthority |= (UINT)SystemAuthorityDef::userManager;
    }

    if (vecChilds[1]->IsSelect())
    {
        m_systemAuthority |= (UINT)SystemAuthorityDef::roleManager;
    }

    auto pUserCenterManager = (CMyCheckedExpandWnd*)m_mapWndList[iIndex++];
    vecChilds = pUserCenterManager->GetAllCheckWnd();
    m_userCenterAuthority = 0;
    if (vecChilds[0]->IsSelect())
    {
        m_userCenterAuthority |= (UINT)UserCenterAuthorityDef::userInfo;
    }
    __super::OnOK();
}

void CSetRoleAuthority::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    Invalidate();
}

void CSetRoleAuthority::OnClickOk(CMyWindow* pWnd)
{
    this->OnOK();
}

void CSetRoleAuthority::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();
}

void CSetRoleAuthority::SetAuthority(UINT dataAuthority,
    UINT systemAuthority,
    UINT userCenterAuthority,
    UINT stasticsAuthority)
{
    m_dataAuthority = dataAuthority;
    m_systemAuthority = systemAuthority;
    m_userCenterAuthority = userCenterAuthority;
    m_stasticsAuthority = stasticsAuthority;
}

void CSetRoleAuthority::GetAuthority(UINT& dataAuthority,
    UINT& systemAuthority,
    UINT& userCenterAuthority,
    UINT& stasticsAuthority)
{
    dataAuthority = m_dataAuthority;
    systemAuthority = m_systemAuthority;
    userCenterAuthority = m_userCenterAuthority;
    stasticsAuthority = m_stasticsAuthority;
}


void CSetRoleAuthority::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    bool bMouseMoveInWnd = false;
    for each (auto item in m_mapWndList)
    {
        auto pWnd = item.second;
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


void CSetRoleAuthority::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here
    if (m_mapWndList.size() == 0)
    {
        return;
    }
    Resize();
}
void CSetRoleAuthority::Resize()
{

    CRect rect;
    GetClientRect(&rect);
    int iWidth = rect.Width();
    int iHeight = rect.Height();

    int iWindIndex = 0;

    int iOffsetx = 0;
    int iSplitHeight = 5;
    int iOffsety = iSplitHeight;
    int iExpandWidth = iWidth;
    auto pDataManager = (CMyCheckedExpandWnd*)m_mapWndList[iWindIndex++];
    pDataManager->MoveWindow(CPoint(iOffsetx, iOffsety));
    auto size = pDataManager->GetDisplayWndSize();
    int iExpandHeight = 0;
    if (pDataManager->IsExpand())
    {
        iExpandHeight = size.cy + NavigationItemHeight;
    }
    else
    {
        iExpandHeight = NavigationItemHeight;
    }
    pDataManager->SetDimention(iExpandWidth, iExpandHeight);

    iOffsety += iExpandHeight + iSplitHeight;
    auto pSystemManager = (CMyCheckedExpandWnd*)m_mapWndList[iWindIndex++];
    pSystemManager->MoveWindow(CPoint(iOffsetx, iOffsety));
    size = pSystemManager->GetDisplayWndSize();
    if (pSystemManager->IsExpand())
    {
        iExpandHeight = size.cy + NavigationItemHeight;
    }
    else
    {
        iExpandHeight = NavigationItemHeight;
    }
    pSystemManager->SetDimention(iExpandWidth, iExpandHeight);


    iOffsety += iExpandHeight + iSplitHeight;
    auto pUserCenterManager = (CMyCheckedExpandWnd*)m_mapWndList[iWindIndex++];
    pUserCenterManager->MoveWindow(CPoint(iOffsetx, iOffsety));
    size = pUserCenterManager->GetDisplayWndSize();
    if (pUserCenterManager->IsExpand())
    {
        iExpandHeight = size.cy + NavigationItemHeight;
    }
    else
    {
        iExpandHeight = NavigationItemHeight;
    }
    pUserCenterManager->SetDimention(iExpandWidth, iExpandHeight);


    /*iOffsety += iExpandHeight + iSplitHeight;
    auto pStatisticManager = (CMyCheckedExpandWnd*)m_mapWndList[iWindIndex++];
    pStatisticManager->MoveWindow(CPoint(iOffsetx, iOffsety));
    size = pStatisticManager->GetDisplayWndSize();
    if (pStatisticManager->IsExpand())
    {
        iExpandHeight = size.cy + NavigationItemHeight;
    }
    else
    {
        iExpandHeight = NavigationItemHeight;
    }
    pStatisticManager->SetDimention(iExpandWidth, iExpandHeight);*/

    int iButtonWidth = 100;
    int iButtonHeight = 30;
    int iSplitter = 50;
    int x = (iWidth - iButtonWidth * 2 - iSplitter) / 2;
    int y = iHeight - iButtonHeight - 10;
    auto pBtnOK = (CMyButton *)m_mapWndList[iWindIndex++];
    pBtnOK->MoveWindow(CPoint(x, y));
    pBtnOK->SetDimention(iButtonWidth, iButtonHeight);

    x += iButtonWidth + iSplitter;
    auto pBtnCancel = (CMyButton *)m_mapWndList[iWindIndex++];
    pBtnCancel->MoveWindow(CPoint(x, y));
    pBtnCancel->SetDimention(iButtonWidth, iButtonHeight);
    for each (auto item in m_mapWndList)
    {
        item.second->OnSize(0, 0);
    }
}


BOOL CSetRoleAuthority::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default

    return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
