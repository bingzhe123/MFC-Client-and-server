#pragma once

#include "CEditEx.h"
#include "CMyButton.h"
#include <vector>
using std::vector;

// CModifyPatientTypeDlg dialog

class CModifyPatientTypeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyPatientTypeDlg)

public:
	CModifyPatientTypeDlg(const CString &sPatientType,const CString &SID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CModifyPatientTypeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ModifyPatientType };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    CString GetNewType();
private:
    CEditEx m_editPatientType;
    CMyButton* m_pBtnOK;
    CMyButton* m_pBtnCancel;
    CFont m_font;      
    CString m_sPatinietType;
    CString m_sID;
    vector<CEdit*> m_vecEdit;
public:
    virtual BOOL OnInitDialog();
    virtual void OnOK();
};

