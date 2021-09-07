#include "pch.h"
#include "CMyBoderWnd.h"
#include <cassert>

void CMyBoderWnd::AddChild(CMyWindow* pWnd)
{
    m_vecChildWnd.push_back(pWnd);
}

void CMyBoderWnd::OnSize(int cx, int cy)
{
    if (m_vecChildWnd.size() != 1)
    {
        assert(false);
        return;
    }
    auto rect = GetClientRect();
    int x = m_iBoder;
    int y = m_iBoder;
    int w = rect.Width() - 2 * x;
    int h = rect.Height() - 2 * y;
    m_vecChildWnd[0]->MoveWindow(x, y);
    m_vecChildWnd[0]->SetDimention(w, h);
    m_vecChildWnd[0]->OnSize(0, 0);
}

void CMyBoderWnd::SetBoderWeight(int iBoderWeight)
{
    m_iBoder = iBoderWeight;
}
