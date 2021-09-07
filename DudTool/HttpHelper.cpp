#pragma once
#include "pch.h"
#include "httpHelper.h"
#include "picosha2.h"

#include <locale>
#include <codecvt>

#include <iostream>
//char* Gb2312ToUtf8(const char* gb2312)
//{
//    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
//    wchar_t* wstr = new wchar_t[len + 1];
//    memset(wstr, 0, len + 1);
//    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
//    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
//    char* str = new char[len + 1];
//    memset(str, 0, len + 1);
//    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
//    if (wstr) delete[] wstr;
//    return str;
//}

//将string转换成wstring  
static std::wstring _string2wstring(std::string str)
{
    std::wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}
std::string Gb2312ToUtf8(const char* gb2312)
{
    return string(gb2312);
    //std::string ret;
    //const std::wstring wstr = _string2wstring(gb2312);
    //try {
    //    std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
    //    ret = wcv.to_bytes(wstr);
    //}
    //catch (const std::exception & e) {
    //    std::cerr << e.what() << std::endl;
    //}
    //return ret;
}

//void Gb2312ToUtf8(const char* pcGb2312, std::string& strUtf8)
//
//{
//    //wchar_t pcUnicode[CHAR_BUFSIZE];
//
//    //gb2312 to unicode
//
//    int nUnicodeLen = MultiByteToWideChar(CP_ACP, 0, pcGb2312, -1, NULL, 0);
//
//    wchar_t* pcUnicode = new wchar_t[nUnicodeLen + 1];
//    memset(pcUnicode, 0, nUnicodeLen * 2 + 2);
//
//    MultiByteToWideChar(CP_ACP, 0, pcGb2312, -1, pcUnicode, nUnicodeLen);
//
//
//    //unicode to utf8
//
//    int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, pcUnicode, -1, NULL, 0, NULL, NULL);
//
//    char* pcUtf8 = new char[nUtf8Len + 1];
//    memset(pcUtf8, 0, nUtf8Len + 1);
//
//    WideCharToMultiByte(CP_UTF8, 0, pcUnicode, -1, pcUtf8, nUtf8Len, NULL, NULL);
//
//    strUtf8 = pcUtf8;
//    delete[] pcUtf8;
//    delete[] pcUnicode;
//}
//

HttpHelper::HttpHelper(std::shared_ptr<httplib::Client> spProxy)
{
    m_spClientProxy = spProxy;
}

bool HttpHelper::IsValidUser(const string& sUser,
    const string& sPassword,
    string &sID,
    bool & bIsManager,
    UserAuthoriy& userAuthority)
{
   //bIsManager = true;

   // return true;  
    bIsManager = false;

    httplib::Headers header;
   // header.emplace("ForJson", "ForJson");
    //header.emplace("User", sUser);
    //std::string sCodePassword = picosha2::hash256_hex_string(sPassword);
    //header.emplace("Password", sCodePassword);

    header.emplace("User", Gb2312ToUtf8(sUser.c_str()));
    std::string sCodePassword = picosha2::hash256_hex_string(sPassword);
    header.emplace("Password", Gb2312ToUtf8(sCodePassword.c_str()));

    auto res = m_spClientProxy->Get("/VerifyUser", header);
    bool bSucess = false;
    if (res)
    {
        if (res->body == "pass")
        {
            bSucess = true;

            auto it = res->headers.find("IsManager");
            if (it != res->headers.end())
            {
                bIsManager = it->second == "1";
            }

            it = res->headers.find("id");
            if (it != res->headers.end())
            {
                sID = it->second;
            }

            char* pStopString;
            string sTemp;
            it = res->headers.find("DataAuthority");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiDataAuthority = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }

            it = res->headers.find("SystemAuthoriy");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiSystemAuthoriy = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }

            it = res->headers.find("UserCenterAuthoriy");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiUserCenterAuthoriy = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }

            it = res->headers.find("uiStasticsAuthoriy");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiStasticsAuthoriy = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }
        }
    }
    return bSucess;
}

