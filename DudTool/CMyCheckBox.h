#pragma once
#include "CMyWindow.h"

using CheckChangedFunc = std::function<void(CMyWindow*)>;

class CMyCheckBox :
    public CMyWindow
{
public:
    CMyCheckBox(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    virtual ~CMyCheckBox();
    void SetText(const CString& sText);
    //check,uncheckµÄÍ¼Æ¬
    void SetIconPic(UINT nResNormalID, UINT nResSelectID);
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    bool Refresh(CDC* pMemDC, CPoint ptOffset);

    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    void SetChecked(bool bCheck);
    void RegisterCheckChangedFunc(CheckChangedFunc f);
private:
    CString m_text;
    CPoint m_ptCheckBoxPos;
    CImage* m_pNormalIcon = nullptr;
    CImage* m_pSelectIcon = nullptr;
    CheckChangedFunc m_pChangedFunc = nullptr;
};

