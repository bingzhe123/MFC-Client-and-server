#pragma once
#include "CMyContainerWnd.h"
#include "CMylistCtrlWnd.h"
#include "CBottomNavigateWnd.h"
#include <map>
using std::map;

using EditDataFunc = std::function<void(CMyWindow*,vector<CString>&)>;
enum class DataColumn
{
    ID,
    name,
    age,
    gender,
    type,
    registerTime,
    hospital,
    hasAttachment,
    description,
    operation
};

struct ContentItem
{
    CMyWindow* pWnd;
    vector<CString> vecItems;
};

class CDataShowrListWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual void OnSize(int cx, int cy);
    void Initialize();
    CMyLable* CreateLable(int x, int y, int w, int h, const CString& sContent);

    void AddItem(vector<CString>& vecContent, int index);
    void AddTopItem(vector<CString>& vecContent);

    void ChangeItem(CMyListItemWnd *pItem, vector<CString>& vecContent);
    void ClearPageItems();
    void SetBottomNavigatewnd(CBottomNavigateWnd* pWnd);
    //�ļ��鿴��ť
    void OnFileInfo(CMyWindow* pWnd);
    void OnEditInfo(CMyWindow* pWnd);
    vector<CString> GetContent(CMyWindow* pWnd);
        //���¸���ͼ��
    void SetAttachmentMark(const CString &sFileOwerID, bool bHasMark);
    void RegisterEditFunc(EditDataFunc f);
    int GetIndex(CMyWindow* pWnd);
private:
    CMyListHeaderWnd* m_pHeadWnd = nullptr;
    int m_iItemCount = 0;
    int m_iCurrentPageCount = 0;//��ǰҳ���м���
    int m_iTopIndex = 0;//��һ�е�����        
    int m_iAttachmentIndex = 8;
    //sql = "select id, data_name,age, gender,data_type, register_date,hospital,\
    //    description,data_id_list, oweruser_id from filemanager.data_info";
    //��ǰҳ����ʾ������:key ��Ӧ������itemWindow
    //map<CMyWindow*, vector<CString>> m_mapContent;
    vector<ContentItem> m_vecContent;

    CBottomNavigateWnd* m_pBottom = nullptr;
    EditDataFunc m_pFuncEditData = nullptr;
};

