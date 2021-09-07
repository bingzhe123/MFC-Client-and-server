#pragma once

#include "CEditEx.h"
#include "CMyButton.h"
 
// CNewRoleDlg dialog

class CNewRoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewRoleDlg)

public:
	CNewRoleDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CNewRoleDlg();
    virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NEW_ROLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    vector<string> GetNewItem();
private:
    CEditEx m_editRoleDescription;
    CEditEx m_editRoleName;    
    CMyButton * m_pBtnOK;
    CMyButton * m_pBtnCancel;
    CFont m_font;
    vector<string> m_vecNewItem;//新加入的一行内容
    vector<CEdit*> m_vecEdit;
};
