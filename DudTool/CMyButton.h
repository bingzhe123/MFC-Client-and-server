#pragma once
#include "CMyWindow.h"
#include <memory>
class CMyButton :
    public CMyWindow
{
public:
    CMyButton(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    virtual ~CMyButton();
    virtual bool Refresh(CDC* pMemDC , CRect rect);
    virtual bool OnMouseMove(UINT nFlags, CPoint point);
    void SetText(const CString& sText);
    CString GetText();
    void SetIconPic(UINT nResNormalID, UINT nResSelectID);
private:
    CImage* m_pNormalIcon = nullptr;
    CImage* m_pSelectIcon = nullptr;;
    CString m_text;
};

