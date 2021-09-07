#pragma once

#include "resource.h"
#include <afx.h>
//#include <atlstr.h>
#include <array>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "CMyWindow.h"
using std::vector;
using std::string;
using std::map;

#define CA_CERT_FILE "./ca.crt"

#define WM_PROGRESS_UPDATE (WM_USER + 1000 )


const int PerPageItemDataCount = 10;//每页显示几条信息
const int PerPageButtonCount = 5;//每页显示几条信息


const UINT UWM_IDD_PAGESELECT = WM_USER + 1;
const UINT UWM_IDD_USER_SEARCH = WM_USER + 2;
const UINT UWM_IDD_USER_LOGIN_NAME = WM_USER + 3;
const UINT UWM_IDD_USER_ROLE_DESCRIPTION = WM_USER + 4;
const UINT UWM_IDD_USER_ROLE_NAME = WM_USER + 5;

const UINT UWM_IDD_EDIT_ROLE_DESCRIPTION = WM_USER + 6;
const UINT UWM_IDD_EDIT_ROLE_NAME = WM_USER + 7;

const UINT UWM_IDD_EDIT_UPLOAD_NAME = WM_USER + 8;
const UINT UWM_IDD_EDIT_UPLOAD_AGE = WM_USER + 9;
const UINT UWM_IDD_EDIT_UPLOAD_HOSPITAL = WM_USER + 10;
const UINT UWM_IDD_EDIT_UPLOAD_DESCRIPTION = WM_USER + 11;
const UINT UWM_IDD_EDIT_UPLOAD_PROGRESS = WM_USER + 12;

//新增用户
const UINT UWM_IDD_EDIT_ADDUSER_USERNAME = WM_USER + 13;
const UINT UWM_IDD_EDIT_ADDUSER_LOGINNAME = WM_USER + 14;
const UINT UWM_IDD_EDIT_ADDUSER_PASSWORD = WM_USER + 15;
const UINT UWM_IDD_EDIT_ADDUSER_CONFIRM_PASSWORD = WM_USER + 16;
const UINT UWM_IDD_EDIT_ADDUSER_EMAIL = WM_USER + 17;






const int MinDisplayWidth = 1024;
const int MinDisplayHeight = 768;
const int NavigationRegionWidth = 200;
const int NavigationItemHeight = 50;
const int NavigationItSubTextHeight = 40;
const int IntervalBetweenNavigationAndDisplay = 10;
const int IntervalRightDisplay = 10;//和右边的距离
const int DispalyRegionToolBarHeight = 130;
const int ComboxItemHeight = 40;//combox中的item的高度
const int TextSpace = 3;

const COLORREF DataShowBkColor = RGB(32, 41, 53);
const COLORREF CalendarBkColor = RGB(0, 0, 0);
const int DISPLAY_LIST_FONT_SIZE = 25;


enum class WndID
{
    LOGIN_WND_ID,
    LEFT_PANNEL_ID, 
    HOMTE_CHART_ID,
    DATA_SHOW_WND_ID,
    USER_MANAGER_WND_ID,
    ROLE_MANAGER_WND_ID,
    USER_INFO_WND_ID,
    QUIT_BUTTON,
};
enum class LeftPannelChildWndID
{
    HOME_ID,
    DATA_MANAGER_ID,
    DATA_3D_FILE ,
    DATA_ARIGIN,
    USER_MANAGER_ID,
    ROLE_MANAGER_ID,
    USER_INFO_ID
};

const int SUB_TASK_DATA_SIZE = 1024 * 1024;//10K

struct UploadFileInfo
{
    string sFileName;
    string sFilePath;
    int64_t iFileLength;
    string sFileOwerID;//这条数据属于数据库中哪一条
    string sUserOwerID;//这条数据属于哪个用户
    string sServerResponds; //数据库返回信息
};

struct UserDataInfo
{
    string sID;
    string sName;
    string sAge;
    string sGender;
    string sHospital;
    string sType;
    string sTime;
    string sDescription;
    string sFilePath;
};

enum class UserInfoKey
{
    userName,
    passWord,
    email,
    Role
};

enum class RoleDef
{
    developer = 0,
    tester,
    serviceStaff,
    doctor,
};

enum class DataAuthorityDef
{
    selfMRDataVisible = 0x00000001,
    self3DDataVisible = 0x00000002,
    selfAriginDataVisible = 0x00000004,
    allMRDataVisible = 0x10000008,
    all3DDataVisible = 0x10000010,
    allAriginDataVisible = 0x10000020,
    allDataMask = 0x10000000,
};

enum class SystemAuthorityDef
{
    userManager = 0x00000001,
    roleManager = 0x00000002,
};

