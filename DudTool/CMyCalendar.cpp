#include "pch.h"
#include "CMyCalendar.h"
#include "resource.h"
#include <iostream>
#include "CommonDef.h"

#define BaseYear 1971

void CDateWindow::Initilize(MyDate date)
{
    m_currentDate = date;
    auto pLable = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLable->SetText(L"一");
    m_vecWeek.push_back(pLable);
    m_vecChildWnd.push_back(pLable);

    pLable = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLable->SetText(L"二");
    m_vecWeek.push_back(pLable);
    m_vecChildWnd.push_back(pLable);

    pLable = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLable->SetText(L"三");
    m_vecWeek.push_back(pLable);
    m_vecChildWnd.push_back(pLable);

    pLable = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLable->SetText(L"四");
    m_vecWeek.push_back(pLable);
    m_vecChildWnd.push_back(pLable);

    pLable = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLable->SetText(L"五");
    m_vecWeek.push_back(pLable);
    m_vecChildWnd.push_back(pLable);

    pLable = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLable->SetText(L"六");
    m_vecWeek.push_back(pLable);
    m_vecChildWnd.push_back(pLable);

    pLable = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLable->SetText(L"日");
    m_vecWeek.push_back(pLable);
    m_vecChildWnd.push_back(pLable);

    m_vecDate.resize(42);
    for (int i = 0; i < m_vecDate.size(); i++)
    {
        auto pButton = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
        pButton->SetName(L"Date");
        pButton->SetText(L"");
        pButton->SetMouseHoverSensitive(true);
        pButton->SetBkGroundColor(CalendarBkColor, RGB(0, 0, 255));
        pButton->RegisterClickFun([this](CMyWindow* pWnd)
        {
            OnClickDay(pWnd);
        });
        m_vecChildWnd.push_back(pButton);
        m_vecDate[i] = pButton;

    }
}

void CDateWindow::OnSize(int cx, int cy)
{
    int iStartX = 10;
    int x = iStartX;
    int y = 5;
    auto w = (m_rect.Width() - m_vecWeek.size() * m_iLabelWidth - x * 2);
    int iOffsetX = w / (int)(m_vecWeek.size() - 1);
    for each (auto var in m_vecWeek)
    {
        var->MoveWindow(x, y);
        var->SetDimention(m_iLabelWidth, m_iLabelHeight);
        x += m_iLabelWidth + iOffsetX;
    }

    x = iStartX;
    y += m_iLabelHeight + 30;
    int iLineCount = (m_vecDate.size() / 7);
    int iOffsetY = 15;
    for (int i = 1; i <= m_vecDate.size(); i++)
    {
      
        auto pDate = m_vecDate[i-1];
        pDate->MoveWindow(x, y);
        pDate->SetDimention(m_iLabelWidth, m_iLabelHeight);

        if (i % 7 == 0)
        {
            x = iStartX;
            y += iOffsetY + m_iLabelHeight;
        }
        else
        {
            x += m_iLabelWidth + iOffsetX;
        }
    }
}
int CDateWindow::GetDays(int iYear, int iMonth)
{
    iMonth = iMonth - 1;
    int iDays = 30;
    if (1 == iMonth)
        iDays = ((0 == iYear % 4) && (0 != (iYear % 100))) || (0 == iYear % 400) ? 29 : 28;
    else
        iDays = days_in_months[iMonth];
    return iDays;
}

void CDateWindow::OnClickDay(CMyWindow* pWnd)
{
    auto pButtonDay = dynamic_cast<CMyButton *>(pWnd);
    auto sDay = pButtonDay->GetText();
    int iDay = _wtoi(sDay);
        
    m_pThisDay->SetBkGroundColor(CalendarBkColor, RGB(0, 0, 255));
    pButtonDay->SetBkGroundColor(RGB(0, 0, 255), RGB(0, 0, 255));
    m_pThisDay = pButtonDay;
    m_pDayChange(iDay);
}

