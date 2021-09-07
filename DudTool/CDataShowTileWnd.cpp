#include "pch.h"
#include "CDataShowTileWnd.h"
#include "CDataInfoRegisterDlg.h"
#include "CNewPatientTypeDlg.h"
#include "HttpHelper.h"
#include "DudToolClient.h"
#include "CComboxWndManger.h"
#include "CommonDef.h"

CDataShowTileWnd::~CDataShowTileWnd()
{
}

void CDataShowTileWnd::Initialize()
{
    m_sName = L"CDataShowTileWnd";
    m_EditBoxfont.DeleteObject();
    m_EditBoxfont.CreateFont(23,                    //   字体的高度   
        0,                                          //   字体的宽度  
        0,                                          //  nEscapement 
        0,                                          //  nOrientation   
        FW_NORMAL,                                  //   nWeight   
        FALSE,                                      //   bItalic   
        FALSE,                                      //   bUnderline   
        0,                                                   //   cStrikeOut   
        ANSI_CHARSET,                             //   nCharSet   
        OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
        CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
        DEFAULT_QUALITY,                       //   nQuality   
        DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
        _T("微软雅黑"));

    SetBkGroundColor(RGB(25, 31, 40));
    CRect rect = GetClientRect();
    int iBtnHeight = rect.Height() - 50;
    int iBtnWidth = 150;
    int iOffset = 10;
    m_pBtnRegisterInfo = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnRegisterInfo->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    int x = iOffset;
    int y = (rect.Height() - iBtnHeight) / 2;
    m_pBtnRegisterInfo->MoveWindow(CPoint(x, y));
    m_pBtnRegisterInfo->SetDimention(iBtnWidth, iBtnHeight);
    m_pBtnRegisterInfo->SetText(CString(L"信息注册"));
    m_pBtnRegisterInfo->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnRegister(vector<CString>());
    });
    m_vecChildWnd.push_back(m_pBtnRegisterInfo);

    m_pBtnPatientType = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnPatientType->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    x += iOffset + iBtnWidth;
    m_pBtnPatientType->MoveWindow(CPoint(x, y));
    m_pBtnPatientType->SetDimention(iBtnWidth, iBtnHeight);
    m_pBtnPatientType->SetText(CString(L"病人类型创建"));
    m_pBtnPatientType->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnCratePatientType();
    });
    m_vecChildWnd.push_back(m_pBtnPatientType);
    
    m_pBtnExportExcel = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnExportExcel->SetDimention(iBtnWidth, iBtnHeight);
    m_pBtnExportExcel->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    x += iOffset + iBtnWidth;
    m_pBtnExportExcel->MoveWindow(CPoint(x, y));
    m_pBtnExportExcel->SetText(CString(L"导出到文件"));
    m_pBtnExportExcel->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnExportExcel();
    });
    m_vecChildWnd.push_back(m_pBtnExportExcel);

    int iOffsety = 20;
    int iComboxWidht = 300;
    int iComboxHeight = 35;
    x += iOffset + iBtnWidth;
    int iLabelWidth = 200;
    int iLabelHeight = 35;
    int iSearchx = x;
    int iSearchy = (rect.Height() - iLabelHeight * 2 - iOffsety) / 2;
    m_pRegisterTime = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pRegisterTime->SetText(L"注册时间:");
    m_pRegisterTime->SetAlignMode(Align::right);
    m_pRegisterTime->MoveWindow(x, y);
    m_pRegisterTime->SetDimention(iLabelWidth, iLabelHeight);
    m_vecChildWnd.push_back(m_pRegisterTime);


    //x = iSearchx;
    //y += iLabelHeight + iOffsety;
    x += iOffset + iComboxWidht + 100;
    m_pPatientType = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pPatientType->SetText(L"病人类型:");
    m_pPatientType->SetAlignMode(Align::right);
    m_pPatientType->MoveWindow(x, y);
    m_pPatientType->SetDimention(iLabelWidth, iLabelHeight);
    m_vecChildWnd.push_back(m_pPatientType);

    x += iOffset + iLabelWidth;
    m_pComboxPatientType = new CMyComboxWnd(this, m_pRootWnd, m_refreshImpFun, true);
    // m_pComboxHospital->AddItems(vecType);
    m_pComboxPatientType->MoveWindow(CPoint(x, y));
    m_pComboxPatientType->SetDimention(iComboxWidht, iComboxHeight);
    m_pComboxPatientType->SetLayer(1);
    //显示5条
    m_pComboxPatientType->SetExpandHeight((iComboxHeight + TextSpace) * 4);
    m_pComboxPatientType->OnSize(0, 0);
    m_vecChildWnd.push_back(m_pComboxPatientType);
    CComboxWndManger::Instance().AddWnd(m_pComboxPatientType);
    x = iSearchx;
    //x += iOffset + iLabelWidth;
    y += iLabelHeight + iOffsety;
    int iSearchPos = x;
    m_pHospital = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    m_pHospital->SetText(L"医院:");
    m_pHospital->SetAlignMode(Align::right);
    m_pHospital->MoveWindow(x, y);
    m_pHospital->SetDimention(iLabelWidth, iLabelHeight);
    m_vecChildWnd.push_back(m_pHospital);

    x += iOffset + iLabelWidth;
    m_pComboxHospital = new CMyComboxWnd(this, m_pRootWnd, m_refreshImpFun, true);
    // m_pComboxHospital->AddItems(vecType);
    m_pComboxHospital->MoveWindow(CPoint(x, y));
    m_pComboxHospital->SetDimention(iComboxWidht, iComboxHeight);
    m_pComboxHospital->SetLayer(2);
    //显示5条
    m_pComboxHospital->SetExpandHeight((iComboxHeight + TextSpace) * 4);
    m_pComboxHospital->OnSize(0, 0);
    m_vecChildWnd.push_back(m_pComboxHospital);
    CComboxWndManger::Instance().AddWnd(m_pComboxHospital);

    m_pBtnSearch = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnSearch->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    x = x + iComboxWidht + 110;
    iBtnWidth = 140;
    m_pBtnSearch->MoveWindow(CPoint(x, y));
    m_pBtnSearch->SetDimention(iBtnWidth, iComboxHeight);
    m_pBtnSearch->SetText(CString(L"查询"));
    m_pBtnSearch->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnSearch();
    });
    m_vecChildWnd.push_back(m_pBtnSearch);

    m_pBtnDetailSearch = new CMyButton(this, m_pRootWnd, m_refreshImpFun);
    m_pBtnDetailSearch->SetBkGroundColor(RGB(32, 41, 53), RGB(69, 91, 178));
    x += iBtnWidth + 20;
    m_pBtnDetailSearch->MoveWindow(CPoint(x, y));
    m_pBtnDetailSearch->SetDimention(iBtnWidth, iComboxHeight);
    m_pBtnDetailSearch->SetText(CString(L"详细查询"));
    m_pBtnDetailSearch->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnSearch();
    });
    //m_vecChildWnd.push_back(m_pBtnDetailSearch);



    CRect r;
    m_startTextBox.Create(ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, r, m_pRootWnd, UWM_IDD_PAGESELECT);
    m_startTextBox.ModifyStyleEx(WS_TABSTOP, 0, 0);
    auto dwStyle = m_startTextBox.GetStyle();
    SetWindowLong(m_startTextBox.m_hWnd, GWL_STYLE, dwStyle);
    m_startTextBox.SetWindowTextW(L"开始日期");

    m_startTextBox.SetTextColor(RGB(255, 255, 255));
    m_startTextBox.SetBackColor(RGB(0, 0, 0));
    m_startTextBox.SetFont(&m_EditBoxfont);
   // m_startTextBox.ShowWindow(SW_HIDE);
    ::PostMessage(m_startTextBox.m_hWnd, EM_SETSEL, 0, 0);
    m_startTextBox.PostMessageW(WM_KILLFOCUS, 0, 0);    //失去焦点
    auto finishEditFunc = std::bind(&CDataShowTileWnd::OnStartTimeSet, this, std::placeholders::_1);
    m_startTextBox.SetEditFinishFun(finishEditFunc);
    m_startTextBox.SetClickFun([this](const CString& sText)
    {
        this->OnClickStartTime(sText);
    });

    m_endTextBox.Create(ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER, r, m_pRootWnd, UWM_IDD_PAGESELECT);
    m_endTextBox.ModifyStyleEx(WS_TABSTOP, 0, 0);
    SetWindowLong(m_endTextBox.m_hWnd, GWL_STYLE, dwStyle);
    m_endTextBox.SetWindowTextW(L"结束日期");

    m_endTextBox.SetTextColor(RGB(255, 255, 255));
    m_endTextBox.SetBackColor(RGB(0, 0, 0));
    m_endTextBox.SetFont(&m_EditBoxfont);
   // m_endTextBox.ShowWindow(SW_HIDE);
    ::PostMessage(m_startTextBox.m_hWnd, EM_SETSEL, 0, 0);
    m_endTextBox.PostMessageW(WM_KILLFOCUS, 0, 0);    //失去焦点
    finishEditFunc = std::bind(&CDataShowTileWnd::OnEndTimeSet, this, std::placeholders::_1);
    m_endTextBox.SetEditFinishFun(finishEditFunc);
    m_endTextBox.SetClickFun([this](const CString& sText)
    {
        this->OnClickEndTime(sText);
    });
   

    int w = 325;
    int h = 430;
    m_pCalendar = new CMyCalendar(this, m_pRootWnd, m_refreshImpFun);
    m_pCalendar->MoveWindow(x, iSearchy);
    m_pCalendar->SetDimention(w, h);
    m_pCalendar->SetBkGroundColor(CalendarBkColor);
    m_pCalendar->SetLayer(100);
    m_pCalendar->RegisterSelectDate([this](MyDate date)
    {
        this->OnSelectDate(date);
    });
    //m_pCalendar->SetBkGroundColor(RGB(0, 0, 0));
    m_pCalendar->Initialize();
    m_pCalendar->SetVisible(false);

    m_vecChildWnd.push_back(m_pCalendar);
    CCrosWndManger::Instance().AddWnd(m_pCalendar);
}