enum class UserCenterAuthorityDef
{
    userInfo = 0x00000001,
};

enum class StasticsAuthorityDef
{
    hospitalStatic = 0x00000001,
    typeStastic = 0x00000002,
    reconStastic = 0x00000004,
    ageStastic = 0x00000008,
};

enum class DataCategory
{
    Data_COMMON = 1,
    Data_3D = 2,
    Data_Arigin = 3
};


struct UserAuthoriy
{
    uint64_t uiDataAuthority = 0;
    uint64_t uiSystemAuthoriy = 0;
    uint64_t uiUserCenterAuthoriy = 0;
    uint64_t uiStasticsAuthoriy = 0;
};


struct NewUserInfo
{
    CString userName;
    CString loginName;
    CString password;
    CString confirmPassword;
    CString email;
};


struct Date
{
    int iYear = -1;
    int iMonth = -1;
    int iDay = -1;
    bool isValid()
    {
        return  iYear != -1 && iMonth != -1 && iDay != -1;
    }
    string ToString()
    {
        string sTemp = std::to_string(iYear) + "&" + std::to_string(iMonth) + "&" + std::to_string(iDay);
        return sTemp;
    }
};
struct SearchCondition
{
    string sHospital;
    string sPatientType;
    Date startDate;
    Date endDate;

    SearchCondition()
    {
        sHospital = "";
        sPatientType = "";
    }
    bool IsConditionEmpty()
    {
        return sHospital.empty() && sPatientType.empty() && !startDate.isValid() && !endDate.isValid();
    }
};


static vector<string> SplitString(const string& str, char splitchar)
{
    std::istringstream is(str);
    string temp;
    vector<string> vecResult;
    while (std::getline(is, temp, splitchar))
        vecResult.push_back(temp);
    return std::move(vecResult);
}


static string JoinString(const vector<string>& vecString, char splitchar)
{
    string sTemp;
    for (auto it = vecString.begin(); it != vecString.end();)
    {
        sTemp += *it;
        it++;
        if (it != vecString.end())
        {
            sTemp += splitchar;
        }
    }
    return std::move(sTemp);
}

static vector<CString> SplitString(CString strSrc, wchar_t division)
{

    if (strSrc.IsEmpty())
    {
        return vector<CString>();
    }
    vector <CString> vecRes;
    int nPos = strSrc.Find(division);
    CString strLeft = _T("");

    while (0 <= nPos)
    {
        strLeft = strSrc.Left(nPos);
        //if (strLeft.IsEmpty())
        {
            vecRes.push_back(strLeft);
        }

        strSrc = strSrc.Right(strSrc.GetLength() - nPos - 1);
        nPos = strSrc.Find(division);
    }
    //if (!strSrc.IsEmpty())
    {
        vecRes.push_back(strSrc);
    }

    return std::move(vecRes);
}

static vector<string> CString2String(const vector<CString>& vecSor)
{
    vector<string> vecDes;
    for each (auto  item in vecSor)
    {
        vecDes.emplace_back(CW2A(item.GetString()));
    }
    return std::move(vecDes);
}

static vector<CString> String2CString(const vector<string>& vecSor)
{
    vector<CString> vecDes;
    for each (auto  item in vecSor)
    {
        vecDes.emplace_back(CString(item.c_str()));
    }
    return std::move(vecDes);
}

 template<typename T>
static bool IsMaskValid(UINT value, T mask)
{
    UINT uValue = (UINT)value;
    UINT uMask = (UINT)mask;
    return uValue & uMask;
}

static std::vector<CMyWindow*> SortByLayer(const map<WndID, CMyWindow*>& mapWnd)
{
    vector<CMyWindow*> vecTemp;

    for each (auto item in mapWnd)
    {
        if (item.second->IsVisible() == false)
        {
            continue;
        }
        auto pWnd = item.second;
        vecTemp.push_back(item.second);
    }
    std::sort(vecTemp.begin(), vecTemp.end(), [](CMyWindow* a, CMyWindow* b)
    {
        return a->GetLayer() < b->GetLayer();
    });
    return std::move(vecTemp);
}

static std::vector<CMyWindow*> SortByLayer(const vector<CMyWindow*>& vecWnd)
{
    vector<CMyWindow*> vecTemp = vecWnd;
 
    std::sort(vecTemp.begin(), vecTemp.end(), [](CMyWindow* a, CMyWindow* b)
    {
        return a->GetLayer() < b->GetLayer();
    });
    return std::move(vecTemp);
}