void CDateWindow::RegisterModifyDay(FuncDayChange dayChanged)
{
    m_pDayChange = dayChanged;
}

void CDateWindow::SetCurrentDate(MyDate date)
{
    m_currentDate = date;
    SetDate(date);
}

MyDate CDateWindow::GetNextMonth(const MyDate& _date)
{
    auto date = _date;
    if (date.m_iMonth + 1 > 12)
    {
        date.m_iMonth = 1;
        date.m_iYear++;
    }
    else
    {
        date.m_iMonth++;
    }
        
    return  date;
}

MyDate CDateWindow::GetLastMonth(const MyDate& _date)
{
    auto date = _date;
    if (date.m_iMonth - 1 == 0 )
    {
        date.m_iMonth = 12;
        date.m_iYear--;
        if (date.m_iYear < BaseYear)
        {
            date.m_iYear = BaseYear;
        }
    }
    else
    {
        date.m_iMonth--;
    }

    return  date;
}

BOOL CDateWindow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (zDelta < 0)
    {//向下滚动
        m_mouseWheelFunc(this, true);
    }
    else
    {
        m_mouseWheelFunc(this, false);
    }
    return 0;
}

void CDateWindow::RegisterMouseWheelFunc(FuncMouseWheelFunc f)
{
    m_mouseWheelFunc = f;
}

void CDateWindow::SetDate(MyDate date)
{
    if (date.m_iYear < BaseYear)
    {
        date.m_iYear = BaseYear;
    }
    auto time = CTime(date.m_iYear, date.m_iMonth, date.m_iDay,0,0,0);
    int iWeek = time.GetDayOfWeek();  // 星期日1； 星期一 2；  、、、 星期六 7；
    if (iWeek == 1)
    {
        iWeek = 8;
    }
    iWeek -= 2;//0,1,2...6;
    //计算一个月有多少天
    auto iDays = GetDays(date.m_iYear, date.m_iMonth);
    //
    auto lastDate = GetLastMonth(date);
   
    auto firstDayTime = CTime(date.m_iYear, date.m_iMonth, 1, 0, 0, 0);
    auto iFristDayWeek = firstDayTime.GetDayOfWeek();
    if (iFristDayWeek == 1)
    {
        iFristDayWeek = 8;
    }
    iFristDayWeek -= 2;//0,1,2...6;

    auto iLastDays = GetDays(lastDate.m_iYear, lastDate.m_iMonth);
    int iDate = 1;
    bool bNextMonth = false;
    auto tempDate = date;

    for (size_t i = 0; i < m_vecDate.size(); i++)
    {

        if (i < iFristDayWeek)
        {
            CString stemp;
            int iNumber = iLastDays - iFristDayWeek + 1 + i;
            stemp.Format(L"%d", iNumber);
            m_vecDate[i]->SetText(stemp);
            m_vecDate[i]->SetTextColor(RGB(128, 128, 128), RGB(255, 255, 255));
            m_vecDate[i]->SetBkGroundColor(CalendarBkColor, RGB(0, 0, 255));
            m_vecDate[i]->SetMouseHoverSensitive(false);

        }
        else
        {
            CString stemp;
            stemp.Format(L"%d", iDate);
            m_vecDate[i]->SetText(stemp);
            
            if (bNextMonth)
            {
                m_vecDate[i]->SetTextColor(RGB(128, 128, 128), RGB(255, 255, 255));
                m_vecDate[i]->SetMouseHoverSensitive(false);
            }
            else
            {
                m_vecDate[i]->SetTextColor(RGB(160, 160, 160), RGB(255, 255, 255));
                m_vecDate[i]->SetMouseHoverSensitive(true);
            }
            tempDate.m_iDay = iDate;
            if (iDate >= iDays)
            {
                iDate = 0;
                bNextMonth = true;
                tempDate = GetNextMonth(date);
                tempDate.m_iDay = iDate + 1;
            }
            if (tempDate == m_currentDate && !bNextMonth)
            {
                m_vecDate[i]->SetBkGroundColor(RGB(0, 0, 255), RGB(0, 0, 255));
                m_pThisDay = m_vecDate[i];
            }
            else if (tempDate.m_iDay == m_currentDate.m_iDay && !bNextMonth)
            {
                m_vecDate[i]->SetBkGroundColor(RGB(0, 255, 255), RGB(0, 0, 255));
                m_pThisDay = m_vecDate[i];
            }
            else
            {
                m_vecDate[i]->SetBkGroundColor(CalendarBkColor, RGB(0, 0, 255));
            }
            iDate++;
        }
    }

}

