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
    //�����û�����ȡ
    bool GetUserInfoByUserName(const string & sName, vector<string> &vecUserInfo);
    //���ݵ�¼��
    bool GetUserInfoByLoginName(const string& sName, vector<string>& vecUserInfo);
    //�����û�id��ȡ
    bool GetUserInfoByUserID(const string& sID, vector<string>& vecUserInfo);
    bool HasSameName(const string& sName, const string& sLoginName);
    bool HasSameNameExceptUser(const string &sUserID, const string& sName, const string& sLoginName);

    bool AddNewUser(const vector<string>& vecUserInfo, const string& sPassword, string& sSalt);
    bool ModifyUserPassword(const string& sUserID, string& sPassword);
    bool ModifyUserInfo(const string &sUserID, const vector<string>& vecUserInfo, const string& sPassword, string& sSalt);
    //�޸��û���ɫ
    ErrorCode ModifyUserRole(const string& sUserID, const string& sNewRole);
    //ͨ����ɫid��ȡ��ɫ��
    ErrorCode GetRoleNameByRoleID(const string& sRoleID, string& sRoleName);
    bool DeleteUser(const string& sID);
    //��ȡ�����û���Ϣ
    auto GetAllUserInfo(const string &sUserName, 
        int iStartIndex, int iCount, string & sTotalCount) ->vector< vector<string> >;
    //��ȡUserId�û���Ϣ
    ErrorCode GetUserInfo(const string& sUserID, string &sUserInfo);

    bool AddUserData(const string& sUserName,
        const httplib::Headers & headers,
        const string& sPath,
        int64_t &iNewID, 
        string &sNewID);
    //ָ����sDataInfoID���ֶΣ�data_id_list����һ��ID
    void AppendDataId2DataInfo(const string& sDataInfoID, const string &sNewDataId);
    //ָ����sDataInfoID���ֶΣ�data_id_listɾ��һ��ID
    void RemoveDataIdFromeDataInfo(const string& sDataInfoID, const string& sRemoveDataId);

    //����һ�����ݣ�vecItems�����������䣬�Ա����ͣ�ҽԺ������
    ErrorCode AddUserDataItem(const string& sUserID, const vector<string>& vecDataInfo, string &sNewInfo);
    //�޸�һ������
    ErrorCode ModifyUserDateaItem(const string& sUserID, const string& sDataID, const vector<string>& vecDataInfo, string& sNewInfo);

    //��ȡָ���û�id������ע����Ϣ �ӵ�iStartIndex��ʼ��ȡ iCount����
    string GetUserDataItems(const string &sUserID, uint64_t iStartIndex,
        uint64_t iCount, string& sTotalCount,
        SearchCondition &searchCondition);
    //��ȡ���е�����ע����Ϣ,�ӵ�iStartIndex��ʼ��ȡ iCount����
    string GetAllDataItems(uint64_t iStartIndex, uint64_t iCount, string& sTotalCount, SearchCondition &searchCondition);
    //����dataInfo id��ȡ�����������������Ϣ�б�
    string GetDataListByOwerIDHandler(const string& sDataRegiserID);
    //����dataIdɾ������
    ErrorCode DeleteData(const string& sID);
    string GetDataPath(const string& sID);

    string GetDataName(const string& sID);

    string GetOwerDataInfoId(const string& sID);

    string GetUserDataByID(const string& sID);
    string PacketUserData();
    //��ȡ��ɫ��Ϣ���ӵ�iStartIndex��ʼ��ȡ iCount��
    string GetAllRoleInfo(int iStartIndex, int iCount, string &sTotalCount);
    //�޸Ľ�ɫȨ��
    bool ModifyRoleAuthority(const string& sNewInfo);
    //�����û�ID��ȡ�û���Ȩ��
    ErrorCode GetUserAuthority(const string& sUserID,
        uint64_t& uiDataAuthority, uint64_t& uiSystemAuthoriy,
        uint64_t& uiUserCenterAuthoriy, uint64_t& uiStasticsAuthoriy);

    string AddRoleInfo(const string& sRoleDes, const string &sRoleName);
    ErrorCode ModifyRoleInfo(const string& sRoleDes, const string& sRoleName, const string& sUID);
    ErrorCode DeleteRoleInfo(const string& sUID);

    //���Ӳ�������
    ErrorCode AddPatientType(const string& sPatientType);
    //��ȡ��������
    ErrorCode GetPatientType(string& sPatientType);
    //��ȡҽԺ
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