ErrorCode HttpHelper::GetAllUserInfo(const string& sUserName, int iStartIndex, int iCount, string& sAllUserInfo, int& iTotalCount)
{
    httplib::Headers header;
    header.emplace("StartIndex", Gb2312ToUtf8(std::to_string(iStartIndex).c_str()));
    header.emplace("Count", Gb2312ToUtf8(std::to_string(iCount).c_str()));
    header.emplace("UserName", Gb2312ToUtf8(sUserName.c_str()));

    auto res = m_spClientProxy->Get("/GetAllUserInfo", header);
    if (res == nullptr || res->body == "failed")
    {
        return ErrorCode::Failed;
    }

    if (res->body == "pass")
    {
        auto it = res->headers.find("TotalCount");
        if (it != res->headers.end())
        {
            iTotalCount = atoi(it->second.c_str());
        }
        it = res->headers.find("AllUserInfo");
        if (it != res->headers.end())
        {
            sAllUserInfo = it->second;
        }
    }
    return ErrorCode::Success;
}
   
ErrorCode HttpHelper::ModifyUserRole(const string& sUserID, const string& sNewRole, string& sNewInfo)
{
    httplib::Headers header;
    header.emplace("NewRole", Gb2312ToUtf8(sNewRole.c_str()));
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));

    auto res = m_spClientProxy->Get("/ModifyUserRole", header);

    if (res->body == "pass")
    {
        auto it = res->headers.find("NewUserInfo");
        if (it != res->headers.end())
        {
            sNewInfo = it->second;
        }
        return ErrorCode::Success;
    }
    return ErrorCode::Failed;
}


ErrorCode HttpHelper::AddNewUser(const NewUserInfo& userInfo, string &sNewUserInfo)
{
    string sNewInfo = CW2A(userInfo.userName.GetString()) + ",";
    sNewInfo += CW2A(userInfo.loginName.GetString()) + ",";
    string sTemp = CW2A(userInfo.password.GetString());
    string passWord = picosha2::hash256_hex_string(sTemp);
    sNewInfo += passWord + ",";
    sNewInfo += CW2A(userInfo.email.GetString());

    httplib::Headers header;
    header.emplace("NewUserInfo", Gb2312ToUtf8(sNewInfo.c_str()));

    auto res = m_spClientProxy->Get("/AddUser", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("UserInfo");
            if (it != res->headers.end())
            {
                sNewUserInfo = it->second;
                return ErrorCode::Success;
            }
        }
        if (res->body == "Repeat")
        {
            return ErrorCode::Repeat;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::DeleteUsers(const string& sUsers)
{
    httplib::Headers header;
    header.emplace("Users", Gb2312ToUtf8(sUsers.c_str()));
    auto res = m_spClientProxy->Get("/DeleteUser", header);
    std::map<string, string> vecRemoveInfo;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            return ErrorCode::Success;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::ModifyUserInfo(const string& sUserID, const NewUserInfo& userInfo, string& sNewUserInfo)
{
    string sNewInfo = CW2A(userInfo.userName.GetString()) + ",";
    sNewInfo += CW2A(userInfo.loginName.GetString()) + ",";
    string sTemp = CW2A(userInfo.password.GetString());
    string passWord = sTemp;
    if (!sTemp.empty())
    {
        passWord = picosha2::hash256_hex_string(sTemp);
    }
    sNewInfo += passWord + ",";
    sNewInfo += CW2A(userInfo.email.GetString());

    httplib::Headers header;
    header.emplace("NewUserInfo", Gb2312ToUtf8(sNewInfo.c_str()));
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));
    auto res = m_spClientProxy->Get("/ModifyUser", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("UserInfo");
            if (it != res->headers.end())
            {
                sNewUserInfo = it->second;
                return ErrorCode::Success;
            }
        }
        if (res->body == "Repeat")
        {
            return ErrorCode::Repeat;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::GetUserInfo(const string& sUserID, string &sUserInfo)
{
    httplib::Headers header;
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));
    auto res = m_spClientProxy->Get("/GetUserInfo", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("UserInfo");
            if (it != res->headers.end())
            {
                sUserInfo = it->second;
                return ErrorCode::Success;
            }
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::ModifyUserPassword(const string& sUserID, const string& sOld, const string& sNew)
{
    httplib::Headers header;
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));
    string newPassWord = picosha2::hash256_hex_string(sNew);
    header.emplace("NewPassword", Gb2312ToUtf8(newPassWord.c_str()));
    string oldPassWord = picosha2::hash256_hex_string(sOld);
    header.emplace("OldPassword", Gb2312ToUtf8(oldPassWord.c_str()));
    auto res = m_spClientProxy->Get("/ModifyUserPassword", header);
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            return ErrorCode::Success;
        }
        if (res->body == "OldFailed")
        {
            return ErrorCode::OldCodeError;
        }
    }
    return ErrorCode::Failed;
}