void CMonthWnd::Initilize()
{
    m_vecMonth.resize(12);
    for (int i = 0; i < m_vecMonth.size(); i++)
    {
        auto pButton = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
        CString sMonth;
        sMonth.Format(L"%d月", i + 1);
        pButton->SetName(sMonth);
        pButton->SetText(sMonth);
        pButton->SetMouseHoverSensitive(true);

        pButton->RegisterClickFun([this](CMyWindow* pWnd)
        {
            OnClickMonth(pWnd);
        });
        m_vecChildWnd.push_back(pButton);
        m_vecMonth[i] = pButton;
    }
}

void CMonthWnd::OnClickMonth(CMyWindow* pWnd)
{
    if (m_pMonthChange == nullptr)
    {
        return;
    }
    
    auto sMonth = ((CMyButton *)pWnd)->GetText();
    int iMonth = _wtoi(sMonth.GetString());
    m_pMonthChange(iMonth);
}

void CMonthWnd::RegisterMonthClick(FuncMonthChange func)
{
    m_pMonthChange = func;
}

void CMonthWnd::SetDate(MyDate date)
{
    for (int i = 0; i < m_vecMonth.size(); i++)
    {
        if (m_currentDate.m_iMonth == i + 1 )
        {
            if (date.m_iYear == m_currentDate.m_iYear)
            {
                m_vecMonth[i]->SetBkGroundColor(RGB(0, 0, 255), RGB(0, 0, 255));
            }
            else
            {
                m_vecMonth[i]->SetBkGroundColor(RGB(0, 255, 255), RGB(0, 0, 255));
            }
        }
        else
        {
            m_vecMonth[i]->SetBkGroundColor(CalendarBkColor, RGB(0, 0, 255));
        }
    }
}

void CMonthWnd::SetCurrentDate(MyDate date)
{
    m_currentDate = date;
    SetDate(date);
}

void CMonthWnd::OnSize(int cx, int cy)
{
    int iStartx = 10;
    int x = iStartx;
    int y = 15;
    int w = 60;
    int h = 60;
    int iOffsetX = (m_rect.Width() - x * 2 - w * 4) / 3;
    int iOffsetY = iOffsetX + 30;
    for (int i = 1; i <= m_vecMonth.size(); i++)
    {
        m_vecMonth[i-1]->MoveWindow(x, y);
        m_vecMonth[i-1]->SetDimention(w, h);
        x += iOffsetX + w;
        if (i % 4 == 0)
        {
            x = iStartx;
            y += iOffsetY + h;
        }
    }
}

bool CMonthWnd::Refresh(CDC* pMemDC, CRect rect)
{
    std::cout << "------------CMonthWnd::Refresh" << std::endl;
    return __super::Refresh(pMemDC, rect);;
}

void CMonthWnd::SetVisible(bool bVisible)
{
    return __super::SetVisible(bVisible);
}

BOOL CMonthWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (zDelta < 0)
    {//向下滚动
        m_mouseWheelFunc(this, true);
    }
    else
    {
        m_mouseWheelFunc(this, false);
    }
    return 0;
}

void CMonthWnd::RegisterMouseWheelFunc(FuncMouseWheelFunc f)
{
    m_mouseWheelFunc = f;
}

void CYearWnd::Initilize()
{
    m_vecYear.resize(16);
    for (int i = 0; i < m_vecYear.size(); i++)
    {
        auto pButton = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
        pButton->SetName(L"Year Wnd");
        pButton->SetText(L"");
        pButton->SetMouseHoverSensitive(true);
        pButton->RegisterClickFun([this](CMyWindow* pWnd)
        {
            OnClickYear(pWnd);
        });
        m_vecChildWnd.push_back(pButton);
        m_vecYear[i] = pButton;
    }

}

