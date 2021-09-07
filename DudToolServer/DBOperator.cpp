#include "DBOperator.h"
#include <mutex>
#include <iostream> 
#include <cassert>
#include <algorithm>
#include "commondef.h"
#include <future>
#include <thread>
#include <chrono>
using namespace std;
string ansi_to_utf8(string strAnsi)
{
    UINT nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, NULL, NULL);
    WCHAR* wszBuffer = new WCHAR[nLen + 1];
    nLen = MultiByteToWideChar(936, NULL, strAnsi.c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;
    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    CHAR* szBuffer = new CHAR[nLen + 1];
    nLen = WideCharToMultiByte(CP_UTF8, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;
    strAnsi = szBuffer;
    delete[]wszBuffer;
    delete[]szBuffer;
    return strAnsi;
    /*auto ws = string2wstring(strAnsi);
    return UnicodeToUTF8(ws);*/
}
string utf8_to_ansi(string strUTF8) 
{
    UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
    WCHAR* wszBuffer = new WCHAR[nLen + 1];
    nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
    wszBuffer[nLen] = 0;
    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
    CHAR* szBuffer = new CHAR[nLen + 1];
    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
    szBuffer[nLen] = 0;
    strUTF8 = szBuffer;
    delete[]szBuffer;
    delete[]wszBuffer;
    return strUTF8;

    /*auto ws = UTF8ToUnicode(strUTF8);
    auto s = wstring2string(ws);
    return s;*/
}
string utf8_to_ansi(char* _strUTF8)
{
    if (_strUTF8 == nullptr)
        return"";
    else
        return utf8_to_ansi(string(_strUTF8));
}
//string utf8_to_ansi(char* _strUTF8)
//{
//    if (nullptr == _strUTF8)
//    {
//        return "";
//    }
//    string strUTF8 = string(_strUTF8);
//    UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
//    WCHAR* wszBuffer = new WCHAR[nLen + 1];
//    nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
//    wszBuffer[nLen] = 0;
//    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
//    CHAR* szBuffer = new CHAR[nLen + 1];
//    nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
//    szBuffer[nLen] = 0;
//    strUTF8 = szBuffer;
//    delete[]szBuffer;
//    delete[]wszBuffer;
//    return strUTF8;
//}

 


DBOperator::DBOperator()
{
}

DBOperator::~DBOperator()
{
    mysql_close(&m_myConnect);
    mysql_library_end();
}

bool DBOperator::Connect()
{
    string user = "root";
    string pswd = "123456";
    string host = "localhost";
    unsigned int port = 3306;
    ifstream  in("ServerConfig.ini", ios::in | ios::_Nocreate);
    if (!in.is_open())
    {
        std::cout << "打开文件 ServerConfig.ini 失败, 使用默数据库配置 user= root，pwd=123456,host=localhost, port=3306" << std::endl;
    }
    else
    {
        char buffer[1024];
        while (!in.eof())
        {
            in.getline(buffer, sizeof(buffer));
            auto vecTemp = SplitString(string(buffer), ':');
            if (vecTemp.size() != 2)
            {
                continue;
            }
            if (vecTemp[0] == "server_user")
            {
                user = vecTemp[1];
            }
            if (vecTemp[0] == "server_pwd")
            {
                pswd = vecTemp[1];
            }

            if (vecTemp[0] == "server_host")
            {
                host = vecTemp[1];

            }

            if (vecTemp[0] == "server_port")
            {
                port = atoi(vecTemp[1].c_str());
            }
        }
    }


    const char db[] = "filemanager";
    mysql_init(&m_myConnect);


    if (nullptr == mysql_real_connect(&m_myConnect,
        host.c_str(), user.c_str(), pswd.c_str(), db, port, NULL, 0))
    {
        return false;
    }
    mysql_set_character_set(&m_myConnect, "utf8");
    char optval = 1;
    mysql_options(&m_myConnect, MYSQL_OPT_RECONNECT, &optval);

    auto error = mysql_errno(&m_myConnect);

    auto t = std::thread( [this]()
    {
        //查询是否断开，并自动重连
        while (true)
        {
            {
                std::lock_guard<std::mutex> lock(m_connectMutext);
                mysql_ping(&m_myConnect);   //会修改查询的结果，所以要加锁
            }
           
            this_thread::sleep_for(chrono::seconds(5));
        }
    });
    t.detach();

    /*这句话是设置查询编码为utf8，这样支持中文*/
   /* mysql_query(&m_myConnect, "set names utf8");
    mysql_free_result()*/
    return true; ;
    //mysql_free_result()
   // mysql_close(&m_myConnect);
    //mysql_library_end
    
}

vector<vector<string> > DBOperator::GetAllUserInfo(const string& sUserName, int iStartIndex, int iCount, string& sTotalCount)
{
    vector<vector<string> > vecUserInfo;
    {
        std::lock_guard<std::mutex> lock(m_connectMutext);
        string sql = "select count(id) from filemanager.user_info";
        if (!sUserName.empty())
        {
            sql += " where binary ucase(name) like ucase('%" + sUserName + "%')";
        }
        auto utf8Sql = ansi_to_utf8(sql);
        std::cout << utf8Sql << std::endl;
        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return vector<vector<string>>();
        }

        MYSQL_RES* res_ptr_count; /*指向查询结果的指针*/
        res_ptr_count = mysql_store_result(&m_myConnect);
        auto column = mysql_num_fields(res_ptr_count);
        auto row = mysql_num_rows(res_ptr_count);
        for (int i = 0; i < row; i++)
        {
            vector<string> vecInfo;
            auto result_row = mysql_fetch_row(res_ptr_count);
            for (int j = 0; j < column; j++)
            {
                sTotalCount = (utf8_to_ansi((char*)result_row[j]));
                break;
            }
            break;
        }
        mysql_free_result(res_ptr_count);

        sql = "select id, name, login_name, manager, email, role, create_time from filemanager.user_info";
        if (!sUserName.empty())
        {
            sql += " where binary ucase(name) like ucase('%" + sUserName + "%')";
        }
        sql += " order by id desc limit ";
        sql += std::to_string(iStartIndex) + ",";
        sql += std::to_string(iCount);
        utf8Sql = ansi_to_utf8(sql);
        std::cout << utf8Sql << std::endl;

        res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
        }
        MYSQL_RES* res_ptr; /*指向查询结果的指针*/
        res_ptr = mysql_store_result(&m_myConnect);
        column = mysql_num_fields(res_ptr);
        assert(column == 7);
        row = mysql_num_rows(res_ptr);
        for (int i = 0; i < row; i++)
        {
            vector<string> vecInfo;
            auto result_row = mysql_fetch_row(res_ptr);
            for (int j = 0; j < column; j++)
            {
                vecInfo.push_back(utf8_to_ansi((char*)result_row[j]));
            }
            vecUserInfo.push_back(std::move(vecInfo));
        }
        mysql_free_result(res_ptr);
    }

    for (auto& it : vecUserInfo)
    {
        auto& roleIds = it[5];
        string sRoleName;
        GetRoleNameByRoleID(roleIds, sRoleName);
        it.push_back(sRoleName);
    }

    return std::move(vecUserInfo);
}

