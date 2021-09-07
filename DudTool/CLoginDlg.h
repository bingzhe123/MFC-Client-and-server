#pragma once
#include "CMyWindow.h"
#include <map>
#include <vector>
#include "CEditEx.h"
#include "CMyButton.h"
using std::map;
using std::vector;

// CLoginDlg dialog

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Login };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void SetDirtyRect(CMyWindow* pWnd, CRect rect);


public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void Resize();
    void Onlogin(CMyWindow *pWnd);
    bool VerifyUser(const CString& sUser, const CString& sPassWord);
private:
    CMyWindow* m_pBkWnd;
    CMyWindow* m_pBottom;
    CImage* m_pBkImage;
    CImage* m_pBottomImage;
    CImage* m_pLoginWndImage;
    CFont m_font;
    CEditEx m_editUserName;
    CEditEx m_editCode;
    CMyButton * m_pBtnLogin = nullptr;
    std::map<CMyWindow*, CRect> m_mapDirtyRect;
    bool m_bInit = false;
    vector<CEdit*> m_vecEdit;

public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnOK();
};
