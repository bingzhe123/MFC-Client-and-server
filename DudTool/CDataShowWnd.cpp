#include "pch.h"
#include "CDataShowWnd.h"
#include "CommonDef.h"
#include "HttpHelper.h"
#include "DudToolClient.h"
#include "CDataInfoRegisterDlg.h"
#include <locale> 
#include"afxdb.h"
CDataShowWnd::CDataShowWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
    :CMyContainerWnd(pParent, pRootWnd, fun)
{
}

CDataShowWnd::~CDataShowWnd()
{
}

void CDataShowWnd::SetVisible(bool bVisible)
{
    __super::SetVisible(bVisible);
    m_pBottom->ShowEdit(bVisible);
}

void CDataShowWnd::Initialize()
{
    int w = m_rect.Width();
    int iToolBarHeight = DispalyRegionToolBarHeight;
    m_pDataShowTileWnd = new CDataShowTileWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pDataShowTileWnd->SetDimention(w, iToolBarHeight);
    m_pDataShowTileWnd->Initialize();
    m_pDataShowTileWnd->RegisterAddNewItem([this](const vector<CString>& vecItems)->bool
    {
        return OnAddNewItems(vecItems);
    });
    m_pDataShowTileWnd->RegisterExportExcel([this]()
    {
        this->OnExportExcel();
    });
    m_pDataShowTileWnd->RegisterSearch([this](SearchCondition &sSearch)
    {
        this->OnSearch(sSearch);
    });
    m_vecChildWnd.push_back(m_pDataShowTileWnd);

    m_pDataShowListWnd = new CDataShowrListWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pDataShowListWnd->Initialize();
    m_pDataShowListWnd->SetMouseHoverSensitive(false);
    m_pDataShowListWnd->RegisterEditFunc([this](CMyWindow* pWnd, vector<CString> &vecContent)
    {
        OnEditInfo(pWnd, vecContent);
    });
    m_vecChildWnd.push_back(m_pDataShowListWnd);

    m_pBottom = new CBottomNavigateWnd(this, m_pRootWnd, m_refreshImpFun);
    m_pBottom->Initialize(1000);
    int iBottomHeight = 50;
    m_pBottom->SetDimention(w, iBottomHeight);
    m_pBottom->RegisterOnpageFunction([this](int iStart, int iCount)
    {
        this->OnPage(iStart, iCount);
    });
    m_vecChildWnd.push_back(m_pBottom);
    m_pDataShowTileWnd->SetBottom(m_pBottom);
}

void CDataShowWnd::OnSize(int cx, int cy)
{
    SetDimention(cx, cy);
    int w = m_rect.Width();
    int iToolBarHeight = DispalyRegionToolBarHeight;
    m_pDataShowTileWnd->SetDimention(w, iToolBarHeight);
    int h = 50;
    int iOffsetx = 0;
    int iOffsety = iToolBarHeight + 5;
    int iBottomHeight = 50;
    int iBottomOffset = 5;
    int iContentHeight = m_rect.Height() - iToolBarHeight - iBottomHeight - iBottomOffset*2;
    m_pDataShowListWnd->MoveWindow(iOffsetx, iOffsety);
    m_pDataShowListWnd->SetDimention(w, iContentHeight);
    m_pDataShowListWnd->OnSize(cx, cy);
    iOffsety += iContentHeight ;
    m_pBottom->SetDimention(w, iBottomHeight- iBottomOffset);
    m_pBottom->MoveWindow(iOffsetx, iOffsety + 5);
    m_pBottom->OnSize(cx, cy);
    m_pDataShowTileWnd->OnSize(cx, cy);
}

void CDataShowWnd::SetCurrentPageItems(const string& sContents, int iStart, int iTotalCount)
{
    m_pDataShowListWnd->ClearPageItems();
    auto vecInfos = SplitString(sContents, '|');
    int i = 0;
    auto iEnd = iTotalCount - iStart;
    for each (auto item in vecInfos)
    {
        CString sTemp = CString(item.c_str());
        auto vecItem = SplitString(sTemp, L',');
        int iIndex = iEnd--;
        m_pDataShowListWnd->AddItem(vecItem, iIndex);
    }
}

void CDataShowWnd::SetTotalCount(int iTotalCount)
{
    m_pBottom->SetTotalCount(iTotalCount);
}

void CDataShowWnd::SetHospital(const string& sHospital)
{  
    m_pDataShowTileWnd->SetHospital(sHospital);
}

