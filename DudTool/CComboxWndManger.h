#pragma once
#include "CMyWindow.h"
#include <vector>

//因为combox展开的时候会跨窗口，所以要特殊处理
class CComboxWndManger
{
public:
    static CComboxWndManger& Instance();
    void AddWnd(CMyWindow* pWnd);
    vector<CMyWindow*> GetWnd();
private:
    vector<CMyWindow*> m_vecWnd;

};

//因为combox展开的时候会跨窗口，所以要特殊处理
class CCrosWndManger :public CComboxWndManger
{
public:
    static CCrosWndManger& Instance();
};