void CYearWnd::OnSize(int cx, int cy)
{
    int iStartx = 10;
    int x = iStartx;
    int y = 15;
    int w = 60;
    int h = 60;
    int iOffsetX = (m_rect.Width() - x * 2 - w * 4) / 3;
    int iOffsetY = iOffsetX;
    for (int i = 1; i <= m_vecYear.size(); i++)
    {
        m_vecYear[i - 1]->MoveWindow(x, y);
        m_vecYear[i - 1]->SetDimention(w, h);
        x += iOffsetX + w;
        if (i % 4 == 0)
        {
            x = iStartx;
            y += iOffsetY + h;
        }
    }
}

bool CYearWnd::Refresh(CDC* pMemDC, CRect rect)
{
    return __super::Refresh(pMemDC, rect);
}

void CYearWnd::OnClickYear(CMyWindow* pWnd)
{
    CString sTemp = ((CMyButton *)pWnd)->GetText();
    int iYear = _wtoi(sTemp.GetString());
    m_pYearChange(iYear);
}

void CYearWnd::RegisterYearClick(FuncYearChange func)
{
    m_pYearChange = func;
}

void CYearWnd::SetDate(MyDate date)
{
    int iStartYear = date.m_iYear - 4;
    for (int i = 0; i < m_vecYear.size(); i++)
    {
        CString sTemp;
        sTemp.Format(L"%d", iStartYear);
        m_vecYear[i]->SetText(sTemp);
        if (iStartYear == m_currentDate.m_iYear)
        {
            m_vecYear[i]->SetBkGroundColor(RGB(0, 0, 255), RGB(0, 0, 255));
        }
        else
        {
            m_vecYear[i]->SetBkGroundColor(CalendarBkColor, RGB(0, 0, 255));
        }
        ++iStartYear;
    }
}

void CYearWnd::SetCurrentDate(MyDate date)
{
    m_currentDate = date;
    SetDate(date);
}

BOOL CYearWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    if (zDelta < 0)
    {//向下滚动
        m_mouseWheelFunc(this, true);
    }
    else
    {
        m_mouseWheelFunc(this, false);
    }
    return 0;
}

void CYearWnd::RegisterMouseWheelFunc(FuncMouseWheelFunc f)
{
    m_mouseWheelFunc = f;
}


