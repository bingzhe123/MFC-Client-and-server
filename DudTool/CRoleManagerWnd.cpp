#include "pch.h"
#include "CRoleManagerWnd.h"
#include "CommonDef.h"
#include "CNewRoleDlg.h"
#include "HttpHelper.h"
#include "DudToolClient.h"
#include "CMyButtonInList.h"
#include "CEditRoleDlg.h"
#include "DudToolClient.h"
#include "CSetRoleAuthority.h"
#include "CConfirmDlg.h"
void CRoleManagerTileWnd::Initialize()
{
    m_EditBoxfont.DeleteObject();
    m_EditBoxfont.CreateFont(25,                    //   字体的高度   
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
    SetBkGroundColor(RGB(25, 31, 40));

    m_pBtnAddRole = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnAddRole->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    m_pBtnAddRole->SetText(CString(L"新建角色"));
    auto clickFun = std::bind(&CRoleManagerTileWnd::OnNewRole,this, std::placeholders::_1);
    m_pBtnAddRole->RegisterClickFun(clickFun);
    m_vecChildWnd.push_back(m_pBtnAddRole);


    m_pBtnSearch = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnSearch->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    m_pBtnSearch->SetText(CString(L"搜索"));
    //m_vecChildWnd.push_back(m_pBtnSearch);


    //CRect r;
    //m_editUserName.Create(ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, r, m_pRootWnd, UWM_IDD_USER_SEARCH);
    //m_editUserName.ModifyStyleEx(WS_TABSTOP, 0, 0);
    ////auto dwStyle = m_editUserName.GetStyle();
    ////dwStyle |= ES_NUMBER;
    ////SetWindowLong(m_editUserName.m_hWnd, GWL_STYLE, dwStyle);
    //m_editUserName.SetWindowTextW(L"");
    //m_editUserName.SetTextColor(RGB(255, 255, 255));
    //m_editUserName.SetBackColor(RGB(0, 50, 20));
    //m_editUserName.SetFont(&m_EditBoxfont);
    //m_editUserName.ShowWindow(SW_HIDE);
    //::PostMessage(m_editUserName.m_hWnd, EM_SETSEL, 0, 0);

    m_pLabelUserName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pLabelUserName->SetMouseHoverSensitive(false);
    m_pLabelUserName->SetText(L"角色简称");
   // m_vecChildWnd.push_back(m_pLabelUserName);
}

void CRoleManagerTileWnd::OnSize(int cx, int cy)
{
    CRect rect = GetClientRect();
    int iBtnHeight = rect.Height() - 50;
    int iBtnWidth = 150;
    int iOffset = 10;
    int x = iOffset;
    int y = (rect.Height() - iBtnHeight) / 2;
    m_pBtnAddRole->MoveWindow(CPoint(x, y));
    m_pBtnAddRole->SetDimention(iBtnWidth, iBtnHeight);

    int iEditHeight = 30;
    int iSearchHeight = iEditHeight;
    x = rect.Width() - iBtnWidth - iOffset;
    y = (rect.Height() - iSearchHeight) / 2;
    m_pBtnSearch->MoveWindow(CPoint(x, y));
    m_pBtnSearch->SetDimention(iBtnWidth, iSearchHeight);

    //CRect r;
    //int iEditWidth = 200;
    //int iTop = (rect.Height() - iEditHeight) / 2;
    //x = rect.right - iOffset * 2 - iEditWidth - iBtnWidth;
    //y = rect.top + iTop;
    //r.SetRect(x, y, x + iEditWidth, y + iEditHeight);
    //m_editUserName.MoveWindow(r);

  /*  int iLabelWidth = 100;
    x = rect.Width() - iOffset * 3 - iEditWidth - iBtnWidth - iLabelWidth;
    y = (rect.Height() - iSearchHeight) / 2;
    m_pLabelUserName->MoveWindow(CPoint(x, y));
    m_pLabelUserName->SetDimention(iLabelWidth, iSearchHeight);*/

}
void CRoleManagerTileWnd::ShowEdit(bool bEdit)
{
    return;
    if (bEdit)
    {
        m_editUserName.ShowWindow(SW_SHOW);
    }
    else
    {
        m_editUserName.ShowWindow(SW_HIDE);
    }
}

void CRoleManagerTileWnd::OnNewRole(CMyWindow* pWnd)
{
    CNewRoleDlg dlg;
    if (IDCANCEL == dlg.DoModal())
    {
        return;
    }
    auto vecNewItem = dlg.GetNewItem();

    if (m_funAddNewItem != nullptr)
    {
        m_funAddNewItem(String2CString(vecNewItem));
    }
}

void CRoleManagerTileWnd::RegisterAddNewItem(UploadFileFun f)
{
    m_funAddNewItem = f;
}                                                       


void CRoleManagerListWnd::Initialize()
{
    auto rect = GetClientRect();
    m_pHeadWnd = new CMyListHeaderWnd(this, m_pRootWnd, m_refreshImpFun);
    int w = rect.Width();
    int iHeadHeight = 50;
    vector<CString> vecHeader;
    vecHeader.push_back(L"角色简称");
    vecHeader.push_back(L"角色名称");
    vecHeader.push_back(L"创建时间");
    vecHeader.push_back(L"操作");        

    float fRatio = 1.0 / vecHeader.size();
    float fBtnRatio = fRatio * 2;
    fRatio = (1 - fBtnRatio) / (vecHeader.size() - 1);
    vector<float> vecRatio;
    for (size_t i = 0; i < vecHeader.size(); i++)
    {
        vecRatio.push_back(fRatio);
    }
    vecRatio[3] = fBtnRatio;

    m_pHeadWnd->Initialize(vecHeader, vecRatio);
    m_vecChildWnd.push_back(m_pHeadWnd);
}

void CRoleManagerListWnd::OnAddNewItem(vector<CString>& vecContent)
{
    m_pBottom->OnForceFirst(nullptr);
  /* if (m_pBottom != nullptr)
   {
       auto iTotal = m_pBottom->GetTotalCount();
       m_pBottom->SetTotalCount(iTotal + 1);
   }
   AddRoleItem(vecContent);*/
}

void CRoleManagerListWnd::AddRoleItem(vector<CString>& vecContent)
{
    if (m_mapContent.size() >= PerPageItemDataCount)
    {//如果当前这一页已经满了，就不用加了
        return;
    }
    //增加一条信息
    int iHeadHeight = 50;
    auto rect = GetClientRect();
    int iSplitHeight = 2;//每行间距
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (PerPageItemDataCount - 1)) / PerPageItemDataCount;
    int iOffsety = iHeadHeight + iSplitHeight + (iItemHeight + iSplitHeight )* m_iCurrentPageCount;
    CMyListItemWnd* pItemWnd = new CMyListItemWnd(this, m_pRootWnd, m_refreshImpFun);
    
    vector<CString> vecDisplayInfo;
    vecDisplayInfo.assign(vecContent.begin() + 5, vecContent.end());
    
    pItemWnd->AddItem(vecDisplayInfo, m_pHeadWnd, iOffsety, iItemHeight);


    auto iIndex = m_mapContent.size() - 1;
    vector<CMyWindow*> vecButton;
    auto pBtnEdit = new CMyButtonInList(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnEdit->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnEdit->SetText(CString(L"编辑"));
    pBtnEdit->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    pBtnEdit->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnEdit(pWnd);
    });
    vecButton.push_back(pBtnEdit);

    auto pBtnEditAuthority = new CMyButtonInList(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnEditAuthority->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnEditAuthority->SetText(CString(L"编辑角色权限"));
    pBtnEditAuthority->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));

    pBtnEditAuthority->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnEditRoleAuthority(pWnd);
    });
    vecButton.push_back(pBtnEditAuthority);


    auto pBtnDelete = new CMyButtonInList(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnDelete->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnDelete->SetText(CString(L"删除"));
    pBtnDelete->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    pBtnDelete->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnDelete(pWnd);
    });
    vecButton.push_back(pBtnDelete);
    pItemWnd->AddColumnWnd(3, vecButton);


    iOffsety += iItemHeight + iSplitHeight;
    m_vecChildWnd.push_back(pItemWnd);
    if (m_iCurrentPageCount % 2 == 0)
    {
        pItemWnd->SetBkPic(IDB_PNG_DATA_ITEM_LAN_BK);
    }
    else
    {
        pItemWnd->SetBkPic(IDB_PNG_DATA_ITEM_HUANG_BK);
    }
    pItemWnd->UpdateMyWnd(pItemWnd->GetClientRect());
    m_mapContent[pItemWnd] = vecContent;
    m_iCurrentPageCount++;
}

