#pragma once
#include "CMyWindow.h"
#include <vector>
#include "CMyCheckWndGroup.h"
#include "CMyContainerWnd.h"
#include "CMyScrollbarWnd.h"
using std::vector;
class CMyExapndHeaderWnd :
    public CMyWindow
{
public:
    CMyExapndHeaderWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    ~CMyExapndHeaderWnd();
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    //check,uncheck��ͼƬ
    void SetIconPic(UINT nResNormalID, UINT nResSelectID);
    //չ�������ļ�ͷ���
    void SetExpandMark(UINT nResExpandID, UINT nResUnexpandID);
    CSize GetBkPicSize();
    void SetText(const CString& sText);
    CString GetText();
    void SetExpand(bool bExpand);

protected:
    bool m_bExpand = false;
    CString m_text;
    CImage* m_pNormalIcon = nullptr;
    CImage* m_pSelectIcon = nullptr;

    CImage* m_pExpandIcon = nullptr;
    CImage* m_pUnexpandIcon = nullptr;
};

class CMyExpandDisplayWnd:public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    ~CMyExpandDisplayWnd();
    virtual void OnSize(int cx, int cy);

    virtual void AddChildWnd(CMyWindow* pChildWnd, int iHeight, int iSpaceY = 2);
    virtual void SetSelectStateFromChild(bool bSelect);
    void SetFixedHeight(int iHeight);
    CSize GetDisplayWndSize();
    bool HasDisplayItem();
private:
    int m_iContentHeight = 0;
    int m_iFixedHeight = 0;
    int m_iSpaceY = 2;
    int m_iItemHeight = 0;
    CMyScrollbarWnd* m_pScrollBar = nullptr;
};



class CMyExpandWnd :
    public CMyContainerWnd
{
public:
    CMyExpandWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun, CGroupManager *pGroupManager);
    virtual ~CMyExpandWnd();
    virtual bool Refresh(CDC* pMemDC , CRect rect);
    virtual void OnSize(int cx, int cy);

    virtual bool OnLButtonDown(UINT nFlags, CPoint point);
    virtual void SetSelectState(bool bSelect);
    virtual void SetSelectStateFromChild(bool bSelect);
    virtual CRect GetDirtyRect(); //
    virtual CMyWindow* HitTest(CPoint pt);
    virtual void OnMouseLeave();
    virtual void OnMouseIn(UINT nFlags, CPoint point);
    virtual bool OnMouseMove(UINT nFlags, CPoint point);

    virtual void ForceUpdate();
    //�Ӵ���������ڵ�ǰ�������Ͻǵ�ƫ������������Ҫ����˳�����
    virtual void AddChildWnd(CMyWindow* pChildWnd, int iHeight, int iSpaceY = 2);
    virtual void SetBkPic(UINT nResNormalID, UINT nResSelectID);


    void SetText(const CString& sText);
    CString GetText();
    //check,uncheck��ͼƬ
    void SetIconPic(UINT nResNormalID, UINT nResSelectID);
    //չ�������ļ�ͷ���
    void SetExpandMark(UINT nResExpandID, UINT nResUnexpandID);
    void SetBkGroundColor(COLORREF rgb);
    void SetIsExpand(bool bIsExpand);
    bool IsExpand();
    void OnClickTilte();
    void ExpandChild();
    void UnexpandChild();
    CSize GetDisplayWndSize();

    vector<CMyWindow*> GetChildWnds();
    CRect GetDisplayRect();
protected:
    bool m_bExpand = false;
    CRect m_dirtyRect;
    CGroupManager* m_pChildGroupManager;//�Ӵ��ڵĻ������
    CMyExapndHeaderWnd* m_pHeaderWnd = nullptr;
    CMyExpandDisplayWnd* m_pDisplayWnd = nullptr;
};

