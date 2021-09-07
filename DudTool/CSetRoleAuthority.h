#pragma once
#include "CMyCheckedExpandWnd.h"
#include "CMyCheckWndGroup.h"
#include "CommonDef.h"
 
class CSetRoleAuthority : public CDialogEx
{
	DECLARE_DYNAMIC(CSetRoleAuthority)

public:
	CSetRoleAuthority(CString &sRoleName, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSetRoleAuthority();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SET_ROLE_AUTHORITY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnMove(int x, int y);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    virtual void OnOK();
    void Resize();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    void SetAuthority(UINT dataAuthority,
        UINT systemAuthority,
        UINT userCenterAuthority,
        UINT stasticsAuthority);
    void GetAuthority(UINT& dataAuthority,
        UINT& systemAuthority,
        UINT& userCenterAuthority,
        UINT& stasticsAuthority);
private:
    CString m_sCaption;
    map<int, CMyWindow*> m_mapWndList;

    CExpandGroup* m_pExpandGroup;
    CMyWindow* m_pCurrentMouseInWnd = nullptr;
    UINT m_dataAuthority;
    UINT m_systemAuthority;
    UINT m_userCenterAuthority;
    UINT m_stasticsAuthority;


public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
