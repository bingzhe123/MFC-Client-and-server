#include "pch.h"
#include "CComboxWndManger.h"

CComboxWndManger& CComboxWndManger::Instance()
{
    // TODO: insert return statement here
    static CComboxWndManger instance;
    return instance;
}

void CComboxWndManger::AddWnd(CMyWindow* pWnd)
{
    m_vecWnd.push_back(pWnd);
}

vector<CMyWindow*> CComboxWndManger::GetWnd()
{
    return m_vecWnd;
}

CCrosWndManger& CCrosWndManger::Instance()
{
    // TODO: insert return statement here
    static CCrosWndManger instance;
    return instance;
}