void CMyCalendar::Initialize()
{
    m_sName = L"CMyCalendar";
    m_pBtnMonthIncrease = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    //m_pBtnMonthIncrease->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    m_pBtnMonthIncrease->SetBkPic(IDB_PNG_ARROW_D , IDB_PNG_D_S );
    m_pBtnMonthIncrease->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnIncreate(pWnd);
    });
    m_vecChildWnd.push_back(m_pBtnMonthIncrease);

    m_pBtnMonthDecrease = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnMonthDecrease->SetBkPic(IDB_PNG_ARROW_UP, IDB_PNG_ARROW_UP_SELECT);
    m_pBtnMonthDecrease->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnDecreate(pWnd);
    });
    m_vecChildWnd.push_back(m_pBtnMonthDecrease);

    //获取当前日期
    CTime time = CTime::GetCurrentTime();
    //auto time = CTime(2019, 3, 15,0,0,0);
    int iYear = time.GetYear();
    int iMonth = time.GetMonth();
    int iDay = time.GetDay();
    int iWeek = time.GetDayOfWeek(); // 星期日1； 星期一 2；  、、、 星期六 7；
    CString sYear;
    sYear.Format(L"%d年", iYear);
    m_pBtnYear = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnYear->SetText(sYear);
    m_pBtnYear->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnYear(pWnd);
    });
    //m_pBtnYear->SetBkGroundColor()
    m_vecChildWnd.push_back(m_pBtnYear);

    m_pLabel = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pLabel->SetVisible(false);
    m_vecChildWnd.push_back(m_pLabel);


    CString sMonth;
    sMonth.Format(L"%d月", iMonth);
    m_pBtnMonth = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnMonth->SetText(sMonth);
    m_pBtnMonth->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnMonth(pWnd);
    });
    m_vecChildWnd.push_back(m_pBtnMonth);

    m_pUpLine = new CMyWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pUpLine->SetBkPic(IDB_PNG_LINE);
    m_pUpLine->SetExtendDrawBK(true);
    m_vecChildWnd.push_back(m_pUpLine);

    m_pDownLine = new CMyWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pDownLine->SetBkPic(IDB_PNG_LINE);
    m_pDownLine->SetExtendDrawBK(true);
    m_vecChildWnd.push_back(m_pDownLine);

    m_pDataWnd = new CDateWindow(this, m_pRootWnd, m_refreshImpFun);
    m_pDataWnd->RegisterModifyDay([this](int iDay)
    {
        OnDayChanged(iDay);
    });
    m_currentDate = MyDate(iYear, iMonth, iDay);
    m_pDataWnd->Initilize(m_currentDate);
    m_pDataWnd->SetDate(m_currentDate);
    m_pDataWnd->SetVisible(true);
    m_pDataWnd->RegisterMouseWheelFunc([this](CMyWindow* pWnd, bool bIncreate)
    {
        this->OnMouseWheel(pWnd, bIncreate);
    });
    m_vecChildWnd.push_back(m_pDataWnd);

    
    m_pMonthWnd = new CMonthWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pMonthWnd->Initilize();
    m_pMonthWnd->SetVisible(false);
    m_pMonthWnd->RegisterMonthClick([this](int iMonth)
    {
        this->OnMonthChanged(iMonth);
    });
    m_pMonthWnd->RegisterMouseWheelFunc([this](CMyWindow* pWnd, bool bIncreate)
    {
        this->OnMouseWheel(pWnd, bIncreate);
    });
    m_vecChildWnd.push_back(m_pMonthWnd);

    m_pYearWnd = new CYearWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pYearWnd->Initilize();
    m_pYearWnd->SetVisible(false);
    m_pYearWnd->RegisterYearClick([this](int iMonth)
    {
        this->OnYearChanged(iMonth);
    });
    m_pYearWnd->RegisterMouseWheelFunc([this](CMyWindow* pWnd, bool bIncreate)
    {
        this->OnMouseWheel(pWnd, bIncreate);
    });
    m_vecChildWnd.push_back(m_pYearWnd);
    

}
void CMyCalendar::OnSize(int cx, int cy)
{    
    auto size = m_pBtnMonthIncrease->GetBkPicSize();      
    int x = m_rect.Width() - 50;
    int y = 20;
    m_pBtnMonthIncrease->MoveWindow(x, y);
    m_pBtnMonthIncrease->SetDimention(size.cx, size.cy);

    x = x - size.cx - 50;
    m_pBtnMonthDecrease->MoveWindow(x, y);
    m_pBtnMonthDecrease->SetDimention(size.cx, size.cy);
    x =  10;
    int iBtnWidth = 75;
    m_pBtnYear->MoveWindow(x, y);
    m_pBtnYear->SetDimention(iBtnWidth, size.cy);
    m_pLabel->MoveWindow(x, y);
    m_pLabel->SetDimention(iBtnWidth + 40, size.cy);



    x = x + iBtnWidth;
    m_pBtnMonth->MoveWindow(x, y);
    m_pBtnMonth->SetDimention(50, size.cy);

    x = 0;
    y += size.cy + 10;
    auto lineSize = m_pUpLine->GetBkPicSize();
    m_pUpLine->MoveWindow(x, y);
    m_pUpLine->SetDimention(m_rect.Width(), lineSize.cy);

    int iDownLineY = m_rect.Height() - lineSize.cy - 5;
    m_pDownLine->MoveWindow(x, iDownLineY);
    m_pDownLine->SetDimention(m_rect.Width(), lineSize.cy);

    x = 0;
    int iOffsetY = y;
    y += lineSize.cy + 5;
    m_pDataWnd->MoveWindow(x, y);
    m_pDataWnd->SetDimention(m_rect.Width(), m_rect.Height() - iOffsetY);
    m_pDataWnd->OnSize(cx, cy);

    m_pMonthWnd->MoveWindow(x, y);
    m_pMonthWnd->SetDimention(m_rect.Width(), m_rect.Height() - iOffsetY);
    m_pMonthWnd->OnSize(cx, cy);

    m_pYearWnd->MoveWindow(x, y);
    m_pYearWnd->SetDimention(m_rect.Width(), m_rect.Height() - iOffsetY);
    m_pYearWnd->OnSize(cx, cy);

}
bool CMyCalendar::Refresh(CDC* pMemDC, CRect r)
{
    auto rect = GetClientRect();
    if (!IsRectIntersect(rect, r))
    {
        return false;
    }
                       
    return __super::Refresh(pMemDC, r);
}

