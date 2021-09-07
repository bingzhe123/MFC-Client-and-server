#pragma once


#include "CEditEx.h"
#include "CMyButton.h"
#include "CommonDef.h"
#include <vector>
using std::vector;
// CChangePasswordDlg dialog

class CChangePasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangePasswordDlg)

public:
	CChangePasswordDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CChangePasswordDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_PASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    void GetNewPasswordInfo(CString &sOld, CString &sNew );
private:
    CEditEx m_editOld;
    CEditEx m_editNew;
    CEditEx m_editConfirm;
    CMyButton * m_pBtnOK;
    CMyButton * m_pBtnCancel;
    CFont m_font;
    CString m_oldPassword;
    CString m_newPassword;
    CString m_confrimPassword;
    vector<CEdit*> m_vecEdit;

};
