#include "pch.h"
#include "CHomeChartWnd.h"
#include "CommonDef.h"
#include "CMyStaticWindow.h"

CHomeChartWnd::~CHomeChartWnd()
{
}

void CHomeChartWnd::Initialize()
{
#pragma region Ê×Ò³Í¼±í
    int iOffsetx = 0;
    int iOffsety = 0;
    int iSplitterOffsety = 5;
    m_pTable = new CMyStaticWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pTable->SetBkPic(IDB_PNG_TUBIAO);
    m_pTable->SetBkGroundColor(RGB(25, 31, 40));
    m_pTable->SetMouseHoverSensitive(false);
    auto picSize = m_pTable->GetBkPicSize();
    float fScale = 1.0;
    int iWidth = picSize.cx * fScale;
    int iHeight = picSize.cy * fScale;
    iWidth = max(picSize.cx, iWidth);
    m_pTable->SetDimention(iWidth, iHeight);
    m_pTable->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pTable->SetBkPicStretch(true);
    m_vecChildWnd.push_back(m_pTable);

    m_pHospital = new CMyStaticWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pHospital->SetBkPic(IDB_PNG_HOME_HOSPITAL);
    m_pHospital->SetBkGroundColor(RGB(25, 31, 40));
    m_pHospital->SetMouseHoverSensitive(false);
    iOffsety += iHeight + iSplitterOffsety;
    picSize = m_pHospital->GetBkPicSize();
    iWidth = picSize.cx * fScale;
    iHeight = picSize.cy * fScale;
    m_pHospital->SetDimention(iWidth, iHeight);
    m_pHospital->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pHospital->SetBkPicStretch(true);
    m_vecChildWnd.push_back(m_pHospital);

    m_pHomeUserInfo = new CMyStaticWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pHomeUserInfo->SetBkPic(IDB_PNG_HOME_USERINFO);
    m_pHomeSoftWare = new CMyStaticWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pHomeSoftWare->SetBkPic(IDB_PNG_HOME_SOFTWARE);

    auto userInfo = m_pHomeUserInfo->GetBkPicSize();
    auto softWareSize = m_pHomeUserInfo->GetBkPicSize();

    int iSplitterOffsetx = (m_rect.Width() - picSize.cx - userInfo.cx - softWareSize.cx) / 2;

    iSplitterOffsetx = iSplitterOffsetx < 0 ? 0 : iSplitterOffsetx;

    m_pHomeUserInfo->SetBkGroundColor(RGB(25, 31, 40));
    m_pHomeUserInfo->SetMouseHoverSensitive(false);
    iOffsetx += iWidth + iSplitterOffsetx;
    iWidth = userInfo.cx;
    m_pHomeUserInfo->SetDimention(iWidth, iHeight);
    m_pHomeUserInfo->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pHomeUserInfo->SetBkPicStretch(true);
    m_vecChildWnd.push_back(m_pHomeUserInfo);
    
    iOffsetx += iWidth + iSplitterOffsetx;
    iWidth = softWareSize.cx;
    m_pHomeSoftWare->SetBkGroundColor(RGB(25, 31, 40));
    m_pHomeSoftWare->SetMouseHoverSensitive(false);
    m_pHomeSoftWare->SetDimention(iWidth, iHeight);
    m_pHomeSoftWare->MoveWindow(CPoint(iOffsetx, iOffsety));
    m_pHomeSoftWare->SetBkPicStretch(true);
    m_vecChildWnd.push_back(m_pHomeSoftWare);
#pragma endregion
}

void CHomeChartWnd::OnSize(int cx, int cy)
{

    this->SetDimention(cx, cy);

    int iOffsetx = 0;
    int iOffsety = 0;
    int iSplitterOffsety = 5;
    auto picSize = m_pTable->GetBkPicSize();

    CRect rect = GetClientRect();
    float fScale = 1.0;
    int iWidth = picSize.cx * fScale;
    int iHeight = picSize.cy * fScale;
    int iTopWidth = iWidth;

    iWidth = min(picSize.cx, iWidth);
    m_pTable->SetDimention(iWidth, iHeight);
    m_pTable->MoveWindow(CPoint(iOffsetx, iOffsety));

    iOffsety += iHeight + iSplitterOffsety;
    picSize = m_pHospital->GetBkPicSize();
    iWidth = picSize.cx * fScale;
    iHeight = picSize.cy * fScale;
    m_pHospital->SetDimention(iWidth, iHeight);
    m_pHospital->MoveWindow(CPoint(iOffsetx, iOffsety));

    auto userInfo = m_pHomeUserInfo->GetBkPicSize();
    auto softWareSize = m_pHomeSoftWare->GetBkPicSize();
    int iSplitterOffsetx = (iTopWidth - picSize.cx - userInfo.cx - softWareSize.cx) / 2;
    iSplitterOffsetx = iSplitterOffsetx < 0 ? 0 : iSplitterOffsetx;

    iOffsetx += iWidth + iSplitterOffsetx;
    iWidth = userInfo.cx  *fScale;
    m_pHomeUserInfo->SetDimention(iWidth, iHeight);
    m_pHomeUserInfo->MoveWindow(CPoint(iOffsetx, iOffsety));

    iOffsetx += iWidth + iSplitterOffsetx;
    iWidth = softWareSize.cx;
    m_pHomeSoftWare->SetDimention(iWidth, iHeight);
    m_pHomeSoftWare->MoveWindow(CPoint(iOffsetx, iOffsety));
}
