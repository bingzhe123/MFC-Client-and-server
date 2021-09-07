#ifndef UPLOAD_FILE_H_
#define UPLOAD_FILE_H_
#include "CommonDef.h"
#include "httplib/httplib.h"
#include <string>
#include <share.h>
#include <vector>
#include<fstream>

using namespace std;
using std::string;
using std::vector;
using namespace httplib;
//using UploadProgress = std::function<bool(uint64_t current, uint64_t total)>;



struct BlockInfo
{
    httplib::Headers m_header;
    int64_t m_iDataSize;
    int64_t m_iDataOffset;
    const char* m_pData;
    std::shared_ptr<Client> m_spClient;
};

class UploadFile 
{
public:
    UploadFile() = delete;
  //explicit UploadFile(string sHostName, int iPort);
  explicit UploadFile(std::shared_ptr<Client> spClient, const CString& sProgressID,
      const CString &sFilePath);

public:           
    bool SendData(FILE*pFile, UploadFileInfo &info);

private: 
    bool SplitTask(FILE* pFile, int64_t iLength);
    bool TaskFunc(FILE* pFile, const BlockInfo &, int64_t iLenght);
   
    BlockInfo InitializeBlockInfo(std::shared_ptr<Client> spClient,
        int64_t iDataOffset,
        int64_t iCurrentDataSize,
        int64_t iTotalLenght,
        const string& sGUID);
private:
  string m_sHostName = "localhost";
  int m_iPort = 8080; 
  std::shared_ptr<Client>          m_spClient;
  UploadFileInfo m_uploadFileInfo;
  CString m_sProgressID;
  CString m_sFilePath;
};
#endif // !UPLOAD_FILE_H_
