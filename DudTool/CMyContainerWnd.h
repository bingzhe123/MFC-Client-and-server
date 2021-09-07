#pragma once
#include "CMyWindow.h"
#include <vector>   
#include "CEditEx.h"

class IWidthEditWnd
{
public:
    virtual vector<CEdit*> GetEditCtrl() { return vector<CEdit*>(); };
};

using std::vector;
class CMyContainerWnd: public CMyWindow, public IWidthEditWnd
{
public:
    using CMyWindow::CMyWindow;
    virtual ~CMyContainerWnd();
    virtual bool Refresh(CDC* pMemDC , CRect rect);
    virtual void OnMouseLeave();
    virtual bool OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual void SetDirty(bool isDirty);
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void SetVisible(bool bVisible);
    virtual void OnSize(int cx, int cy);
    virtual int GetMaxlayerIncludePt(CPoint pt);
    //ptOffset::����ڵ�ǰλ�õ�ƫ����
    void AddChildWnd(CMyWindow *pWnd,const CPoint &ptOffset);  
    vector<CMyWindow*>   GetChildWnds();
    CMyWindow* GetTopMouseInWnd(CPoint point);
    virtual vector<CEdit*> GetEditCtrl();

protected:
    vector<CMyWindow*> m_vecChildWnd;
    CMyWindow* m_pCurrentMouseInWnd = nullptr;//��ǰ������е�����
};

