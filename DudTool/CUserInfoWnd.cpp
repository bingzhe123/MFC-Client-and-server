#include "pch.h"
#include "CUserInfoWnd.h"

#include "CChangePasswordDlg.h"
#include "HttpHelper.h"
#include "DudToolClient.h"

void CUserInfoWnd::Initialize()
{
    SetBkGroundColor(RGB(25, 31, 40));
    CMyLable* pName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pName->SetText(L"姓名:");
    pName->SetMouseHoverSensitive(false);
    pName->SetAlignMode(Align::right);
    int iTextWidth = 300;
    int h = 50;
    int iOffsest2Left = 100;
    int spliterHeight = 10;
    int spliterWidth = 10;
    int offsetx = iOffsest2Left;
    int offsety = 50;
    pName->MoveWindow(offsetx, offsety);
    pName->SetDimention(iTextWidth, h);
    m_vecChildWnd.push_back(pName);

    m_pLableName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pLableName->SetText(L"");
    m_pLableName->SetMouseHoverSensitive(false);
    m_pLableName->SetSelectState(true);//高亮显示
    m_pLableName->SetAlignMode(Align::left);
    offsetx += iTextWidth + spliterWidth;
    m_pLableName->MoveWindow(offsetx, offsety);
    m_pLableName->SetDimention(iTextWidth, h);
    m_vecChildWnd.push_back(m_pLableName);

    auto pLoginName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLoginName->SetText(L"登录名:");
    pLoginName->SetMouseHoverSensitive(false);
    pLoginName->SetAlignMode(Align::right);
    offsetx = iOffsest2Left;
    offsety += spliterHeight + h;
    pLoginName->MoveWindow(offsetx, offsety);
    pLoginName->SetDimention(iTextWidth, h);
    m_vecChildWnd.push_back(pLoginName);

    m_pLableLoginName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pLableLoginName->SetText(L"");
    m_pLableLoginName->SetMouseHoverSensitive(false);
    m_pLableLoginName->SetSelectState(true);//高亮显示
    m_pLableLoginName->SetAlignMode(Align::left);
    offsetx += iTextWidth + spliterWidth;
    m_pLableLoginName->MoveWindow(offsetx, offsety);
    m_pLableLoginName->SetDimention(iTextWidth, h);
    m_vecChildWnd.push_back(m_pLableLoginName);

    auto pRoleName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pRoleName->SetText(L"角色:");
    pRoleName->SetMouseHoverSensitive(false);
    pRoleName->SetAlignMode(Align::right);
    offsetx = iOffsest2Left;
    offsety += spliterHeight + h;
    pRoleName->MoveWindow(offsetx, offsety);
    pRoleName->SetDimention(iTextWidth, h);
    m_vecChildWnd.push_back(pRoleName);

    m_pRoleName = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pRoleName->SetText(L"");
    m_pRoleName->SetMouseHoverSensitive(false);
    m_pRoleName->SetSelectState(true);//高亮显示
    m_pRoleName->SetAlignMode(Align::left);
    offsetx += iTextWidth + spliterWidth;
    m_pRoleName->MoveWindow(offsetx, offsety);
    m_pRoleName->SetDimention(iTextWidth, h);
    m_vecChildWnd.push_back(m_pRoleName);


    m_pBtnModifyCode = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnModifyCode->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    m_pBtnModifyCode->SetText(CString(L"修改密码"));
    m_pBtnModifyCode->RegisterClickFun([this](CMyWindow*pWnd)
    {
        this->OnChangePassword(pWnd);
    });
    offsetx = iOffsest2Left;
    offsety += spliterHeight + h;
    m_pBtnModifyCode->MoveWindow(offsetx, offsety);
    m_pBtnModifyCode->SetDimention(iTextWidth, h);
    m_vecChildWnd.push_back(m_pBtnModifyCode);
}
void CUserInfoWnd::OnSize(int cx, int cy)
{
    if (m_pBtnModifyCode == nullptr)
    {
        return;
    }
    int h = 300;
    auto rect = m_pBtnModifyCode->GetMyRect();
    int x = (cx - rect.Width()) / 2;
    int y = rect.top;
    m_pBtnModifyCode->MoveWindow(x, y);
    this->SetDimention(cx, h);
}

void CUserInfoWnd::SetUserInfo(const CString& sName, const CString& sLoginName, const CString& sRole)
{
    m_pLableName->SetText(sName);
    m_pLableLoginName->SetText(sLoginName);
    m_pRoleName->SetText(sRole);
}

void CUserInfoWnd::OnChangePassword(CMyWindow *pWnd)
{
    CChangePasswordDlg dlg;
    auto res = dlg.DoModal();
    if (res == IDCANCEL)
    {
        return; 
    }
    CString sOld, sNew;
    dlg.GetNewPasswordInfo(sOld, sNew);
    HttpHelper httHelper(theApp.m_spClient);
    string _sOld = CW2A(sOld.GetString());
    string _sNew = CW2A(sNew.GetString());
    auto code = httHelper.ModifyUserPassword(theApp.m_sUserID, _sOld, _sNew);
    if (code == ErrorCode::Failed)
    {
        AfxMessageBox(L"操作失败！");
    }
    else if (code == ErrorCode::OldCodeError)
    {
        AfxMessageBox(L"原密码错误！");
    }
}

