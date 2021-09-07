#include "httplib/httplib.h"
#include <string>
#include "ReceiveData.h"
#include <map>
#include <mutex>
#include "DBOperator.h"

using std::string;
using std::map;
using namespace httplib;

class HttpsServer
{


public :
    void StartServer();
private:
    void DownLoadHandler(const Request&, Response&);
    void DownLoadHandler_Content(const Request& req, Response& res, const ContentReader& content_reader);

    void UpLoadHandler(const Request&, Response&);
    void VerifyUserHandler(const Request&, Response&);
    //��ȡ���е��û���Ϣ
    void GetAllUserHandler(const Request&, Response&);
    //��ȡ�û���Ϣ
    void GetUserHandler(const Request&, Response&);
    void AddUserHandler(const Request&, Response&);
    void DeleteUserHandler(const Request&, Response&);
    //�޸��û���Ϣ
    void ModifyUserHandler(const Request&, Response&);
    //�޸�����
    void ModifyUserPassword(const Request&, Response&);

    //����һ�����ݣ�vecItems�����������䣬�Ա����ͣ�ҽԺ������
    void AddUserDataHandler(const Request&, Response&);
    //�޸�һ�����ݣ�vecItems�����������䣬�Ա����ͣ�ҽԺ������
    void ModifyUserDataHandler(const Request&, Response&);

    void GetUserDataHandler(const Request&, Response&);
    //��ȡ�û�����������
    void GetUserAllDataHandler(const Request&, Response&);
    //����owerid��ȡ���µ������б�
    void GetDataListByOwerIDHandler(const Request&, Response&);
    //�޸��û���ɫ
    void ModifyUserRoleHandler(const Request&, Response&);
    //���Ӳ�������
    void AddPatientType(const Request&, Response&);
    //��ȡ��������
    void GetPatientType(const Request&, Response&);

    void GetAllRoleInfoHandler(const Request&, Response&);
    void AddRoleInfoHandler(const Request&, Response&);
    void ModifyRoleAuthorityHandler(const Request&, Response&);
    void ModifyRoleInfoHandler(const Request&, Response&);
    void DeleteRoleInfoHandler(const Request&, Response&);
    //����dataidɾ������
    void DeleteDataHandler(const Request&, Response&);
    //��ȡҽԺ
    void GetHospitalHandler(const Request&, Response&);
    //��ȡȨ��
    void GetUserAuthority (const Request&, Response&);

    //ɾ��PatientType
    void DeletePatientType(const Request&, Response&);
    //�޸�PatientType
    void ModifyPatientType(const Request&, Response&);
    

    void ProduceRandomNumberCRand(UINT* RandomArray, UINT size);
    std::shared_ptr<ReceiveData> GetReceiveDataOperator(const string& sID, const Request& req, bool &isNew);

    void OnTimeOut(const string& sUID);

    void PacketData(const string &sKey,
        const string &sValue,
        Headers& headers,
        string &sJsonData,
        bool bForJson = false,
        bool bAddQuotationMark = true);
    bool IsForJson(const Headers& header);
    void EndPacketJson(string& sJsonData, Response& res);
    void PacketRepeat(Response& res, bool bForJson );
    void PacketFailed(Response& res, bool bForJson );
    void PacketSuccess(Response& res, bool bForJson);


    void PacketJsonArrary(string& sJsonData, const string& sItems);
    void PacketJsonArrary(string& sJsonData, vector<string> &vecItems);

private:
    //map<string, std::shared_ptr<ReceiveData>>      m_mapReceiveData;
    std::mutex  m_mutex;
    std::shared_ptr<DBOperator>      m_spDBOperator;
};

