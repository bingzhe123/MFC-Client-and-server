#pragma once
#include "CMyWindow.h"

enum class Align
{
    left,
    right,
    center,
};
class CMyLable :
    public CMyWindow
{
public:
    CMyLable(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    virtual ~CMyLable();
    void SetText(const CString &text, bool showStarMarker = false);
    CString GetText();
    void SetAlignMode(Align align);
    virtual bool OnMouseMove(UINT nFlags, CPoint point);
    virtual bool Refresh(CDC* pMemDC , CRect rect);
private:
    CString m_text;
    Align m_alignMode = Align::center;
    CImage* m_pImageStar = nullptr;
    bool m_bShowStarMarker = false;
};

class CComboxItem :public CMyLable
{
public:
    using CMyLable::CMyLable;
public:
    int m_iIndex = 0;
};