void CRoleManagerListWnd::ClearPageItems()
{
    m_iCurrentPageCount = 0;
    for each (auto item in m_vecChildWnd)
    {
        if (item != m_pHeadWnd)
        {
            delete item;                
        }
        if (item == m_pCurrentMouseInWnd)
        {
            m_pCurrentMouseInWnd = nullptr;
        }
    }
    m_vecChildWnd.clear();
    m_vecChildWnd.push_back(m_pHeadWnd);
    m_mapContent.clear();
    UpdateMyWnd(GetClientRect());
}


void CRoleManagerListWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    int w = rect.Width();
    int iHeadHeight = 50;
    m_pHeadWnd->MoveWindow(0, 0);
    m_pHeadWnd->SetDimention(w, iHeadHeight);
    m_pHeadWnd->OnSize(cx, cy);

    int iSplitHeight = 2;//每行间距
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (PerPageItemDataCount - 1)) / PerPageItemDataCount;

    int iOffsety = iHeadHeight + iSplitHeight;
    for (int i = 1; i < m_vecChildWnd.size(); i++)
    {
        auto* pItemWnd = m_vecChildWnd[i];
        pItemWnd->MoveWindow(0, iOffsety);
        pItemWnd->SetDimention(w, iItemHeight);
        pItemWnd->OnSize(cx, cy);
        iOffsety += iItemHeight + iSplitHeight;
    }
}

