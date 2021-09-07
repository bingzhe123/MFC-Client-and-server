#include "HttpsServer.h"
#include <iostream>
#include "picosha2.h"
#include "ReceiveManager.h"
#include "commondef.h"
#include <direct.h>
#include <fstream>
#include "Directory Help.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"	//引入rapidjson源文件


using namespace httplib;

using namespace std;

void HttpsServer::StartServer()
{
   
    int iPort = 8080;

    ifstream  in("ServerConfig.ini", ios::in | ios::_Nocreate);
    if (!in.is_open())
    {
        std::cout << "打开文件 ServerConfig.ini 失败, 使用默认端口8080" << std::endl;
    }
    else
    {       
        char buffer[1024];
        in.getline(buffer, sizeof(buffer));
        auto vecTemp = SplitString(string(buffer), ':');
        if (vecTemp.size() == 2)
        {
            iPort = atoi(vecTemp[1].c_str());
        }
    }
    std::cout << "端口号：" << iPort << std::endl;

    m_spDBOperator = std::make_shared< DBOperator>();
    if (!m_spDBOperator->Connect())
    {
        std::cout << "************ Connect DB failed" << std::endl;
    }


    /*UINT randomCode[8];
    ProduceRandomNumberCRand(randomCode, 8);
    char temp[33];
    char* pCode = (char*)randomCode;
    for (size_t i = 0; i < 32; i++)
    {
        temp[i] = (abs(*(pCode + i))) % 57 + 65;
    }
    temp[32] = 0;
    string sSalt = string(temp);
    string sPassword = "password_123";
    std::string sCodePassword1 = picosha2::hash256_hex_string(sPassword);
    auto sSaltPassWord = sCodePassword1 + sSalt;
    std::string sCodePassword = picosha2::hash256_hex_string(sSaltPassWord);
    m_spDBOperator->AddNewUser("SuperUser", sCodePassword, sSalt)*/;



#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    SSLServer svr(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
    Server svr;
#endif
    if (!svr.is_valid()) {
        printf("server has an error...\n");
        return;
    }
    svr.Get("/hi", [](const Request& /*req*/, Response& res) {
        static int i = 0;
        std::cout << "************" << i++ << std::endl;
        std::cout << "Server Get" << std::endl;
        res.set_content("Hello World!\n", "text/plain");
    });
    printf("Register verifyHandler...\n");
    Server::Handler verifyHandler = std::bind(&HttpsServer::VerifyUserHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/VerifyUser", verifyHandler);

    printf("Register verifyHandler...\n");
    Server::Handler verifyHandlerutf8 = std::bind(&HttpsServer::VerifyUserHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/VerifyUser-utf8", verifyHandlerutf8);


    printf("Register getAllUserHandler...\n");
    Server::Handler getAllUserHandler = std::bind(&HttpsServer::GetAllUserHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetAllUserInfo", getAllUserHandler);

    printf("Register getUserHandler...\n");
    Server::Handler getUserHandler = std::bind(&HttpsServer::GetUserHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetUserInfo", getUserHandler);


    printf("Register addUserHandler...\n");
    Server::Handler addUserHandler = std::bind(&HttpsServer::AddUserHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/AddUser", addUserHandler);
    
    printf("Register deleteUserHandler...\n");
    Server::Handler deleteUserHandler = std::bind(&HttpsServer::DeleteUserHandler,
            this,
            std::placeholders::_1,
            std::placeholders::_2);
    svr.Get("/DeleteUser", deleteUserHandler);

        
    printf("Register modifyUserHandler...\n");
    Server::Handler modifyUserHandler = std::bind(&HttpsServer::ModifyUserHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/ModifyUser", modifyUserHandler);
    
        
    printf("Register modifyUserPasswordHandler...\n");
    Server::Handler modifyUserPasswordHandler = std::bind(&HttpsServer::ModifyUserPassword,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/ModifyUserPassword", modifyUserPasswordHandler);

    printf("Register DownLoadHandler...\n");
    Server::Handler clientHandler = std::bind(&HttpsServer::DownLoadHandler,
        this, 
        std::placeholders::_1, 
        std::placeholders::_2);
    svr.Post("/ClientUploadFile", clientHandler);



    //web，或app上传
    printf("Register DownLoadHandler...\n");
    Server::HandlerWithContentReader appHandler = std::bind(&HttpsServer::DownLoadHandler_Content,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3);
    svr.Post("/UploadFile", appHandler);



    printf("Register upLoadHandler...\n");
    Server::Handler upLoadHandler = std::bind(&HttpsServer::UpLoadHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/DownLoad", upLoadHandler);

    printf("Register getUserDataHandler...\n");
    Server::Handler getUserDataHandler = std::bind(&HttpsServer::GetUserDataHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetUserData", getUserDataHandler);

    
        printf("Register getUserAllDataHandler...\n");
    Server::Handler getUserAllDataHandler = std::bind(&HttpsServer::GetUserAllDataHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetUserAllData", getUserAllDataHandler);

    printf("Register AddUserDataHandler...\n");
    Server::Handler addUserDataHandler = std::bind(&HttpsServer::AddUserDataHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/AddUserDataInfo", addUserDataHandler);

    printf("Register ModifyUserDataHandler...\n");
    Server::Handler modifyUserDataHandler = std::bind(&HttpsServer::ModifyUserDataHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/ModifyUserDataInfo", modifyUserDataHandler);

    printf("Register getDataListByOwerIDHandler...\n");
    Server::Handler getDataListByOwerIDHandler = std::bind(&HttpsServer::GetDataListByOwerIDHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetDataList", getDataListByOwerIDHandler);

        
    printf("Register ModifyUserRoleHandler...\n");
    Server::Handler modifyUserRoleHandler = std::bind(&HttpsServer::ModifyUserRoleHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/ModifyUserRole", modifyUserRoleHandler);

    printf("Register addPatientTypeHandler...\n");
    Server::Handler addPatientTypeHandler = std::bind(&HttpsServer::AddPatientType,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/AddPatientType", addPatientTypeHandler);

    printf("Register getPatientTypeHandler...\n");
    Server::Handler getPatientTypeHandler = std::bind(&HttpsServer::GetPatientType,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetPatientType", getPatientTypeHandler);

    printf("Register GetAllRoleInfoHandler...\n");
    Server::Handler getAllRoleInfoHandler = std::bind(&HttpsServer::GetAllRoleInfoHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetAllRoleInfo", getAllRoleInfoHandler);

    printf("Register modifyRoleAuthorityHandler...\n");
    Server::Handler modifyRoleAuthorityHandler = std::bind(&HttpsServer::ModifyRoleAuthorityHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/ModifyRoleAuthority", modifyRoleAuthorityHandler);

    printf("Register ModifyRoleInfoHandler...\n");
    Server::Handler ModifyRoleInfoHandler = std::bind(&HttpsServer::ModifyRoleInfoHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/ModifyRoleInfo", ModifyRoleInfoHandler);

    printf("Register addRoleInfoHandler...\n");
    Server::Handler addRoleInfoHandler = std::bind(&HttpsServer::AddRoleInfoHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/AddRole", addRoleInfoHandler);

    printf("Register deleteRoleInfoHandler...\n");
    Server::Handler deleteRoleInfoHandler = std::bind(&HttpsServer::DeleteRoleInfoHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/DeleteRoleInfo", deleteRoleInfoHandler);


    printf("Register deleteDataHandler...\n");
    Server::Handler deleteDataHandler = std::bind(&HttpsServer::DeleteDataHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/DeleteData", deleteDataHandler);


    printf("Register getHospitalHandler...\n");
    Server::Handler getHospitalHandler = std::bind(&HttpsServer::GetHospitalHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetHospital", getHospitalHandler);

    printf("Register getUserAuthorityHandler...\n");
    Server::Handler getUserAuthorityHandler = std::bind(&HttpsServer::GetUserAuthority,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/GetUserAuthority", getUserAuthorityHandler);

    printf("Register DeletePatientType...\n");
    Server::Handler deletePatientType = std::bind(&HttpsServer::DeletePatientType,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/DeletePatientType", deletePatientType);

    printf("Register ModifyPatientType...\n");
    Server::Handler modifyPatientType = std::bind(&HttpsServer::ModifyPatientType,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/ModifyPatientType", modifyPatientType);
  /*  printf("Register searchDataInfoHandler...\n");
    Server::Handler searchDataInfoHandler = std::bind(&HttpsServer::SearchDataInfoHandler,
        this,
        std::placeholders::_1,
        std::placeholders::_2);
    svr.Get("/SearchDataInfo", searchDataInfoHandler);*/


    printf("server begin lisen... port = %d\n",iPort);
    auto res = svr.listen("0.0.0.0", iPort);
    if (!res)
    {
        printf("start lisend failed ...\n");
    }

}
void HttpsServer::DownLoadHandler_Content(const Request& req,
    Response& res, const ContentReader& content_reader)
{
    std::cout << "threadID = " << std::this_thread::get_id() << std::endl;
    std::cout << "DownLoadHandler_Content---begin" << endl;

    Request* pTemp = const_cast<Request*>(&req);

    auto &header = pTemp->headers;
    auto bForJson = IsForJson(header);


    string sUserID;
    auto it = header.find("UserOwerID");
    if (it != header.end())
    {
        sUserID = it->second;
    }
    else
    {
        PacketFailed(res, bForJson);
        return;
    }
    string sTotalLength;

    it = header.find("TotalLength");
    if (it != header.end())
    {
        sTotalLength = it->second;
    }
    else
    {
        PacketFailed(res, bForJson);
        return;
    }
    int64_t iTotalLength = _atoi64(sTotalLength.data());

    
    string s = R"(/)";
    auto sPath = "UserData" + s + sUserID;
    std::replace(sPath.begin(), sPath.end(), '\\', '/');
    if (!MkDir(sPath))
    {
        std::cout << __FUNCTION__ << " MkDir failed" << std::endl;
        return PacketFailed(res, bForJson);
    }
    
    string sFilePath = "";
    string sFileName = "";

    size_t   iRecivedLenght = 0;
    FILE* pFile = nullptr;
    if (req.is_multipart_form_data())
    {
        MultipartFormDataItems files;
        content_reader([&](const MultipartFormData& file)
        {
            files.push_back(file);     

            sFileName = file.filename;
            auto sFile = sPath + s + file.filename;
            //解决重名的问题
            sFilePath = ReceiveData::GetRealFilePath(sFile);

            fopen_s(&pFile, sFilePath.data(), "wb");
            if (pFile == nullptr)
            {
                std::cout << __FUNCTION__ << " foen file failed" << std::endl;
            }

            return true;
        },
            [&](const char* data, size_t data_length)
        {
            //files.back().content.append(data, data_length);
            std::cout << "threadID = " << std::this_thread::get_id() << std::endl;
            fwrite(data, sizeof(char), data_length, pFile);
            iRecivedLenght += data_length;
            return true;
        });

        fclose(pFile);
    }
    if (iRecivedLenght != iTotalLength)
    {
        PacketFailed(res, bForJson);
        return;
    }

    std::cout << "DownLoadHandler_Content----end" << endl;
                             
    header.emplace("FileName", sFileName);           
    string sNewInfo;
    int64_t iNewID;
    auto bRes = m_spDBOperator->AddUserData(sUserID,
        header,
        sFilePath, iNewID, sNewInfo);

    string sDataOwerID;
    it = header.find("FileOwerID");
    if (it != header.end())
    {
        sDataOwerID = it->second;
    }
    assert(!sDataOwerID.empty());
    m_spDBOperator->AppendDataId2DataInfo(sDataOwerID, std::to_string(iNewID));

    auto vecNewInfo = SplitString(sNewInfo, ',');
    auto& resHeader = res.headers;
    string sJsonData;
    PacketData("UID", vecNewInfo[0], res.headers, sJsonData, bForJson);
    PacketData("FileName", vecNewInfo[1], res.headers, sJsonData, bForJson);
    PacketData("FilePath", vecNewInfo[2], res.headers, sJsonData, bForJson);
    PacketData("DataSize", vecNewInfo[3], res.headers, sJsonData, bForJson);
    EndPacketJson(sJsonData, res);
    res.headers.emplace("NewDataInfo", std::move(sNewInfo));
}

void HttpsServer::DownLoadHandler(const Request& req, Response& res)
{
 
    //std::cout << req.body << std::endl;
    auto &header = req.headers;
    auto it = header.find("ID");
    string sID;
    if (it != header.end())
    {
        sID = it->second;
    }
    assert(!sID.empty());
    bool bNew = false;
    auto spReceiveData = GetReceiveDataOperator(sID, req, bNew);
    if (spReceiveData == nullptr)
    {
        res.body = "Disconnect";
        return;
    }
    if (bNew)
    {
        spReceiveData->StartTimeOutThread();
    }
    std::cout << "Recevie begin" << std::endl;

    auto statusCode = spReceiveData->Recevie(req, res);
    std::cout << "Recevie end statusCode="<<(int) statusCode << std::endl;

   
    if (statusCode == StatusCode::Failed)
    {
        res.body = "Disconnect";
        auto sUID = spReceiveData->GetUID();
        ReceiveManager::Instance()->DeleteReceiveData(sUID, true);
        std::cout << "*********Receive Result failed" << std::endl;
    }
    else if (statusCode == StatusCode::Finished)
    {
        auto sUID = spReceiveData->GetUID();
        ReceiveManager::Instance()->DeleteReceiveData(sUID, false);
        std::cout << "*********Receive Result success" << std::endl;
    }
    else
    {
        std::cout << "*********Receive Result waiting data" << std::endl;
    }

}

std::shared_ptr<ReceiveData> HttpsServer::GetReceiveDataOperator(const string& sID, const Request& req, bool& isNew)
{
    isNew = false;
    auto spReceiveData = ReceiveManager::Instance()->GetReceiveData(sID);
    if (spReceiveData != nullptr)
    {
        return spReceiveData;
    }

    auto &header = req.headers;
    string sOffet;
    auto it = header.find("OffSet");
    if (it != header.end())
    {
        sOffet = it->second;
    }
    int64_t iOffset = _atoi64(sOffet.data());
    if (iOffset != 0)
    { //说明中间断了，要重新发送
        return nullptr;
    }
    isNew = true;
    string sTotalLength;
    it = header.find("TotalLength");
    if (it != header.end())
    {
        sTotalLength = it->second;
    }
    assert(!sTotalLength.empty());
    int64_t iTotalLength = _atoi64(sTotalLength.data());

    string sFileName;
    it = header.find("FileName");
    if (it != header.end())
    {
        sFileName = it->second;
    }
    assert(!sFileName.empty());

    string sUserName;
    it = header.find("UserOwerID");
    if (it != header.end())
    {
        sUserName = it->second;
    }
    assert(!sUserName.empty());

    spReceiveData = std::make_shared<ReceiveData>(iTotalLength, 
        sFileName, 
        sID, 
        sUserName,
        m_spDBOperator);
    spReceiveData->m_funWatingTimeOut = std::bind(&HttpsServer::OnTimeOut, this, std::placeholders::_1);
 
    ReceiveManager::Instance()->AddReceiveData(sID, spReceiveData);
    return spReceiveData;
}

void HttpsServer::OnTimeOut(const string& sUID)
{
    std::cout << "Waiting Time Out End Receive:sUID=" << sUID << std::endl;
    ReceiveManager::Instance()->DeleteReceiveData(sUID, true);
}      


void HttpsServer::UpLoadHandler(const Request& req, Response& res)
{
    std::cout<<"***********UpLoadHandler----UID:" << std::endl;
  
    auto& header = req.headers;
    auto it = header.find("UID");
    string sUID;
    if (it != header.end())
    {
        sUID = it->second;
    }
    else
    {
        auto parIt = req.params.find("UID");
        if (parIt == req.params.end())
        {
            PacketFailed(res, IsForJson(header));
            return;
        }
        sUID = parIt->second;
    }

    std::cout << "***********UpLoadHandler----UID:" << sUID<< std::endl;
    auto sPath = m_spDBOperator->GetDataPath(sUID);
    if (sPath.empty())
    {
        res.set_content("File not exist", "text/plain");
        return;
    }
    std::cout << "***********UpLoadHandler----UID:" << sPath << std::endl;
    auto buffer = _getcwd(NULL, 0);
    FILE* pFile;
    fopen_s(&pFile, sPath.c_str(), "rb");
    if (pFile == nullptr)
    {
        std::cout << "Open File Failed,error code = " << errno << std::endl;
        res.status = 404;
        return;

    }

    auto sName = m_spDBOperator->GetDataName(sUID);

    res.headers.emplace("Content-Type", "application/force-download");//4
    string sFileName = "attachment; filename=" + sName;
    res.headers.emplace("Content-Disposition", sFileName);

    _fseeki64(pFile, 0, SEEK_END);
    auto iFileLength = _ftelli64(pFile);
    _fseeki64(pFile, 0, SEEK_SET);
    //fread(pData, sizeof(char), iFileLength, pFile);
              //数据读取错误
    const size_t DATA_CHUNK_SIZE = 1024;
    res.set_content_provider(iFileLength, "application/octet-stream", [pFile, DATA_CHUNK_SIZE](size_t offset, size_t length, DataSink& sink)
    {
        FILE* _pFile = (FILE * )pFile;

        const size_t DATA_CHUNK_SIZE = 1024;
        char buffer[DATA_CHUNK_SIZE];
        //char* pData = new char[DATA_CHUNK_SIZE];
        auto iSize = std::min(length, DATA_CHUNK_SIZE);
        fread(buffer, sizeof(char), iSize, pFile);

        sink.write(buffer, std::min(length, DATA_CHUNK_SIZE));
        //delete[] pData;
        return true; // return 'false' if you want to cancel the process.
    },
    [pFile] { fclose(pFile); });
}

void HttpsServer::VerifyUserHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sPassword;
    string sUser;
    if (bForJson)
    {
        auto& param = req.params;
        auto it = param.find("User");
        if (it != param.end())
        {
            sUser = Utf8ToGb2312( it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

        it = param.find("Password");
        if (it != param.end())
        {
            sPassword = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
    else
    {
        auto it = header.find("User");
        if (it != header.end())
        {
            sUser = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

        it = header.find("Password");
        if (it != header.end())
        {
            sPassword = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }

    
    std::cout << "sPassword:" << sPassword << std::endl;
    vector<string> vecUserInfo;
    m_spDBOperator->GetUserInfoByLoginName(sUser, vecUserInfo);
    if (vecUserInfo.empty())
    {
        PacketFailed(res, bForJson);
        return;
    }
    //id, name, password, salt, manager, email, role
    auto sSaltPassWord = sPassword + vecUserInfo[4];
    std::cout << "sPassword:" << sPassword<< std::endl;
    std::cout << "sSalt:" << vecUserInfo[4]<<std::endl;

    std::string sCodePassword = picosha2::hash256_hex_string(sSaltPassWord);
    std::cout << "sCodePassword:" << sCodePassword<< std::endl;
                          
    string sJsonData;
    if (vecUserInfo[3] == sCodePassword)
    {
        PacketData("id", vecUserInfo[0], res.headers, sJsonData, bForJson);
        auto sManager = vecUserInfo[5];
        PacketData("IsManager", sManager, res.headers, sJsonData, bForJson);
        PacketData("Role", vecUserInfo[7], res.headers, sJsonData, bForJson);
       // res.headers.emplace("id", vecUserInfo[0]);
       // res.headers.emplace("IsManager", sManager);
        //res.headers.emplace("Role", vecUserInfo[7]);
        uint64_t uiDataAuthority = 0;
        uint64_t uiSystemAuthoriy = 0;
        uint64_t uiUserCenterAuthoriy = 0;
        uint64_t uiStasticsAuthoriy = 0;
        auto erroCode = m_spDBOperator->GetUserAuthority(vecUserInfo[0], uiDataAuthority, uiSystemAuthoriy, uiUserCenterAuthoriy, uiStasticsAuthoriy);
        if (erroCode == ErrorCode::Failed)
        {
            PacketFailed(res, bForJson);

            return;
        }
        char temp[100] = { 0 };
        memset(temp, 0, sizeof(temp));
        _ui64toa_s(uiDataAuthority, temp, 100, 10);
       // res.headers.emplace("DataAuthority", string(temp));
        PacketData("DataAuthority", string(temp), res.headers, sJsonData, bForJson);

        memset(temp, 0, sizeof(temp));
        _ui64toa_s(uiSystemAuthoriy, temp, 100, 10);
        //res.headers.emplace("SystemAuthoriy", string(temp));
        PacketData("SystemAuthoriy", string(temp), res.headers, sJsonData, bForJson);

        memset(temp, 0, sizeof(temp));
        _ui64toa_s(uiUserCenterAuthoriy, temp, 100, 10);
        //res.headers.emplace("UserCenterAuthoriy", string(temp));
        PacketData("UserCenterAuthoriy", string(temp), res.headers, sJsonData, bForJson);

        memset(temp, 0, sizeof(temp));
        _ui64toa_s(uiStasticsAuthoriy, temp, 100, 10);
        //res.headers.emplace("StasticsAuthoriy", string(temp));
        PacketData("StasticsAuthoriy", string(temp), res.headers, sJsonData, bForJson);
        
        if (!bForJson)
        {
            res.set_content("pass", "text/plain");
        }
        else
        {
            EndPacketJson(sJsonData, res);
        }
        std::cout << " VerifyUserHandler success" << std::endl;
    }
    else
    {
        PacketFailed(res, bForJson);
    }
    std::cout << " VerifyUserHandler end" << std::endl;
}

void HttpsServer::GetAllUserHandler(const Request&req, Response&res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    int iStartIndex;
    int iCount;
    string sUserName("");
    if (bForJson)
    {
        auto& param = req.params;

        auto it = param.find("StartIndex");
        if (it != param.end())
        {
            iStartIndex = atoi(Utf8ToGb2312(it->second));
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("Count");
        if (it != param.end())
        {
            iCount = atoi(Utf8ToGb2312(it->second));
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

        it = param.find("UserName");
        if (it != param.end())
        {
            sUserName = Utf8ToGb2312(it->second);
        }
    }
    else
    {
        auto it = header.find("StartIndex");
        if (it != header.end())
        {
            iStartIndex = atoi(it->second.c_str());
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("Count");
        if (it != header.end())
        {
            iCount = atoi(it->second.c_str());
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

        it = header.find("UserName");
        if (it != header.end())
        {
            sUserName = it->second.c_str();
        }
    }

    
    string sTotalCount;
    auto vecUserInfo = m_spDBOperator->GetAllUserInfo(sUserName, iStartIndex, iCount, sTotalCount);
    std::cout << "TotalUserCount:" << sTotalCount << std::endl;
    auto size = vecUserInfo.size();
    std::cout << "vecUserInfo:" << size << std::endl;
    if (size == 0)
    {
        PacketFailed(res, bForJson);
        return;
    }
    if (bForJson)
    {
        string sJson;
        string keys[] = {"id", "name", "login_name", "manager", "email", "role", "create_time","role_name"};
        for (size_t i = 0; i < size; i++)
        {
            string sJsonItem;
            auto& vecInfo = vecUserInfo[i];
            for (int i = 0; i < vecInfo.size(); i++)
            {
                if (i == 7)
                {
                    auto vecRoleName = SplitString(vecInfo[7], ';');
                    string sTemp;
                    PacketJsonArrary(sTemp, vecRoleName);
                    PacketData(keys[i], sTemp, res.headers, sJsonItem, bForJson, false);
                }
                else
                {
                    PacketData(keys[i], vecInfo[i], res.headers, sJsonItem, bForJson);
                }
            }
            sJsonItem += "}";
            PacketJsonArrary(sJson, sJsonItem);
        }
        sJson += "]";
        string sResult;
        PacketData("TotalCount", sTotalCount, res.headers, sResult, bForJson);
        PacketData("AllUserInfo", sJson, res.headers, sResult, bForJson, false);
        EndPacketJson(sResult, res);
    }
    else
    {
        string sBody;
        for (size_t i = 0; i < size; i++)
        {
            auto& vecInfo = vecUserInfo[i];
            auto sTemp = JoinString(vecInfo, ',');
            sBody += sTemp;
            if (i != size - 1)
            {
                sBody += "|";
            }
        }
        res.headers.emplace("TotalCount", sTotalCount);
        res.headers.emplace("AllUserInfo", sBody);
        res.body = "pass";
    }
    
}

void HttpsServer::GetUserHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUserID;
    if (bForJson)
    {
        auto& param = req.params;
        auto it = param.find("UserID");
        if (it != param.end())
        {
            sUserID = Utf8ToGb2312( it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
          

    string sUserInfo;
    if (ErrorCode::Success == m_spDBOperator->GetUserInfo(sUserID, sUserInfo))
    {
        if (bForJson)
        {
            string sJsonItem;
            auto vecRoleName = SplitString(sUserInfo, ',');
            string keys[] = { "id", "name", "login_name", "manager", "email", "role", "create_time","role_name" };
            for (size_t i = 0; i < vecRoleName.size(); i++)
            {
                PacketData(keys[i], vecRoleName[i], res.headers, sJsonItem, bForJson);
            }
            EndPacketJson(sJsonItem, res);
        }
        else
        {
            res.headers.emplace("UserInfo", sUserInfo);
            res.body = "pass";
        }
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}
void HttpsServer::AddUserHandler(const Request& req , Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sNewUserInfo;
    if (bForJson)
    {
        auto it = req.params.find("NewUserInfo");
        if (it != req.params.end())
        {
            sNewUserInfo = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("NewUserInfo");
        if (it != header.end())
        {
            sNewUserInfo = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
          
    auto vecUserInfo = SplitString(sNewUserInfo, ',');
    assert(vecUserInfo.size() == 4);

    auto sUserName = vecUserInfo[0];
    auto sLoginName = vecUserInfo[1];
    auto bRepeat = m_spDBOperator->HasSameName(sUserName, sLoginName);
    if (bRepeat)
    {
        PacketRepeat(res, bForJson);
        return;
    }

    string sPassword = vecUserInfo[2];
    UINT randomCode[32];
    ProduceRandomNumberCRand(randomCode, 32);
    char temp[33];
    char* pCode = (char*)randomCode;
    for (size_t i = 0; i < 32; i++)
    {
        temp[i] = (abs(*(pCode + i))) % 24 + 65;
    }
    temp[32] = 0;
    string sSalt = string(temp);

    std::string sCodePassword1 = sPassword;
    auto sSaltPassWord = sCodePassword1 + sSalt;
    std::string sCodePassword = picosha2::hash256_hex_string(sSaltPassWord);
    if (m_spDBOperator->AddNewUser(vecUserInfo, sCodePassword, sSalt))
    {         
        vector<string> vecUserInfo;
        //id, name, login_name, password, salt, manager, email, role, create_time
        m_spDBOperator->GetUserInfoByLoginName(sLoginName, vecUserInfo);
        vecUserInfo.erase(vecUserInfo.begin() + 3); //密码
        vecUserInfo.erase(vecUserInfo.begin() + 3); //salt
        vecUserInfo.push_back("");//增加角色名字在最后
                           
        if (bForJson)
        {
            string sJsonItem;                                   
            string keys[] = { "id", "name", "login_name", "manager", "email", "role", "create_time","role_name" };
            for (size_t i = 0; i < vecUserInfo.size(); i++)
            {
                PacketData(keys[i], vecUserInfo[i], res.headers, sJsonItem, bForJson);
            }
            EndPacketJson(sJsonItem, res);
        }
        else
        {
            res.body = "pass";
            string sUserInfo = JoinString(vecUserInfo, ',');
            res.headers.emplace("UserInfo", sUserInfo);
        }

    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::DeleteUserHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUser;
    if (bForJson)
    {
        auto it = req.params.find("Users");
        if (it != req.params.end())
        {
            sUser = Utf8ToGb2312( it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("Users");
        if (it != header.end())
        {
            sUser = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    
    string sBody;
    if (m_spDBOperator->DeleteUser(sUser))
    {
        PacketSuccess(res, bForJson);
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::ModifyUserHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUserID;
    string sNewUserInfo;
    if (bForJson)
    {
        auto& para = req.params;
        auto it = para.find("UserID");
        if (it != para.end())
        {
            sUserID = Utf8ToGb2312( it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = para.find("NewUserInfo");
        if (it != para.end())

        {
            sNewUserInfo = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }

    else
    {
        auto it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("NewUserInfo");
        if (it != header.end())

        {
            sNewUserInfo = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    
    auto vecUserInfo = SplitString(sNewUserInfo, ',');
    assert(vecUserInfo.size() == 4);

    auto sUserName = vecUserInfo[0];
    auto sLoginName = vecUserInfo[1];
    auto bRepeat = m_spDBOperator->HasSameNameExceptUser(sUserID, sUserName, sLoginName);
    if (bRepeat)
    {
        PacketRepeat(res, bForJson);
        return;
    }

    string sPassword = vecUserInfo[2];
    std::string sCodePassword;
    string sSalt;
    if (!sPassword.empty())
    {
        UINT randomCode[32];
        ProduceRandomNumberCRand(randomCode, 32);
        char temp[33];
        char* pCode = (char*)randomCode;
        for (size_t i = 0; i < 32; i++)
        {
            temp[i] = (abs(*(pCode + i))) % 24 + 65;
        }
        temp[32] = 0;
        sSalt = string(temp);

        //std::string sCodePassword1 = picosha2::hash256_hex_string(sPassword);
        auto sSaltPassWord = sPassword + sSalt;
        sCodePassword = picosha2::hash256_hex_string(sSaltPassWord);
    }

    if (m_spDBOperator->ModifyUserInfo(sUserID, vecUserInfo, sCodePassword, sSalt))
    {
        res.body = "pass";
        vector<string> vecUserInfo;
        //id, name, login_name, password, salt, manager, email, role, create_time
        m_spDBOperator->GetUserInfoByUserName(sUserName, vecUserInfo);
        vecUserInfo.erase(vecUserInfo.begin() + 3); //密码
        vecUserInfo.erase(vecUserInfo.begin() + 3); //salt
        vecUserInfo.push_back("");//增加角色名字在最后
        if (bForJson)
        {
            string sJsonItem;
            string keys[] = { "id", "name", "login_name", "manager", "email", "role", "create_time","role_name" };
            for (size_t i = 0; i < vecUserInfo.size(); i++)
            {
                PacketData(keys[i], vecUserInfo[i], res.headers, sJsonItem, bForJson);
            }
            EndPacketJson(sJsonItem, res);
        }
        else
        {
            string sUserInfo = JoinString(vecUserInfo, ',');
            res.headers.emplace("UserInfo", sUserInfo);
            res.body = "pass";
        }
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::ModifyUserPassword(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUserID;
    string sOld;
    string sNew;
    if (bForJson)
    {
        auto& param = req.params;
        auto it = param.find("UserID");
        if (it != param.end())
        {
            sUserID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("OldPassword");
        if (it != param.end())
        {
            sOld = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("NewPassword");
        if (it != param.end())
        {
            sNew = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("OldPassword");
        if (it != header.end())
        {
            sOld = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("NewPassword");
        if (it != header.end())
        {
            sNew = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    
    vector<string> vecUserInfo;
    if (!m_spDBOperator->GetUserInfoByUserID(sUserID, vecUserInfo))
    {
        PacketFailed(res, bForJson);
        return;
    }
    auto sSaltPassWord = sOld + vecUserInfo[4];
    std::string sCodePassword = picosha2::hash256_hex_string(sSaltPassWord);
    if (vecUserInfo[3] != sCodePassword)
    {
        if (bForJson)
        {
            string sJsonData;
            sJsonData += "{";
            sJsonData += R"(,"result")";
            sJsonData += R"(: "Old_Password_Failed")";
            sJsonData += "}";
            res.set_content(sJsonData, "text/plain");
        }
        else
        {
            res.body = "OldFailed";
        }
        return;
    }

    sSaltPassWord = sNew + vecUserInfo[4];
    sCodePassword = picosha2::hash256_hex_string(sSaltPassWord);
    if (!m_spDBOperator->ModifyUserPassword(sUserID, sCodePassword))
    {
        PacketFailed(res, bForJson);
    }
    else
    {
        if (bForJson)
        {
            string  sJsonItem;
            PacketData("result", "pass", res.headers, sJsonItem, bForJson);
            EndPacketJson(sJsonItem, res);
        }
        else
        {
            res.body = "pass";
        }

    }
}


//新增一条数据：vecItems：姓名，年龄，性别，类型，医院，需求
void HttpsServer::AddUserDataHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUserID;
    string sNewDataInfo;

    if (bForJson)
    {
        auto itParam = req.params.find("UserID");
        if (itParam != req.params.end())
        {
            auto sTemp = itParam->second;
            sUserID = Utf8ToGb2312(sTemp.c_str());
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        itParam = req.params.find("DataInfo");
        if (itParam != req.params.end())
        {
            sNewDataInfo = Utf8ToGb2312(itParam->second.c_str());
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }

    else
    {
        auto it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("DataInfo");
        if (it != header.end())
        {
            sNewDataInfo = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }


   

   
    vector<string> vecTemp = SplitString(sNewDataInfo, ',');
    string sNewItem;
    auto r = m_spDBOperator->AddUserDataItem(sUserID, vecTemp, sNewItem);

    if (bForJson)
    {
        if (ErrorCode::Success == r)
        {
            string keys[] = { "id", "data_name", "age", "gender", "data_type",
         "register_date", "hospital","description" ,"data_id_list", "oweruser_id" };
            string sJsonItem;
            auto vecNewInfo = SplitString(sNewItem, ',');
            for (size_t i = 0; i < vecNewInfo.size(); i++)
            {
                PacketData(keys[i], vecNewInfo[i], res.headers, sJsonItem, bForJson);
            }
            EndPacketJson(sJsonItem, res);
        }
        else if (ErrorCode::Repeat == r)
        {
            PacketRepeat( res, bForJson);

        }
        else
        {
            PacketFailed(res, bForJson);
        }
    }
    else
    {
        if (ErrorCode::Success == r)
        {
            res.body = "pass";
            res.headers.emplace("DataInfo", std::move(sNewItem));
        }
        else if (ErrorCode::Repeat == r)
        {
            res.set_content("Repeat", "text/plain");
        }
        else
        {
            res.set_content("failed", "text/plain");
        }
    }
}

void HttpsServer::ModifyUserDataHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sDataID;
    string sUserID;
    string sNewDataInfo;
    if (bForJson)
    {
        auto& param = req.params;
        auto it = param.find("DataID");
        if (it != param.end())
        {
            sDataID = Utf8ToGb2312( it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("UserID");
        if (it != param.end())
        {
            sUserID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("DataInfo");
        if (it != param.end())
        {
            sNewDataInfo = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("DataID");
        if (it != header.end())
        {
            sDataID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("DataInfo");
        if (it != header.end())
        {
            sNewDataInfo = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    
    vector<string> vecTemp = SplitString(sNewDataInfo, ',');
    string sNewItem;
    auto r = m_spDBOperator->ModifyUserDateaItem(sUserID, sDataID, vecTemp, sNewItem);
    if (bForJson)
    {
        if (ErrorCode::Success == r)
        {
            string keys[] = { "id", "data_name", "age", "gender", "data_type",
          "register_date", "hospital","description" ,"data_id_list", "oweruser_id" };
            string sJsonItem;
            auto vecNewInfo = SplitString(sNewItem, ',');
            for (size_t i = 0; i < vecNewInfo.size(); i++)
            {
                if (i == 8)
                {//Dataidlist
                    auto vecRoleName = SplitString(vecNewInfo[i], '&');
                    string sTemp;
                    PacketJsonArrary(sTemp, vecRoleName);
                    PacketData(keys[i], sTemp, res.headers, sJsonItem, bForJson, false);
                }
                else
                {
                    PacketData(keys[i], vecNewInfo[i], res.headers, sJsonItem, bForJson);
                }
            }
            EndPacketJson(sJsonItem, res); 
        }     
        else if (ErrorCode::Repeat == r)
        {
            PacketRepeat(res, bForJson);

        }
        else
        {
            PacketFailed(res, bForJson);
        }
    }
    else
    {
        if (ErrorCode::Success == r)
        {
            res.body = "pass";
            res.headers.emplace("DataInfo", std::move(sNewItem));
        }
        else if (ErrorCode::Repeat == r)
        {
            res.set_content("Repeat", "text/plain");

        }
        else
        {
            res.set_content("failed", "text/plain");
        }
    }
   
}

void HttpsServer::GetUserDataHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUserID;
    SearchCondition searchCondition;
    uint64_t iStartIndex = 0;
    uint64_t iCategory = 1;
    uint64_t iCount = 0;
    if (bForJson)
    {
        auto &params = req.params;
        auto it = params.find("UserID");
        if (it != params.end())
        {
            sUserID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            std::cout << " UserID failed  " << sUserID << std::endl;
            return;
        }
        //SearchCondition
        it = params.find("Hospital");
        if (it != params.end())
        {
            searchCondition.sHospital = Utf8ToGb2312(it->second);
        }
        std::cout << " Hospital:  " << searchCondition.sHospital << std::endl;

        it = params.find("PatientType");
        if (it != params.end())
        {
            searchCondition.sPatientType = Utf8ToGb2312(it->second);
        }

        it = params.find("StartDate");
        if (it != params.end())
        {
            searchCondition.startDate = SearchCondition::CovertDate(Utf8ToGb2312(it->second));
        }

        it = params.find("EndDate");
        if (it != params.end())
        {
            searchCondition.endDate = SearchCondition::CovertDate(Utf8ToGb2312(it->second));
        }

        it = params.find("Category");
        if (it != params.end())
        {
            searchCondition.sCategory = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

        it = params.find("StartIndex");
        char* pStopString;
        if (it != params.end())
        {
            auto pTemp = Utf8ToGb2312(it->second);
            iStartIndex = _strtoui64(pTemp, &pStopString, 10);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = params.find("Count");

        if (it != params.end())
        {
            auto pTemp = Utf8ToGb2312(it->second);
            iCount = _strtoui64(pTemp, &pStopString, 10);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            std::cout << " UserID failed  " << sUserID << std::endl;
            return;
        }
        //SearchCondition
        it = header.find("Hospital");
        if (it != header.end())
        {
            searchCondition.sHospital = it->second;
        }
        std::cout << " Hospital:  " << searchCondition.sHospital << std::endl;

        it = header.find("PatientType");
        if (it != header.end())
        {
            searchCondition.sPatientType = it->second;
        }

        it = header.find("StartDate");
        if (it != header.end())
        {
            searchCondition.startDate = SearchCondition::CovertDate(it->second);
        }

        it = header.find("EndDate");
        if (it != header.end())
        {
            searchCondition.endDate = SearchCondition::CovertDate(it->second);
        }

        it = header.find("Category");
        if (it != header.end())
        {
            searchCondition.sCategory = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }


        it = header.find("StartIndex");
        char* pStopString;
        if (it != header.end())
        {
            iStartIndex = _strtoui64(it->second.c_str(), &pStopString, 10);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("Count");

        if (it != header.end())
        {
            iCount = _strtoui64(it->second.c_str(), &pStopString, 10);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
    
    uint64_t uiDataAuthority = 0;
    uint64_t uiSystemAuthoriy = 0;
    uint64_t uiUserCenterAuthoriy = 0;
    uint64_t uiStasticsAuthoriy = 0;
    auto erroCode = m_spDBOperator->GetUserAuthority(sUserID, uiDataAuthority, uiSystemAuthoriy, uiUserCenterAuthoriy, uiStasticsAuthoriy);

    if (erroCode == ErrorCode::Failed)
    {
        PacketFailed(res, bForJson);
        return;
    }



    searchCondition.dataAuthoriy = uiDataAuthority;
    string sDataInfo;
    string sCount;  
    sDataInfo = m_spDBOperator->GetUserDataItems(sUserID,
            iStartIndex, iCount, sCount, searchCondition);
    
    if (!sDataInfo.empty() && sDataInfo != "NoData")
    {
        if (bForJson)
        {
            string sJson;
            string keys[] = { "id", "data_name", "age", "gender", "data_type",
                "register_date", "hospital","description" ,"data_id_list", "oweruser_id" };
            auto vecDataInfo = SplitString(sDataInfo, '|');
            for (auto item : vecDataInfo)
            {
                string sJsonItem;
                auto vecPerData = SplitString(item, ',');

                for (size_t i = 0; i < vecPerData.size(); i++)
                {
                    if (i == 8)
                    {//Dataidlist
                        auto vecRoleName = SplitString(vecPerData[i], '&');
                        string sTemp;
                        PacketJsonArrary(sTemp, vecRoleName);
                        PacketData(keys[i], sTemp, res.headers, sJsonItem, bForJson, false);
                    }
                    else
                    {
                        PacketData(keys[i], vecPerData[i], res.headers, sJsonItem, bForJson);
                    }
                }

                sJsonItem += "}";
                PacketJsonArrary(sJson, sJsonItem);
            }
            sJson += "]";
            string sResult;
            PacketData("AllCount", sCount, res.headers, sResult, bForJson);
            PacketData("DataInfo", sJson, res.headers, sResult, bForJson, false);
            EndPacketJson(sResult, res);
        }
        else
        {

            res.body = "pass";
            res.headers.emplace("AllCount", std::move(sCount));
            res.headers.emplace("DataInfo", std::move(sDataInfo));
        }
    }
    else
    {
        if (sDataInfo == "NoData" && bForJson)
        {
            string sJsonData;
            sJsonData += "{";
            sJsonData += R"("result")";
            sJsonData += R"(: "NoData")";
            sJsonData += "}";
            res.set_content(sJsonData, "text/plain");
        }
        else
        {
            PacketFailed(res, bForJson);
        }
    }
}

 
void HttpsServer::GetUserAllDataHandler(const Request& req, Response& res)
{
                                               
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    SearchCondition searchCondition;
    string sUserID;
    if (bForJson)
    {
        auto &param = req.params;
        auto it = param.find("UserID");
        if (it != param.end())
        {
            sUserID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            std::cout << " UserID failed  " << sUserID << std::endl;
            return;
        }
        it = param.find("Hospital");
        if (it != param.end())
        {
            searchCondition.sHospital = Utf8ToGb2312(it->second);
        }
        searchCondition.sHospital = searchCondition.sHospital;
        it = param.find("PatientType");
        if (it != param.end())
        {
            searchCondition.sPatientType = Utf8ToGb2312(it->second);
        }
        it = param.find("Category");
        if (it != param.end())
        {
            searchCondition.sCategory = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            std::cout << " UserID failed  " << sUserID << std::endl;
            return;
        }
        it = header.find("Hospital");
        if (it != header.end())
        {
            searchCondition.sHospital = it->second;
        }

        searchCondition.sHospital = searchCondition.sHospital;
        it = header.find("PatientType");
        if (it != header.end())
        {
            searchCondition.sPatientType = it->second;
        }
        it = header.find("Category");
        if (it != header.end())
        {
            searchCondition.sCategory = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
          
    uint64_t uiDataAuthority = 0;
    uint64_t uiSystemAuthoriy = 0;
    uint64_t uiUserCenterAuthoriy = 0;
    uint64_t uiStasticsAuthoriy = 0;

    auto erroCode = m_spDBOperator->GetUserAuthority(sUserID,
        uiDataAuthority,
        uiSystemAuthoriy,
        uiUserCenterAuthoriy,
        uiStasticsAuthoriy);

    if (erroCode == ErrorCode::Failed)
    {
        PacketFailed(res, bForJson);
    }
    else
    {
        string sDataInfo;
        string sCount;     
        searchCondition.dataAuthoriy = uiDataAuthority;
        sDataInfo = m_spDBOperator->GetUserDataItems(sUserID, 0, 
            INT64_MAX, sCount,
            searchCondition);
        if (!sDataInfo.empty() && sDataInfo != "NoData")
        {
            if (bForJson)
            {
                string sJson;
                string keys[] = { "id", "data_name", "age", "gender", "data_type",
                    "register_date", "hospital","description" ,"data_id_list", "oweruser_id" };
                auto vecDataInfo = SplitString(sDataInfo, '|');
                for (auto item : vecDataInfo)
                {
                    string sJsonItem;
                    auto vecPerData = SplitString(item, ',');

                    for (size_t i = 0; i < vecPerData.size(); i++)
                    {
                        if (i == 8)
                        {//Dataidlist
                            auto vecRoleName = SplitString(vecPerData[i], '&');
                            string sTemp;
                            PacketJsonArrary(sTemp, vecRoleName);
                            PacketData(keys[i], sTemp, res.headers, sJsonItem, bForJson, false);
                        }
                        else
                        {
                            PacketData(keys[i], vecPerData[i], res.headers, sJsonItem, bForJson);
                        }
                    }

                    sJsonItem += "}";
                    PacketJsonArrary(sJson, sJsonItem);
                }
                sJson += "]";
                string sResult;
                PacketData("AllCount", sCount, res.headers, sResult, bForJson);
                PacketData("DataInfo", sJson, res.headers, sResult, bForJson, false);
                EndPacketJson(sResult, res);
            }
            else
            {
                res.body = "pass";
                res.headers.emplace("AllCount", std::move(sCount));
                res.headers.emplace("DataInfo", std::move(sDataInfo));
            }
        }
        else
        {
            if (sDataInfo == "NoData" && bForJson)
            {
                string sJsonData;
                sJsonData += "{";
                sJsonData += R"("result")";
                sJsonData += R"(: "NoData")";
                sJsonData += "}";
                res.set_content(sJsonData, "text/plain");
            }
            else
            {
                PacketFailed(res, bForJson);
            }
        }
    }
}

void HttpsServer::GetDataListByOwerIDHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sOwerDataID;
    if (bForJson)
    {
        auto it = req.params.find("OwerDataID");
        if (it != req.params.end())
        {
            sOwerDataID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("OwerDataID");
        if (it != header.end())
        {
            sOwerDataID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
  
    auto sDataList = m_spDBOperator->GetDataListByOwerIDHandler(sOwerDataID);
    if (!sDataList.empty())
    {
        if (bForJson)
        {
            string sJson;
            string keys[] = { "id", "file_name", "file_path", "file_length"};

            auto vecDataItem = SplitString(sDataList, '|');
            for (auto item : vecDataItem)
            {
                string sJsonItem;
                auto vecPerData = SplitString(item, ',');
                for (size_t i = 0; i < vecPerData.size(); i++)
                {        
                    PacketData(keys[i], vecPerData[i], res.headers, sJsonItem, bForJson);
                }
                sJsonItem += "}";
                PacketJsonArrary(sJson, sJsonItem);
            }
            sJson += "]";
            string sResult;
            PacketData("DataList", sJson, res.headers, sResult, bForJson, false);
            EndPacketJson(sResult, res);
        }
        else
        {
            res.body = "pass";
            res.headers.emplace("DataList", std::move(sDataList));
        }
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}


void HttpsServer::ProduceRandomNumberCRand(UINT* RandomArray, UINT size)
{
    time_t t;
    srand((unsigned)time(&t));     
    for (UINT i = 0; i < size; i++)
    {
        RandomArray[i] = (rand() << 17) | (rand() << 3) | (rand());
    }
}  
void HttpsServer::ModifyUserRoleHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sNewRole;
    string sUserID;
    if (bForJson)
    {
        auto &param = req.params;
        auto it = param.find("NewRole");
        if (it != param.end())
        {
            sNewRole = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

        it = param.find("UserID");
        if (it != param.end())
        {
            sUserID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
    else
    {
        auto it = header.find("NewRole");
        if (it != header.end())
        {
            sNewRole = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

        it = header.find("UserID");
        if (it != header.end())
        {
            sUserID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
   

    if (ErrorCode::Success == m_spDBOperator->ModifyUserRole(sUserID, sNewRole))
    {
        vector<string> vecUserInfo;
        auto bResult = m_spDBOperator->GetUserInfoByUserID(sUserID, vecUserInfo);
        assert(bResult);
        vecUserInfo.erase(vecUserInfo.begin() + 3); //密码
        vecUserInfo.erase(vecUserInfo.begin() + 3); //salt
        string sNewRoleName;
        m_spDBOperator->GetRoleNameByRoleID(sNewRole, sNewRoleName);
        vecUserInfo.push_back(sNewRoleName);//增加角色名字在最后
        string sNewUserInfo = JoinString(vecUserInfo, ',');
        if (bForJson)
        {
            string sJson;
            string keys[] = { "id", "name", "login_name", "manager", "email", "role", "create_time","role_name" };
            string sJsonItem;
            auto vecInfo = SplitString(sNewUserInfo, ',');
            for (int i = 0; i < vecInfo.size(); i++)
            {
                if (i == 7)
                {
                    auto vecRoleName = SplitString(vecInfo[7], ';');
                    string sTemp;
                    PacketJsonArrary(sTemp, vecRoleName);
                    PacketData(keys[i], sTemp, res.headers, sJsonItem, bForJson, false);
                }
                else
                {
                    PacketData(keys[i], vecInfo[i], res.headers, sJsonItem, bForJson);
                }
            }
            EndPacketJson(sJsonItem, res);
        }
        else
        {
            res.headers.emplace("NewUserInfo", sNewUserInfo);
            res.body = "pass";
        }
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::AddPatientType(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sType;

    if (bForJson)
    {
        auto parIt = req.params.find("Type");
        if (parIt == req.params.end())
        {
            PacketFailed(res, bForJson);
            return;
        }
        sType = Utf8ToGb2312(parIt->second.c_str());
    }
    else
    {
        auto it = header.find("Type");
        if (it != header.end())
        {
            sType = it->second.c_str();
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }


    auto errorCode = m_spDBOperator->AddPatientType(sType);
    if (bForJson)
    {
        if (errorCode == ErrorCode::Success)
        {
            string sJsonData;
            PacketData("Type", sType, res.headers, sJsonData, bForJson);
            EndPacketJson(sJsonData, res);
        }
        else if (errorCode == ErrorCode::Repeat)
        {
            PacketRepeat(res, bForJson);
        }
        else
        {
            PacketFailed(res, bForJson);
        }
    }
    else
    {
        if (errorCode == ErrorCode::Success)
        {
            res.body = "pass";
            res.headers.emplace("Type", sType);
        }
        else if (errorCode == ErrorCode::Repeat)
        {
            res.body = "Repeat";
        }
        else
        {
            res.body = "failed";
        }
    }
   
}

void HttpsServer::GetPatientType(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sType;
    auto errorCode = m_spDBOperator->GetPatientType(sType);
    if (errorCode == ErrorCode::Success)
    {
        if (bForJson)
        {
            string sJsonData;
            auto vecType = SplitString(sType, ',');
            PacketJsonArrary(sJsonData, vecType);
            string sResult;
            PacketData("Type", sJsonData, res.headers, sResult, bForJson,false);
            EndPacketJson(sResult, res);
        }
        else
        {
            res.body = "pass";
            res.headers.emplace("Type", sType);
        }
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::GetAllRoleInfoHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    int iStartIndex;
    int iCount;
    if (bForJson)
    {
        auto& param = req.params;
        auto it = param.find("StartIndex");
        if (it != param.end())
        {
            iStartIndex = atoi(Utf8ToGb2312( it->second));
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("Count");
        if (it != param.end())
        {
            iCount = atoi(Utf8ToGb2312(it->second));
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("StartIndex");
        if (it != header.end())
        {
            iStartIndex = atoi(it->second.c_str());
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("Count");
        if (it != header.end())
        {
            iCount = atoi(it->second.c_str());
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    

    string sCount;
    auto sRoleInfo = m_spDBOperator->GetAllRoleInfo(iStartIndex, iCount, sCount);
    if (!sRoleInfo.empty())
    {
        if (bForJson)
        {                    
            string sJson;                       
            string keys[] = { "id", "data_authority", "system_authority", "usercenter_authority",
                "stastics_authority", "role_short_name","role_name" ,"create_time"};
            auto vecDataInfo = SplitString(sRoleInfo, '|');
            for (auto item : vecDataInfo)
            {
                string sJsonItem;
                auto vecPerData = SplitString(item, ',');

                for (size_t i = 0; i < vecPerData.size(); i++)
                {
                    PacketData(keys[i], vecPerData[i], res.headers, sJsonItem, bForJson);
                }

                sJsonItem += "}";
                PacketJsonArrary(sJson, sJsonItem);
            }
            sJson += "]";
            string sResult;
            PacketData("AllCount", sCount, res.headers, sResult, bForJson);
            PacketData("AllRoleInfo", sJson, res.headers, sResult, bForJson, false);
            EndPacketJson(sResult, res);
        }
        else
        {
            res.body = "pass";
            res.headers.emplace("AllRoleInfo", sRoleInfo);
            res.headers.emplace("AllCount", sCount);
        }
        
    }
    else
    {
        res.headers.emplace("AllCount", sCount);
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::ModifyRoleAuthorityHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sRoleInfos;
    if (bForJson)
    {
        auto it = req.params.find("RoleInfo");
        if (it != req.params.end())
        {
            sRoleInfos = Utf8ToGb2312( it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("RoleInfo");
        if (it != header.end())
        {
            sRoleInfos = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
   
    auto bSuccess = m_spDBOperator->ModifyRoleAuthority(sRoleInfos);
    if (bSuccess)
    {
        PacketSuccess(res, bForJson);
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::ModifyRoleInfoHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sRoleDes;
    string sRoleName;
    string sUID;
    if (bForJson)
    {
        auto& param = req.params;
        auto it = param.find("RoleDes");
        if (it != param.end())
        {
            sRoleDes = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("RoleName");
        if (it != param.end())
        {
            sRoleName = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("UID");
        if (it != param.end())
        {
            sUID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("RoleDes");
        if (it != header.end())
        {
            sRoleDes = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("RoleName");
        if (it != header.end())
        {
            sRoleName = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("UID");
        if (it != header.end())
        {
            sUID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    
    auto errorCode = m_spDBOperator->ModifyRoleInfo(sRoleDes, sRoleName, sUID);
    if (errorCode == ErrorCode::Success)
    {
        PacketSuccess(res, bForJson);
    }
    else if (errorCode == ErrorCode::Repeat)
    {
        PacketRepeat(res, bForJson);
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::DeleteRoleInfoHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUID;
    if (bForJson)
    {
        auto it = req.params.find("UID");
        if (it != req.params.end())
        {
            sUID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("UID");
        if (it != header.end())
        {
            sUID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
   

    auto errorCode = m_spDBOperator->DeleteRoleInfo(sUID);
    if (errorCode == ErrorCode::Success)
    {
        PacketSuccess(res, bForJson);
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}
 
void HttpsServer::AddRoleInfoHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sRoleDes;
    string sRoleName;
    if (bForJson)
    {
        auto& param = req.params;

        auto it = param.find("RoleDes");
        if (it != param.end())
        {
            sRoleDes = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = param.find("RoleName");
        if (it != param.end())
        {
            sRoleName = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("RoleDes");
        if (it != header.end())
        {
            sRoleDes = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("RoleName");
        if (it != header.end())
        {
            sRoleName = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    

    auto sNewRole = m_spDBOperator->AddRoleInfo(sRoleDes, sRoleName);
    if (!sNewRole.empty() && sNewRole != "Repeat")
    {
        if (bForJson)
        {
            string sJsonData;
            string keys[] = { "id", "data_authority", "system_authority", "usercenter_authority",
                "stastics_authority", "role_short_name","role_name" ,"create_time" };
            auto vecRole = SplitString(sNewRole, ',');
            for (int i = 0; i<vecRole.size(); i++)
            {
                PacketData(keys[i], vecRole[i], res.headers, sJsonData, bForJson);

            }
           // PacketData("RoleInfo", sNewRole, res.headers, sJsonData, bForJson);
            EndPacketJson(sJsonData, res);
        }
        else
        {
            res.body = "pass";
            res.headers.emplace("RoleInfo", sNewRole);
        }
;
    }
    else if (sNewRole == "Repeat")
    {
        PacketRepeat(res, bForJson);
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::DeleteDataHandler(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUID;
    if (bForJson)
    {
        auto it = req.params.find("UID");
        if (it != req.params.end())
        {
            sUID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("UID");
        if (it != header.end())
        {
            sUID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    
    auto sPath = m_spDBOperator->GetDataPath(sUID);
    if (!sPath.empty())
    {
        remove(sPath.c_str());
    }
    auto sOwerDataInfoId = m_spDBOperator->GetOwerDataInfoId(sUID);
    auto errorCode = m_spDBOperator->DeleteData(sUID);
    if (errorCode == ErrorCode::Success)
    {
        PacketSuccess(res, bForJson);
        if (!sOwerDataInfoId.empty())
        {
            m_spDBOperator->RemoveDataIdFromeDataInfo(sOwerDataInfoId, sUID);
        }
    }
    else
    {
        PacketFailed(res, bForJson);
    }
}

void HttpsServer::GetHospitalHandler(const Request& req, Response& res)
{
    std::cout << "GetHospitalHandler begin" <<  std::endl;
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUID;
    string sCategory;
    if (bForJson)
    {
        auto& param = req.params;
        auto it = param.find("UserID");
        if (it != param.end())
        {
            sUID = Utf8ToGb2312(it->second);
        }
        else
        {
            std::cout << "GetUserAuthority get  sUID failed ,sUID = " << sUID << std::endl;
            PacketFailed(res, bForJson);
            return;
        }

        it = param.find("Category");
        if (it != param.end())
        {
            sCategory = Utf8ToGb2312(it->second);
        }
        else
        {
            std::cout << "GetUserAuthority get  Category failed ,Category = " << std::endl;
            PacketFailed(res, bForJson);
            return;
        }
    }
    else
    {
        auto it = header.find("UID");
        if (it != header.end())
        {
            sUID = it->second;
        }
        else
        {
            std::cout << "GetUserAuthority get  sUID failed ,sUID = " << sUID << std::endl;
            PacketFailed(res, bForJson);
            return;
        }

        it = header.find("Category");
        if (it != header.end())
        {
            sCategory = it->second;
        }
        else
        {
            std::cout << "GetUserAuthority get  Category failed ,Category = " << std::endl;
            PacketFailed(res, bForJson);
            return;
        }
    }
    

    uint64_t uiDataAuthority = 0;
    uint64_t uiSystemAuthoriy = 0;
    uint64_t uiUserCenterAuthoriy = 0;
    uint64_t uiStasticsAuthoriy = 0;
    auto erroCode = m_spDBOperator->GetUserAuthority(sUID, uiDataAuthority, uiSystemAuthoriy, uiUserCenterAuthoriy, uiStasticsAuthoriy);
    if (erroCode == ErrorCode::Failed)
    {
        std::cout << "GetUserAuthority failed" << std::endl;
        PacketFailed(res, bForJson);
        return;
    }

    string sHospital;
    ErrorCode errorCode; 
    errorCode = m_spDBOperator->GetHospital(sUID, sCategory, uiDataAuthority, sHospital);
    if (erroCode == ErrorCode::Failed)
    {
        std::cout << "DB GetHospital failed" << std::endl;
        PacketFailed(res, bForJson);
        return;
    }

    if (bForJson)
    {
        auto vecHospital = SplitString(sHospital, ',');
        string sJsonData;
        for (size_t i = 0; i < vecHospital.size(); i++)
        {
            PacketJsonArrary(sJsonData, vecHospital[i]);
        }
        sJsonData += "]";
        string sResult;
        PacketData("Hospital", sJsonData, res.headers, sResult, bForJson, false);
        EndPacketJson(sResult, res);

    }
    else
    {
        res.body = "pass";
        res.headers.emplace("Hospital", sHospital);
    }
}

void HttpsServer::GetUserAuthority(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUID;
    if (bForJson)
    {
        auto it = req.params.find("UserID");
        if (it != req.params.end())
        {
            sUID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
    else
    {
        auto it = header.find("UID");
        if (it != header.end())
        {
            sUID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
   
    uint64_t uiDataAuthority = 0;
    uint64_t uiSystemAuthoriy = 0;
    uint64_t uiUserCenterAuthoriy = 0;
    uint64_t uiStasticsAuthoriy = 0;
    __int64 a;
    auto erroCode = m_spDBOperator->GetUserAuthority(sUID, uiDataAuthority, uiSystemAuthoriy, uiUserCenterAuthoriy, uiStasticsAuthoriy);
    if (erroCode == ErrorCode::Failed)
    {
        PacketFailed(res, bForJson);
        return;
    }
    res.body = "pass";
    //_strtoui64
    char temp[100] = { 0 };
    memset(temp, 0, sizeof(temp));
    _ui64toa_s(uiDataAuthority, temp, 100, 10);
    string sResult;
    if (bForJson)
    {
        PacketData("DataAuthority", temp, res.headers, sResult, bForJson, false);
    }
    else
    {
        res.headers.emplace("DataAuthority", string(temp));
    }

    memset(temp, 0, sizeof(temp));
    _ui64toa_s(uiSystemAuthoriy, temp, 100, 10);

    if (bForJson)
    {
        PacketData("SystemAuthoriy", temp, res.headers, sResult, bForJson, false);
    }
    else
    {
        res.headers.emplace("SystemAuthoriy", string(temp));
    }               
    memset(temp, 0, sizeof(temp));
    _ui64toa_s(uiUserCenterAuthoriy, temp, 100, 10);

    if (bForJson)
    {
        PacketData("UserCenterAuthoriy", temp, res.headers, sResult, bForJson, false);
    }
    else
    {
        res.headers.emplace("UserCenterAuthoriy", string(temp));
    }

    memset(temp, 0, sizeof(temp));
    _ui64toa_s(uiStasticsAuthoriy, temp, 100, 10);

    if (bForJson)
    {
        PacketData("StasticsAuthoriy", temp, res.headers, sResult, bForJson, false);
        EndPacketJson(sResult, res);
    }
    else
    {
        res.headers.emplace("StasticsAuthoriy", string(temp));
    }
}


//删除PatientType
void HttpsServer::DeletePatientType(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUID;
    if (bForJson)
    {
        auto it = req.params.find("UID");
        if (it != req.params.end())
        {
            sUID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
    else
    {
        auto it = header.find("UID");
        if (it != header.end())
        {
            sUID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    auto errorCode = m_spDBOperator->DeletePatientType(sUID);

    if (errorCode == ErrorCode::Failed)
    {
        PacketFailed(res, bForJson);
    }
    else
    {
        PacketSuccess(res, bForJson);
    }
}

void HttpsServer::ModifyPatientType(const Request& req, Response& res)
{
    auto& header = req.headers;
    auto bForJson = IsForJson(header);
    string sUID;
    string sPatientType;
    if (bForJson)
    {
        auto it = req.params.find("UID");
        if (it != req.params.end())
        {
            sUID = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = req.params.find("PatientType");
        if (it != req.params.end())
        {
            sPatientType = Utf8ToGb2312(it->second);
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }

    }
    else
    {
        auto it = header.find("UID");
        if (it != header.end())
        {
            sUID = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
        it = header.find("PatientType");
        if (it != header.end())
        {
            sPatientType = it->second;
        }
        else
        {
            PacketFailed(res, bForJson);
            return;
        }
    }
    auto errorCode = m_spDBOperator->ModifyPatientType(sUID, sPatientType);

    if (errorCode == ErrorCode::Failed)
    {
        PacketFailed(res, bForJson);
    }
    else if (errorCode == ErrorCode::Repeat)
    {
        PacketRepeat(res, bForJson);
    }
    else
    {
        PacketSuccess(res, bForJson);
    }
}



void HttpsServer::PacketData(const string& sKey,
    const string &sValue,
    Headers& headers,
    string& sJsonData,
    bool bForJson,
    bool bAddQuotationMark)
{
    if (!bForJson)
    {
        headers.emplace(sKey, sValue);
    }
    else
    {
        if (sJsonData.empty())
        {
            sJsonData += "{";
        }
        else
        {
            sJsonData += ",";
        }
        sJsonData += R"(")" + sKey + R"(")" + ":";
        if (bAddQuotationMark)
        {
            sJsonData += R"(")" + sValue + R"(")";
        }
        else
        {
            sJsonData += sValue ;
        }
    }
}

void HttpsServer::EndPacketJson(string& sJsonData, Response& res)
{
    sJsonData += R"(,"result")";
    sJsonData += R"(: "pass")";
    sJsonData += "}";
    res.set_content(sJsonData, "text/plain");
}
void HttpsServer::PacketRepeat( Response& res, bool bForJson)
{
    if (bForJson)
    {
        string sJsonData;
        sJsonData += "{";
        sJsonData += R"("result")";
        sJsonData += R"(: "Repeat")";
        sJsonData += "}";
        res.set_content(sJsonData, "text/plain");
    }
    else
    {
        res.set_content("Repeat", "text/plain");
    }
}

void HttpsServer::PacketFailed( Response& res, bool bForJson)
{
    if (bForJson)
    {
        string sJsonData;
        sJsonData += "{";
        sJsonData += R"("result")";
        sJsonData += R"(: "failed")";
        sJsonData += "}";
        res.set_content(sJsonData, "text/plain");
    }
    else
    {
        res.set_content("failed", "text/plain");
    }
    
}

void HttpsServer::PacketSuccess(Response& res, bool bForJson)
{
    if (bForJson)
    {
        string sJsonData;
        sJsonData += "{";
        sJsonData += R"("result")";
        sJsonData += R"(: "pass")";
        sJsonData += "}";
        res.set_content(sJsonData, "text/plain");
    }
    else
    {
        res.set_content("pass", "text/plain");
    }
}


void HttpsServer::PacketJsonArrary(string& sJsonData, const string& sItems)
{
    if (sJsonData.empty())
    {
        sJsonData += "[";
    }
    else
    {
        sJsonData += ",";
    }
    sJsonData += sItems;
}
  
void HttpsServer::PacketJsonArrary(string& sJsonData, vector<string>& vecItems)
{
    sJsonData = "[";
    for (auto & item : vecItems)
    {
        sJsonData += R"(")" + item + R"(")" + ",";
    }
    if (sJsonData.length() > 1)
    {
        sJsonData = sJsonData.substr(0, sJsonData.length() - 1);
    }
    sJsonData += "]";
}

bool HttpsServer::IsForJson(const Headers& header)
{
    bool bForJson = false;
    auto it = header.find("ForJson");
    if (it != header.end())
    {
        bForJson = true;
    }
    return bForJson;
}