std::map<string, string> HttpHelper::DeleteUserData(const string& sUser, const string& sIDs)
{
    httplib::Headers header;
    header.emplace("User", Gb2312ToUtf8(sUser.c_str()));
    header.emplace("DataIDs", Gb2312ToUtf8(sIDs.c_str()));

    auto res = m_spClientProxy->Get("/DeleteUserData", header);
    std::map<string, string> vecRemoveInfo;
    if (res != nullptr)
    {
        auto vecResult = SplitString(res->body, ',');
        for (int i = 0; i < vecResult.size(); i += 2)
        {
            vecRemoveInfo.emplace(vecResult[i], vecResult[i + 1]);
        }
    }
    return vecRemoveInfo;
}

ErrorCode HttpHelper::AddUserDataInfo(const string& sUserID, const vector<string>& vecItems, string &sReturn)
{
    httplib::Headers header;
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));
    auto sTemp = JoinString(vecItems, ',');
    header.emplace("DataInfo", Gb2312ToUtf8(sTemp.c_str()));
    auto res = m_spClientProxy->Get("/AddUserDataInfo", header);
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("DataInfo");
            sReturn = it->second;
            return ErrorCode::Success;
        }
        if (res->body == "Repeat")
        {
            return ErrorCode::Repeat;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::ModifyUserDataInfo(const string& sUserID, const string& sDataID, const vector<string>& vecItems, string& sReturn)
{
    httplib::Headers header;
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));
    header.emplace("DataID", Gb2312ToUtf8(sDataID.c_str()));
    auto sTemp = JoinString(vecItems, ',');
    header.emplace("DataInfo", Gb2312ToUtf8(sTemp.c_str()));
    auto res = m_spClientProxy->Get("/ModifyUserDataInfo", header);
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("DataInfo");
            sReturn = it->second;
            return ErrorCode::Success;
        }
        else if (res->body == "Repeat")
        {
            return ErrorCode::Repeat;
        }
    }
    return ErrorCode::Failed;
}

string HttpHelper::GetUserDataInfo(const string& sUserID,
    SearchCondition& searConditon,
    int iStartIndex, int iCount, int& iAllCount,
    DataCategory eCategory)
{
    httplib::Headers header;
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));
    header.emplace("StartIndex", std::to_string(iStartIndex));
    header.emplace("Count", std::to_string(iCount));

    string sHospital = (searConditon.sHospital);
    string sPatientType = (searConditon.sPatientType);
                                                       


    header.emplace("Hospital", Gb2312ToUtf8(sHospital.c_str()));
    header.emplace("PatientType", Gb2312ToUtf8(sPatientType.c_str()));
    header.emplace("StartDate", searConditon.startDate.ToString());
    header.emplace("EndDate", searConditon.endDate.ToString());
    header.emplace("Category", std::to_string((int)eCategory));

    auto res = m_spClientProxy->Get("/GetUserData", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto itCount = res->headers.find("AllCount");
            iAllCount = atoi(itCount->second.c_str());
            auto it = res->headers.find("DataInfo");
            return it->second;
        }
    }
    return "";
}

