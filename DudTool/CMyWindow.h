#pragma once
#include "pch.h"
#include <functional>
#include "CMyCheckWndGroup.h"

class CMyWindow;
using RefreshImplFunc = std::function<void(CMyWindow * pWnd, CRect rect)>;
using ClickFunc = std::function<void(CMyWindow * pWnd)>;

class CMyWindow
{

public:
    CMyWindow( CMyWindow *pParent, CWnd *pRootWnd, RefreshImplFunc fun);
    void SetParent(CMyWindow* pParent);
    virtual ~CMyWindow();
    virtual bool Refresh(CDC* pMemDC , CRect rect);


    virtual void OnMouseLeave();
    virtual bool OnMouseMove(UINT nFlags, CPoint point);

    virtual void OnMouseIn(UINT nFlags, CPoint point);
    virtual void OnMouseHover(UINT nFlags, CPoint point);
    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void SetSelectState(bool bSelect);
    virtual void SetSelectStateFromChild(bool bSelect);
    virtual CRect GetClientRect(); //容器查看要包含子窗口的区域
    virtual CRect GetDirtyRect(); //
    virtual bool IsMouseIn();
    virtual CMyWindow* HitTest(CPoint pt);
    virtual void SetDirty(bool isDirty);
    virtual void ForceUpdate();
    virtual void SetVisible(bool bVisible);
    virtual void OnSize(int cx, int cy);
    virtual void SetDimention(int w, int h);
    virtual void SetBkPic(UINT nResID);
    virtual void SetBkPic(UINT nResNormalID, UINT nResSelectID);
    virtual int GetMaxlayer(CPoint pt);
    void SetExtendDrawBK(bool bExtend);//是否是扩张画
    virtual void SetLayer(int iLayer) final;
    int GetLayer();

     bool IsRectIntersect(CRect r1, CRect r2);
    void SetFontHeight(int iHeight);
    void MoveWindow(const CPoint& pt);
    void MoveWindow(int x, int y);
    void SetBkGroundColor(COLORREF rgb);
    void SetBkGroundColor(COLORREF rgbNormal, COLORREF rgbSelect);
    void SetTextColor(COLORREF rgbNormal, COLORREF rgbSelect);
    void SetName(const CString& sName);
    void SetSupportClick(bool bCanClick);

    static BOOL LoadImageFromResource(CImage* pImage, UINT nResID, LPCTSTR lpTyp);
    CRect GetMyRect();
    CSize GetBkPicSize();
    CMyWindow* GetParent();
    bool IsSetBkGroundColor();
    bool IsSelect();
    bool IsVisible();
    bool IsDirty();
    void UpdateMyWnd(CRect rect);

    //设置鼠标移动上去是否会高亮
    virtual  void SetMouseHoverSensitive(bool bMouseHoverSensitive);
    void SetGroupManager(CGroupManager* pGroupManager);
    void RegisterClickFun(ClickFunc fun);

protected:
    CMyWindow* m_pParent;
    CWnd* m_pRootWnd;
    CRect  m_rect;
    CString m_sName;
    bool   m_isMouseIn = false;
    CImage* m_pBkImage = nullptr;
    CImage* m_pBkSelectedImage = nullptr;

    int m_iLayer = 0;//当前窗口所在层

    bool m_bSetBkGroundColor = false;
    bool m_bSetHigthLightBkGroundColor = false;
    bool m_bVisible = true;
    bool m_bIsDirty = true;
    bool m_bSelected = false;
    bool m_bMouseHoverSensitive = false;
    bool m_bSupportClick = true;
    bool m_bExtendBk = false;

    CFont m_font;
    CFont m_EditBoxfont;

    COLORREF m_bkColor;
    COLORREF m_bkHigthLightColor;
    COLORREF m_textHightLigtColor;
    COLORREF m_textNormalColor;
    CGroupManager* m_pGroupManager = nullptr;//窗口互斥管理
    RefreshImplFunc m_refreshImpFun = nullptr;
    ClickFunc m_clickFunc = nullptr;

};

