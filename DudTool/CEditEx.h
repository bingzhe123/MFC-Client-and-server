#pragma once
#include <afxwin.h> 
#include "CMyWindow.h"
#include <functional>
// CEditEx window
using EndEditFuc = std::function<void(const CString &sText)>;
using ClickFuc = std::function<void(const CString & sText)>;


class CEditEx : public CEdit
{
    // Construction
public:
    CEditEx();

    // Attributes
public:

    // Operations
public:

    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CEditEx)
        //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CEditEx();
    void SetTextColor(COLORREF rgb);
    void SetBackColor(COLORREF rgb);
    virtual BOOL PreTranslateMessage(MSG* pMsg);             
    void SetEditFinishFun(EndEditFuc fun);
    void SetClickFun(ClickFuc fun);

    // Generated message map functions
protected:
    //text and text background colors
    COLORREF m_crText;
    COLORREF m_crBackGnd;
    //background brush
    CBrush m_brBackGnd;
    //{{AFX_MSG(CEditEx)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
    EndEditFuc m_endEidtFuc = nullptr;
    ClickFuc m_clickFun = nullptr;
};                      
