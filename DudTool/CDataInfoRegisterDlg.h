#pragma once

#include "CMyWindow.h"
#include <map>
#include "CEditEx.h"
#include "CMyComboxWnd.h"
using std::map;

// CDataInfoRegisterDlg dialog

class CDataInfoRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDataInfoRegisterDlg)

public:
	CDataInfoRegisterDlg(vector<CString> &vecItems, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDataInfoRegisterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER_DATA_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void OnClickOk(CMyWindow* pWnd);
    void OnClickCancel(CMyWindow* pWnd);
    vector<CString> GetItems();
    void SetItems(vector<CString>& vecItems);
private:
    CFont m_font;
    map<int, CMyWindow*> m_mapWndList;
    CMyWindow* m_pCurrentMouseInWnd = nullptr;
    CEditEx m_editName;
    CEditEx m_editAge;
    //CEditEx m_editType;
    CEditEx m_editHospital;
    CEditEx m_editDescription;
    CMyComboxWnd* m_pGender;
    CMyComboxWnd* m_pType;


    vector<CString> m_vecItems;
    vector<CEdit*> m_vecEdit;
    vector<CString> m_TypeID;


public:
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
