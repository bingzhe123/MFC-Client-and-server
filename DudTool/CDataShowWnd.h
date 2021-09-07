#pragma once
#include "CMyContainerWnd.h"
#include "CDataShowTileWnd.h"
#include "CBottomNavigateWnd.h"
#include "CDataShowListWnd.h"
#include <string>
#include "CommonDef.h"
using std::string;
class CDataShowWnd :
    public CMyContainerWnd
{
public:
    CDataShowWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);
    virtual ~CDataShowWnd();
    virtual void SetVisible(bool bVisible);
    void Initialize();
    virtual void OnSize(int cx, int cy);

    //每一行以逗号分隔
    void SetCurrentPageItems(const string& sContents,int iStart, int iTotalCount);
    void SetTotalCount(int iTotalCount);
    void SetHospital(const string& sHospital);
    void SetPatientType(const string& sPatientType);

    //点击下面的页面导航按钮，iStart：新的页面显示的开始索引，iEnd:新的页面显示的结束索引
    void OnPage(int iStart, int iCount);
    //新增一条数据：vecItems：姓名，年龄，性别，类型，医院，需求
    bool OnAddNewItems(const vector<CString>& vecItems);
    void OnExportExcel();
    void OnSearch(SearchCondition &sSearch);
    void OnEditInfo(CMyWindow* pWnd, vector<CString> &vecContent);
    SearchCondition GetSearchCondition();
    void SetDataCategory(DataCategory eDataCategory);
private:
    CDataShowTileWnd* m_pDataShowTileWnd = nullptr;
    CBottomNavigateWnd* m_pBottom = nullptr;
    CDataShowrListWnd* m_pDataShowListWnd = nullptr;   
    DataCategory m_eDataCategory;
};