void CDataShowTileWnd::OnSize(int cx, int cy)
{
    auto rect = m_pRegisterTime->GetClientRect();
    CRect r;
    int iEditHeight = 30;
    int iOffset = 8;
    int x = rect.right + iOffset;;
    int y = rect.top + 5;
    r.SetRect(x, y, x + m_EditWidth, y + iEditHeight);
    m_startTextBox.MoveWindow(r);

   /* int iLabelHeight = 35;
    y += iLabelHeight + iOffset + 5;*/
    x += m_EditWidth + iOffset;
    r.SetRect(x, y, x + m_EditWidth, y + iEditHeight);
    m_endTextBox.MoveWindow(r);
    m_pCalendar->OnSize(cx, cy);
}

void CDataShowTileWnd::SetVisible(bool bVisible)
{
    if (bVisible)
    {
        m_startTextBox.ShowWindow(SW_SHOW);
        m_endTextBox.ShowWindow(SW_SHOW);
    }
    else
    {
        m_startTextBox.ShowWindow(SW_HIDE);
        m_endTextBox.ShowWindow(SW_HIDE);
    }
    __super::SetVisible(bVisible);
    m_pCalendar->SetVisible(false);
}

void CDataShowTileWnd::OnRegister(vector<CString> vecItems)
{
    CDataInfoRegisterDlg dlg(vecItems);
    auto res = dlg.DoModal();
    if (res == IDOK)
    {
        auto vecItems = dlg.GetItems();
        bool bRepeat = false;
        if (m_funAddNewItem != nullptr)
        {
            bRepeat = m_funAddNewItem(vecItems);
        }
        if (bRepeat)
        {
            auto vecItems = dlg.GetItems();
            vector<CString> vecTemp;
            vecTemp.resize(8);
            vecTemp[1] = vecItems[0];
            vecTemp[2] = vecItems[1];
            vecTemp[3] = vecItems[3];
            vecTemp[4] = vecItems[4];
            vecTemp[5] = vecItems[0];
            vecTemp[6] = vecItems[2];
            vecTemp[7] = vecItems[5];

            OnRegister(vecTemp);
        }
    }
  
}
void CDataShowTileWnd::OnCratePatientType()
{
    CNewPatientTypeDlg dlg;
    auto res = dlg.DoModal();
    //if (res == IDCANCEL)
    //{
    //    return;
    //}
    m_pBottom->ForceUpdateThisPage();
 
   /* auto wsType = dlg.GetType();
    string sType = CW2A(wsType.GetString());
    HttpHelper httpHelper(theApp.m_spClient);
    auto errorCode = httpHelper.AddNewPatientType(sType);
    if (errorCode == ErrorCode::Repeat)
    {                    
        AfxMessageBox(L"输入类型已经存在！");
        return;
    }
    if (errorCode == ErrorCode::Failed)
    {
        AfxMessageBox(L"执行失败！");
        return;
    }


    string sPatientType;
    if (ErrorCode::Success == httpHelper.GetPatientType(sPatientType))
    {
        SetPatientType(sPatientType);
    } */
}
void CDataShowTileWnd::OnExportExcel()
{
    if (m_funExportExcel != nullptr)
    {
        m_funExportExcel();
    }
   

}