CMyLable* CRoleManagerListWnd::CreateLable(int x, int y, int w, int h, const CString& sContent)
{
    auto pLabel = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLabel->SetText(sContent);
    pLabel->MoveWindow(x, y);
    pLabel->SetDimention(w, h);
    return pLabel;
}

void CRoleManagerListWnd::SetBottomNavigatewnd(CBottomNavigateWnd* pWnd)
{
    m_pBottom = pWnd;
}

void CRoleManagerListWnd::OnEdit(CMyWindow* pWnd)
{
    auto pListButton = (CMyButtonInList*)pWnd;
    auto pItemWnd = (CMyListItemWnd*)pWnd->GetParent();
    auto &vecItem = m_mapContent[pItemWnd];

    //CString sUID = vecItem[0];
    CEditRoleDlg dlg(vecItem);
    auto res = dlg.DoModal();
    if (IDCANCEL == res)
    {
        return;
    }
    CString sRoleDes;
    CString sRoleName;
    dlg.GetNewInfo(sRoleDes, sRoleName);
    vecItem[3] = sRoleDes;
    vecItem[4] = sRoleName;
    pItemWnd->ModifyColumnText(0, sRoleDes);
    pItemWnd->ModifyColumnText(1, sRoleName);
}

void CRoleManagerListWnd::OnEditRoleAuthority(CMyWindow* pWnd)
{
    auto pListButton = (CMyButtonInList*)pWnd;
    auto pItemWnd = (CMyListItemWnd*)pWnd->GetParent();
    auto& vecItem = m_mapContent[pItemWnd];
    auto& sName = vecItem[5];
    auto eDataAuthority = (_ttol(vecItem[1]));
    auto eSystemAuthority = (_ttol(vecItem[2]));
    auto eUserCenterAuthority = (_ttol(vecItem[3]));
    auto eStasticsAuthority = (_ttol(vecItem[4]));

    CSetRoleAuthority dlg(sName);
    dlg.SetAuthority(eDataAuthority, eSystemAuthority, eUserCenterAuthority, eStasticsAuthority);
    auto res = dlg.DoModal();
    if (IDCANCEL == res)
    {
        return;
    }
    UINT uDataAuthority = 0;
    UINT uSystemAuthority = 0;
    UINT uUserCenterAuthority = 0;
    UINT uStasticsAuthority = 0;
    dlg.GetAuthority(uDataAuthority, uSystemAuthority, uUserCenterAuthority, uStasticsAuthority);
    HttpHelper httpHelper(theApp.m_spClient);
    string sID = CW2A(vecItem[0]);
    string sNewRoleInfo = sID + "," + std::to_string(uDataAuthority) + "," +
        std::to_string(uSystemAuthority) + "," +
        std::to_string(uUserCenterAuthority) + "," +
        std::to_string(uStasticsAuthority);
    if (ErrorCode::Failed == httpHelper.ModifyRoleAuthority(sNewRoleInfo))
    {
        AfxMessageBox(L"修改失败");
    }
    else
    {
        CString sTemp;
        sTemp.Format(L"%d", uDataAuthority);
        vecItem[1] = sTemp;

        sTemp.Format(L"%d", uSystemAuthority);
        vecItem[2] = sTemp;

        sTemp.Format(L"%d", uUserCenterAuthority);
        vecItem[3] = sTemp;

        sTemp.Format(L"%d", uStasticsAuthority);
        vecItem[4] = sTemp;
    }
}