ErrorCode DBOperator::GetUserInfo(const string& sUserID, string& sUserInfo)
{
    vector<string> vecInfo;
    {
        std::lock_guard<std::mutex> lock(m_connectMutext);
        string sql = "select id, name, login_name, manager, email, role, create_time from filemanager.user_info where id = '" + sUserID + "'";
        string utf8Sql = ansi_to_utf8(sql);
        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            return ErrorCode::Failed;
            std::cout << " 执行sql失败：" << sql << std::endl;
        }
        MYSQL_RES* res_ptr; /*指向查询结果的指针*/
        res_ptr = mysql_store_result(&m_myConnect);
        int column = mysql_num_fields(res_ptr);
        assert(column == 7);
        int row = mysql_num_rows(res_ptr);
        if (row == 0)
        {
            return ErrorCode::Failed;
            std::cout << " 执行sql失败：" << sql << std::endl;
        }
        auto result_row = mysql_fetch_row(res_ptr);
        for (int j = 0; j < column; j++)
        {
            vecInfo.push_back(utf8_to_ansi((char*)result_row[j]));
        }
        mysql_free_result(res_ptr);
    }

    string sRoleName;
    GetRoleNameByRoleID(vecInfo[5], sRoleName);
    vecInfo.push_back(sRoleName);
    sUserInfo = std::move(JoinString(vecInfo, ','));
    return ErrorCode::Success;
}

bool DBOperator::GetUserInfoByUserName(const string& sName, vector<string>& vecUserInfo)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    vecUserInfo.clear();

    string sql = "SELECT id, name,login_name, password,salt, manager,email, role, create_time FROM filemanager.user_info where binary name = '" + sName + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }
    MYSQL_RES* res_ptr; /*指向查询结果的指针*/
    res_ptr = mysql_store_result(&m_myConnect);
    /*按行输出結果*/
    auto row = mysql_num_rows(res_ptr);
    auto column = mysql_num_fields(res_ptr);
    assert(column == 9);
    if (row != 1)
    {
        return false;
    }
    auto result_row = mysql_fetch_row(res_ptr);
    for (int i = 0; i < column; i++)
    {
        vecUserInfo.push_back(utf8_to_ansi((char*)result_row[i]));
    }
    mysql_free_result(res_ptr);
    return true;
}

bool DBOperator::GetUserInfoByLoginName(const string& sName, vector<string>& vecUserInfo)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    vecUserInfo.clear();
    string sql = "SELECT id, name,login_name, password,salt, manager,email, role, create_time FROM filemanager.user_info where binary login_name = '" + sName + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }
    MYSQL_RES* res_ptr; /*指向查询结果的指针*/
    res_ptr = mysql_store_result(&m_myConnect);
    /*按行输出結果*/
    auto row = mysql_num_rows(res_ptr);
    auto column = mysql_num_fields(res_ptr);
    assert(column == 9);
    if (row != 1)
    {
        return false;
    }
    auto result_row = mysql_fetch_row(res_ptr);
    for (int i = 0; i < column; i++)
    {
        vecUserInfo.push_back(utf8_to_ansi((char*)result_row[i]));
    }
    mysql_free_result(res_ptr);
    return true;
}

bool DBOperator::GetUserInfoByUserID(const string& sID, vector<string>& vecUserInfo)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "SELECT id, name,login_name, password,salt, manager,email, role, create_time FROM filemanager.user_info where id = '" + sID + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }
    MYSQL_RES* res_ptr; /*指向查询结果的指针*/
    res_ptr = mysql_store_result(&m_myConnect);
    /*按行输出結果*/
    auto row = mysql_num_rows(res_ptr);
    auto column = mysql_num_fields(res_ptr);
    assert(column == 9);
    if (row != 1)
    {
        return false;
    }
    auto result_row = mysql_fetch_row(res_ptr);
    for (int i = 0; i < column; i++)
    {
        vecUserInfo.push_back(utf8_to_ansi((char*)result_row[i]));
    }
    mysql_free_result(res_ptr);
    return true;
}

ErrorCode DBOperator::GetRoleNameByRoleID(const string& sRoleID, string& sRoleName)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    auto vecRole = SplitString(sRoleID, '&');
    int iSize = vecRole.size();
    if (iSize == 0)
    {
        return ErrorCode::Success;
    }

    string sql = "select role_short_name from filemanager.role_table where id = ";
    for (int i = 0; i < iSize; i++)
    {
        if (i == iSize - 1)
        {
            sql += vecRole[i];
        }
        else
        {
            sql += vecRole[i] + " or id = ";
        }
    }
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());

    MYSQL_RES* res_ptr; /*指向查询结果的指针*/
    res_ptr = mysql_store_result(&m_myConnect);
    auto column = mysql_num_fields(res_ptr);
    assert(column == 1);
    auto row = mysql_num_rows(res_ptr);
    for (int j = 0; j < row; j++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr);
        if (j == row - 1)
        {
            sRoleName += utf8_to_ansi((char*)result_row[0]);
        }
        else
        {
            sRoleName += utf8_to_ansi((char*)result_row[0]) + ";";
        }
    }
    mysql_free_result(res_ptr);
    return ErrorCode::Success;
}

bool DBOperator::HasSameName(const string& sName, const string& sLoginName)
{
    vector<string> vecUserInfo;
  /*  if (GetUserInfoByUserName(sName, vecUserInfo))
    {
        return true;
    }*/
    if (GetUserInfoByLoginName(sLoginName, vecUserInfo))
    {
        return true;
    }
    return false;
}

bool DBOperator::HasSameNameExceptUser(const string& sUserID, const string& sName, const string& sLoginName)
{
    //存在和sUserID不一样的用户拥有相同的名字
    vector<string> vecUserInfo;
    if (GetUserInfoByUserName(sName, vecUserInfo) && sUserID != vecUserInfo[0])
    {
        if (vecUserInfo.size() != 0 && sUserID != vecUserInfo[0])
        {
            return true;
        }
    }
    if (GetUserInfoByLoginName(sLoginName, vecUserInfo))
    {
        if (vecUserInfo.size() != 0 && sUserID != vecUserInfo[0])
        {
            return true;
        }
    }
    return false;
}