void CMyCalendar::OnDecreate(CMyWindow* pWnd)
{
    if (m_pMonthWnd->IsVisible())
    {
        DecreateYear(pWnd);
    }
    else if (m_pDataWnd->IsVisible())
    {
        DecreateMonth(pWnd);
    }
    else
    {
        DecreateYear_10(pWnd);
    }
}

void CMyCalendar::OnIncreate(CMyWindow* pWnd)
{
    if (m_pMonthWnd->IsVisible())
    {
        IncreateYear(pWnd);
    }
    else if (m_pDataWnd->IsVisible())
    {
        IncreateMonth(pWnd);
    }
    else
    {
        IncreateYear_10(pWnd);
    }
}

void CMyCalendar::DecreateMonth(CMyWindow* pWnd)
{
    auto date = m_pDataWnd->GetLastMonth(GetDisplayDate());
    m_pDataWnd->SetDate(date);
    SetDisplayDate(date);
}

void CMyCalendar::IncreateMonth(CMyWindow* pWnd)
{
    auto date = m_pDataWnd->GetNextMonth(GetDisplayDate());
    m_pDataWnd->SetDate(date);
    SetDisplayDate(date);

}

void CMyCalendar::DecreateYear(CMyWindow* pWnd)
{
    auto date = GetDisplayDate();
    date.m_iYear--;
    if (date.m_iYear < BaseYear)
    {
        date.m_iYear = BaseYear;
    }
    m_pMonthWnd->SetDate(date);
    SetDisplayDate(date);
}

void CMyCalendar::IncreateYear(CMyWindow* pWnd)
{
    auto date = GetDisplayDate();
    date.m_iYear++;
    m_pMonthWnd->SetDate(date);
    SetDisplayDate(date);
}

void CMyCalendar::DecreateYear_10(CMyWindow* pWnd)
{
    auto date = GetDisplayDate();
    date.m_iYear -= 10;
    if (date.m_iYear < BaseYear)
    {
        date.m_iYear = BaseYear;
    }
    m_pYearWnd->SetDate(date);
    int iStartYear = date.m_iYear - 4;
    int iEndYear = iStartYear + 15;
    CString sTemp;
    sTemp.Format(L"%d-%d", iStartYear, iEndYear);
    m_pLabel->SetText(sTemp);

    SetDisplayDate(date);
}

void CMyCalendar::IncreateYear_10(CMyWindow* pWnd)
{
    auto date = GetDisplayDate();
    date.m_iYear += 10;
    m_pYearWnd->SetDate(date);
    int iStartYear = date.m_iYear - 4;
    int iEndYear = iStartYear + 15;
    CString sTemp;
    sTemp.Format(L"%d-%d", iStartYear, iEndYear);
    m_pLabel->SetText(sTemp);
    SetDisplayDate(date);
}

void CMyCalendar::OnDayChanged(int iDay)
{
    m_currentDate = GetDisplayDate();
    m_currentDate.m_iDay = iDay;
    m_pDataWnd->SetCurrentDate(m_currentDate);

    m_funcDateSelected(m_currentDate);
}

