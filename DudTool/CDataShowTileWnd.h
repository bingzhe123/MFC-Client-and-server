#pragma once
#include "CMyContainerWnd.h"
#include "CMyButton.h"
#include "CMyLable.h"
#include "CMyComboxWnd.h"
#include "CommonDef.h"
#include "CEditEx.h"
#include "CMyCalendar.h"
#include "CommonDef.h"
#include "CBottomNavigateWnd.h"

using UploadFileFun = std::function<void(vector<CString>&)>;
using ExportExcelFun = std::function<void()>;
using SearchFun = std::function<void(SearchCondition&)>;
using RegisterNewItemFunc = std::function<bool(vector<CString>&)>;

class CDataShowTileWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    ~CDataShowTileWnd();
    void Initialize();
    virtual void OnSize(int cx, int cy);
    virtual void SetVisible(bool bVisible);
    void OnRegister(vector<CString> vecItems);
    void OnCratePatientType();
    void OnExportExcel();
    void OnSearch();
    void RegisterAddNewItem(RegisterNewItemFunc f);
    void RegisterExportExcel(ExportExcelFun f);
    void RegisterSearch(SearchFun f);
    void SetHospital(const string& sHospital);
    void SetPatientType(const string& sPatientType);

    SearchCondition GetSearchConditon();
    void OnStartTimeSet(const CString& sText);
    void OnEndTimeSet(const CString& sText);
    void OnClickStartTime(const CString& sText);
    void OnClickEndTime(const CString& sText);

    void OnSelectDate(MyDate date);

    bool IsValidDate(const CString& sDate, Date &date);
    Date GetStartTime();
    Date GetEndTime();
    virtual vector<CEdit*> GetEditCtrl();
    void ClearSearchCondition();
    void SetBottom(CBottomNavigateWnd* pBottom);

private:
    CMyButton *m_pBtnRegisterInfo;
    CMyButton *m_pBtnPatientType;
    CMyButton * m_pBtnExportExcel;
    RegisterNewItemFunc m_funAddNewItem = nullptr;
    ExportExcelFun m_funExportExcel = nullptr;
    SearchFun m_funSearch = nullptr;

    CMyLable* m_pHospital = nullptr;
    CMyComboxWnd* m_pComboxHospital = nullptr;

    CMyLable* m_pPatientType = nullptr;
    CMyComboxWnd* m_pComboxPatientType = nullptr;
    CMyButton * m_pBtnSearch = nullptr;
    CMyButton * m_pBtnDetailSearch = nullptr;


    CMyLable* m_pRegisterTime = nullptr;
    CEditEx  m_startTextBox;  
    CEditEx  m_endTextBox;
    CMyCalendar *m_pCalendar = nullptr;
    int m_EditWidth = 145;
    bool m_bSelectStartTime = false;
    CBottomNavigateWnd* m_pBottom = nullptr;
    vector<CString> m_vecType;


};