bool DBOperator::AddNewUser(const vector<string>& vecUserInfo, const string& sPassword, string& sSalt)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    auto sTime = getCurrentTimeStr();
    string sql = "insert filemanager.user_info(name, login_name, password,salt, manager, email,role, create_time) values('";
    sql += vecUserInfo[0] + "','";
    sql += vecUserInfo[1] + "','";
    sql += sPassword + "','";
    sql += sSalt + "','";
    sql += string("0") + "','";
    sql += vecUserInfo[3] + "','";
    sql += string("0") + "','";
    sql += sTime +"')";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }
    return true;
}
bool DBOperator::ModifyUserPassword(const string& sUserID, string& sPassword)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "update filemanager.user_info set password = '";
    sql += sPassword + "' where id = ";
    sql += sUserID;
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }

    return true;
}

bool DBOperator::ModifyUserInfo(const string& sUserID, const vector<string>& vecUserInfo, 
    const string& sPassword, string& sSalt)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "update filemanager.user_info set name = '";
    sql += vecUserInfo[0] + "', login_name = '";
    sql += vecUserInfo[1] + "'";
    if (!sPassword.empty())
    {
        sql += ", password = '" +  sPassword + "', salt = '";
        sql += sSalt + "'";
    }
    if (!vecUserInfo[3].empty())
    {
        sql += ", email = '";
        sql += vecUserInfo[3] + "'";
    }
    sql += " where id =";
    sql += sUserID;

    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }
    return true;
}
ErrorCode DBOperator::ModifyUserRole(const string& sUserID, const string& sNewRole)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "update filemanager.user_info set role='";
    sql += sNewRole + "' where id = '" + sUserID + "'";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ErrorCode::Failed;
    }
    return ErrorCode::Success;
}

bool DBOperator::DeleteUser(const string& sID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "delete from filemanager.user_info where id = '" + sID + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }
    return true;
}

bool DBOperator::AddUserData(const string& sUserName,
    const httplib::Headers & header,
    const string& _sPath,
    int64_t& iNewID,
    string& sNewID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sDataName;
    auto it = header.find("FileName");
    if (it != header.end())
    {
        sDataName = it->second;
    }
    assert(!sDataName.empty());

    string sTotalLength;
    it = header.find("TotalLength");
    if (it != header.end())
    {
        sTotalLength = it->second;
    }
    assert(!sTotalLength.empty());

    string sFileOwerID;
    it = header.find("FileOwerID");
    if (it != header.end())
    {
        sFileOwerID = it->second;
    }
    assert(!sFileOwerID.empty());

    string sPath = _sPath;
    std::replace(sPath.begin(), sPath.end(), '\\' ,'/');

    string sql = "insert filemanager.data_table(file_name, file_path, file_length,ower_data_info_id) values('";
    sql += sDataName + "','";
    sql += sPath + "','";
    sql += sTotalLength + "','";    
    sql += sFileOwerID + "')";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());

    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        std::cout << mysql_error(&m_myConnect) << std::endl;
        return false;
    }
    //获取id
    iNewID = m_myConnect.insert_id;
    sNewID = std::to_string(iNewID) +
        "," + sDataName + 
        "," + sPath + 
        "," + sTotalLength;
    return true;
}
void DBOperator::AppendDataId2DataInfo(const string& sDataInfoID, const string& sNewDataId)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "SELECT data_id_list FROM filemanager.data_info where id = '" + sDataInfoID + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ;
    }
    MYSQL_RES* res_ptr; /*指向查询结果的指针*/
    res_ptr = mysql_store_result(&m_myConnect);
    /*按行输出結果*/
    auto row = mysql_num_rows(res_ptr);
    auto column = mysql_num_fields(res_ptr);
    if (row != 1 || column != 1)
    {
        return;
    }
    auto result_row = mysql_fetch_row(res_ptr);
    string sDataIDList = utf8_to_ansi((char*)result_row[0]);
    if (sDataIDList.empty())
    {
        sDataIDList = sNewDataId;
    }
    else
    {
        sDataIDList += "&" + sNewDataId;
    }
    mysql_free_result(res_ptr);

    sql = "update filemanager.data_info set data_id_list='";    
    sql += sDataIDList + "' where id = '" + sDataInfoID + "'";
    utf8Sql = ansi_to_utf8(sql);
    res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
    }
}

void DBOperator::RemoveDataIdFromeDataInfo(const string& sDataInfoID, const string& sRemoveDataId)
{
    try
    {
        std::lock_guard<std::mutex> lock(m_connectMutext);
        string sql = "SELECT data_id_list FROM filemanager.data_info where id = '" + sDataInfoID + "'";
        auto utf8Sql = ansi_to_utf8(sql);

        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return;
        }
        MYSQL_RES* res_ptr; /*指向查询结果的指针*/
        res_ptr = mysql_store_result(&m_myConnect);
        /*按行输出結果*/
        auto row = mysql_num_rows(res_ptr);
        auto column = mysql_num_fields(res_ptr);
        if (row != 1 || column != 1)
        {
            return;
        }
        auto result_row = mysql_fetch_row(res_ptr);
        string sDataIDList = utf8_to_ansi((char*)result_row[0]);
        mysql_free_result(res_ptr);

        auto vecDataIdList = SplitString(sDataIDList, '&');
        auto it = std::find(vecDataIdList.begin(), vecDataIdList.end(), sRemoveDataId);
        if (it != vecDataIdList.end())
        {
            vecDataIdList.erase(it);
            sDataIDList = JoinString(vecDataIdList, '&');
            sql = "update filemanager.data_info set data_id_list='";
            sql += sDataIDList + "' where id = '" + sDataInfoID + "'";
            utf8Sql = ansi_to_utf8(sql);
            res = mysql_query(&m_myConnect, utf8Sql.c_str());
            if (res)
            {
                std::cout << " 执行sql失败：" << sql << std::endl;
            }
        }
    }
    catch (...)
    {
        return;
    }
}
bool DBOperator::IsDataRepeat(const string& sUserID, const vector<string>& vecDataInfo)
{
    string sTotalCount("");
    //判断是否重复
    string sql = "select count(id) from filemanager.data_info where binary data_name = '";
    sql += vecDataInfo[0] + "' and age = '";
    sql += vecDataInfo[1] + "' and gender = '";
    sql += vecDataInfo[3] + "' and binary data_type = '";
    sql += vecDataInfo[4] + "' and binary hospital = '";
    sql += vecDataInfo[2] + "' and  oweruser_id = '";
    sql += sUserID + "' and  data_category = '";
    sql += vecDataInfo[6] + "'";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        sTotalCount = "";
    }
    else
    {

        MYSQL_RES* res_ptr_count; /*指向查询结果的指针*/
        res_ptr_count = mysql_store_result(&m_myConnect);
        auto column = mysql_num_fields(res_ptr_count);
        auto row = mysql_num_rows(res_ptr_count);
        for (int i = 0; i < row; i++)
        {
            auto result_row = mysql_fetch_row(res_ptr_count);
            for (int j = 0; j < column; j++)
            {
                sTotalCount = (utf8_to_ansi((char*)result_row[j]));
                break;
            }
            break;
        }
        mysql_free_result(res_ptr_count);
    }
    if (!sTotalCount.empty() && sTotalCount != "0")
    {
        std::cout << " 执行sql失败： 字段重复" << sql << std::endl;
        return true;
    }
    return false;
}

