#include "pch.h"
#include "CUserManagerWnd.h"
#include "CommonDef.h"
#include "CNewUserDlg.h"
#include "HttpHelper.h"
#include "DudToolClient.h"
#include "CSetUserRoleDlg.h"
#include "CModifyUserInfoDlg.h"
#include "CConfirmDlg.h"

void CUserManagerTileWnd::Initialize()
{
    SetBkGroundColor(RGB(25, 31, 40));
   
    m_pBtnUserRegister = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnUserRegister->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    m_pBtnUserRegister->SetText(CString(L"增加用户"));
    m_pBtnUserRegister->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnNewUser(pWnd);
    });
    m_vecChildWnd.push_back(m_pBtnUserRegister);


    m_pBtnSearch = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnSearch->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    m_pBtnSearch->SetText(CString(L"搜索"));
    m_pBtnSearch->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnSearch(pWnd);
    });
    m_vecChildWnd.push_back(m_pBtnSearch);


    CRect r;
    m_editUserName.Create(ES_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER, r, m_pRootWnd, UWM_IDD_USER_SEARCH);
    m_editUserName.ModifyStyleEx(WS_TABSTOP, 0, 0);
    //auto dwStyle = m_editUserName.GetStyle();
    //dwStyle |= ES_NUMBER;
    //SetWindowLong(m_editUserName.m_hWnd, GWL_STYLE, dwStyle);
    m_editUserName.SetWindowTextW(L"");
    m_editUserName.SetTextColor(RGB(255, 255, 255));
    m_editUserName.SetBackColor(RGB(0, 50, 20));
    m_editUserName.SetFont(&m_font);
    m_editUserName.ShowWindow(SW_HIDE);
    ::PostMessage(m_editUserName.m_hWnd, EM_SETSEL, 0, 0);
    // m_textBox.PostMessageW(WM_KILLFOCUS, 0, 0);    //失去焦点  

    m_pLabelUserName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pLabelUserName->SetMouseHoverSensitive(false);
    m_pLabelUserName->SetText(L"用户姓名");
    m_vecChildWnd.push_back(m_pLabelUserName);
}

void CUserManagerTileWnd::OnSize(int cx, int cy)
{
    CRect rect = GetClientRect();
    int iBtnHeight = rect.Height() - 50;
    int iBtnWidth = 150;
    int iOffset = 10;
    int x = iOffset;
    int y = (rect.Height() - iBtnHeight) / 2;
    m_pBtnUserRegister->MoveWindow(CPoint(x, y));
    m_pBtnUserRegister->SetDimention(iBtnWidth, iBtnHeight);

    int iEditHeight = 35;
    int iSearchHeight = iEditHeight;
    x = rect.Width() - iBtnWidth - iOffset;
    y = (rect.Height() - iSearchHeight) / 2;
    m_pBtnSearch->MoveWindow(CPoint(x, y));
    m_pBtnSearch->SetDimention(iBtnWidth, iSearchHeight);

    CRect r;
    int iEditWidth = 200;
    int iTop = (rect.Height() - iEditHeight) / 2;
    x = rect.right - iOffset*2 - iEditWidth - iBtnWidth;
    y = rect.top + iTop;
    r.SetRect(x, y, x + iEditWidth, y + iEditHeight);
    m_editUserName.MoveWindow(r);

    int iLabelWidth = 100;
    x = rect.Width() - iOffset * 3 - iEditWidth - iBtnWidth - iLabelWidth;
    y = (rect.Height() - iSearchHeight) / 2;
    m_pLabelUserName->MoveWindow(CPoint(x, y));
    m_pLabelUserName->SetDimention(iLabelWidth, iSearchHeight);

}
void CUserManagerTileWnd::ShowEdit(bool bEdit)
{
    if (bEdit)
    {
        m_editUserName.ShowWindow(SW_SHOW);
    }
    else
    {
        m_editUserName.ShowWindow(SW_HIDE);
    }
}

void CUserManagerTileWnd::OnNewUser(CMyWindow* pWnd)
{
    if (m_funAddNewInfo != nullptr)
    {
        m_funAddNewInfo();
    }
}

void CUserManagerTileWnd::OnSearch(CMyWindow* pWnd)
{
    if (m_funSearch != nullptr)
    {
        CString sUserName;
        m_editUserName.GetWindowTextW(sUserName);
   /*     if (sUserName.IsEmpty())
        {    
            return;
        }*/
        m_funSearch(sUserName);
    }
}

