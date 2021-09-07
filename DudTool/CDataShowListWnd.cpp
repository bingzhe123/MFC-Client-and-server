#include "pch.h"
#include "CDataShowListWnd.h"
#include "CMyButtonInlist.h"
#include "CMyStaticWindow.h"
#include "resource.h"
#include "CommonDef.h"
#include "CFileOperatorDlg.h"
#include "DudToolClient.h"
#include "CommonDef.h"

void CDataShowrListWnd::Initialize()
{
    auto rect = GetClientRect();
    m_pHeadWnd = new CMyListHeaderWnd(this, m_pRootWnd, m_refreshImpFun);
    int w = rect.Width();
    int iHeadHeight = 50;
    vector<CString> vecHeader;
    vecHeader.push_back(L"编号");
    vecHeader.push_back(L"姓名");
    vecHeader.push_back(L"年龄");
    vecHeader.push_back(L"性别");
    vecHeader.push_back(L"类型");
    vecHeader.push_back(L"注册时间");
    vecHeader.push_back(L"医院");
    vecHeader.push_back(L"需求描述");
    vecHeader.push_back(L"附件");
    vecHeader.push_back(L"操作");
    float lengTextDelat = 0.15;
    float nameTextDelat = 0.1;
    float last = 0.11;
    float fRatio = (1 - lengTextDelat * 2 - nameTextDelat - last) / (vecHeader.size() - 4);
    //需求描述和医院占比0.2,姓名0.15， 其他平分宽度
    /* float lengTextDelat = 0.25;
     float nameTextDelat = 0.1; */
    vector<float> vecRatio;
    for (size_t i = 0; i < vecHeader.size(); i++)
    {
        vecRatio.push_back(fRatio);
    }
    vecRatio[1] = nameTextDelat;
    vecRatio[6] = lengTextDelat;
    vecRatio[7] = lengTextDelat;
    vecRatio[9] = last;


    m_pHeadWnd->Initialize(vecHeader, vecRatio);
    m_vecChildWnd.push_back(m_pHeadWnd);
}


void CDataShowrListWnd::OnSize(int cx, int cy)
{
    auto rect = GetClientRect();
    int w = rect.Width();
    int iHeadHeight = 50;
    m_pHeadWnd->MoveWindow(0, 0);
    m_pHeadWnd->SetDimention(w, iHeadHeight);
    m_pHeadWnd->OnSize(cx, cy);

    int iSplitHeight = 2;//每行间距
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (PerPageItemDataCount - 1)) / PerPageItemDataCount;

    int iOffsety = iHeadHeight + iSplitHeight;
    for (int i = 1; i < m_vecChildWnd.size(); i++)
    {
        auto* pItemWnd = m_vecChildWnd[i];
        pItemWnd->MoveWindow(0, iOffsety);
        pItemWnd->SetDimention(w, iItemHeight);
        pItemWnd->OnSize(cx, cy);
        iOffsety += iItemHeight + iSplitHeight;
    }
}

CMyLable* CDataShowrListWnd::CreateLable(int x, int y, int w, int h, const CString& sContent)
{
    auto pLabel = new CMyLable(this, m_pRootWnd, m_refreshImpFun);
    pLabel->SetText(sContent);
    pLabel->MoveWindow(x, y);
    pLabel->SetDimention(w, h);
    return pLabel;
}

