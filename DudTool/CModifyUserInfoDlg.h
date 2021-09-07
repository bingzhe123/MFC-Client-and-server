#pragma once

#include "CEditEx.h"
#include "CMyButton.h"
#include "CommonDef.h"
#include <vector>
using std::vector;
// CModifyUserInfoDlg dialog

class CModifyUserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyUserInfoDlg)

public:
	CModifyUserInfoDlg(vector<CString>& vecUserInfo, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CModifyUserInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODIFY_USERINFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnEnChangeEditModifyuserinfoUsername();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    NewUserInfo GetNewUserInfo();
private:
    CMyButton * m_pBtnOK;
    CMyButton * m_pBtnCancel;
    CFont m_font;
    vector<CString> m_vecOriUserInfo;
public:
    CEditEx m_editUserName;
    CEditEx m_editLoginName;
    CEditEx m_editPassword;
    CEditEx m_editConformPassword;
    CEditEx m_editEmail;
    NewUserInfo m_newUserInfo;
    vector<CEdit*> m_vecEdit;
};