bool DBOperator::IsDataRepeatExceptSelf(const string& sDataID, const vector<string>& vecDataInfo)
{
    string sTotalCount("");
    //判断是否重复
    string sql = "select count(id) from filemanager.data_info where binary data_name = '";
    sql += vecDataInfo[0] + "' and age = '";
    sql += vecDataInfo[1] + "' and gender = '";
    sql += vecDataInfo[3] + "' and binary data_type = '";
    sql += vecDataInfo[4] + "' and binary hospital = '";
    sql += vecDataInfo[2] + "' and  id != '";
    sql += sDataID + "' and  data_category = '";
    sql += vecDataInfo[6] + "'";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        sTotalCount = "";
    }
    else
    {

        MYSQL_RES* res_ptr_count; /*指向查询结果的指针*/
        res_ptr_count = mysql_store_result(&m_myConnect);
        auto column = mysql_num_fields(res_ptr_count);
        auto row = mysql_num_rows(res_ptr_count);
        for (int i = 0; i < row; i++)
        {
            auto result_row = mysql_fetch_row(res_ptr_count);
            for (int j = 0; j < column; j++)
            {
                sTotalCount = (utf8_to_ansi((char*)result_row[j]));
                break;
            }
            break;
        }
        mysql_free_result(res_ptr_count);
    }
    if (!sTotalCount.empty() && sTotalCount != "0")
    {
        std::cout << " 执行sql失败： 字段重复" << sql << std::endl;
        return true;
    }
    return false;
}



//新增一条数据：vecItems：姓名，年龄，性别，类型，医院，需求
ErrorCode DBOperator::AddUserDataItem(const string& sUserID,
    const vector<string>& vecDataInfo, 
    string &sNewInfo)
{
    uint64_t iNewID;
    {

        std::lock_guard<std::mutex> lock(m_connectMutext);
       
        if (IsDataRepeat(sUserID, vecDataInfo))
        {
            return ErrorCode::Repeat;
        }

        auto sTime = getCurrentTimeStr();
        string sql = "insert filemanager.data_info(data_name, age, gender, data_type,\
     register_date, hospital, description, oweruser_id, data_category) values('";
        sql += vecDataInfo[0] + "','";  //data_name
        sql += vecDataInfo[1] + "','";  //age
        sql += vecDataInfo[3] + "','";  //gender
        sql += vecDataInfo[4] + "','"; //类型
        sql += sTime + "','"; //注册时间

        sql += vecDataInfo[2] + "','"; //医院
        sql += vecDataInfo[5] + "','";   //需求描述
        sql += sUserID + "','";   //所属用户
        sql += vecDataInfo[6] + "')";   //data_category

        auto utf8Sql = ansi_to_utf8(sql);
        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            string sErr;
            auto iErroNo = mysql_errno(&m_myConnect);
            if (iErroNo == 1062)//字段值重复，入库失败
            {
                std::cout << " 执行sql失败：名字重复" << sql << std::endl;
                return ErrorCode::Repeat;
            }
            std::cout << " 执行sql失败：" << sql << std::endl;
            std::cout << mysql_error(&m_myConnect) << std::endl;
            return ErrorCode::Failed;
        }         
        iNewID = m_myConnect.insert_id;
    }
    sNewInfo = GetUserDataByID(std::to_string(iNewID));
    if (sNewInfo.empty())
    {
        return ErrorCode::Failed;
    }
    return ErrorCode::Success;
}

ErrorCode DBOperator::ModifyUserDateaItem(const string& sUserID, const string& sDataID, const vector<string>& vecDataInfo, string& sNewInfo)
{
    {
        std::lock_guard<std::mutex> lock(m_connectMutext);

        if (IsDataRepeatExceptSelf(sDataID, vecDataInfo))
        {
            return ErrorCode::Repeat;
        }
        string sql = "update filemanager.data_info set data_name = '";
        sql += vecDataInfo[0] + "', age = '";
        sql += vecDataInfo[1] + "', gender = '";
        sql += vecDataInfo[3] + "', data_type = '";
        sql += vecDataInfo[4] + "', hospital = '";
        sql += vecDataInfo[2] + "', description = '";
        sql += vecDataInfo[5] + "' where id = '";
        sql += sDataID + "'";
        auto utf8Sql = ansi_to_utf8(sql);
        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {

            std::cout << " 执行sql失败：" << sql << std::endl;
            std::cout << mysql_error(&m_myConnect) << std::endl;
            return ErrorCode::Failed;
        }
    }

    sNewInfo = GetUserDataByID(sDataID);
    if (sNewInfo.empty())
    {
        return ErrorCode::Failed;
    }
    return ErrorCode::Success;
}

