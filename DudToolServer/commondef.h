#ifndef COMMON_DEF_H_
#define COMMON_DEF_H_

#include <string>
#include <vector>
#include <sstream>
using std::string;
using std::vector;

#define SERVER_CERT_FILE "./server.pem"
#define SERVER_PRIVATE_KEY_FILE "./server.key.pem"



enum class DataAuthorityDef
{
    selfMRDataVisible =     0x00000001,
    self3DDataVisible =     0x00000002,
    selfAriginDataVisible = 0x00000004,
    allMRDataVisible =      0x10000008,
    all3DDataVisible =      0x10000010,
    allAriginDataVisible =  0x10000020,
    allDataMask =           0x10000000,
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
struct Date
{
    int iYear = -1;
    int iMonth = -1;
    int iDay = -1;
    bool isValid()
    {
        return  iYear != -1 && iMonth != -1 && iDay != -1;
    }
    string ToSqlSearString()
    {
        char buffer[20];
        memset(buffer, 0, sizeof(buffer));
        sprintf_s(buffer, "%04d", iYear);
        string sTemp = string(buffer) + "-";
        //+std::to_string(iMonth) + "-" + std::to_string(iDay);
        memset(buffer, 0, sizeof(buffer));
        sprintf_s(buffer, "%02d", iMonth);
        sTemp += string(buffer) + "-";
        memset(buffer, 0, sizeof(buffer));
        sprintf_s(buffer, "%02d", iDay);
        sTemp += string(buffer);

        return sTemp;
    }
};


static vector<string> SplitString(const string &_strSrc, wchar_t division)
{

    auto strSrc = _strSrc;
    if (strSrc.empty())
    {
        return vector<string>();
    }
    vector <string> vecRes;
    int nPos = strSrc.find_first_of(division);
    string strLeft = ("");

    while (0 <= nPos)
    {
        strLeft = strSrc.substr(0, nPos);
        //if (strLeft.IsEmpty())
        {
            vecRes.push_back(strLeft);
        }

        strSrc = strSrc.substr(nPos+1);
        nPos = strSrc.find_first_of(division);
    }
    //if (!strSrc.IsEmpty())
    {
        vecRes.push_back(strSrc);
    }

    return std::move(vecRes);
}

static string JoinString(const vector<string>& vecString, char splitchar)
{
    string sTemp;
    for(auto it = vecString.begin(); it != vecString.end();)
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

static string  getCurrentTimeStr()
{
    time_t t = time(NULL);
    tm _tm;
    localtime_s(&_tm, &t);
    char ch[64] = { 0 };
    strftime(ch, sizeof(ch) - 1, "%Y-%m-%d", &_tm);     //年-月-日
    return ch;
}


struct SearchCondition
{
    string sHospital;
    string sPatientType;
    Date startDate;
    Date endDate;
    string sCategory;
    uint64_t dataAuthoriy;

    SearchCondition()
    {
        sHospital = "";
        sPatientType = "";
        sCategory = "1";
    }
    bool IsConditionEmpty()
    {
        return sHospital.empty() && sPatientType.empty() && !startDate.isValid() && !endDate.isValid();
    }
    static Date CovertDate(const string& sDate)
    {
        Date d;
        auto vecDate = SplitString(sDate, '&');
        d.iYear = atoi(vecDate[0].c_str());
        d.iMonth = atoi(vecDate[1].c_str());
        d.iDay = atoi(vecDate[2].c_str());
        return d;
    }

};

static char* Utf8ToGb2312(const char* p)                                               //unicode转国标
{
    DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, NULL, 0);
    char* psText;                                                         //存储文件名
    wchar_t* pwText = (wchar_t*)malloc(dwNum * sizeof(wchar_t));
    dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, pwText, dwNum);
    dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL);
    psText = (char*)malloc(dwNum * sizeof(char));
    dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, psText, dwNum, NULL, NULL);
    free(pwText);
    return psText;
}

static char* Utf8ToGb2312(const string s)                                               //unicode转国标
{
    const char* p = s.c_str();
    DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, NULL, 0);
    char* psText;                                                         //存储文件名
    wchar_t* pwText = (wchar_t*)malloc(dwNum * sizeof(wchar_t));
    dwNum = MultiByteToWideChar(CP_UTF8, 0, p, -1, pwText, dwNum);
    dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL);
    psText = (char*)malloc(dwNum * sizeof(char));
    dwNum = WideCharToMultiByte(CP_ACP, 0, pwText, -1, psText, dwNum, NULL, NULL);
    free(pwText);
    return psText;
}
#endif