void CDataShowTileWnd::OnSearch()
{
    auto s = GetSearchConditon();
    m_funSearch(s);    
}

void CDataShowTileWnd::RegisterAddNewItem(RegisterNewItemFunc f)
{
    m_funAddNewItem = f;
}

void CDataShowTileWnd::RegisterExportExcel(ExportExcelFun f)
{
    m_funExportExcel = f;
}

void CDataShowTileWnd::RegisterSearch(SearchFun f)
{
    m_funSearch = f;
}

void CDataShowTileWnd::SetHospital(const string& sHospital)
{
    CString sTemp = CString(sHospital.c_str());
    auto vecHospital = SplitString(sTemp, ',');
    m_pComboxHospital->AddItems(vecHospital, false);
}

void CDataShowTileWnd::SetPatientType(const string& sPatientType)
{
    CString sTemp = CString(sPatientType.c_str());
    auto vecTemp = SplitString(sTemp, '|');
    vector<CString> vecItems;
    for (size_t i = 0; i < vecTemp.size(); i++)
    {
        auto vecType = SplitString(vecTemp[i], ',');
        vecItems.push_back(vecType[1]);
        m_vecType.push_back(vecType[0]);
    }
    m_pComboxPatientType->AddItems(vecItems, false);
}

SearchCondition CDataShowTileWnd::GetSearchConditon()
{
    CString sHospital = m_pComboxHospital->GetSelectItem();
    CString sPatientType = m_pComboxPatientType->GetSelectItem();
    if (!sPatientType.IsEmpty())
    {
        auto index = m_pComboxPatientType->GetSelectIndex();
        sPatientType = m_vecType[index];
    }
   
    SearchCondition s;
    s.sHospital = CW2A(sHospital.GetString());
    s.sPatientType = CW2A(sPatientType.GetString());
    s.startDate = GetStartTime();
    s.endDate = GetEndTime();
    return s;
}
bool CDataShowTileWnd::IsValidDate(const CString& sDate, Date &date)
{
    vector<WCHAR> vecSplit;
    vecSplit.push_back(L'_');
    vecSplit.push_back(L'.');
    vecSplit.push_back( L'-');
    vecSplit.push_back(L'-');
    vector<CString> vecDate;

    for each (auto item in vecSplit)
    {
        vecDate = SplitString(sDate, item);
        if (vecDate.size() == 3)
        {
            break;
        }
    }

    if (vecDate.size() != 3)
    {
        return false;
    }
    //年，月，日
    int iYear = _wtoi(vecDate[0].GetString());
    if (iYear < 0)
    {
        return false;
    }
    int iMonth = _wtoi(vecDate[1].GetString());
    if (iMonth > 12 || iMonth < 1)
    {
        return false;
    }
    int iDay = _wtoi(vecDate[2].GetString());
    if (iDay < 1 || iDay>31)
    {
        return false;
    }
    if (iMonth == 2)
    {
        int iDaysOfMonth = GetDays(iYear, 2);
        if (iDay > iDaysOfMonth)
        {
            return false;
        }
    }
    date.iYear = iYear;
    date.iMonth = iMonth;
    date.iDay = iDay;
    return true;
}