string DBOperator::GetUserDataItems(const string& sUserID, uint64_t iStartIndex,
    uint64_t iCount, string& sTotalCount,
    SearchCondition& searchCondition)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "select count(id) from filemanager.data_info";

    bool hasData = false;
    if (!(searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::allDataMask))
    {
        if (searchCondition.sCategory == "1" && (searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::selfMRDataVisible))
        {//MR数据
            hasData = true;
        }
        if (searchCondition.sCategory == "2" && (searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::self3DDataVisible))
        {//3d数据
            hasData = true;
        }
        if (searchCondition.sCategory == "3" && (searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::selfAriginDataVisible))
        {//Arigin数据
            hasData = true;
        }

        if (hasData)
        {
            sql += " where binary data_category = '" + searchCondition.sCategory + "'";
            sql += " and oweruser_id = '" + sUserID + "'";
        }
    }
    else
    {
        if (searchCondition.sCategory == "1" && (searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::allMRDataVisible))
        {//MR数据
            hasData = true;
        }
        if (searchCondition.sCategory == "2" && (searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::all3DDataVisible))
        {//3d数据
            hasData = true;
        }
        if (searchCondition.sCategory == "3" && (searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::allAriginDataVisible))
        {//Arigin数据
            hasData = true;
        }
        if (hasData)
        {
            sql += " where binary data_category = '" + searchCondition.sCategory + "'";
        }
    }

    if (!hasData)
    {
        std::cout << " 没有符合条件的数据：" << sql << std::endl;
        return "NoData";
    }

    if (!searchCondition.sHospital.empty())
    {
        sql += " and binary hospital = '" + searchCondition.sHospital + "'";
    }
    if (!searchCondition.sPatientType.empty())
    {
        sql += " and binary data_type = '" + searchCondition.sPatientType + "'";
    }

    if (searchCondition.startDate.isValid())
    { 
        sql += " and binary register_date >= '" + searchCondition.startDate.ToSqlSearString() + "'";
    }

    if (searchCondition.endDate.isValid())
    {
        sql += " and binary register_date <= '" + searchCondition.endDate.ToSqlSearString() + "'";
    }

    std::cout<<"GetUserDataItems: sql"<<sql<<std::endl;

    auto utf8Sql = ansi_to_utf8(sql);
    std::cout << "GetUserDataItems: utf8Sql" << utf8Sql << std::endl;

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return "";
    }

    MYSQL_RES* res_ptr_count; /*指向查询结果的指针*/
    res_ptr_count = mysql_store_result(&m_myConnect);
    auto column = mysql_num_fields(res_ptr_count);
    auto row = mysql_num_rows(res_ptr_count);
    for (int i = 0; i < row; i++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr_count);
        for (int j = 0; j < column; j++)
        {
            sTotalCount = (utf8_to_ansi((char*)result_row[j]));
            break;
        }
        break;
    }
    mysql_free_result(res_ptr_count);

    string sDataInfo;
    sql = "select id, data_name,age, gender,data_type, register_date,hospital,\
        description, data_id_list, oweruser_id from filemanager.data_info ";
    sql += " where data_category = '" + searchCondition.sCategory + "'";
    
    if (!(searchCondition.dataAuthoriy & (uint64_t)DataAuthorityDef::allDataMask))
    {
        sql += " and oweruser_id = '" + sUserID + "'";
    }
    if (!searchCondition.sHospital.empty())
    {
        sql += " and binary hospital = '" + searchCondition.sHospital + "'";
    }
    if (!searchCondition.sPatientType.empty())
    {
        sql += " and binary data_type = '" + searchCondition.sPatientType + "'";
    }
    if (searchCondition.startDate.isValid())
    {
        sql += " and binary register_date >= '" + searchCondition.startDate.ToSqlSearString() + "'";
    }

    if (searchCondition.endDate.isValid())
    {
        sql += " and binary register_date <= '" + searchCondition.endDate.ToSqlSearString() + "'";
    }

    sql += " order by id desc limit ";
    sql += std::to_string(iStartIndex) + ",";
    sql += std::to_string(iCount);
    utf8Sql = ansi_to_utf8(sql);
    res = mysql_query(&m_myConnect, utf8Sql.c_str());

    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
    }
    else
    {
        sDataInfo = PacketUserData();
    }
    if (sDataInfo.empty())
    {
        sDataInfo = "NoData";
    }
    std::cout << "***********sDtaInfo = " << sDataInfo << std::endl;
    return std::move(sDataInfo);
}
string DBOperator::GetAllDataItems(uint64_t iStartIndex, uint64_t iCount, string& sTotalCount,
    SearchCondition& searchCondition)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "select count(id) from filemanager.data_info";
    bool bWhereSet = false;
    if (!searchCondition.sHospital.empty())
    {
        sql += " where binary hospital = '" + searchCondition.sHospital + "'";
        bWhereSet = true;
    }
    if (!searchCondition.sPatientType.empty())
    {
        if (!bWhereSet)
        {
            sql += " where binary data_type = '" + searchCondition.sPatientType + "'";
        }
        else
        {
            sql += " and binary data_type = '" + searchCondition.sPatientType + "'";
        }
        bWhereSet = true;
    }
    if (searchCondition.startDate.isValid())
    {
        if (!bWhereSet)
        {
            sql += " where binary register_date >= '" + searchCondition.startDate.ToSqlSearString() + "'";
        }
        else
        {
            sql += " and binary register_date >= '" + searchCondition.startDate.ToSqlSearString() + "'";
        }
        bWhereSet = true;
    }

    if (searchCondition.endDate.isValid())
    {
        if (!bWhereSet)
        {
            sql += " where binary register_date <= '" + searchCondition.endDate.ToSqlSearString() + "'";
        }
        else
        {
            sql += " and binary register_date <= '" + searchCondition.endDate.ToSqlSearString() + "'";
        }
        bWhereSet = true;
    }
    if (!bWhereSet)
    {
        sql += " where data_category =  '" + searchCondition.sCategory+ "'";
    }
    else
    {
        sql += " and data_category = '" + searchCondition.sCategory + "'";
        bWhereSet = true;
    }

    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return "";
    }

    MYSQL_RES* res_ptr_count; /*指向查询结果的指针*/
    res_ptr_count = mysql_store_result(&m_myConnect);
    auto column = mysql_num_fields(res_ptr_count);
    auto row = mysql_num_rows(res_ptr_count);
    for (int i = 0; i < row; i++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr_count);
        for (int j = 0; j < column; j++)
        {
            sTotalCount = (utf8_to_ansi((char*)result_row[j]));
            break;
        }
        break;
    }
    mysql_free_result(res_ptr_count);


    string sDataInfo;
    //data_id_list 不能放到最后一个，因为可为空，会导致拼接的字符串不正确
    sql = "select id, data_name,age, gender,data_type, register_date,hospital,\
        description,data_id_list, oweruser_id from filemanager.data_info"; 

    bWhereSet = false;
    if (!searchCondition.sHospital.empty())
    {
        sql += " where binary hospital = '" + searchCondition.sHospital + "'";
        bWhereSet = true;
    }
    if (!searchCondition.sPatientType.empty())
    {
        if (!bWhereSet)
        {
            sql += " where binary data_type = '" + searchCondition.sPatientType + "'";
        }
        else
        {
            sql += " and binary data_type = '" + searchCondition.sPatientType + "'";
        }
        bWhereSet = true;
    }
    if (searchCondition.startDate.isValid())
    {
        if (!bWhereSet)
        {
            sql += " where binary register_date >= '" + searchCondition.startDate.ToSqlSearString() + "'";
        }
        else
        {
            sql += " and binary register_date >= '" + searchCondition.startDate.ToSqlSearString() + "'";
        }
        bWhereSet = true;
    }

    if (searchCondition.endDate.isValid())
    {
        if (!bWhereSet)
        {
            sql += " where binary register_date <= '" + searchCondition.endDate.ToSqlSearString() + "'";
        }
        else
        {
            sql += " and binary register_date <= '" + searchCondition.endDate.ToSqlSearString() + "'";
        }
        bWhereSet = true;
    }

    if (!bWhereSet)
    {
        sql += " where data_category =  '" + searchCondition.sCategory + "'";
    }
    else
    {
        sql += " and data_category = '" + searchCondition.sCategory + "'";
        bWhereSet = true;
    }
    sql += " order by id desc limit ";
    sql += std::to_string(iStartIndex) + ",";
    sql += std::to_string(iCount);
    utf8Sql = ansi_to_utf8(sql);
    res = mysql_query(&m_myConnect, utf8Sql.c_str());

    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
    }
    else
    {
        sDataInfo = PacketUserData();
    }
    return std::move(sDataInfo);
}