static void RefreshWithEdit(CDialogEx *pDlg, CDC* pDC, CRect rect, vector<CEdit*> vecEdit, CDC &memDC)
{
    int l = 0, t = 0, r = rect.right, b = rect.bottom;
    int n_t = 0;
    int n_b = rect.bottom;
    size_t iSize = vecEdit.size();
    for (size_t i = 0; i < iSize; i++)
    {
        auto pEdit = vecEdit[i];
        CRect r;
        pEdit->GetWindowRect(r);
        pDlg->ScreenToClient(r);

        CRect subRect;
        subRect.top = t;
        subRect.bottom = r.top;
        subRect.left = l;
        subRect.right = rect.right;
        pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);
        
        subRect.top = r.top;
        subRect.bottom = r.bottom;
        subRect.left = l;
        subRect.right = r.left;
        pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);

        subRect.top = r.top;
        subRect.bottom = r.bottom;
        subRect.left = r.right;
        subRect.right = rect.right;
        pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);

        if (i == iSize - 1)
        {
            subRect.top = r.bottom;
            subRect.bottom = rect.bottom;
            subRect.left = l;
            subRect.right = rect.right;
            pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);
        }
        else
        {
            auto pEditNext = vecEdit[i + 1];
            CRect rNext;
            pEditNext->GetWindowRect(rNext);
            pDlg->ScreenToClient(rNext);
            t = r.bottom;
        }
    }
}

static bool IsIntersect(CRect r1, CRect r2)
{
    int iInstanceW = abs(r1.left - r2.left) + abs(r1.right - r2.right);
    auto iInstanceH = abs(r1.top - r2.top) + abs(r1.bottom - r2.bottom);
    if (iInstanceW < r1.Width() + r2.Width() && iInstanceH < r1.Height() + r2.Height())
    {
        return true;
    }
    return false;
}
static void RefreshWithEdit_Horizontal(CDialogEx* pDlg, CDC* pDC, CRect rect, vector<CEdit*> vecEdit, CDC& memDC)
{
    int l = rect.left, t = rect.top, r = rect.right, b = rect.bottom;
    int n_t = 0;
    int n_b = rect.bottom;
    size_t iSize = vecEdit.size();
    auto pEdit = vecEdit[0];
    CRect rEdit;
    pEdit->GetWindowRect(rEdit);
    pDlg->ScreenToClient(rEdit);
    
    for (auto it = vecEdit.begin(); it != vecEdit.end();  )
    {
        auto pEdit = *it;
        CRect rEdit;
        pEdit->GetWindowRect(rEdit);
        pDlg->ScreenToClient(rEdit);
        if (!IsIntersect(rEdit, rect))
        {
            it = vecEdit.erase(it);
        }
        else
        {
            it++;
        }
    }

    if (vecEdit.size() == 0 )
    {
        pDC->BitBlt(l, t, rect.Width(), rect.Height(), &memDC, l, t, SRCCOPY);
        return;
    }
   

    CRect subRect;
    subRect.top = t;
    subRect.bottom = rEdit.top;
    subRect.left = l;
    subRect.right = r;
    if (IsIntersect(subRect, rect))
    {
        pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);
    }
    
    subRect.top = rEdit.top;
    subRect.bottom = rEdit.bottom;
    subRect.left = l;
    subRect.right = rEdit.left;
    if (IsIntersect(subRect, rect))
    {
        pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);
    }

    for (size_t i = 0; i < iSize; i++)
    {
        auto pEdit = vecEdit[i];
        CRect rEdit;
        pEdit->GetWindowRect(rEdit);
        pDlg->ScreenToClient(rEdit);

        subRect.top = rEdit.top;
        subRect.bottom = rEdit.bottom;
        subRect.left = rEdit.right;
        
        if (i == iSize - 1)
        {
            subRect.right = rect.right;
        }
        else
        {
            auto pEditNext = vecEdit[i + 1];
            CRect rNext;
            pEditNext->GetWindowRect(rNext);
            pDlg->ScreenToClient(rNext);
            subRect.right = rNext.left;
        }
        if (IsIntersect(subRect, rect))
        {
            pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);
        }
    }
    subRect.top = rEdit.bottom;
    subRect.bottom = b;
    subRect.left = l;
    subRect.right = r;
    pDC->BitBlt(subRect.left, subRect.top, subRect.Width(), subRect.Height(), &memDC, subRect.left, subRect.top, SRCCOPY);
}



const int days_in_months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };//定义月份的数组，函数运行可以直接返回一个数值.

static int GetDays(int iYear, int iMonth)
{
    iMonth = iMonth - 1;
    int iDays = 30;
    if (1 == iMonth)
        iDays = ((0 == iYear % 4) && (0 != (iYear % 100))) || (0 == iYear % 400) ? 29 : 28;
    else
        iDays = days_in_months[iMonth];
    return iDays;
}