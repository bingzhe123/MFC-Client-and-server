#pragma once

#include "CMyButton.h"
#include "CMyMutiSelectItemWnd.h"

// CSetUserRoleDlg dialog

class CSetUserRoleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetUserRoleDlg)

public:
	CSetUserRoleDlg(vector<RoleInfo>& vecRoleInfo, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSetUserRoleDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ASIGN_ROLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    vector<RoleInfo> GetAllSelectRole();

private:
    CMyButton * m_pBtnOK = nullptr;
    CMyButton * m_pBtnCancel = nullptr;
    CMyMutiSelectItemWnd* m_pMutiComboxWnd = nullptr;
    vector<CMyWindow*> m_vecWnd;
    CMyWindow* m_pCurrentMouseInWnd = nullptr;
    CFont m_font;
    vector<RoleInfo> m_vecRoleInfo;
};