void CRoleManagerListWnd::OnDelete(CMyWindow* pWnd)
{
    CConfirmDlg dlg;
    if (dlg.DoModal() == IDCANCEL)
    {
        return;
    }


    if (!theApp.m_bIsManager)
    {
        AfxMessageBox(L"没有权限删除！");
        return;
    }
    auto pListButton = (CMyButtonInList*)pWnd;
    auto pItemWnd = (CMyListItemWnd*)pWnd->GetParent();
    auto& vecItem = m_mapContent[pItemWnd];

    CString sUID = vecItem[0];
   
    HttpHelper httpHelper(theApp.m_spClient);
    string _sUID = CW2A(sUID);
    auto eErrorCode = httpHelper.DeleteRoleInfo(_sUID);
    if (eErrorCode == ErrorCode::Failed)
    {                                  
        AfxMessageBox(L"删除失败！");
        return;
    }
    if (m_pBottom != nullptr)
    {
        auto iTotalCount = m_pBottom->GetTotalCount();
        m_pBottom->SetTotalCount(--iTotalCount);
        m_pBottom->ForceUpdateThisPage();
    }
}


CRoleManagerWnd::~CRoleManagerWnd()
{
}

void CRoleManagerWnd::Initialize()
{
    m_pTileWnd = new CRoleManagerTileWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pTileWnd->Initialize();
    m_vecChildWnd.push_back(m_pTileWnd);

    m_pUserManagerListWnd = new CRoleManagerListWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pUserManagerListWnd->Initialize();
    m_vecChildWnd.push_back(m_pUserManagerListWnd);

    m_pBottom = new CBottomNavigateWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pBottom->Initialize(3);
    m_pBottom->RegisterOnpageFunction([this](int iStart, int iCount)
    {
        this->OnPage(iStart, iCount);
    });
    m_vecChildWnd.push_back(m_pBottom);

    m_pUserManagerListWnd->SetBottomNavigatewnd(m_pBottom);
    m_pTileWnd->RegisterAddNewItem([&](vector<CString> &vecItem)
    { 
        m_pUserManagerListWnd->OnAddNewItem(vecItem);
    });
}

void CRoleManagerWnd::OnSize(int cx, int cy)
{
    this->SetDimention(cx, cy);
    m_pTileWnd->SetDimention(cx, DispalyRegionToolBarHeight);
    m_pTileWnd->OnSize(cx, cy);

    int iToolBarHeight = DispalyRegionToolBarHeight;
    int w = m_rect.Width();
    int h = 50;
    int iOffsetx = 0;
    int iOffsety = iToolBarHeight + 5;
    int iBottomHeight = 50;
    int iBottomOffset = 5;
    int iContentHeight = m_rect.Height() - iToolBarHeight - iBottomHeight - iBottomOffset * 2;
    m_pUserManagerListWnd->MoveWindow(iOffsetx, iOffsety);
    m_pUserManagerListWnd->SetDimention(w, iContentHeight);
    m_pUserManagerListWnd->OnSize(cx, cy);
    iOffsety += iContentHeight;
    m_pBottom->MoveWindow(iOffsetx, iOffsety);
    m_pBottom->SetDimention(w, iBottomHeight - iBottomOffset);
    m_pBottom->OnSize(cx, cy);
 


}
void CRoleManagerWnd::SetCurrentPageItems(const string &sContent)
{
    m_pUserManagerListWnd->ClearPageItems();
    auto vecInfos = SplitString(sContent, '|');
    for each (auto item in vecInfos)
    {
        CString sTemp = CString(item.c_str());
        auto vecItem = SplitString(sTemp, L',');
        m_pUserManagerListWnd->AddRoleItem(vecItem);
    }
}            
void CRoleManagerWnd::SetTotalCount(int iTotalCount)
{
    m_pBottom->SetTotalCount(iTotalCount);
}
void CRoleManagerWnd::SetVisible(bool bVisible)
{
    __super::SetVisible(bVisible);
    m_pTileWnd->ShowEdit(bVisible);
    m_pBottom->ShowEdit(bVisible);
}


void CRoleManagerWnd::OnPage(int iStart, int iCount)
{
    HttpHelper httpHelper(theApp.m_spClient);
    int iTotalCount = 0;
    auto sRoleInfos = httpHelper.GetAllRoleInfo(iStart, iCount, iTotalCount);
    SetTotalCount(iTotalCount);
    SetCurrentPageItems(sRoleInfos);
}