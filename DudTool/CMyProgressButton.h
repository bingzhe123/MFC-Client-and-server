#pragma once
#include "CMyContainerWnd.h"
#include "CMyButton.h"
#include "CMyprogressBar.h"
class CMyProgressButton :
    public CMyContainerWnd
{
public:
    CMyProgressButton(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual void OnSize(int cx, int cy);
    virtual void SetDimention(int w, int h);
    void SetText(const CString& sText);
    void RegisterClickFun(ClickFunc fun);
    void SetProgressPos(int iPos);
    CMyprogressBar* GetProgressBar();
    virtual  void SetMouseHoverSensitive(bool bMouseHoverSensitive);
private:
    CMyButton * m_pButton = nullptr;
    CMyprogressBar* m_pProgressBar = nullptr;
    int m_iProgressHeight = 20;

};

