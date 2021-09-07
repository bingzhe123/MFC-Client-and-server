#ifndef DB_OPERATOR_H_
#define DB_OPERATOR_H_
#include "httplib/httplib.h"
#include "mysql.h"
#include <string>
#include <vector>
#include "commondef.h"

enum class ErrorCode
{
    Success,
    Failed,
    Repeat,
};

using std::vector;
using std::string;
class DBOperator
{
public:
    DBOperator();
    ~DBOperator();
    bool Connect();
    //根据用户名获取
    bool GetUserInfoByUserName(const string & sName, vector<string> &vecUserInfo);
    //根据登录名
    bool GetUserInfoByLoginName(const string& sName, vector<string>& vecUserInfo);
    //根据用户id获取
    bool GetUserInfoByUserID(const string& sID, vector<string>& vecUserInfo);
    bool HasSameName(const string& sName, const string& sLoginName);
    bool HasSameNameExceptUser(const string &sUserID, const string& sName, const string& sLoginName);

    bool AddNewUser(const vector<string>& vecUserInfo, const string& sPassword, string& sSalt);
    bool ModifyUserPassword(const string& sUserID, string& sPassword);
    bool ModifyUserInfo(const string &sUserID, const vector<string>& vecUserInfo, const string& sPassword, string& sSalt);
    //修改用户角色
    ErrorCode ModifyUserRole(const string& sUserID, const string& sNewRole);
    //通过角色id获取角色名
    ErrorCode GetRoleNameByRoleID(const string& sRoleID, string& sRoleName);
    bool DeleteUser(const string& sID);
    //获取所有用户信息
    auto GetAllUserInfo(const string &sUserName, 
        int iStartIndex, int iCount, string & sTotalCount) ->vector< vector<string> >;
    //获取UserId用户信息
    ErrorCode GetUserInfo(const string& sUserID, string &sUserInfo);

    bool AddUserData(const string& sUserName,
        const httplib::Headers & headers,
        const string& sPath,
        int64_t &iNewID, 
        string &sNewID);
    //指定的sDataInfoID中字段：data_id_list新增一个ID
    void AppendDataId2DataInfo(const string& sDataInfoID, const string &sNewDataId);
    //指定的sDataInfoID中字段：data_id_list删除一个ID
    void RemoveDataIdFromeDataInfo(const string& sDataInfoID, const string& sRemoveDataId);

    //新增一条数据：vecItems：姓名，年龄，性别，类型，医院，需求
    ErrorCode AddUserDataItem(const string& sUserID, const vector<string>& vecDataInfo, string &sNewInfo);
    //修改一条数据
    ErrorCode ModifyUserDateaItem(const string& sUserID, const string& sDataID, const vector<string>& vecDataInfo, string& sNewInfo);

    //获取指定用户id的数据注册信息 从第iStartIndex开始获取 iCount条，
    string GetUserDataItems(const string &sUserID, uint64_t iStartIndex,
        uint64_t iCount, string& sTotalCount,
        SearchCondition &searchCondition);
    //获取所有的数据注册信息,从第iStartIndex开始获取 iCount条，
    string GetAllDataItems(uint64_t iStartIndex, uint64_t iCount, string& sTotalCount, SearchCondition &searchCondition);
    //根据dataInfo id获取其下面的所有数据信息列表
    string GetDataListByOwerIDHandler(const string& sDataRegiserID);
    //根据dataId删除数据
    ErrorCode DeleteData(const string& sID);
    string GetDataPath(const string& sID);

    string GetDataName(const string& sID);

    string GetOwerDataInfoId(const string& sID);

    string GetUserDataByID(const string& sID);
    string PacketUserData();
    //获取角色信息，从第iStartIndex开始获取 iCount条
    string GetAllRoleInfo(int iStartIndex, int iCount, string &sTotalCount);
    //修改角色权限
    bool ModifyRoleAuthority(const string& sNewInfo);
    //根据用户ID获取用户的权限
    ErrorCode GetUserAuthority(const string& sUserID,
        uint64_t& uiDataAuthority, uint64_t& uiSystemAuthoriy,
        uint64_t& uiUserCenterAuthoriy, uint64_t& uiStasticsAuthoriy);

    string AddRoleInfo(const string& sRoleDes, const string &sRoleName);
    ErrorCode ModifyRoleInfo(const string& sRoleDes, const string& sRoleName, const string& sUID);
    ErrorCode DeleteRoleInfo(const string& sUID);

    //增加病人类型
    ErrorCode AddPatientType(const string& sPatientType);
    //获取病人类型
    ErrorCode GetPatientType(string& sPatientType);
    //获取医院
    ErrorCode GetHospital(const string& sUID,
        const string & sCategory,
        uint64_t uiDataAuthority,
        string &sHospital);


    ErrorCode DeletePatientType(const string& sUID);

    ErrorCode ModifyPatientType(const string& sUID, const string& sNewType);
private:
    bool IsDataRepeat(const string& sUserID,const vector<string>& vecDataInfo);
    bool IsDataRepeatExceptSelf(const string& sDataID, const vector<string>& vecDataInfo);

private:
    MYSQL m_myConnect;
    std::mutex m_connectMutext;

};
#endif // !DB_OPERATOR_H_