void CDataShowWnd::SetPatientType(const string& sPatientType)
{
    m_pDataShowTileWnd->SetPatientType(sPatientType);
}

void CDataShowWnd::OnPage(int iStart, int iCount)
{
    HttpHelper httpHelper(theApp.m_spClient);
    int   iTotalCount = 0;
    auto searchCondition = m_pDataShowTileWnd->GetSearchConditon();
    auto sDataInfo = httpHelper.GetUserDataInfo(theApp.m_sUserID,
        searchCondition,
        iStart, PerPageItemDataCount,
        iTotalCount,
        m_eDataCategory);
    SetCurrentPageItems(sDataInfo, iStart, iTotalCount);
}
bool CDataShowWnd::OnAddNewItems(const vector<CString>& vecItems)
{
    HttpHelper httpHelper(theApp.m_spClient);
    vector<string> vecTemp;
    for each (auto  item in vecItems)
    {                         
        string sTemp = CW2A(item);
        vecTemp.push_back(sTemp);
    }
    vecTemp.push_back(std::to_string((int)m_eDataCategory));
    string sNewItem;
    auto iErrorCode = httpHelper.AddUserDataInfo(theApp.m_sUserID, vecTemp, sNewItem);
    if (ErrorCode::Failed == iErrorCode)
    {
        AfxMessageBox(L"操作失败！");
        return true;
    }
    else if (ErrorCode::Repeat == iErrorCode)
    {
        AfxMessageBox(L"字段名重复！");
        return true;
    }
    //vector<string> vecNewItems = SplitString(sNewItem, ',');
    //vector<CString> _vecNewItems = String2CString(vecNewItems);
    //m_pDataShowListWnd->AddTopItem(_vecNewItems);
    m_pBottom->OnForceFirst(nullptr);

    //插入新的一行数据
   /* if (m_pBottom != nullptr)
    {
        auto iTotal = m_pBottom->GetTotalCount();
        m_pBottom->SetTotalCount(iTotal + 1);
    }*/
    //获取数据的所有医院
    string sHospital;
    if (ErrorCode::Success == httpHelper.GetHospitalName(theApp.m_sUserID, sHospital, m_eDataCategory))
    {
        SetHospital(sHospital);
    }
    string sPatientType;
    return false;
}

