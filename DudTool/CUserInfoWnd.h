#pragma once
#include "CMyContainerWnd.h"
#include "CMyLable.h"
#include "CMyButton.h"
class CUserInfoWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual void OnSize(int cx, int cy);
    void Initialize();
    void SetUserInfo(const CString& sName, const CString& sLoginName, const CString& sRole);
    void OnChangePassword(CMyWindow *pWnd);
private:
    CMyLable *m_pLableName;
    CMyLable* m_pLableLoginName;
    CMyLable* m_pRoleName;
    CMyButton * m_pBtnModifyCode = nullptr;
};

