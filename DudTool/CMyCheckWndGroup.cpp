#include "pch.h"
#include "CMyCheckWndGroup.h"
#include "CMyExpandWnd.h"
#include "CMyWindow.h"


CGroupManager::CGroupManager()
{
}

CGroupManager::~CGroupManager()
{
}

CGroupManager* CGroupManager::Instance()
{
    return new  CGroupManager();
}

void CGroupManager::AddWnd(CMyWindow* pWnd)
{
    m_vecCheckWndGroup.push_back(pWnd);
}
void CGroupManager::SelectWnd(CMyWindow* pWnd)
{
    if (!m_bSelectExclusive)
    {
        return;
    }
    for (auto item : m_vecCheckWndGroup)
    {
        if (item == pWnd)
        {
            item->SetSelectState(true);
        }
        else
        {
            item->SetSelectState(false);
        }
    }
}

void CGroupManager::SetSelectExclusive(bool bSet)
{
    m_bSelectExclusive = bSet;
}



CGroupManager* CExpandGroup::Instance()
{        
    return new  CExpandGroup();
}
void CExpandGroup::ExpandWnd(CMyWindow* pWnd)
{
    //把下面的所有窗口刷新,并向下移动
    int iIncreasedHeight = 0;
    bool bExpand = false;
    for each (auto item in m_vecCheckWndGroup)
    {
        if (pWnd == item)
        {
            iIncreasedHeight = ((CMyExpandWnd*)item)->GetDisplayWndSize().cy;
            bExpand = true;
            continue;
        }
        if (!bExpand)
        {
            continue;
        }
        auto pExpandWnd = dynamic_cast<CMyExpandWnd*>(item);
        auto pt = pExpandWnd->GetMyRect().TopLeft();
        pt.y += iIncreasedHeight;
        pExpandWnd->MoveWindow(pt);
    }
}

void CExpandGroup::UnExpandWnd(CMyWindow* pWnd)
{
    //把下面的所有窗口刷新,并向上移动
    bool bNeedRefresh = false;
    int iNarrowHeight = 0;
    CRect dirtyRect;
    for each (auto item in m_vecCheckWndGroup)
    {
        if (pWnd == item)
        {
            iNarrowHeight = ((CMyExpandWnd*)item)->GetDisplayWndSize().cy;
            bNeedRefresh = true;
            dirtyRect = item->GetClientRect(); 
            dirtyRect.bottom += iNarrowHeight;
            continue;
        }
        if (!bNeedRefresh)
        {
            continue;
        }
        auto pExpandWnd = dynamic_cast<CMyExpandWnd*>(item);
        dirtyRect.bottom = pExpandWnd->GetClientRect().bottom;
        auto pt = pExpandWnd->GetMyRect().TopLeft();
        pt.y -= iNarrowHeight;
        pExpandWnd->MoveWindow(pt);
    }
    pWnd->UpdateMyWnd(dirtyRect);
}

CGroupManager* CSystemManagerGroup::Instance()
{
    return new  CSystemManagerGroup();
}