void CDataShowrListWnd::AddItem(vector<CString>& vecContent,int index)
{

    if (m_vecContent.size() >= PerPageItemDataCount)
    {//如果当前这一页已经满了，就不用加了
        return;
    }
    //增加一条信息
    int iHeadHeight = 50;
    auto rect = GetClientRect();
    int iSplitHeight = 2;//每行间距
    int iItemHeight = (rect.Height() - iHeadHeight - iSplitHeight * (PerPageItemDataCount - 1)) / PerPageItemDataCount;
    int iOffsety = iHeadHeight + iSplitHeight + (iItemHeight + iSplitHeight) * m_iCurrentPageCount;
    CMyListItemWnd* pItemWnd = new CMyListItemWnd(this, m_pRootWnd, m_refreshImpFun);

    vector<CString> vecDisplayInfo;
    vecDisplayInfo.assign(vecContent.begin(), vecContent.end()-2);
    CString sTemp;
    sTemp.Format(L"%d", index);
    vecDisplayInfo[0] = sTemp;
    pItemWnd->AddItem(vecDisplayInfo, m_pHeadWnd, iOffsety, iItemHeight);


    if (!vecContent[m_iAttachmentIndex].IsEmpty())
    {
        vector<CMyWindow*> vecAttachment;
        auto pAttachment = new CMyStaticWindow(pItemWnd, m_pRootWnd, m_refreshImpFun);
        pAttachment->SetBkPic(IDB_PNG_ATTACHMENT);
        vecAttachment.push_back(pAttachment);
        pItemWnd->AddColumnWnd(m_iAttachmentIndex, vecAttachment);
    }

    auto iIndex = m_vecContent.size() - 1;
    vector<CMyWindow*> vecButton;
    auto pBtnFile = new CMyButtonInList(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnFile->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnFile->SetText(CString(L"文件查看"));
    pBtnFile->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnFileInfo(pWnd);
    });
    pBtnFile->SetFontHeight(DISPLAY_LIST_FONT_SIZE);
    pBtnFile->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    vecButton.push_back(pBtnFile);

    auto pBtnEdit = new CMyButtonInList(pItemWnd, m_pRootWnd, m_refreshImpFun);
    pBtnEdit->SetBkGroundColor(DataShowBkColor, RGB(255, 91, 178));
    pBtnEdit->SetText(CString(L"编辑"));
    pBtnEdit->RegisterClickFun([this](CMyWindow* pWnd)
    {
        this->OnEditInfo(pWnd);
    });
    pBtnEdit->SetFontHeight(DISPLAY_LIST_FONT_SIZE);
    pBtnEdit->SetTextColor(RGB(170, 160, 175), RGB(220, 255, 255));
    auto sCurrentUserID = CString(theApp.m_sUserID.c_str());
    if (sCurrentUserID != vecContent[9] && !theApp.m_bIsManager)
    {
        pBtnEdit->SetMouseHoverSensitive(false);
    }
    vecButton.push_back(pBtnEdit);


    pItemWnd->AddColumnWnd(9, vecButton);
    
    m_vecChildWnd.push_back(pItemWnd);
    if (m_iCurrentPageCount % 2 == 0)
    {
        pItemWnd->SetBkPic(IDB_PNG_DATA_ITEM_LAN_BK);
    }
    else
    {
        pItemWnd->SetBkPic(IDB_PNG_DATA_ITEM_HUANG_BK);
    }
    pItemWnd->UpdateMyWnd(pItemWnd->GetClientRect());
   // m_mapContent[pItemWnd] = vecContent;
    m_vecContent.push_back(ContentItem{ pItemWnd , vecContent });
   // m_vecChildWnd.push_back(pItemWnd);
    m_iCurrentPageCount++;
    m_iTopIndex = m_iTopIndex < index ? index : m_iTopIndex;
}

void CDataShowrListWnd::AddTopItem(vector<CString>& vecContent)
{
    
    //if (m_vecContent.size() >= PerPageItemDataCount)
    //{//如果当前这一页已经满了，删除最后一个
    //    auto back = m_vecContent.back();
    //    delete back.pWnd;
    //    m_vecContent.pop_back();
    //}
    int iNewIndex = m_iTopIndex + 1;
    vector<vector<CString>> vecItems;
    vecItems.insert(vecItems.begin(), vecContent);//插入到第一条
    for each (auto item in m_vecContent)
    {
        vecItems.push_back(item.vecItems);
    }
    ClearPageItems();
    for each (auto item in vecItems)
    {
        AddItem(item, iNewIndex--);
    }

}

void CDataShowrListWnd::ChangeItem(CMyListItemWnd* pItem, vector<CString>& vecContent)
{
    vector<CString> vecDisplayInfo;
    vecDisplayInfo.assign(vecContent.begin(), vecContent.end() - 2);
    int i = 0;
    for (int i = 1; i < vecDisplayInfo.size(); i++)
    {
        pItem->ModifyColumnText(i, vecDisplayInfo[i]);

    }
    //m_mapContent[pItem] = vecContent;
    m_vecContent[GetIndex(pItem)] = ContentItem{ pItem , vecContent };
}