MyDate CMyCalendar::GetDisplayDate()
{
    auto date = m_currentDate;
    date.m_iYear = _wtoi(((CMyButton *)m_pBtnYear)->GetText());
    date.m_iMonth = _wtoi(((CMyButton *)m_pBtnMonth)->GetText());
    return date;
}

void CMyCalendar::SetDisplayDate(MyDate date)
{
    CString sYear;
    sYear.Format(L"%d年", date.m_iYear);
    m_pBtnYear->SetText(sYear);
    //m_pBtnYear->SetBkGroundColor()
    m_vecChildWnd.push_back(m_pBtnYear);

    CString sMonth;
    sMonth.Format(L"%d月", date.m_iMonth);
    m_pBtnMonth->SetText(sMonth);
}

void CMyCalendar::OnYear(CMyWindow* pWnd)
{
    m_pDataWnd->SetVisible(false);
    m_pMonthWnd->SetVisible(false);
    m_pYearWnd->SetVisible(true);
    m_pYearWnd->SetCurrentDate(m_currentDate);
    m_pLabel->SetText(L"2020-2040");
    m_pLabel->SetVisible(true);

    m_pBtnYear->SetVisible(false);
    m_pBtnMonth->SetVisible(false);

    m_pYearWnd->UpdateMyWnd(GetClientRect());
}

void CMyCalendar::OnMonth(CMyWindow* pWnd)
{
    m_pDataWnd->SetVisible(false);
    m_pYearWnd->SetVisible(false);
    m_pMonthWnd->SetVisible(true);

    m_pBtnMonth->SetVisible(false);
    m_pMonthWnd->SetCurrentDate(m_currentDate);
    m_pMonthWnd->UpdateMyWnd(GetClientRect());
}

void CMyCalendar::OnMonthChanged(int iMonth)
{
    m_currentDate.m_iMonth = iMonth;
    m_pDataWnd->SetCurrentDate(m_currentDate);
    SetDisplayDate(m_currentDate);
    m_pBtnMonth->SetVisible(true);
    m_pDataWnd->SetVisible(true);
    m_pMonthWnd->SetVisible(false);
    m_pMonthWnd->UpdateMyWnd(GetClientRect());
}

void CMyCalendar::OnYearChanged(int iYear)
{
    m_pBtnMonth->SetVisible(true);
    m_pBtnYear->SetVisible(true);
    m_pDataWnd->SetVisible(true);
    m_pMonthWnd->SetVisible(false);
    m_pYearWnd->SetVisible(false);
    m_pLabel->SetVisible(false);
    if (iYear < BaseYear)
    {
        iYear = BaseYear;
    }

    m_currentDate.m_iYear = iYear;
    m_pDataWnd->SetCurrentDate(m_currentDate);
    SetDisplayDate(m_currentDate);
    m_pDataWnd->UpdateMyWnd(GetClientRect());
}

void CMyCalendar::RegisterSelectDate(FuncDateSelected func)
{
    m_funcDateSelected = func;
}

void CMyCalendar::SetCurrentDate(MyDate date)
{
    m_pDataWnd->SetCurrentDate(date);
    m_pMonthWnd->SetVisible(false);
    m_pYearWnd->SetVisible(false);
    UpdateMyWnd(GetClientRect());
}

void CMyCalendar::OnMouseWheel(CMyWindow* pWnd, bool bIncreate)
{
    if (bIncreate)
    {
        OnIncreate(pWnd);
    }
    else
    {
        OnDecreate(pWnd);
    }
}

void CMyCalendar::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
    for each (auto item in m_vecChildWnd)
    {
        item->SetVisible(bVisible);
    }

    m_pDataWnd->SetVisible(bVisible);
    m_pMonthWnd->SetVisible(false);
    m_pYearWnd->SetVisible(false);
    m_pLabel->SetVisible(false);
    UpdateMyWnd(GetClientRect());
}

