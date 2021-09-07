#pragma once
#include "CMyContainerWnd.h"
using ScrollWndFunc = std::function<void(float fScrollPos)>;
class CMyScrollbarWnd :
    public CMyContainerWnd
{
public:
    CMyScrollbarWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    ~CMyScrollbarWnd();
    virtual void OnSize(int cx, int cy);
    virtual bool OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnLButtonUp(UINT nFlags, CPoint point);
    void SetScrollHeight(int iHeight);
    void RegisterScrollWndFun(ScrollWndFunc f);
    void OnSliderMouseMove(CPoint pt);
private:
    //滚动条滑块
    CMyWindow* m_pSliderWnd = nullptr;
    int m_iScrollHeight = 0; //整个窗口的高度，需要滚动的高度
    int m_iCurrentPos = 0;
    int m_iPerScrollHeight = 10; //每次滚动的距离 像素
    int m_iSliderHeight = 0;//滑块的高度
    ScrollWndFunc m_pScrollWndFunc = nullptr;
    bool m_bMouseDown = false;
    CPoint m_ptMousePos;
};