void CUserManagerTileWnd::RegisterAddNewUser(AddNewInfoFun f)
{
    m_funAddNewInfo = f;
}

void CUserManagerTileWnd::RegisterSearchUser(SearchUserFun f)
{
    m_funSearch = f;
}

string CUserManagerTileWnd::GetSearchName()
{
    CString sUserName;
    m_editUserName.GetWindowTextW(sUserName);
    if (sUserName.IsEmpty())
    {
        return string();
    }
    return CW2A(sUserName.GetString());
}

void CUserManagerListWnd::Initialize()
{
    auto rect = GetClientRect();
    m_pHeadWnd = new CMyListHeaderWnd(this, m_pRootWnd, m_refreshImpFun);
    int w = rect.Width();
    vector<CString> vecHeader;
    vecHeader.push_back(L"序号");
    vecHeader.push_back(L"用户姓名");
    vecHeader.push_back(L"登录名");
    vecHeader.push_back(L"邮箱");
    vecHeader.push_back(L"角色");
    vecHeader.push_back(L"创建时间");
    vecHeader.push_back(L"操作");  
 
    float fRatio = 1.0 / vecHeader.size();
    float fNoRatio = fRatio / 2;
    fRatio = (1 - fNoRatio) / (vecHeader.size() - 1);
    vector<float> vecRatio;
    for (size_t i = 0; i < vecHeader.size(); i++)
    {
        vecRatio.push_back(fRatio);
    }        
    vecRatio[0] = fNoRatio;

    m_pHeadWnd->Initialize(vecHeader, vecRatio);
    m_vecChildWnd.push_back(m_pHeadWnd);
}


void CUserManagerListWnd::OnSize(int cx, int cy)
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

CMyLable* CUserManagerListWnd::CreateLable(int x, int y, int w, int h, const CString& sContent)
{
    auto pLabel = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLabel->SetText(sContent);
    pLabel->MoveWindow(x, y);
    pLabel->SetDimention(w, h);
    return pLabel;
}

void CUserManagerListWnd::AddNewItem(vector<CString>& vecContent)
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
    int iOffsety = iHeadHeight + iSplitHeight + (iItemHeight + iSplitHeight) * m_iCurrentPageCount;
    CMyListItemWnd* pItemWnd = new CMyListItemWnd(this, m_pRootWnd, m_refreshImpFun);
    //"select id, name, login_name, manager, email, role, create_time
    vector<CString> vecTemp;
    for (int i = 0; i < vecContent.size(); i++)
    {
        if (i == 3 || i == 7 )
            continue;
        vecTemp.push_back(vecContent[i]);
    }
    vecTemp[4] = vecContent[7];
    pItemWnd->AddItem(vecTemp, m_pHeadWnd, iOffsety, iItemHeight);
    vector<CMyWindow*> vecButton;


    auto pBtnRole = new CMyButton(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnRole->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnRole->SetText(CString(L"分配角色"));
    pBtnRole->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnSetRole(pWnd);
    });
    pBtnRole->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    vecButton.push_back(pBtnRole);

    auto pBtnEdit = new CMyButton(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnEdit->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnEdit->SetText(CString(L"编辑"));
    pBtnEdit->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    pBtnEdit->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnEdit(pWnd);
    });
    vecButton.push_back(pBtnEdit);
    auto pBtnDelete = new CMyButton(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnDelete->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnDelete->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    pBtnDelete->SetText(CString(L"删除"));
    pBtnDelete->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnDelete(pWnd);
    });
    vecButton.push_back(pBtnDelete);
    pItemWnd->AddColumnWnd(6, vecButton);

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

void CUserManagerListWnd::ModifyItem(CMyWindow* pWnd, int iIndex, const CString& sNewContent)
{
    ((CMyListItemWnd*)pWnd)->ModifyColumnText(iIndex, sNewContent);
    pWnd->UpdateMyWnd(pWnd->GetClientRect());
}

void CUserManagerListWnd::ClearPageItems()
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

