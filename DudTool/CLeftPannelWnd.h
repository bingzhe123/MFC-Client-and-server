#pragma once
#include "CMyContainerWnd.h"
#include <map>
#include "CommonDef.h"
#include "CMyExpandWnd.h"
using std::map;
class CLeftPannelWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual ~CLeftPannelWnd();  
    void Initialize();
    virtual  void ForceUpdate();
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual void OnSize(int cx, int cy);

    void RegisterClickFun(LeftPannelChildWndID iPannelChildWndID, ClickFunc f);
    CMyWindow* GetWindow(LeftPannelChildWndID id);
private:
    map<LeftPannelChildWndID, CMyWindow*> m_mapSubWnd;
    CMyWindow* m_pHeadWnd = nullptr;
    CMyExpandWnd* m_pHomeCheckWnd = nullptr;
    CMyExpandWnd* m_pDataCheckWnd = nullptr;
    CMyExpandWnd* m_pSystemWnd = nullptr;
    CMyExpandWnd* m_pUserCenter = nullptr;
};