ErrorCode HttpHelper::GetAllDataInfo(const string& sUserID,
    string& sAllDataInfo, SearchCondition& searConditon,
    DataCategory eCategory)
{
    httplib::Headers header;
    header.emplace("UserID", Gb2312ToUtf8(sUserID.c_str()));
    string sHospital = (searConditon.sHospital);
    string sPatientType = (searConditon.sPatientType);




    header.emplace("Hospital", Gb2312ToUtf8(sHospital.c_str()));



    header.emplace("PatientType", Gb2312ToUtf8(sPatientType.c_str()));
    header.emplace("Category", std::to_string((int)eCategory));

    auto res = m_spClientProxy->Get("/GetUserAllData", header);

    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("DataInfo");
            sAllDataInfo =  it->second;
            return ErrorCode::Success;
        }
    }
    return ErrorCode::Failed;
}

string HttpHelper::GetDataList(const string& sOwerDataId)
{
    httplib::Headers header;
    header.emplace("OwerDataID", sOwerDataId);
    auto res = m_spClientProxy->Get("/GetDataList", header);
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("DataList");
            return it->second;
        }
    }
    return "";
}

string HttpHelper::GetAllRoleInfo(int iStartIndex, int iCount, int &iAllCount)
{
    httplib::Headers header;
    header.emplace("StartIndex", std::to_string(iStartIndex));
    header.emplace("Count", std::to_string(iCount));
    auto res = m_spClientProxy->Get("/GetAllRoleInfo", header);
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto itCount = res->headers.find("AllCount");
            iAllCount = atoi(itCount->second.c_str());
            auto it = res->headers.find("AllRoleInfo");
            return it->second;
        }
        else
        {
            auto itCount = res->headers.find("AllCount");
            if (itCount != res->headers.end())
            {
                iAllCount = atoi(itCount->second.c_str());
            }
        }
    }
    return "";
}

