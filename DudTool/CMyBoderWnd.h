#pragma once
#include "CMyContainerWnd.h"

class CMyBoderWnd :
    public CMyContainerWnd
{
public:
    using  CMyContainerWnd::CMyContainerWnd;
    virtual void OnSize(int cx, int cy);
    void AddChild(CMyWindow* pWnd);
    void SetBoderWeight(int iBoderWeight);
private:
    int m_iBoder = 5;
};
