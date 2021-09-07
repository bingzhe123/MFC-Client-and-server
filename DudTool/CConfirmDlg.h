#pragma once

#include "CMyButton.h"
#include "CommonDef.h"
// CConfirmDlg dialog

class CConfirmDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfirmDlg)

public:
	CConfirmDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CConfirmDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONFIRM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);

private:
    CImage* m_pImageQuestion = nullptr;

    CMyButton * m_pBtnOK;
    CMyButton * m_pBtnCancel;
    CFont m_font;
};
