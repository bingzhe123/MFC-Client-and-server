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
    //获取所有的用户信息
    void GetAllUserHandler(const Request&, Response&);
    //获取用户信息
    void GetUserHandler(const Request&, Response&);
    void AddUserHandler(const Request&, Response&);
    void DeleteUserHandler(const Request&, Response&);
    //修改用户信息
    void ModifyUserHandler(const Request&, Response&);
    //修改密码
    void ModifyUserPassword(const Request&, Response&);

    //新增一条数据：vecItems：姓名，年龄，性别，类型，医院，需求
    void AddUserDataHandler(const Request&, Response&);
    //修改一条数据：vecItems：姓名，年龄，性别，类型，医院，需求
    void ModifyUserDataHandler(const Request&, Response&);

    void GetUserDataHandler(const Request&, Response&);
    //获取用户的所有数据
    void GetUserAllDataHandler(const Request&, Response&);
    //根据owerid获取其下的数据列表
    void GetDataListByOwerIDHandler(const Request&, Response&);
    //修改用户角色
    void ModifyUserRoleHandler(const Request&, Response&);
    //增加病人类型
    void AddPatientType(const Request&, Response&);
    //获取病人类型
    void GetPatientType(const Request&, Response&);

    void GetAllRoleInfoHandler(const Request&, Response&);
    void AddRoleInfoHandler(const Request&, Response&);
    void ModifyRoleAuthorityHandler(const Request&, Response&);
    void ModifyRoleInfoHandler(const Request&, Response&);
    void DeleteRoleInfoHandler(const Request&, Response&);
    //根据dataid删除数据
    void DeleteDataHandler(const Request&, Response&);
    //获取医院
    void GetHospitalHandler(const Request&, Response&);
    //获取权限
    void GetUserAuthority (const Request&, Response&);

    //删除PatientType
    void DeletePatientType(const Request&, Response&);
    //修改PatientType
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

