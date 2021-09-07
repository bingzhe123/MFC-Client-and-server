#pragma once
#include "CMyContainerWnd.h"
#include "CMyStaticWindow.h"
class CHomeChartWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual ~CHomeChartWnd();
    void Initialize();
    virtual void OnSize(int cx, int cy);

private:
    CMyStaticWindow* m_pTable = nullptr;
    CMyStaticWindow* m_pHomeUserInfo;
    CMyStaticWindow* m_pHomeSoftWare;
    CMyStaticWindow* m_pHospital;
};

