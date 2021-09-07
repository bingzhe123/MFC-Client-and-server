#pragma once
#include "CMyContainerWnd.h"
#include "CMyButton.h"
#include "CMyLable.h"




class MyDate
{
public:
    int m_iYear;
    int m_iMonth;
    int m_iDay;
    MyDate() {};
    MyDate(int iYear, int iMonth, int iDay)
    {
        m_iYear = iYear;
        m_iMonth = iMonth;
        m_iDay = iDay;
    };
    bool operator == (const MyDate& date)
    {
        return m_iDay == date.m_iDay &&
            m_iMonth == date.m_iMonth &&
            this->m_iYear == date.m_iYear;
    };
};

using  FuncDayChange = std::function<void(int)>;
using  FuncMonthChange = std::function<void(int)>;
using  FuncYearChange = std::function<void(int)>;
using  FuncDateSelected = std::function<void(MyDate)>;
using  FuncMouseWheelFunc = std::function<void(CMyWindow*, bool)>;


class CMonthWnd :public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    void Initilize();
    virtual void OnSize(int cx, int cy);
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual void SetVisible(bool bVisible);
    void OnClickMonth(CMyWindow *pWnd);
    void RegisterMonthClick(FuncMonthChange func);
    void SetDate(MyDate date);
    void SetCurrentDate(MyDate date);
    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    void RegisterMouseWheelFunc(FuncMouseWheelFunc f);
private:
    vector<CMyButton *> m_vecMonth;
    FuncMonthChange m_pMonthChange = nullptr;
    MyDate m_currentDate;
    FuncMouseWheelFunc m_mouseWheelFunc;
};

class CYearWnd :public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    void Initilize();
    virtual void OnSize(int cx, int cy);
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    void OnClickYear(CMyWindow* pWnd);
    void RegisterYearClick(FuncYearChange func);
    void SetDate(MyDate date);
    void SetCurrentDate(MyDate date);
    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    void RegisterMouseWheelFunc(FuncMouseWheelFunc f);
private:
    vector<CMyButton *> m_vecYear;
    FuncYearChange m_pYearChange = nullptr;
    MyDate m_currentDate;
    FuncMouseWheelFunc m_mouseWheelFunc;
};



class CDateWindow :public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    void Initilize(MyDate date);
    virtual void OnSize(int cx, int cy);
    void SetDate(MyDate date);
    int GetDays(int iYear, int iMonth);
    void OnClickDay(CMyWindow* pWnd);
    void RegisterModifyDay(FuncDayChange dayChanged);
    void SetCurrentDate(MyDate date);
    MyDate GetNextMonth(const MyDate &date);
    MyDate GetLastMonth(const MyDate& date);

    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    void RegisterMouseWheelFunc(FuncMouseWheelFunc f);

private:
    vector<CMyButton *> m_vecDate;
    vector<CMyLable*> m_vecWeek;
    const int m_iLabelWidth = 30;
    const int m_iLabelHeight = 30;
    CMyButton * m_pThisDay = nullptr;
    FuncDayChange m_pDayChange = nullptr;
    MyDate m_currentDate;
    FuncMouseWheelFunc m_mouseWheelFunc;
};

class CMyCalendar :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    void Initialize();
    virtual void OnSize(int cx, int cy);

    virtual bool Refresh(CDC* pMemDC, CRect rect);
    virtual void SetVisible(bool bVisible);

    void OnDecreate(CMyWindow* pWnd);
    void OnIncreate(CMyWindow* pWnd);


    void DecreateMonth(CMyWindow* pWnd);
    void IncreateMonth(CMyWindow* pWnd);
    void DecreateYear(CMyWindow* pWnd);
    void IncreateYear(CMyWindow* pWnd);

    void DecreateYear_10(CMyWindow* pWnd);
    void IncreateYear_10(CMyWindow* pWnd);

    void OnDayChanged(int iDay);
    MyDate GetDisplayDate();
    void SetDisplayDate(MyDate date);
    void OnYear(CMyWindow* pWnd);
    void OnMonth(CMyWindow* pWnd);
    void OnMonthChanged(int iMonth);
    void OnYearChanged(int iMonth);
    void RegisterSelectDate(FuncDateSelected func);
    void SetCurrentDate(MyDate date);
    void OnMouseWheel(CMyWindow *pWnd, bool bIncreate);

private:
    CMyButton * m_pBtnMonthIncrease = nullptr;
    CMyButton * m_pBtnMonthDecrease = nullptr;
    CMyButton * m_pBtnYear = nullptr;
    CMyButton * m_pBtnMonth = nullptr;
    CMyLable* m_pLabel = nullptr;

    CMyWindow* m_pUpLine = nullptr;
    CMyWindow* m_pDownLine = nullptr;
    CDateWindow* m_pDataWnd = nullptr;
    CMonthWnd *m_pMonthWnd = nullptr;
    CYearWnd* m_pYearWnd = nullptr;
    MyDate m_currentDate;
    FuncDateSelected m_funcDateSelected = nullptr;
};

