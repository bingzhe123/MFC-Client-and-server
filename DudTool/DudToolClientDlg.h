
// MFCApplication2Dlg.h : header file
//
#include "CMyWindow.h"
#include <map>
#include "CommonDef.h"
using std::map;
#pragma once      

// CMFCApplication2Dlg dialog
class DudToolClientDlg : public CDialogEx
{
// Construction
public:
	DudToolClientDlg(CWnd* pParent = nullptr);	// standard constructor

    ~DudToolClientDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnNcMButtonUp(UINT nHitTest, CPoint point);
    afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnNcMouseLeave();
    afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
    afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
    afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
    void SetDirtyRect(CMyWindow* pWnd, CRect rect);
    //当窗口被析构的还是要删除
    void RemoveDirtyRect(CMyWindow* pWnd);
    void RefreshFun(CMyWindow* pWnd, CRect);
    void OnDataInfo(CMyWindow* pWnd);
    void OnHome(CMyWindow* pWnd);
    void OnUserManager(CMyWindow* pWnd);
    void OnRoleManager(CMyWindow* pWnd);
    void On3DDataInfo(CMyWindow* pWnd);
    void OnAriginData(CMyWindow* pWnd);
    void OnUserInfo(CMyWindow* pWnd);
    void OnQuit(CMyWindow* pWnd);
    void ShowDataWindow(DataCategory eDataCategory);


    void ForceUpdate();
    void ShowHomeWnds(bool bShow);
    void ResizeWnd();
private:
   // CList<CMyWindow*> m_wndList;
    map<WndID, CMyWindow*> m_mapWndList;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);

    bool m_bForceRefresh = true;
    std::map<CMyWindow*, CRect> m_mapDirtyRect;

    CMyWindow *m_pCurrentMouseInWnd = nullptr;
    CMyWindow* m_pCurrentDisplayWnd = nullptr;
    bool m_bSmallFont = false;

};