Date CDataShowTileWnd::GetStartTime()
{
    CString sStartTime;
    m_startTextBox.GetWindowTextW(sStartTime);
    Date startDate;
    if (IsValidDate(sStartTime, startDate))
    {
        return startDate;
    }
    else
    {
        m_startTextBox.SetWindowTextW(L"开始日期");
    }
    return Date();
}

Date CDataShowTileWnd::GetEndTime()
{
    CString sEndTime;
    m_endTextBox.GetWindowTextW(sEndTime);
    Date endDate;
    if (IsValidDate(sEndTime, endDate))
    {
        return endDate;
    }
    else
    {
        m_endTextBox.SetWindowTextW(L"结束日期");
    }
    return Date();
}

void CDataShowTileWnd::OnStartTimeSet(const CString& sText)
{
    m_pCalendar->SetVisible(false);
    Date date;
    auto bIsValidDate = IsValidDate(sText, date);
    if (!bIsValidDate)
    {
        m_startTextBox.SetWindowTextW(L"开始日期");
    }
}

void CDataShowTileWnd::OnEndTimeSet(const CString& sText)
{
    m_pCalendar->SetVisible(false);
    Date date;
    auto bIsValidDate = IsValidDate(sText, date);
    if (!bIsValidDate)
    {
        m_startTextBox.SetWindowTextW(L"结束日期");
    }
}

