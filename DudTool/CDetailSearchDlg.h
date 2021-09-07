#pragma once


// CDetailSearchDlg dialog

class CDetailSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetailSearchDlg)

public:
	CDetailSearchDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDetailSearchDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DETAIL_SEARCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
