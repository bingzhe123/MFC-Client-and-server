#include "pch.h"
#include "CLeftPannelWnd.h"
#include "CMyExpandWnd.h"
#include "CMyStaticWindow.h"
#include "HttpHelper.h"
#include "DudToolClient.h"
CLeftPannelWnd::~CLeftPannelWnd()
{
}

void CLeftPannelWnd::Initialize()
{
    m_bSupportClick = true;
    m_pHeadWnd = new CMyWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pHeadWnd->SetBkPic(IDB_PNG_HOME_DOCTOR);
    m_pHeadWnd->SetName(L"LeftPannel_HeadWnd");
    m_vecChildWnd.push_back(m_pHeadWnd);


    auto pCheckWndGroup = CExpandGroup::Instance();
    m_pHomeCheckWnd = new CMyExpandWnd(this, m_pRootWnd, m_refreshImpFun, pCheckWndGroup);
    m_pHomeCheckWnd->SetBkGroundColor(RGB(14, 19, 26));
    m_pHomeCheckWnd->SetBkPic(0, IDB_PNG_SELECT_BK);
    m_pHomeCheckWnd->SetIconPic(IDB_PNG_HOME_UNSELECT, IDB_PNG_HOME_SELECT);
    m_pHomeCheckWnd->SetText(CString(L"首页"));
    m_pHomeCheckWnd->SetName(CString(L"首页"));
    m_vecChildWnd.push_back(m_pHomeCheckWnd);
    m_mapSubWnd[LeftPannelChildWndID::HOME_ID] = m_pHomeCheckWnd;

    int iHeight = NavigationItemHeight;


    HttpHelper httpHelper(theApp.m_spClient);
    UserAuthoriy userAuthority;
    httpHelper.GetUserAuthority(theApp.m_sUserID, userAuthority);


#pragma region  存储管理
    m_pDataCheckWnd = new CMyExpandWnd(this, m_pRootWnd, m_refreshImpFun, pCheckWndGroup);
    m_pDataCheckWnd->SetBkGroundColor(RGB(14, 19, 26));
    m_pDataCheckWnd->SetBkPic(0, IDB_PNG_SELECT_BK);
    m_pDataCheckWnd->SetIconPic(IDB_PNG_DATA_UNSELECT, IDB_PNG_DATA_SELECT);
    m_pDataCheckWnd->SetExpandMark(IDB_PNG_EXPAND, IDB_PNG_UNEXPAND);
    m_pDataCheckWnd->SetText(CString(L"存储管理"));
    m_pDataCheckWnd->SetName(CString(L"存储管理"));
    m_vecChildWnd.push_back(m_pDataCheckWnd);

    int iTextOffsetx = 20;
    if ((userAuthority.uiDataAuthority & (UINT64)DataAuthorityDef::allMRDataVisible)
        || (userAuthority.uiDataAuthority & (UINT64)DataAuthorityDef::selfMRDataVisible))
    {
        auto* pCTData = new CMyStaticWindow(m_pDataCheckWnd, m_pRootWnd, m_refreshImpFun);
        pCTData->SetBkGroundColor(RGB(25, 31, 38));
        pCTData->SetText(CString(L"CT/MRI影像数据存储"), CPoint(iTextOffsetx, iHeight / 5));
        pCTData->SetSupportClick(true);
        pCTData->SetMouseHoverSensitive(true);
        pCTData->SetVisible(false);
        m_mapSubWnd[LeftPannelChildWndID::DATA_MANAGER_ID] = pCTData;
        m_pDataCheckWnd->AddChildWnd(pCTData, NavigationItSubTextHeight);
    }
    if ((userAuthority.uiDataAuthority & (UINT64)DataAuthorityDef::all3DDataVisible)
        || (userAuthority.uiDataAuthority & (UINT64)DataAuthorityDef::self3DDataVisible))

    {
        auto* p3DData = new CMyStaticWindow(m_pDataCheckWnd, m_pRootWnd, m_refreshImpFun);
        p3DData->SetBkGroundColor(RGB(25, 31, 38));
        p3DData->SetText(CString(L"术前模型数据存储"), CPoint(iTextOffsetx, iHeight / 5));
        p3DData->SetSupportClick(true);
        p3DData->SetMouseHoverSensitive(true);
        p3DData->SetVisible(false);
        m_mapSubWnd[LeftPannelChildWndID::DATA_3D_FILE] = p3DData;
        m_pDataCheckWnd->AddChildWnd(p3DData, NavigationItSubTextHeight);
    }
    
    if ((userAuthority.uiDataAuthority & (UINT64)DataAuthorityDef::allAriginDataVisible)
        || (userAuthority.uiDataAuthority & (UINT64)DataAuthorityDef::selfAriginDataVisible))
    {
        auto* pAriginData = new CMyStaticWindow(m_pDataCheckWnd, m_pRootWnd, m_refreshImpFun);
        pAriginData->SetBkGroundColor(RGB(25, 31, 38));
        pAriginData->SetText(CString(L"导板数据存储"), CPoint(iTextOffsetx, iHeight / 5));
        pAriginData->SetSupportClick(true);
        pAriginData->SetMouseHoverSensitive(true);
        pAriginData->SetVisible(false);
        m_mapSubWnd[LeftPannelChildWndID::DATA_ARIGIN] = pAriginData;
        m_pDataCheckWnd->AddChildWnd(pAriginData, NavigationItSubTextHeight);

    }
    

#pragma endregion

#pragma region  系统管理导航栏

    bool bUserVisible = false;  
    bool bRoleManger = false;
    bool bUserCenter = false;

    if ((userAuthority.uiSystemAuthoriy & (UINT64)SystemAuthorityDef::userManager))
    {
        bUserVisible = true;
    }
    if ((userAuthority.uiSystemAuthoriy & (UINT64)SystemAuthorityDef::roleManager))
    {
        bRoleManger = true;
    }
    if ((userAuthority.uiUserCenterAuthoriy & (UINT64)UserCenterAuthorityDef::userInfo))
    {
        bUserCenter = true;
    }

    if (bUserVisible || bRoleManger)
    {
        m_pSystemWnd = new CMyExpandWnd(this, m_pRootWnd, m_refreshImpFun, pCheckWndGroup);
        m_pSystemWnd->SetBkGroundColor(RGB(14, 19, 26));
        m_pSystemWnd->SetBkPic(0, IDB_PNG_SELECT_BK);
        m_pSystemWnd->SetIconPic(IDB_PNG_SYSTEM_SELECT, IDB_PNG_SYSTEM_UNSELECT);
        m_pSystemWnd->SetExpandMark(IDB_PNG_EXPAND, IDB_PNG_UNEXPAND);

        m_pSystemWnd->SetText(CString(L"系统管理"));
        m_pSystemWnd->SetName(CString(L"系统管理"));
        m_vecChildWnd.push_back(m_pSystemWnd);
    }
    if (bUserVisible)
    {
        auto* pUserManager = new CMyStaticWindow(m_pSystemWnd, m_pRootWnd, m_refreshImpFun);
        pUserManager->SetBkGroundColor(RGB(25, 31, 38));
        pUserManager->SetText(CString(L"用户管理"), CPoint(iTextOffsetx, iHeight / 5));
        pUserManager->SetSupportClick(true);
        pUserManager->SetMouseHoverSensitive(true);
        pUserManager->SetVisible(false);
        // m_wndList.AddTail(pUserManager);
        m_pSystemWnd->AddChildWnd(pUserManager, NavigationItSubTextHeight);
        m_mapSubWnd[LeftPannelChildWndID::USER_MANAGER_ID] = pUserManager;
    }
   
    if (bRoleManger)
    {
        auto pRoleManager = new CMyStaticWindow(m_pSystemWnd, m_pRootWnd, m_refreshImpFun);
        pRoleManager->SetBkGroundColor(RGB(25, 31, 38));
        pRoleManager->SetText(CString(L"角色管理"), CPoint(iTextOffsetx, iHeight / 5));
        pRoleManager->SetSupportClick(true);
        pRoleManager->SetMouseHoverSensitive(true);
        pRoleManager->SetVisible(false);
        m_pSystemWnd->AddChildWnd(pRoleManager, NavigationItSubTextHeight);
        m_mapSubWnd[LeftPannelChildWndID::ROLE_MANAGER_ID] = pRoleManager;
    }

    
   // httpHelper.GetUserAuthority(theApp.m_sUserID, userAuthority);
   

#pragma endregion

#pragma region 个人中心
    if (bUserCenter)
    {
        m_pUserCenter = new CMyExpandWnd(this, m_pRootWnd, m_refreshImpFun, pCheckWndGroup);
        m_pUserCenter->SetBkGroundColor(RGB(14, 19, 26));
        m_pUserCenter->SetBkPic(0, IDB_PNG_SELECT_BK);
        m_pUserCenter->SetIconPic(IDB_PNG_USER_CENTER, IDB_PNG_USER_CENTER);
        m_pUserCenter->SetExpandMark(IDB_PNG_EXPAND, IDB_PNG_UNEXPAND);

        m_pUserCenter->SetText(CString(L"个人中心"));
        m_pUserCenter->SetName(CString(L"个人中心"));
        // pCheckWndGroup->AddWnd(pUserCenter);
        m_vecChildWnd.push_back(m_pUserCenter);

        auto pUserInfo = new CMyStaticWindow(m_pUserCenter, m_pRootWnd, m_refreshImpFun);
        pUserInfo->SetBkGroundColor(RGB(25, 31, 38));
        pUserInfo->SetText(CString(L"用户信息"), CPoint(iTextOffsetx, iHeight / 5));
        pUserInfo->SetSupportClick(true);

        pUserInfo->SetMouseHoverSensitive(true);
        pUserInfo->SetVisible(false);
        // m_wndList.AddTail(pUserInfo);
        m_pUserCenter->AddChildWnd(pUserInfo, NavigationItSubTextHeight);
        m_mapSubWnd[LeftPannelChildWndID::USER_INFO_ID] = pUserInfo;
    }
#pragma endregion
}

