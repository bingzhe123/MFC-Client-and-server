#pragma once
#include "CMyWindow.h"
class CMyprogressBar :
    public CMyWindow
{
public:
    using CMyWindow::CMyWindow;
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    void SetPos(int iPos);
private:
    int m_iPos = 0;
};