void CDataShowTileWnd::OnClickStartTime(const CString& sText)
{
    m_bSelectStartTime = true;
    auto rectOri = m_pCalendar->GetClientRect();
    UpdateMyWnd(rectOri);
    auto rect = m_pRegisterTime->GetMyRect();
    CRect r;
    int iEditHeight = 30;
    int iOffset = 20;
    int x = rect.left + iOffset + rect.Width();
    int y = rect.top + iEditHeight + 10;
    m_pCalendar->MoveWindow(x, y);
    m_pCalendar->SetVisible(true);

}

void CDataShowTileWnd::OnClickEndTime(const CString& sText)
{
    m_bSelectStartTime = false;
    auto rectOri = m_pCalendar->GetClientRect();
    UpdateMyWnd(rectOri);
    auto rect = m_pRegisterTime->GetMyRect();
    CRect r;
    int iEditHeight = 30;
    int iOffset = 20;
    int x = rect.left + iOffset + rect.Width();
    int y = rect.top + iEditHeight + 10;
    x += m_EditWidth + iOffset;
    CTime time = CTime::GetCurrentTime();
    MyDate date;
    date.m_iYear = time.GetYear();
    date.m_iMonth = time.GetMonth();
    date.m_iDay = time.GetDay();
    m_pCalendar->SetCurrentDate(date);

    m_pCalendar->SetVisible(true);
}

void CDataShowTileWnd::OnSelectDate(MyDate date)
{
    m_pCalendar->SetVisible(false);
    CString sDate;
    sDate.Format(L"%d-%d-%d", date.m_iYear, date.m_iMonth, date.m_iDay);
    if (m_bSelectStartTime)
    {
        m_startTextBox.SetWindowTextW(sDate);
    }
    else
    {
        m_endTextBox.SetWindowTextW(sDate);
    }

}

vector<CEdit*> CDataShowTileWnd::GetEditCtrl()
{
    vector<CEdit*> vecTemp;
    vecTemp.push_back(&m_startTextBox);
    vecTemp.push_back(&m_endTextBox);
    return vecTemp;
}

void CDataShowTileWnd::ClearSearchCondition()
{
    m_startTextBox.SetWindowTextW(L"开始日期");
    m_endTextBox.SetWindowTextW(L"结束日期");
    m_pComboxPatientType->ClearSelectedItem();
    m_pComboxHospital->ClearSelectedItem();
}

void CDataShowTileWnd::SetBottom(CBottomNavigateWnd* pBottom)
{
    m_pBottom = pBottom;
}