void CDataShowWnd::OnExportExcel()
{                       
    //BOOL isOpen = FALSE;		//是否打开(否则为保存)
    //CString  sPath, strTemp;
    //GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
    //sPath.ReleaseBuffer();
    //int nPos = sPath.ReverseFind('\\');
    //strTemp = sPath.Left(nPos);
    //CString defaultDir = strTemp;	//默认打开的文件路径
    //CString fileName = L"data.txt";			//默认打开的文件名
    //CString filter = L"文件 ( *.*; |*.*;||)";	//文件过虑的类型
    //CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
    //INT_PTR result = openFileDlg.DoModal();
    //if (result != IDOK)
    //{
    //    return;
    //}
    //auto sFilePath = openFileDlg.GetPathName();
    //auto sFileName = sFilePath.Right(sFilePath.GetLength() - sFilePath.ReverseFind(L'\\') - 1);
    //setlocale(LC_CTYPE, ("chs"));//设置支持中文
    //auto spFile = std::make_shared< CStdioFile>();
    //bool bRes = spFile->Open(sFilePath, CStdioFile::modeWrite | CFile::modeCreate | CStdioFile::typeText);
    //if (!bRes)
    //{
    //    AfxMessageBox(L"创建文件失败！");
    //    return;
    //}
    CFileDialog fg(false, _T("*.csv"), _T("exprot.csv"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        _T("Excel CSV File(*.csv)"), NULL);
    if (fg.DoModal() == IDOK)
    {
        TCHAR* old_locale = _tcsdup(_tsetlocale(LC_CTYPE, NULL));
        _tsetlocale(LC_CTYPE, _T("chs"));

        CString filename = fg.GetPathName();
        try
        {
            CStdioFile file(filename, CFile::modeCreate | CFile::modeReadWrite | CFile::shareCompat);
            CString str = _T("编号,姓名,年龄,性别,类型,注册时间,医院,需求描述\n");
            file.WriteString(str);

            HttpHelper httpHelper(theApp.m_spClient);
            string sAllData;
            auto searchCondition = m_pDataShowTileWnd->GetSearchConditon();
            httpHelper.GetAllDataInfo(theApp.m_sUserID, sAllData, searchCondition, m_eDataCategory);
            CString sTemp = CString(sAllData.c_str());
            auto vecInfos = SplitString(sTemp, '|');
            int count = vecInfos.size();
            for each (auto item in vecInfos)
            {
                auto vecItem = SplitString(item, L',');
                CString sContent;
                for (int i = 0; i < 8; i++)
                {
                    if (i==0)
                    {
                        sContent += CString(std::to_wstring(count--).c_str());
                    }
                    else
                    {
                        sContent += vecItem[i];
                    }
                    sContent += _T(",");
                }
                sContent += _T("\n");
                file.WriteString(sContent);
            }
            file.Close();

            _tsetlocale(LC_CTYPE, old_locale);
            free(old_locale);

        }
        catch (...)
        {
            AfxMessageBox(L"创建文件失败！");
        }
        
    }
                               



    //HttpHelper httpHelper(theApp.m_spClient);
    //string sAllData;
    //auto searchCondition = m_pDataShowTileWnd->GetSearchConditon();
    //httpHelper.GetAllDataInfo(theApp.m_sUserID, sAllData, searchCondition,m_eDataCategory);
    //CString sTemp = CString(sAllData.c_str());
    //auto vecInfos = SplitString(sTemp, '|');
    //for each (auto item in vecInfos)
    //{
    //    auto vecItem = SplitString(item, L',');
    //    CString sContent;
    //    for (int i = 0; i < 8; i++)
    //    {
    //        sContent += vecItem[i];
    //        sContent += L"\t";
    //    }
    //    sContent += L"\n";
    //    spFile->WriteString(sContent.GetString());
    //}
    //spFile->Close();
}

void CDataShowWnd::OnSearch(SearchCondition& sSearch)
{
    HttpHelper httpHelper(theApp.m_spClient);

    int   iTotalCount = 0;
    auto s = GetSearchCondition();
    auto sDataInfo = httpHelper.GetUserDataInfo(theApp.m_sUserID,
        s,
        0, PerPageItemDataCount,
        iTotalCount,
        m_eDataCategory);
    SetTotalCount(iTotalCount);
    int iEndCount = iTotalCount > PerPageItemDataCount ? iTotalCount : PerPageItemDataCount;

    SetCurrentPageItems(sDataInfo, 0, iTotalCount);
}

void CDataShowWnd::OnEditInfo(CMyWindow* pWnd, vector<CString> &vecContent)
{
    auto pItemWnd = (CMyListItemWnd*)pWnd->GetParent();
    CDataInfoRegisterDlg dlg(vecContent);
    auto res = dlg.DoModal();
    if (res == IDCANCEL)
    {
        return;
    }
    auto vecNewItmes = dlg.GetItems();
    HttpHelper httpHelper(theApp.m_spClient);

    vector<string> vecTemp = CString2String(vecNewItmes);
    vecTemp.push_back(std::to_string((int)m_eDataCategory));

    string sNewItem;
    string sDataId = CW2A(vecContent[0].GetString());
    auto sErrorCode = httpHelper.ModifyUserDataInfo(theApp.m_sUserID, sDataId, vecTemp, sNewItem);
    if (ErrorCode::Failed == sErrorCode)
    {
        AfxMessageBox(L"操作失败！");
        return;
    }
    else if (ErrorCode::Repeat == sErrorCode)
    {
        AfxMessageBox(L"字段名重复！");
        return;
    }

    vector<string> vecNewItems = SplitString(sNewItem, ',');
    vector<CString> _vecNewItems = String2CString(vecNewItems);
    m_pDataShowListWnd->ChangeItem(pItemWnd, _vecNewItems);

    //获取数据的所有医院
    string sHospital;
    if (ErrorCode::Success == httpHelper.GetHospitalName(theApp.m_sUserID, sHospital, m_eDataCategory))
    {
        SetHospital(sHospital);
    }
    string sPatientType;
    if (ErrorCode::Success == httpHelper.GetPatientType(sPatientType))
    {
        SetPatientType(sPatientType);
    }
}

SearchCondition CDataShowWnd::GetSearchCondition()
{
    return m_pDataShowTileWnd->GetSearchConditon();
}

void CDataShowWnd::SetDataCategory(DataCategory eDataCategory)
{
    m_eDataCategory = eDataCategory;
    m_pDataShowTileWnd->ClearSearchCondition();
}
