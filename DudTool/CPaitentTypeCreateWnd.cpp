#include "pch.h"
#include "CPaitentTypeCreateWnd.h"
#include "CommonDef.h"
#include "CNewUserDlg.h"
#include "HttpHelper.h"
#include "DudToolClient.h"
#include "CSetUserRoleDlg.h"
#include "CConfirmDlg.h"
#include "CModifyPatientTypeDlg.h"

CPatientTypeTileWnd::~CPatientTypeTileWnd()
{                                    
}

void CPatientTypeTileWnd::Initialize()
{
    SetBkGroundColor(RGB(25, 31, 40));

    m_pBtnNewType = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnNewType->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    m_pBtnNewType->SetText(CString(L"保存"));
    m_pBtnNewType->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnSave(pWnd);
    });
    m_vecChildWnd.push_back(m_pBtnNewType);

                           

    m_pLabelPatientName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pLabelPatientName->SetMouseHoverSensitive(false);
    m_pLabelPatientName->SetText(L"名称");
    m_vecChildWnd.push_back(m_pLabelPatientName);
}

void CPatientTypeTileWnd::OnSize(int cx, int cy)
{
    CRect rect = GetClientRect();
    int iBtnHeight = rect.Height() - 50;
    int iEditWidth = 100;
    int iOffset = 10;
    int x = iOffset;
    int y = (rect.Height() - iBtnHeight) / 2;
    m_pLabelPatientName->MoveWindow(CPoint(x, y));
    m_pLabelPatientName->SetDimention(iEditWidth, iBtnHeight);

    int iEditHeight = 35;
    int iSearchHeight = iEditHeight;

    CRect r;
    int iTop = (rect.Height() - iEditHeight) / 2;
    x = rect.left  + iEditWidth + iOffset;
    iEditWidth = 300;
    y = rect.top + iTop;
    r.SetRect(x, y, x + iEditWidth, y + iEditHeight);
    m_pEdit->MoveWindow(r);

    x += iEditWidth + iOffset;
    int iBtnWidth = 200;
    m_pBtnNewType->MoveWindow(CPoint(x, y));
    m_pBtnNewType->SetDimention(iBtnWidth, iSearchHeight);
}
void CPatientTypeTileWnd::ShowEdit(bool bEdit)
{
    if (bEdit)
    {
        m_pEdit->ShowWindow(SW_SHOW);
    }
    else
    {
        m_pEdit->ShowWindow(SW_HIDE);
    }
}

void CPatientTypeTileWnd::OnSave(CMyWindow* pWnd)
{
    if (m_funAddNewPatientType != nullptr)
    {                             
        CString sType;
        m_pEdit->GetWindowTextW(sType);
        m_pEdit->SetWindowTextW(L"");
        m_funAddNewPatientType(sType);
    }
}
                  

void CPatientTypeTileWnd::RegisterSave(AddNewInfoFun f)
{
    m_funAddNewPatientType = f;
}

void CPatientTypeTileWnd::SetEdit(CEditEx* pEdit)
{
    m_pEdit = pEdit;
}
void CPatientTypeListWnd::SetBottom(CBottomNavigateWnd* pBottom)
{
    m_pBottom = pBottom;
}

void CPatientTypeListWnd::RegisterModifyPatientType(ModifyPatientTypeFun f)
{
    m_pModifyPatientType = f;
}

void CPatientTypeListWnd::Initialize()
{
    auto rect = GetClientRect();
    m_pHeadWnd = new CMyListHeaderWnd(this, m_pRootWnd, m_refreshImpFun);
    int w = rect.Width();
    vector<CString> vecHeader;
    vecHeader.push_back(L"序号");
    vecHeader.push_back(L"名称");
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


void CPatientTypeListWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    int w = rect.Width();
    int iHeadHeight = 50;
    m_pHeadWnd->MoveWindow(0, 0);
    m_pHeadWnd->SetDimention(w, iHeadHeight);
    m_pHeadWnd->OnSize(cx, cy);

    int iSplitHeight = 2;//每行间距
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (PatientListCount - 1)) / PatientListCount;

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

CMyLable* CPatientTypeListWnd::CreateLable(int x, int y, int w, int h, const CString& sContent)
{
    auto pLabel = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLabel->SetText(sContent);
    pLabel->MoveWindow(x, y);
    pLabel->SetDimention(w, h);
    return pLabel;
}