void CUserManagerListWnd::OnSetRole(CMyWindow* pWnd)
{
    HttpHelper httpHelper(theApp.m_spClient);
    int iTotalCount = 0;
    auto sAllRoleInfo = httpHelper.GetAllRoleInfo(0, MAXINT32, iTotalCount);
    CString sTemp(sAllRoleInfo.c_str());
    auto vecAllRoleInfo = SplitString(sTemp, '|');
    vector<RoleInfo> vecRoleInfo;
    for each (auto item in vecAllRoleInfo)
    {
        auto vecItem = SplitString(item, ',');
        RoleInfo info{ vecItem[0], vecItem[5], false };
        vecRoleInfo.push_back(info);
    }
    auto vecUserInfo = m_mapContent[pWnd->GetParent()];
    auto sRole = vecUserInfo[5];
    auto vecRole = SplitString(sRole, '&');
    for each ( auto item in vecRole)
    {
        auto it = std::find_if(vecRoleInfo.begin(), vecRoleInfo.end(), [&item](RoleInfo &roleInfo)
        {
            return roleInfo.id == item;
        });
        if (it != vecRoleInfo.end())
        {
            it->isSelected = true;
        }
    }

    CSetUserRoleDlg dlg(vecRoleInfo);
    if (IDCANCEL == dlg.DoModal())
    {
        return;
    }
    auto vecSlectRoleInfo = dlg.GetAllSelectRole();
    CString sRoleId;
    for each (auto item in vecSlectRoleInfo)
    {
        sRoleId += item.id + L"&";
    }
    if (!sRoleId.IsEmpty())
    {
        sRoleId = sRoleId.Left(sRoleId.GetLength() - 1);
    }
    string sUserID = CW2A(vecUserInfo[0].GetString());
    string sNewRole = CW2A(sRoleId.GetString());
    string sNewUserInfo;
    if (ErrorCode::Failed == httpHelper.ModifyUserRole(sUserID, sNewRole, sNewUserInfo))
    {
        AfxMessageBox(L"修改失败!");
        return;
    }
    CString wsTemp = CString(sNewUserInfo.c_str());
    auto vecNewUserInfo = SplitString(wsTemp, ',');
    m_mapContent[pWnd->GetParent()] = vecNewUserInfo;

    ModifyItem(pWnd->GetParent(), 4, vecNewUserInfo[7]);

}
void CUserManagerListWnd::OnEdit(CMyWindow* pWnd)
{
    auto pItemWnd = dynamic_cast<CMyListItemWnd*>(pWnd->GetParent());
    assert(pItemWnd != nullptr);
    auto vecUserInfo = m_mapContent[pItemWnd];

    CModifyUserInfoDlg dlg(vecUserInfo);
    auto res = dlg.DoModal();
    if (IDCANCEL == res)
    {
        return;
    }
    auto newInfo = dlg.GetNewUserInfo();
   
    string sUserID = CW2A(vecUserInfo[0].GetString());
    HttpHelper httpHelper(theApp.m_spClient);
    string sNewInfo;
    auto erroCode = httpHelper.ModifyUserInfo(sUserID, newInfo, sNewInfo);
    if (erroCode == ErrorCode::Repeat)
    {
        AfxMessageBox(L"用户信息重合!");
        OnEdit(pWnd);
        return;
    }
    if (erroCode == ErrorCode::Failed)
    {
        AfxMessageBox(L"操作失败!");
        return;
    }
    CString sTemp = CString(sNewInfo.c_str());
    auto vecNewInfo = SplitString(sTemp, ',');
    m_mapContent[pItemWnd] = vecNewInfo;
    m_mapContent[pItemWnd].pop_back();
    m_mapContent[pItemWnd].push_back(vecUserInfo.back());
    ModifyItem(pItemWnd, 0, vecNewInfo[0]);
    ModifyItem(pItemWnd, 1, vecNewInfo[1]);
    ModifyItem(pItemWnd, 2, vecNewInfo[2]);
}
void CUserManagerListWnd::OnDelete(CMyWindow* pWnd)
{
    auto pItemWnd = dynamic_cast<CMyListItemWnd*>(pWnd->GetParent());
    assert(pItemWnd != nullptr);
    auto vecUserInfo = m_mapContent[pItemWnd];
    if (vecUserInfo[1] == L"SuperUser")
    {
        AfxMessageBox(L"不能删除SueprUser!");
        return;
    }
    m_pDeleteFun(vecUserInfo[0]);
}

void CUserManagerListWnd::RegisterDeleteUserFun(DeleteUserFun f)
{
    m_pDeleteFun = f;
}

CUserManagerWnd::~CUserManagerWnd()
{
}