string DBOperator::GetDataListByOwerIDHandler(const string& sDataRegiserID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    //data_id_list 不能放到最后一个，因为可为空，会导致拼接的字符串不正确
    string sql = "select id, file_name, file_path, file_length from filemanager.data_table where ower_data_info_id='";
    sql += sDataRegiserID + "'";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());

    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return "";
    }
    MYSQL_RES* res_ptr_count; /*指向查询结果的指针*/
    res_ptr_count = mysql_store_result(&m_myConnect);
    auto column = mysql_num_fields(res_ptr_count);
    auto row = mysql_num_rows(res_ptr_count);
    assert(column == 4);
    string sDataInfo;
    for (int i = 0; i < row; i++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr_count);
        sDataInfo += (utf8_to_ansi((char*)result_row[0]));
        sDataInfo += "," + (utf8_to_ansi((char*)result_row[1]));
        sDataInfo += "," + (utf8_to_ansi((char*)result_row[2]));
        sDataInfo += "," + (utf8_to_ansi((char*)result_row[3]));
        if (i != row - 1)
        {
            sDataInfo += "|";
        }
    }
    mysql_free_result(res_ptr_count);
    return std::move(sDataInfo);
}

string DBOperator::GetUserDataByID(const string& sID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sDataInfo;
    //data_id_list 不能放到最后一个，因为可为空，会导致拼接的字符串不正确
    string sql = "select id, data_name,age, gender,data_type, register_date,hospital,\
        description,data_id_list, oweruser_id from filemanager.data_info where id='";
    sql += sID + "'";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());

    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
    }
    else
    {
        sDataInfo = PacketUserData();
    }
    return std::move(sDataInfo);
}

string DBOperator::PacketUserData()
{
    MYSQL_RES* res_ptr; /*指向查询结果的指针*/
    res_ptr = mysql_store_result(&m_myConnect);
    auto column = mysql_num_fields(res_ptr);
    assert(column == 10);
    auto row = mysql_num_rows(res_ptr);
    vector < vector<string> > vecInfo;
    for (int i = 0; i < row; i++)
    {
        auto result_row = mysql_fetch_row(res_ptr);
        vector<string> vecTemp;
        vecTemp.push_back(utf8_to_ansi((char*)result_row[0]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[1]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[2]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[3]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[4]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[5]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[6]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[7]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[8]));
        vecTemp.push_back(utf8_to_ansi((char*)result_row[9]));
        vecInfo.push_back(vecTemp);

        //sDataInfo += utf8_to_ansi((char*)result_row[0])
        //    + string(",") + utf8_to_ansi((char*)result_row[1])
        //    + "," + utf8_to_ansi((char*)result_row[2])
        //    + "," + utf8_to_ansi((char*)result_row[3])
        //    + "," + utf8_to_ansi((char*)result_row[4])
        //    + "," + utf8_to_ansi((char*)result_row[5])
        //    + "," + utf8_to_ansi((char*)result_row[6])
        //    + "," + utf8_to_ansi((char*)result_row[7]);
        //string sDataIdList = utf8_to_ansi((char*)result_row[8]);

        //sDataInfo += "," + sDataIdList
        //    + "," + utf8_to_ansi((char*)result_row[9]);
        //if (i != row - 1)
        //{
        //    sDataInfo += "|";
        //}
    }
    mysql_free_result(res_ptr);
    
    string sPatientType;
    while(true)
    {
        string sql = "select * from filemanager.patienttype_table order by id desc ";
        string utf8Sql = ansi_to_utf8(sql);
        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            break;
        }
        auto res_ptr = mysql_store_result(&m_myConnect);
        int column = mysql_num_fields(res_ptr);
        int row = mysql_num_rows(res_ptr);
        if (row == 0 || column != 3)
        {
            std::cout << " 没有数据：" << sql << std::endl;
            break;
        }

        for (int i = 0; i < row; i++)
        {
            vector<string> vecInfo;
            auto result_row = mysql_fetch_row(res_ptr);
            for (int j = 0; j < column; j++)
            {
                auto sTemp = (utf8_to_ansi((char*)result_row[j]));
                sPatientType += sTemp;
                if (j != column - 1)
                {
                    sPatientType += ",";
                }
            }
            if (i != row - 1)
            {
                sPatientType += "|";
            }
        }
        mysql_free_result(res_ptr);
        break;
    }


   
    auto vecType = SplitString(sPatientType, '|');
    vector<vector<string>> vecPatientType;
    for (size_t i = 0; i < vecType.size(); i++)
    {
        auto vecTemp = SplitString(vecType[i], ',');
        vecPatientType.push_back(vecTemp);
    }
   

    for (size_t i = 0; i < vecInfo.size(); i++)
    {
        string sPateintID = vecInfo[i][4];
        vecInfo[i][4] = "";
        for (size_t k = 0; k < vecType.size(); k++)
        {
            string id = vecPatientType[k][0];
            if (id == sPateintID)
            {
                vecInfo[i][4] = vecPatientType[k][1];
            }
        }
    }
    string sDataInfo;

    vector<string> vecTemp;
    for (size_t i = 0; i < vecInfo.size(); i++)
    {
        auto sTemp = JoinString(vecInfo[i], ',');
        vecTemp.push_back(sTemp);
    }
    sDataInfo = JoinString(vecTemp, '|');
    return std::move(sDataInfo);
}

string DBOperator::GetDataPath(const string& sID)
{
    try
    {
        std::lock_guard<std::mutex> lock(m_connectMutext);
        string sql = "select file_path from filemanager.data_table where id = '" + sID + "'";
        auto utf8Sql = ansi_to_utf8(sql);

        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return "";
        }

        string sPath;

        MYSQL_RES* res_ptr = nullptr; /*指向查询结果的指针*/
        res_ptr = mysql_store_result(&m_myConnect);
        auto column = mysql_num_fields(res_ptr);
        assert(column == 1);
        auto row = mysql_num_rows(res_ptr);
        if (row == 0)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return "";
        }
        assert(row == 1);
        auto result_row = mysql_fetch_row(res_ptr);
        sPath = result_row[0];
        mysql_free_result(res_ptr);
        return utf8_to_ansi(sPath);
    }
    catch (...)
    {
        return "";
    }
}