void CPatientTypeListWnd::AddNewItem(vector<CString>& vecContent)
{
    if (m_mapContent.size() >= PatientListCount)
    {//如果当前这一页已经满了，就不用加了
        return;
    }
    //增加一条信息
    int iHeadHeight = 50;
    auto rect = GetClientRect();
    int iSplitHeight = 2;//每行间距
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (PatientListCount - 1)) / PatientListCount;
    int iOffsety = iHeadHeight + iSplitHeight + (iItemHeight + iSplitHeight) * m_iCurrentPageCount;
    CMyListItemWnd* pItemWnd = new CMyListItemWnd(this, m_pRootWnd, m_refreshImpFun);
    //"select id, name, login_name, manager, email, role, create_time
    vector<CString> vecTemp;
    for (int i = 0; i < 3; i++)
    {
        vecTemp.push_back(vecContent[i]);
    }
    pItemWnd->AddItem(vecTemp, m_pHeadWnd, iOffsety, iItemHeight);
    vector<CMyWindow*> vecButton;

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
    pItemWnd->AddColumnWnd(3, vecButton);

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

void CPatientTypeListWnd::ModifyItem(CMyWindow* pWnd, int iIndex, const CString& sNewContent)
{
    ((CMyListItemWnd*)pWnd)->ModifyColumnText(iIndex, sNewContent);
    pWnd->UpdateMyWnd(pWnd->GetClientRect());
}

void CPatientTypeListWnd::ClearPageItems()
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