ErrorCode HttpHelper::ModifyRoleAuthority(const string& sNewInfo)
{
    httplib::Headers header;
    header.emplace("RoleInfo", Gb2312ToUtf8(sNewInfo.c_str()));
    auto res = m_spClientProxy->Get("/ModifyRoleAuthority", header);
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            return ErrorCode::Success;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::DeleteData(const string& sUID)
{
    httplib::Headers header;
    header.emplace("UID", sUID);
    auto res = m_spClientProxy->Get("/DeleteData", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            return ErrorCode::Success;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::AddNewPatientType(const string& sType)
{
    httplib::Headers header;
    header.emplace("Type", Gb2312ToUtf8(sType.c_str()));
    auto res = m_spClientProxy->Get("/AddPatientType", header);
    string sID;
    if (res == nullptr)
    {
        return ErrorCode::Failed;
    }

    if (res->body == "pass")
    {
        return ErrorCode::Success;
    }
    else if (res->body == "Repeat")
    {
        return ErrorCode::Repeat;
    }
    return ErrorCode::Success;
}

ErrorCode HttpHelper::GetPatientType(string& sType)
{
    httplib::Headers header;
    auto res = m_spClientProxy->Get("/GetPatientType", header);
    string sID;
    if (res == nullptr)
    {
        return ErrorCode::Failed;
    }

    if (res->body == "pass")
    {
        auto it = res->headers.find("Type");
        if (it == res->headers.end())
        {
            return ErrorCode::Failed;
        }
        sType = it->second;
        return ErrorCode::Success;
    }                     
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::DeletePatientType(const string& sID)
{
    httplib::Headers header;
    header.emplace("UID", sID);
    auto res = m_spClientProxy->Get("/DeletePatientType", header);
    if (res == nullptr)
    {
        return ErrorCode::Failed;
    }

    if (res->body == "pass")
    {
        return ErrorCode::Success;
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::GetHospitalName(const string& sUserID, string& sHospitals,
    DataCategory eCategory)
{
    httplib::Headers header;
    header.emplace("UID", sUserID);
    header.emplace("Category", std::to_string(int(eCategory)));
    auto res = m_spClientProxy->Get("/GetHospital", header);
    string sID;
    if (res == nullptr)
    {
        return ErrorCode::Failed;
    }

    if (res->body == "pass")
    {
        auto it = res->headers.find("Hospital");
        if (it == res->headers.end())
        {
            return ErrorCode::Failed;
        }
        sHospitals = it->second;
        return ErrorCode::Success;
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::SearchDataInfo(const string& sUserID, const SearchCondition& searchContent)
{
    httplib::Headers header;
    header.emplace("UID", sUserID);
    string sHospital = (searchContent.sHospital);
    string sPatientType = (searchContent.sPatientType);

    header.emplace("Hospital", Gb2312ToUtf8(sHospital.c_str()));
    header.emplace("PatientType", Gb2312ToUtf8(sPatientType.c_str()));
    auto res = m_spClientProxy->Get("/SearchDataInfo", header);
    string sID;
    if (res == nullptr)
    {
        return ErrorCode::Failed;
    }

    /*if (res->body == "pass")
    {
        auto it = res->headers.find("Hospital");
        if (it == res->headers.end())
        {
            return ErrorCode::Failed;
        }
        sHospitals = it->second;
        return ErrorCode::Success;
    }*/
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::GetUserAuthority(const string& sUserID, UserAuthoriy& userAuthority)
{
    httplib::Headers header;
    header.emplace("UID", sUserID);

    auto res = m_spClientProxy->Get("/GetUserAuthority", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            char* pStopString;
            string sTemp;
            auto it = res->headers.find("DataAuthority");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiDataAuthority = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }

            it = res->headers.find("SystemAuthoriy");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiSystemAuthoriy = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }

            it = res->headers.find("UserCenterAuthoriy");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiUserCenterAuthoriy = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }

            it = res->headers.find("uiStasticsAuthoriy");
            if (it != res->headers.end())
            {
                sTemp = it->second;
                userAuthority.uiStasticsAuthoriy = _strtoui64(sTemp.c_str(), &pStopString, 10);
            }
            return ErrorCode::Success;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::ModifyPatientType(const string& sID, const string& sType)
{
    httplib::Headers header;
    header.emplace("UID", Gb2312ToUtf8(sID.c_str()));
    header.emplace("PatientType", Gb2312ToUtf8(sType.c_str()));
    auto res = m_spClientProxy->Get("/ModifyPatientType", header);
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            return ErrorCode::Success;
        }
        if (res->body == "Repeat")
        {
            return ErrorCode::Repeat;

        }
    }
    return ErrorCode::Failed;
}


bool HttpHelper::AddRoleInfo(const string& sRoleDes, const string& sRoleName, vector<string>& vecNewInfo)
{
    httplib::Headers header;
    header.emplace("RoleDes", Gb2312ToUtf8(sRoleDes.c_str()));
    header.emplace("RoleName", Gb2312ToUtf8(sRoleName.c_str()));

    auto res = m_spClientProxy->Get("/AddRole", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            auto it = res->headers.find("RoleInfo");
            if (it != res->headers.end())
            {
                vecNewInfo = SplitString(it->second, ',');
                return true;
            }
        }
        if (res->body == "Repeat")
        {
            vecNewInfo.push_back("Repeat");
            return false;
        }
    }
    return false;
}

ErrorCode HttpHelper::ModifyRoleInfo(const string& sRoleDes, const string& sRoleName, const string& sUID)
{
    httplib::Headers header;
    header.emplace("RoleDes", Gb2312ToUtf8(sRoleDes.c_str()));
    header.emplace("RoleName", Gb2312ToUtf8(sRoleName.c_str()));
    header.emplace("UID", sUID);
    auto res = m_spClientProxy->Get("/ModifyRoleInfo", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            return ErrorCode::Success;
        }
        if (res->body == "Repeat")
        {
             
            return ErrorCode::Repeat;
        }
    }
    return ErrorCode::Failed;
}

ErrorCode HttpHelper::DeleteRoleInfo(const string& sUID)
{
    httplib::Headers header;
    header.emplace("UID", sUID);
    auto res = m_spClientProxy->Get("/DeleteRoleInfo", header);
    string sID;
    if (res != nullptr)
    {
        if (res->body == "pass")
        {
            return ErrorCode::Success;
        }
    }
    return ErrorCode::Failed;
}

