#pragma once


#include "CEditEx.h"
#include "CMyButton.h"
#include <vector>
using std::vector;
// CEditRoleDlg dialog

class CEditRoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditRoleDlg)
     
public:         
	CEditRoleDlg(const vector<CString> &vecRoleInfo, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEditRoleDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROLE_EDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    void GetNewInfo(CString& sRoleDes, CString& sRoleName);
private:
    CEditEx m_editRoleDescription;
    CEditEx m_editRoleName;
    CMyButton * m_pBtnOK;
    CMyButton * m_pBtnCancel;
    CFont m_font;
    vector<CString> m_vecRoleInfo;//  m_sRoleUID;//数据控对应的本条信息的id
    CString m_sRoleDes;
    CString m_sRoleName;
    vector<CEdit*> m_vecEdit;
};
