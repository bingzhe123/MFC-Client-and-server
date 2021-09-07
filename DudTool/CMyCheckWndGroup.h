#pragma once
#include <vector>
using std::vector;
class CMyWindow;
class CGroupManager
{
public:
    CGroupManager();
    ~CGroupManager();
    static CGroupManager* Instance();

    void AddWnd(CMyWindow* pWnd);
    void SelectWnd(CMyWindow* pWnd);
    //一组窗口的选择是否是排他的
    void SetSelectExclusive(bool bSet);
protected:
    vector<CMyWindow*>  m_vecCheckWndGroup;
    bool m_bSelectExclusive = true;
};

class CExpandGroup :public CGroupManager
{
public:
    static CGroupManager* Instance();
    void ExpandWnd(CMyWindow* pWnd);
    void UnExpandWnd(CMyWindow* pWnd);
};
class CSystemManagerGroup :public CGroupManager
{
public:
    static CGroupManager* Instance();
};
