#pragma once

#include "CEditEx.h"
#include "CMyButton.h"
#include "CommonDef.h"
#include "CPaitentTypeCreateWnd.h"
#include <share.h>
// CNewPatientTypeDlg dialog
         
class CNewPatientTypeDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CNewPatientTypeDlg)

public:
    CNewPatientTypeDlg(CWnd* pParent = nullptr);   // standard constructor
    virtual ~CNewPatientTypeDlg();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_ADD_PATIENT_TYPE };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual BOOL OnInitDialog();
    void RefreshFun(CMyWindow* pWnd, CRect rect);
private:
    CPatientTypeWnd *m_pPatientTypeWnd = nullptr;
    std::map<CMyWindow*, CRect> m_mapDirtyRect;
    CEditEx m_editType;
    CFont m_font;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    
    virtual void OnOK();
};