void CPatientTypeListWnd::OnSetRole(CMyWindow* pWnd)
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
    for each (auto item in vecRole)
    {
        auto it = std::find_if(vecRoleInfo.begin(), vecRoleInfo.end(), [&item](RoleInfo& roleInfo)
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
void CPatientTypeListWnd::OnEdit(CMyWindow* pWnd)
{
    auto pItemWnd = dynamic_cast<CMyListItemWnd*>(pWnd->GetParent());
    assert(pItemWnd != nullptr);
    auto vecUserInfo = m_mapContent[pItemWnd];
    auto sType = vecUserInfo[1];
    auto sID = vecUserInfo[3];
    if (m_pModifyPatientType != nullptr)
    {
        m_pModifyPatientType(sID, sType);
    }


    /*auto pItemWnd = dynamic_cast<CMyListItemWnd*>(pWnd->GetParent());
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
    ModifyItem(pItemWnd, 2, vecNewInfo[2]);*/
}
void CPatientTypeListWnd::OnDelete(CMyWindow* pWnd)
{
    auto pItemWnd = dynamic_cast<CMyListItemWnd*>(pWnd->GetParent());
    assert(pItemWnd != nullptr);
    if (!theApp.m_bIsManager)
    {
        AfxMessageBox(L"没有权限删除!");
        return;
    }
    CConfirmDlg dlg;
    if (dlg.DoModal() == IDCANCEL)
    {
        return;
    }
    auto vecUserInfo = m_mapContent[pItemWnd];
    
    m_pDeleteFun(vecUserInfo[3]);
}

void CPatientTypeListWnd::RegisterDeleteUserFun(DeleteUserFun f)
{
    m_pDeleteFun = f;
}

CPatientTypeWnd::~CPatientTypeWnd()
{
}

void CPatientTypeWnd::Initialize()
{
    m_pTileWnd = new CPatientTypeTileWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pTileWnd->Initialize();
    m_pTileWnd->RegisterSave([this](CString &sType)
    {
        this->OnAddNewPatientType(sType);
    });
    m_vecChildWnd.push_back(m_pTileWnd);

    m_pPatientTypeListWnd = new CPatientTypeListWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pPatientTypeListWnd->Initialize();
    m_pPatientTypeListWnd->RegisterDeleteUserFun([this](const CString& sUser)
    {
        this->OnDelete(sUser);
    });
    m_pPatientTypeListWnd->RegisterModifyPatientType([this](const CString& sID, const CString& sNewType)
    {
        this->MoidfyPatientType(sID, sNewType);
    });
    m_vecChildWnd.push_back(m_pPatientTypeListWnd);

    m_pBottom = new CBottomNavigateWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pBottom->RegisterOnpageFunction([this](int iStart, int iCount)
    {
        this->OnPage(iStart, iCount);
    });
    m_pBottom->Initialize(1);
    m_pBottom->SetMaxItemCount(PatientListCount);
    m_vecChildWnd.push_back(m_pBottom);
    m_pPatientTypeListWnd->SetBottom(m_pBottom);

}

void CPatientTypeWnd::OnSize(int cx, int cy)
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
    m_pPatientTypeListWnd->MoveWindow(iOffsetx, iOffsety);
    m_pPatientTypeListWnd->SetDimention(w, iContentHeight);
    m_pPatientTypeListWnd->OnSize(cx, cy);
    iOffsety += iContentHeight;
    m_pBottom->MoveWindow(iOffsetx, iOffsety);
    m_pBottom->SetDimention(w, iBottomHeight - iBottomOffset);
    m_pBottom->ShowEdit(true);
    m_pBottom->OnSize(cx, cy);
}
void CPatientTypeWnd::OnAddNewPatientType(CString &sType)
{
    HttpHelper httpHelper(theApp.m_spClient);
    string sNewInfo = CW2A(sType.GetString());
    auto erroCode = httpHelper.AddNewPatientType(sNewInfo);
    if (erroCode == ErrorCode::Failed)
    {
        AfxMessageBox(L"操作失败!");
        return;
    }
    if (erroCode == ErrorCode::Repeat)
    {
        AfxMessageBox(L"字段重复!");
        return;
    }
    string sAllInfo;
    auto errorCode = httpHelper.GetPatientType(sAllInfo);
    auto vecInfos = SplitString(sAllInfo, '|');
    SetTotalCount(vecInfos.size());
    SetAllItems(sAllInfo);
    OnPage(0, PatientListCount);
}
void CPatientTypeWnd::OnDelete(const CString& sUID)
{
    HttpHelper httpHelper(theApp.m_spClient);
    string sid= CW2A(sUID.GetString());
    if (ErrorCode::Failed == httpHelper.DeletePatientType(sid))
    {
        AfxMessageBox(L"删除失败！");
        return;
    }
    string sAllInfo;
    auto errorCode = httpHelper.GetPatientType(sAllInfo);
    auto vecInfos = SplitString(sAllInfo, '|');
    SetTotalCount(vecInfos.size());
    SetAllItems(sAllInfo);
    OnPage(0, PatientListCount);
}
void CPatientTypeWnd::SetVisible(bool bVisible)
{
    __super::SetVisible(bVisible);
    m_pTileWnd->ShowEdit(bVisible);
    m_pBottom->ShowEdit(bVisible);
}

void CPatientTypeWnd::SetAllItems(const string& sContent)
{
    m_vecInfo.clear();
    m_pPatientTypeListWnd->ClearPageItems();
    CString sTemp = CString(sContent.c_str());

    auto vecInfos = SplitString(sTemp, L'|');
    int size = vecInfos.size();
    for (size_t i = 0; i < vecInfos.size(); i++)
    {
        auto vecTemp = SplitString(vecInfos[i], L',');
        vecTemp.push_back(vecTemp[0]);//ID放到最后
        vecTemp[0] = CString(std::to_wstring(size--).c_str());
        m_vecInfo.push_back(vecTemp);
    }
    OnPage(0, PatientListCount);
}
void CPatientTypeWnd::SetTotalCount(int iTotalCount)
{
    m_pBottom->SetTotalCount(iTotalCount);
}
void CPatientTypeWnd::OnPage(int iStart, int iCount)
{     
    m_pPatientTypeListWnd->ClearPageItems();
    for (int i = iStart; (i < iCount + iStart) && i < m_vecInfo.size(); i++)
    {
        m_pPatientTypeListWnd->AddNewItem(m_vecInfo[i]);
    }
}                            
void CPatientTypeWnd::SetEdit(CEditEx* pEdit)
{
    m_pTileWnd->SetEdit(pEdit);
}

CEditEx* CPatientTypeWnd::GetBottomEidt()
{
    return m_pBottom->GetEidt();
}

void CPatientTypeWnd::MoidfyPatientType(const CString& sID, const CString& sType)
{
    CModifyPatientTypeDlg dlg(sType, sID);
    auto res = dlg.DoModal();
    if (IDCANCEL == res)
    {
        return;
    }
    auto sNewType = dlg.GetNewType();
    for (size_t i = 0; i < m_vecInfo.size(); i++)
    {
        bool bFind = false;
        for each (auto &temp in m_vecInfo[i])
        {
            if (temp == sID)
            {
                bFind = true;
                break;
            }
        }
        if (bFind)
        {
            m_vecInfo[i][1] = sNewType;
            break;
        }

    }


    m_pBottom->ForceUpdateThisPage();
}
