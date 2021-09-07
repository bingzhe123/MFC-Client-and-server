#ifndef  DOWN_LOAD_FILE_H_H
#define  DOWN_LOAD_FILE_H_H

#include <afx.h>
#include "httplib/httplib.h"
#include <string>
#include <share.h>
#include <vector>
using std::string;
using std::vector;
using namespace httplib;
class DownLoadFile
{
public:
    DownLoadFile(std::shared_ptr<Client> spClient,
        std::shared_ptr<CStdioFile> spFile, const CString &sProgressID);
    DownLoadFile(const string& hostName, int iPort);
    void DownLoadURL(const string& sUrl);
    bool DownLoad(const string& sUID);
private:
    bool ContentReciver2Buffer(const char* data, size_t data_length);
    bool ContentReciver2File(const char* data, size_t data_length);

    bool Process_buffer(uint64_t current, uint64_t total);
    bool Process_file(uint64_t current, uint64_t total);

private:
    string m_sHostName = "localhost";
    int m_iPort = 8080;
    string m_sFileName;
    std::shared_ptr<Client>          m_spClient;
    char m_buffer[CPPHTTPLIB_RECV_BUFSIZ];

    std::unique_ptr<char []>  m_spData = nullptr;
    size_t      m_iCurrent = 0;
    size_t      m_itotal = 0;
    Progress  m_progress;
    CString m_sProgressID;
    std::shared_ptr<CStdioFile> m_spStdioFile;

};

#endif // ! DOWN_LOAD_FILE_H_H
