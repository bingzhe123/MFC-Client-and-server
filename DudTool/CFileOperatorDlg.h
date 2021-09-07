#pragma once

#include "CFileOperatorDlg.h"
#include "CFileOperatorWnd.h"
#include "CDataShowListWnd.h"

// CFileOperatorDlg dialog

class CFileOperatorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileOperatorDlg)

public:
	CFileOperatorDlg(CDataShowrListWnd*pDataShowListWnd, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFileOperatorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE_OPERATOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg LRESULT OnCustomMsgHandler(WPARAM, LPARAM);
    afx_msg void OnClose();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
    void ResizeWnd();
    void SetDataRegisterInfo(const vector<CString>& vecFileInfo);
    void SetDataList(const CString& sFileList);
private:
    CFont m_font;
    CFileOperatorWnd* m_pFileOperatorWnd = nullptr;
    std::map<CMyWindow*, CRect> m_mapDirtyRect;
    vector<CString> m_vecDataRegisterInfo;
    CString m_sFileList;
    CDataShowrListWnd* m_pDataShowListWnd = nullptr;
public:
};