void CUserManagerWnd::Initialize()
{
    m_pTileWnd = new CUserManagerTileWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pTileWnd->Initialize();
    m_pTileWnd->RegisterAddNewUser([this]()
    {
        this->OnAddNewUser(NewUserInfo());
    });
    m_pTileWnd->RegisterSearchUser([this](const CString& sUserName)
    {
        this->OnSearch(sUserName);
    });
    m_vecChildWnd.push_back(m_pTileWnd);

    m_pUserManagerListWnd = new CUserManagerListWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pUserManagerListWnd->Initialize();
    m_pUserManagerListWnd->RegisterDeleteUserFun([this](const CString& sUser)
    {
        this->OnDeleteUser(sUser);
    });
    m_vecChildWnd.push_back(m_pUserManagerListWnd);

    m_pBottom = new CBottomNavigateWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pBottom->RegisterOnpageFunction([this](int iStart, int iCount)
    {
        this->OnPage(iStart, iCount);
    });
    m_pBottom->Initialize(1000);
    m_vecChildWnd.push_back(m_pBottom);

}

void CUserManagerWnd::OnSize(int cx, int cy)
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
void CUserManagerWnd::OnAddNewUser(NewUserInfo newUserInfo)
{
    CNewUserDlg dlg(newUserInfo);
    if (IDCANCEL == dlg.DoModal())
    {
        return;
    }
    auto newInfo = dlg.GetNewUserInfo();
    HttpHelper httpHelper(theApp.m_spClient);
    string sNewInfo;
    auto erroCode = httpHelper.AddNewUser(newInfo, sNewInfo);
    if (erroCode == ErrorCode::Repeat)
    {
        AfxMessageBox(L"用户信息重合!");
        OnAddNewUser(newInfo);
        return;
    }
    if (erroCode == ErrorCode::Failed)
    {
        AfxMessageBox(L"操作失败!");
        return;
    }
    CString sTemp = CString(sNewInfo.c_str());
    auto vecNewInfo = SplitString(sTemp, ',');
    m_pUserManagerListWnd->AddNewItem(vecNewInfo);
    m_pBottom->OnForceFirst(nullptr);
    //OnPage(0, PerPageItemDataCount);
}
void CUserManagerWnd::OnDeleteUser(const CString& sUID)
{
    CConfirmDlg dlg;
    auto res = dlg.DoModal();
    if (IDCANCEL == res)
    {
        return;
    }

    HttpHelper httpHelper(theApp.m_spClient);
    string sUser = CW2A(sUID.GetString());
    if (ErrorCode::Failed == httpHelper.DeleteUsers(sUser))
    {
        AfxMessageBox(L"删除失败！");
        return;
    }
    m_pBottom->ForceUpdateThisPage();
}
void CUserManagerWnd::SetVisible(bool bVisible)
{
    __super::SetVisible(bVisible);
    m_pTileWnd->ShowEdit(bVisible);
    m_pBottom->ShowEdit(bVisible);
}

void CUserManagerWnd::SetCurrentPageItems(const string& sContent)
{
    m_pUserManagerListWnd->ClearPageItems();
    auto vecInfos = SplitString(sContent, '|');
    for each (auto item in vecInfos)
    {
        CString sTemp = CString(item.c_str());
        auto vecItem = SplitString(sTemp, L',');
        m_pUserManagerListWnd->AddNewItem(vecItem);
    }
}
void CUserManagerWnd::SetTotalCount(int iTotalCount)
{
    m_pBottom->SetTotalCount(iTotalCount);
}
void CUserManagerWnd::OnPage(int iStart, int iCount)
{
    HttpHelper httpHelper(theApp.m_spClient);
    int iTotalCount = 0;
    string sAllUserInfo;
    string sSearchName = m_pTileWnd->GetSearchName();
    auto eCode = httpHelper.GetAllUserInfo(sSearchName, iStart, iCount, sAllUserInfo, iTotalCount);
    SetTotalCount(iTotalCount);
    SetCurrentPageItems(sAllUserInfo);
}

void CUserManagerWnd::OnSearch(const CString& sUserName)
{
    HttpHelper httpHelper(theApp.m_spClient);
    int iTotalCount = 0;
    string sAllUserInfo;
    string sSearchName = m_pTileWnd->GetSearchName();
    auto eCode = httpHelper.GetAllUserInfo(sSearchName, 0, PerPageItemDataCount, sAllUserInfo, iTotalCount);
    SetTotalCount(iTotalCount);
    SetCurrentPageItems(sAllUserInfo);
}