string DBOperator::GetDataName(const string& sID)
{
    try
    {
        std::lock_guard<std::mutex> lock(m_connectMutext);
        string sql = "select file_name from filemanager.data_table where id = '" + sID + "'";
        auto utf8Sql = ansi_to_utf8(sql);

        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return "";
        }

        string sName;

        MYSQL_RES* res_ptr; /*指向查询结果的指针*/
        res_ptr = mysql_store_result(&m_myConnect);
        auto column = mysql_num_fields(res_ptr);
        assert(column == 1);
        auto row = mysql_num_rows(res_ptr);
        if (row == 0)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return "";
        }
        assert(row == 1);
        auto result_row = mysql_fetch_row(res_ptr);
        sName = result_row[0];
        mysql_free_result(res_ptr);
        return utf8_to_ansi(sName);
    }
    catch (...)
    {
        return "";
    }
}



string DBOperator::GetOwerDataInfoId(const string& sID)
{
    try
    {
        std::lock_guard<std::mutex> lock(m_connectMutext);
        string sql = "select ower_data_info_id from filemanager.data_table where id = '" + sID + "'";
        auto utf8Sql = ansi_to_utf8(sql);

        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return "";
        }

        string sValue;

        MYSQL_RES* res_ptr; /*指向查询结果的指针*/
        res_ptr = mysql_store_result(&m_myConnect);
        auto column = mysql_num_fields(res_ptr);
        assert(column == 1);
        auto row = mysql_num_rows(res_ptr);
        assert(row == 1);
        auto result_row = mysql_fetch_row(res_ptr);
        sValue = result_row[0];
        mysql_free_result(res_ptr);
        return utf8_to_ansi(sValue);
    }
    catch (...)
    {
        return "";
    }
}

string DBOperator::GetAllRoleInfo(int iStartIndex, int iCount, string& sTotalCount)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "select count(id) from filemanager.role_table";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return "";
    }

    MYSQL_RES* res_ptr_count; /*指向查询结果的指针*/
    res_ptr_count = mysql_store_result(&m_myConnect);
    auto column = mysql_num_fields(res_ptr_count);
    auto row = mysql_num_rows(res_ptr_count);
    for (int i = 0; i < row; i++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr_count);
        for (int j = 0; j < column; j++)
        {
            sTotalCount = (utf8_to_ansi((char*)result_row[j]));
            break;
        }
        break;
    }
    mysql_free_result(res_ptr_count);

    vector<vector<string> > vecUserInfo;
    sql = "select id, data_authority,system_authority,usercenter_authority,\
          stastics_authority, role_short_name,  \
        role_name, create_time from filemanager.role_table order by id  desc limit ";
    sql += std::to_string(iStartIndex) + ",";
    sql += std::to_string(iCount);

    utf8Sql = ansi_to_utf8(sql);
    res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return "";
    }
    auto res_ptr = mysql_store_result(&m_myConnect);
    column = mysql_num_fields(res_ptr);
    row = mysql_num_rows(res_ptr);
    string sResult;
    for (int i = 0; i < row; i++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr);
        for (int j = 0; j < column; j++)
        {    
            auto sTemp = (utf8_to_ansi((char*)result_row[j]));
            sResult += sTemp;
            if (j != column - 1)
            {
                sResult += ",";
            }
        }
        if (i != row - 1)
        {
            sResult += "|";
        }
    }
    mysql_free_result(res_ptr);
    return sResult;
}

bool DBOperator::ModifyRoleAuthority(const string& sNewInfo)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    auto vecRoleInfos = SplitString(sNewInfo, ',');
    assert(vecRoleInfos.size() == 5);
    auto id = vecRoleInfos[0];
    string sql = "update filemanager.role_table set data_authority='";
    sql += vecRoleInfos[1] + "', system_authority = '";
    sql += vecRoleInfos[2] + "', usercenter_authority = '";
    sql += vecRoleInfos[3] + "', stastics_authority = '";
    sql += vecRoleInfos[4] + "' where id = '";
    sql += id + "'";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return false;
    }
    
    return true;
}

ErrorCode DBOperator::GetUserAuthority(
    const string& sUserID,
    uint64_t& uiDataAuthority, uint64_t& uiSystemAuthoriy,
    uint64_t& uiUserCenterAuthoriy, uint64_t& uiStasticsAuthoriy)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    uiDataAuthority = 0;
    uiSystemAuthoriy = 0;
    uiUserCenterAuthoriy = 0;
    uiStasticsAuthoriy = 0;

    string sRole;
    {
        string sql = "select role from filemanager.user_info where id = '" + sUserID + "'";
        auto utf8Sql = ansi_to_utf8(sql);   
        auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
        if (res)
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            return ErrorCode::Failed;
        }
        MYSQL_RES* res_ptr; /*指向查询结果的指针*/
        res_ptr = mysql_store_result(&m_myConnect);
        auto column = mysql_num_fields(res_ptr);
        assert(column == 1);
        auto row = mysql_num_rows(res_ptr);
        assert(row == 1);
        if (row == 0)
        {
            mysql_free_result(res_ptr);
            return ErrorCode::Failed;
        }
        auto result_row = mysql_fetch_row(res_ptr);
        sRole = (utf8_to_ansi((char*)result_row[0]));
        mysql_free_result(res_ptr);
    }
    auto vecRole = SplitString(sRole, '&');
    for (auto& it : vecRole)
    {
        char* pStopString;
        auto sRoleId = it;
        {
            string sql = "select data_authority,system_authority,usercenter_authority,stastics_authority from filemanager.role_table where id = '" + sRoleId + "'";
            auto utf8Sql = ansi_to_utf8(sql);
            auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
            if (res)
            {
                std::cout << " 执行sql失败：" << sql << std::endl;
                return ErrorCode::Failed;
            }
            MYSQL_RES* res_ptr; /*指向查询结果的指针*/
            res_ptr = mysql_store_result(&m_myConnect);
            auto column = mysql_num_fields(res_ptr);
            assert(column == 4);
            auto row = mysql_num_rows(res_ptr);
            if (row == 0)
            {//不存在此id
                continue;

            }
            assert(row == 1);

            auto result_row = mysql_fetch_row(res_ptr);
            auto sDataAuthoriy = (utf8_to_ansi((char*)result_row[0]));
            auto sStyemAuthoriy = (utf8_to_ansi((char*)result_row[1]));
            auto sUserCenterAuthoriy = (utf8_to_ansi((char*)result_row[2]));
            auto sStasticsAuthoriy = (utf8_to_ansi((char*)result_row[3]));

            uiDataAuthority |= _strtoui64(sDataAuthoriy.c_str(), &pStopString, 10);
            uiSystemAuthoriy |= _strtoui64(sStyemAuthoriy.c_str(), &pStopString, 10);
            uiUserCenterAuthoriy |= _strtoui64(sUserCenterAuthoriy.c_str(), &pStopString, 10);
            uiStasticsAuthoriy |= _strtoui64(sStasticsAuthoriy.c_str(), &pStopString, 10);

            mysql_free_result(res_ptr);
        }
    }
    return ErrorCode::Success;
}

