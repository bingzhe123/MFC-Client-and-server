#pragma once

#include "CEditEx.h"
#include "CMyButton.h"
#include "CommonDef.h"

// CNewUserDlg dialog
class CNewUserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewUserDlg)

public:
	CNewUserDlg(CWnd* pParent = nullptr);   // standard constructor
    CNewUserDlg(const NewUserInfo& newInfo, CWnd* pParent = nullptr);
	virtual ~CNewUserDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_NEWUSER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    NewUserInfo GetNewUserInfo();
private:
    CEditEx m_editUserName;
    CEditEx m_editLoginName;
    CEditEx m_editPassword;
    CEditEx m_editConfirmPassword;
    CEditEx m_editEmail;

    CMyButton * m_pBtnOK;
    CMyButton * m_pBtnCancel;
    CFont m_font;
    NewUserInfo m_newUserInfo; 
    vector<CEdit*> m_vecEdit;
    CImage* m_pImageStar = nullptr;

};
