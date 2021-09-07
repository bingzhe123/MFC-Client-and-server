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
    //��ȡ�����û���Ϣ,sUserName:�����Ϊ�գ�ֻ����sUserName
    ErrorCode GetAllUserInfo(const string &sUserName, int iStartIndex, int iCount, string &sAllUserInfo, int &iTotalCount);
    //�޸��û���ɫ    sNewInfo: ���ݿⷵ�ص�ǰ�û����µ���Ϣ
    ErrorCode ModifyUserRole(const string &sUserID, const string& sNewRole, string &sNewInfo);

    //�������û�
    ErrorCode AddNewUser(const NewUserInfo &userInfo, string &sNewUserInfo);
    //ɾ���û�
    ErrorCode DeleteUsers(const string& sUsers);
    //�޸��û���Ϣ
    ErrorCode ModifyUserInfo(const string& sUserID, const NewUserInfo& userInfo, string& sNewUserInfo);
    //��ȡ�û���Ϣ
    ErrorCode GetUserInfo(const string& sUserID, string &sUserInfo);
    //�޸��û�����
    ErrorCode ModifyUserPassword(const string& sUserID, const string& sOld, const string& sNew);

    //ɾ���û�һ����Ϣ��¼
    std::map<string, string> DeleteUserData(const string& sUsers, const string& sIDs);
    //��ǰ�û�����һ����Ϣ
    ErrorCode AddUserDataInfo(const string& sUserID, const vector<string>& vecItems, string &sReturn);
    //�޸�һ���û���Ϣ
    ErrorCode ModifyUserDataInfo(const string& sUserID, const string& sDataID, const vector<string>& vecItems, string& sReturn);

    //�����û�id��Ȩ�޻�ȡ����
    string GetUserDataInfo(const string& sUserID,
        SearchCondition &SearConditon,
        int iStartIndex, int iCount, int& iAllCount,
        DataCategory eCategory);
    //��ȡ�û����еĿɼ�����
    ErrorCode GetAllDataInfo(const string& sUserID, 
        string &sAllDataInfo,
        SearchCondition& searConditon,
        DataCategory eCategory);
    //����datainfoid��ȡ���������������б�
    string GetDataList(const string& sOwerDataId);

    //��ȡiCount����ɫ��Ϣ���ӵ�iStartIndex��ʼ�� �������е���Ŀ����
    string GetAllRoleInfo(int iStartIndex, int iCount, int& iAllCount);

    //���ӽ�ɫ,��ɫ�����ͽ�ɫ���ֲ����ظ�
    bool AddRoleInfo(const string& sRoleDes, const string& sRoleName, vector<string> &vecNewInfo);
    ErrorCode ModifyRoleInfo(const string& sRoleDes, const string& sRoleName, const string& sUID);
    //ɾ����ɫ��Ϣ
    ErrorCode DeleteRoleInfo(const string& sUID);
    //�޸Ľ�ɫȨ��
    ErrorCode ModifyRoleAuthority(const string& sNewInfo);
    //����idɾ������
    ErrorCode DeleteData(const string& sUID);
    //����  ��������
    ErrorCode AddNewPatientType(const string& sType);
                         
    //��ȡ�������ͣ�
    ErrorCode GetPatientType(string& sType);
    //ɾ���������ͣ�
    ErrorCode DeletePatientType(const string& sID);
    //��ȡ����ҽԺ
    ErrorCode GetHospitalName(const string& sUserID, string& sHospitals, DataCategory eCategory);
    //��������
    ErrorCode SearchDataInfo(const string& sUserID, const SearchCondition& searchContent);
    //��ȡ�û�Ȩ��
    ErrorCode GetUserAuthority(const string& sUserID, UserAuthoriy& userAuthority);
    //�޸Ĳ�������
    ErrorCode ModifyPatientType(const string& sID, const string& sType);

private:
    std::shared_ptr<httplib::Client> m_spClientProxy;
};
#endif // !HTTP_HERLPER_H_