void CDataShowrListWnd::ClearPageItems()
{
    m_iCurrentPageCount = 0;
    m_iTopIndex = 0;
    for each (auto item in m_vecChildWnd)
    {
        if (item != m_pHeadWnd)
        {
            delete item;
        }
        if (item == m_pCurrentMouseInWnd)
        {
            m_pCurrentMouseInWnd = nullptr;
        }
    }
    m_vecChildWnd.clear();
    m_vecChildWnd.push_back(m_pHeadWnd);
    m_vecContent.clear();
    UpdateMyWnd(GetClientRect());
}

void CDataShowrListWnd::SetBottomNavigatewnd(CBottomNavigateWnd* pWnd)
{
    m_pBottom = pWnd;
}

void CDataShowrListWnd::OnFileInfo(CMyWindow* pWnd)
{
    auto pListButton = (CMyButtonInList*)pWnd;
    auto pItemWnd = (CMyListItemWnd*)pWnd->GetParent();
    //auto& vecItem = m_mapContent[pItemWnd];
    auto& vecItem = GetContent(pItemWnd);
    auto &sDataList = vecItem[9];
    auto vecDataId = SplitString(sDataList, ',');
    auto sId = vecItem[0];

    HttpHelper httpHelper(theApp.m_spClient);
    string sOwerID = CW2A(sId.GetString());
    string sOwerDataList = httpHelper.GetDataList(sOwerID);
    
    CFileOperatorDlg dlg(this);
    dlg.SetDataRegisterInfo(vecItem);
    dlg.SetDataList(CString(sOwerDataList.c_str()));
    dlg.DoModal();
}
void CDataShowrListWnd::OnEditInfo(CMyWindow* pWnd)
{
    if (m_pFuncEditData != nullptr)
    {
        auto pItemWnd = (CMyListItemWnd*)pWnd->GetParent();
        //auto& vecItem = m_mapContent[pItemWnd];
        auto& vecItem = GetContent(pItemWnd);
        m_pFuncEditData(pWnd, vecItem);
    }
}

vector<CString> CDataShowrListWnd::GetContent(CMyWindow* pWnd)
{
    for each (auto item in m_vecContent)
    {
        if (item.pWnd == pWnd)
        {
            return item.vecItems;
        }
    }
    return vector<CString>();
}

void CDataShowrListWnd::SetAttachmentMark(const CString& sFileOwerID, 
    bool bHasMark)
{
     if (!m_bVisible)
    {
        return;
    }
    for each (auto item in m_vecContent)
    {
       // if (item.second[0] == sFileOwerID)
        if (item.vecItems[0] == sFileOwerID)
        {
           // auto pItemWnd = (CMyListItemWnd*)item.first;
            auto pItemWnd = (CMyListItemWnd*)item.pWnd;
            auto vecColumnWnd = pItemWnd->GetColumnWnd(m_iAttachmentIndex);
            if (vecColumnWnd.size() == 0)
            {
                vector<CMyWindow*> vecAttachment;
                //auto pAttachment = new CMyStaticWindow(item.first, m_pRootWnd, m_refreshImpFun);
                auto pAttachment = new CMyStaticWindow(pItemWnd, m_pRootWnd, m_refreshImpFun);
                pAttachment->SetBkPic(IDB_PNG_ATTACHMENT);
                vecAttachment.push_back(pAttachment);
                pItemWnd->AddColumnWnd(m_iAttachmentIndex, vecAttachment);
            }
            else
            {
                vecColumnWnd[0]->SetVisible(bHasMark);
            }
            break;
        }
    }
}

void CDataShowrListWnd::RegisterEditFunc(EditDataFunc f)
{
    m_pFuncEditData = f;
}

int CDataShowrListWnd::GetIndex(CMyWindow* pWnd)
{
    int iIndex = -1;
    for each (auto item in m_vecContent)
    {
        iIndex++;
        if (item.pWnd == pWnd)
        {
            break;
        }
        
    }
    return iIndex;
}

