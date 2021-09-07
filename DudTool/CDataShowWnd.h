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

    //ÿһ���Զ��ŷָ�
    void SetCurrentPageItems(const string& sContents,int iStart, int iTotalCount);
    void SetTotalCount(int iTotalCount);
    void SetHospital(const string& sHospital);
    void SetPatientType(const string& sPatientType);

    //��������ҳ�浼����ť��iStart���µ�ҳ����ʾ�Ŀ�ʼ������iEnd:�µ�ҳ����ʾ�Ľ�������
    void OnPage(int iStart, int iCount);
    //����һ�����ݣ�vecItems�����������䣬�Ա����ͣ�ҽԺ������
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

