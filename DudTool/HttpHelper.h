#pragma once
#ifndef HTTP_HERLPER_H_
#define HTTP_HERLPER_H_
#include "CommonDef.h"

#include "httplib/httplib.h"

enum class ErrorCode
{
    Success,
    Failed,
    Repeat,
    OldCodeError,
};

class HttpHelper
{
public:
    explicit HttpHelper(std::shared_ptr<httplib::Client> spProxy);
    bool IsValidUser(const string& sUser,
        const string& sPassword,
        string &sID,
        bool & bIsManager,
        UserAuthoriy& userAuthority);
    //获取所有用户信息,sUserName:如果不为空，只返回sUserName
    ErrorCode GetAllUserInfo(const string &sUserName, int iStartIndex, int iCount, string &sAllUserInfo, int &iTotalCount);
    //修改用户角色    sNewInfo: 数据库返回当前用户的新的信息
    ErrorCode ModifyUserRole(const string &sUserID, const string& sNewRole, string &sNewInfo);

    //增加新用户
    ErrorCode AddNewUser(const NewUserInfo &userInfo, string &sNewUserInfo);
    //删除用户
    ErrorCode DeleteUsers(const string& sUsers);
    //修改用户信息
    ErrorCode ModifyUserInfo(const string& sUserID, const NewUserInfo& userInfo, string& sNewUserInfo);
    //获取用户信息
    ErrorCode GetUserInfo(const string& sUserID, string &sUserInfo);
    //修改用户密码
    ErrorCode ModifyUserPassword(const string& sUserID, const string& sOld, const string& sNew);

    //删除用户一条信息记录
    std::map<string, string> DeleteUserData(const string& sUsers, const string& sIDs);
    //当前用户增加一条信息
    ErrorCode AddUserDataInfo(const string& sUserID, const vector<string>& vecItems, string &sReturn);
    //修改一条用户信息
    ErrorCode ModifyUserDataInfo(const string& sUserID, const string& sDataID, const vector<string>& vecItems, string& sReturn);

    //根据用户id和权限获取数据
    string GetUserDataInfo(const string& sUserID,
        SearchCondition &SearConditon,
        int iStartIndex, int iCount, int& iAllCount,
        DataCategory eCategory);
    //获取用户所有的可见数据
    ErrorCode GetAllDataInfo(const string& sUserID, 
        string &sAllDataInfo,
        SearchCondition& searConditon,
        DataCategory eCategory);
    //根据datainfoid获取其下面所有数据列表
    string GetDataList(const string& sOwerDataId);

    //获取iCount条角色信息，从第iStartIndex开始， 返回所有的条目数量
    string GetAllRoleInfo(int iStartIndex, int iCount, int& iAllCount);

    //增加角色,角色描述和角色名字不能重复
    bool AddRoleInfo(const string& sRoleDes, const string& sRoleName, vector<string> &vecNewInfo);
    ErrorCode ModifyRoleInfo(const string& sRoleDes, const string& sRoleName, const string& sUID);
    //删除角色信息
    ErrorCode DeleteRoleInfo(const string& sUID);
    //修改角色权限
    ErrorCode ModifyRoleAuthority(const string& sNewInfo);
    //根据id删除数据
    ErrorCode DeleteData(const string& sUID);
    //增加  病人类型
    ErrorCode AddNewPatientType(const string& sType);
                         
    //获取病人类型；
    ErrorCode GetPatientType(string& sType);
    //删除病人类型；
    ErrorCode DeletePatientType(const string& sID);
    //获取所有医院
    ErrorCode GetHospitalName(const string& sUserID, string& sHospitals, DataCategory eCategory);
    //搜索数据
    ErrorCode SearchDataInfo(const string& sUserID, const SearchCondition& searchContent);
    //获取用户权限
    ErrorCode GetUserAuthority(const string& sUserID, UserAuthoriy& userAuthority);
    //修改病人类型
    ErrorCode ModifyPatientType(const string& sID, const string& sType);

private:
    std::shared_ptr<httplib::Client> m_spClientProxy;
};
#endif // !HTTP_HERLPER_H_