void CLeftPannelWnd::ForceUpdate()
{
    for each (auto item in m_vecChildWnd)
    {
        item->ForceUpdate();
    }
}

void CLeftPannelWnd::RegisterClickFun(LeftPannelChildWndID iPannelChildWndID, ClickFunc f)
{
    if (m_mapSubWnd.find(iPannelChildWndID) != m_mapSubWnd.end())
    {
        auto pWnd = m_mapSubWnd[iPannelChildWndID];
        pWnd->RegisterClickFun(f);

    }
}
CMyWindow* CLeftPannelWnd::GetWindow(LeftPannelChildWndID id)
{
    return m_mapSubWnd[id];
}
bool CLeftPannelWnd::Refresh(CDC* pMemDC, CRect rect)
{
    return __super::Refresh(pMemDC, rect);
}
void CLeftPannelWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    int iWidth = m_rect.Width();
    int iHeight = NavigationItemHeight;
    auto bkSize = m_pHeadWnd->GetBkPicSize();
    int iOffsetx = 0;
    int iOffsety = 10;
    m_pHeadWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pHeadWnd->SetDimention(iWidth, bkSize.cy);


    int iSplitHeight = 5;//每项间距
    iOffsety += bkSize.cy + iSplitHeight + 20;
    m_pHomeCheckWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pHomeCheckWnd->SetDimention(iWidth, iHeight);

    auto childRect = m_pHomeCheckWnd->GetClientRect();
    iOffsety += childRect.Height() + iSplitHeight;
    m_pDataCheckWnd->MoveWindow(CPoint(iOffsetx, iOffsety));

    childRect = m_pDataCheckWnd->GetClientRect();
    int iChildiHeight = childRect.Height();
    if (iChildiHeight == 0)
    {
        iChildiHeight = iHeight;
    }
    m_pDataCheckWnd->SetDimention(iWidth, iChildiHeight);

    iOffsety += iChildiHeight + iSplitHeight;
    if (m_pSystemWnd != nullptr)
    {
        m_pSystemWnd->MoveWindow(CPoint(iOffsetx, iOffsety));
        childRect = m_pSystemWnd->GetClientRect();
        iChildiHeight = childRect.Height();
        if (iChildiHeight == 0)
        {
            iChildiHeight = iHeight;
        }
        m_pSystemWnd->SetDimention(iWidth, iChildiHeight);
        iOffsety += iChildiHeight + iSplitHeight;
    }
   
    if (m_pUserCenter != nullptr)
    {
        m_pUserCenter->MoveWindow(CPoint(iOffsetx, iOffsety));
        childRect = m_pUserCenter->GetClientRect();
        iChildiHeight = childRect.Height();
        if (iChildiHeight == 0)
        {
            iChildiHeight = iHeight;
        }
        m_pUserCenter->SetDimention(iWidth, iChildiHeight);
    }
    
    
    
    for each (auto item in m_vecChildWnd)
    {
        item->OnSize(0, 0);
    }
}