string DBOperator::AddRoleInfo(const string& sRoleDes, const string& sRoleName)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    auto sTime = getCurrentTimeStr();
    string sql = "insert filemanager.role_table(data_authority,system_authority,role_short_name, role_name, create_time) values('";
    sql += "0',";
    sql +="'0','";
    sql += sRoleDes + "','";
    sql += sRoleName + "','";
    sql += sTime + "')";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        string sErr;
        auto iErroNo = mysql_errno(&m_myConnect);
        if (iErroNo == 1062)//字段值重复，入库失败
        {
            sErr = "Repeat";
        }
        std::cout << " 执行sql失败：" << sql << std::endl;
        std::cout << mysql_error(&m_myConnect) << std::endl;
        return sErr;
    }
    auto sId = mysql_insert_id(&m_myConnect);
    string sNewLine = std::to_string(sId) +  "," + string("0, 0, 0, 0,")  + sRoleDes + "," + sRoleName + "," + sTime;
    return sNewLine;
}

ErrorCode DBOperator::ModifyRoleInfo(const string& sRoleDes, const string& sRoleName, const string& sUID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "update filemanager.role_table set role_short_name='";
    sql += sRoleDes + "', role_name='";
    sql += sRoleName + "' where id = '" + sUID + "'";
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        string sErr;
        auto iErroNo = mysql_errno(&m_myConnect);
        if (iErroNo == 1062)//字段值重复，入库失败
        {
            return ErrorCode::Repeat;
        }
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ErrorCode::Failed;
    }
    return ErrorCode::Success;
}

ErrorCode DBOperator::DeleteRoleInfo(const string& sUID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "delete from filemanager.role_table where id = '" + sUID + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ErrorCode::Failed;
    }
    return ErrorCode::Success;
}

ErrorCode DBOperator::AddPatientType(const string& sPatientType)
{

    "insert filemanager.user_info(name, login_name, password,salt, manager, email,role, create_time) values('";

    std::lock_guard<std::mutex> lock(m_connectMutext);
    auto sTime = getCurrentTimeStr();
    string sql = "insert filemanager.patienttype_table(type, create_time) values('";
    sql += sPatientType + "','";
    sql += sTime + "')";
                                     
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        auto iErroNo = mysql_errno(&m_myConnect);
        if (iErroNo == 1062)//字段值重复，入库失败
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            std::cout << mysql_error(&m_myConnect) << std::endl;
            return ErrorCode::Repeat;
        }
        else
        {
            return ErrorCode::Failed;
        }
    }
    return ErrorCode::Success;
}
ErrorCode DBOperator::ModifyPatientType(const string& sUID, const string& sNewType)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "update filemanager.patienttype_table set type = '";
    sql += sNewType + "' where id = ";
    sql += sUID;
    auto utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        auto iErroNo = mysql_errno(&m_myConnect);
        if (iErroNo == 1062)//字段值重复，入库失败
        {
            std::cout << " 执行sql失败：" << sql << std::endl;
            std::cout << mysql_error(&m_myConnect) << std::endl;
            return ErrorCode::Repeat;
        }
        else
        {
            return ErrorCode::Failed;
        }
    }
    return ErrorCode::Success;
}


ErrorCode DBOperator::GetPatientType(string& sPatientType)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "select * from filemanager.patienttype_table order by id desc";
    string utf8Sql = ansi_to_utf8(sql);
    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ErrorCode::Failed;
    }
    auto res_ptr = mysql_store_result(&m_myConnect);
    int column = mysql_num_fields(res_ptr);
    int row = mysql_num_rows(res_ptr);
    if (row == 0 || column != 3)
    {
        std::cout << " 没有数据：" << sql << std::endl;
        return ErrorCode::Failed;
    }
 
    for (int i = 0; i < row; i++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr);
        for (int j = 0; j < column; j++)
        {
            auto sTemp = (utf8_to_ansi((char*)result_row[j]));
            sPatientType += sTemp ;
            if (j != column - 1)
            {
                sPatientType += ",";
            }
        }
        if (i != row - 1)
        {
            sPatientType += "|";
        }
    }
    mysql_free_result(res_ptr);
    return ErrorCode::Success;

}

ErrorCode DBOperator::DeletePatientType(const string& sUID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "delete from filemanager.patienttype_table where id = '" + sUID + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ErrorCode::Failed;
    }
    return ErrorCode::Success;
}




ErrorCode DBOperator::GetHospital(const string& sUID, 
    const string& sCategory,
    uint64_t uiDataAuthority,
    string& sHospital)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql;
    if (uiDataAuthority & (uint64_t)DataAuthorityDef::allDataMask)
    {
        sql = "select distinct hospital from filemanager.data_info where data_category = '";
        sql += sCategory + "'";
    }
    else
    {
        sql = "select distinct hospital from filemanager.data_info where oweruser_id = '" + sUID + "'";
        sql += "and data_category = '" + sCategory + "'";
    }
    
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ErrorCode::Failed;
    }

    auto res_ptr = mysql_store_result(&m_myConnect);
    int column = mysql_num_fields(res_ptr);
    assert(column == 1);
    int row = mysql_num_rows(res_ptr);
    std::cout << "GetHospital sql = " << sql << endl;
    std::cout << "GetHospital row = " << row << endl;

    for (int i = 0; i < row; i++)
    {
        vector<string> vecInfo;
        auto result_row = mysql_fetch_row(res_ptr);
        auto sTemp = (utf8_to_ansi((char*)result_row[0]));
        if (sTemp.empty())
        {    
            continue;
        }
        if (i != row - 1)
        {
            sHospital += sTemp + ",";
        }
        else
        {
            sHospital += sTemp;
        }
    }
    mysql_free_result(res_ptr);
    return ErrorCode::Success;
}

ErrorCode DBOperator::DeleteData(const string& sUID)
{
    std::lock_guard<std::mutex> lock(m_connectMutext);
    string sql = "delete from filemanager.data_table where id = '" + sUID + "'";
    auto utf8Sql = ansi_to_utf8(sql);

    auto res = mysql_query(&m_myConnect, utf8Sql.c_str());
    if (res)
    {
        std::cout << " 执行sql失败：" << sql << std::endl;
        return ErrorCode::Failed;
    }
    return ErrorCode::Success;
}
