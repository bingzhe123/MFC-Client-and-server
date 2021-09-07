// CDataInfoRegisterDlg.cpp : implementation file
//

#include "pch.h"
#include "DudToolClient.h"
#include "CDataInfoRegisterDlg.h"
#include "afxdialogex.h"
#include "CommonDef.h"
#include "CMyLable.h"
#include <vector>
#include <algorithm>
#include "CMyButton.h"
#include "HttpHelper.h"
#include "CommonDef.h"
using std::vector;
int LEFT_OFFSET = 20;
int TOP_OFFSET = 50;
// CDataInfoRegisterDlg dialog

IMPLEMENT_DYNAMIC(CDataInfoRegisterDlg, CDialogEx)

CDataInfoRegisterDlg::CDataInfoRegisterDlg(vector<CString>& vecItems, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REGISTER_DATA_INFO, pParent)
{

    m_vecItems = vecItems;
}

CDataInfoRegisterDlg::~CDataInfoRegisterDlg()
{
    m_font.DeleteObject();
    m_editName.DestroyWindow();
    m_editAge.DestroyWindow();
    m_editHospital.DestroyWindow();
    m_editDescription.DestroyWindow();
    for each (auto item in m_mapWndList)
    {
        delete item.second;
    }

}

void CDataInfoRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataInfoRegisterDlg, CDialogEx)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CDataInfoRegisterDlg message handlers

BOOL CDataInfoRegisterDlg::OnInitDialog()
{
    m_font.CreateFont(30,                                    //   字体的高度   
        0,                                          //   字体的宽度  
        0,                                          //  nEscapement 
        0,                                          //  nOrientation   
        FW_NORMAL,                                  //   nWeight   
        FALSE,                                      //   bItalic   
        FALSE,                                      //   bUnderline   
        0,                                                   //   cStrikeOut   
        ANSI_CHARSET,                             //   nCharSet   
        OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
        CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
        DEFAULT_QUALITY,                       //   nQuality   
        DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
        _T("微软雅黑"));
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);

    int iHeight = 600;
    int iWidth = 650;
    int x = (cx - iWidth) / 2;
    int y = (cy - iHeight) / 2;
    MoveWindow(x, y, iWidth, iHeight);

   
    int iEdithHeight = 30;
    int w = 150;
    int iWndIndex = 0;
    x = LEFT_OFFSET;
    y = TOP_OFFSET;
    auto refreshFunc = std::bind(&CDataInfoRegisterDlg::RefreshFun, this, std::placeholders::_1, std::placeholders::_2);
    auto* pWnd = new CMyLable(nullptr, this, refreshFunc);
    pWnd->SetText(L"病人姓名：", true);
    pWnd->SetAlignMode(Align::right);
    pWnd->MoveWindow(x, y);
    pWnd->SetDimention(w, iEdithHeight);
    m_mapWndList[iWndIndex++] = pWnd; 

    int iEditWidth = 400;
    x += w;
    CRect r(x, y, x + iEditWidth, y + iEdithHeight);
    m_editName.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, r, this, UWM_IDD_EDIT_UPLOAD_NAME);
    m_editName.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editName.SetWindowTextW(L"");
    m_editName.SetTextColor(RGB(255, 255, 255));
    m_editName.SetFont(&m_font);
    m_editName.SetBackColor(RGB(64, 64, 64));
    m_editName.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editName);
    //::PostMessage(m_editName.m_hWnd, EM_SETSEL, 0, 0);

    int iSplitter = 30;

    x = LEFT_OFFSET;
    y += iSplitter + iEdithHeight;
    pWnd = new CMyLable(nullptr, this, refreshFunc);
    pWnd->SetText(L"病人年龄：", true);
    pWnd->SetAlignMode(Align::right);
    pWnd->MoveWindow(x, y);
    pWnd->SetDimention(w, iEdithHeight);
    m_mapWndList[iWndIndex++] = pWnd;
    
    x += w;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editAge.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_NUMBER, r, this, UWM_IDD_EDIT_UPLOAD_AGE);
    m_editAge.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editAge.SetWindowTextW(L"");
    m_editAge.SetTextColor(RGB(255, 255, 255));
    m_editAge.SetFont(&m_font);
    m_editAge.SetBackColor(RGB(64, 64, 64));
    m_editAge.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editAge);

    x = LEFT_OFFSET;
    y += iSplitter + iEdithHeight;
    pWnd = new CMyLable(nullptr, this, refreshFunc);
    pWnd->SetText(L"医院名称：", true);
    pWnd->SetAlignMode(Align::right);
    pWnd->MoveWindow(x, y);
    pWnd->SetDimention(w, iEdithHeight);
    m_mapWndList[iWndIndex++] = pWnd;

    x += w;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editHospital.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, r, this, UWM_IDD_EDIT_UPLOAD_HOSPITAL);
    m_editHospital.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editHospital.SetWindowTextW(L"");
    m_editHospital.SetTextColor(RGB(255, 255, 255));
    m_editHospital.SetFont(&m_font);
    m_editHospital.SetBackColor(RGB(64, 64, 64));
    m_editHospital.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editHospital);

    x = LEFT_OFFSET;
    y += iSplitter + iEdithHeight;
    pWnd = new CMyLable(nullptr, this, refreshFunc);
    pWnd->SetText(L"需求描述：");
    pWnd->SetAlignMode(Align::right);
    pWnd->MoveWindow(x, y);
    pWnd->SetDimention(w, iEdithHeight);
    m_mapWndList[iWndIndex++] = pWnd;

    x += w;
    r.SetRect(x, y, x + iEditWidth, y + iEdithHeight);
    m_editDescription.Create(WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, r, this, UWM_IDD_EDIT_UPLOAD_DESCRIPTION);
    m_editDescription.ModifyStyleEx(WS_TABSTOP, 0, 0);
    m_editDescription.SetWindowTextW(L"");
    m_editDescription.SetTextColor(RGB(255, 255, 255));
    m_editDescription.SetFont(&m_font);
    m_editDescription.SetBackColor(RGB(64, 64, 64));
    m_editDescription.ShowWindow(SW_SHOW);
    m_vecEdit.push_back(&m_editDescription);

    x = LEFT_OFFSET;
    y += iSplitter + iEdithHeight;
    pWnd = new CMyLable(nullptr, this, refreshFunc);
    pWnd->SetText(L"病人性别：", true);
    pWnd->SetAlignMode(Align::right);
    pWnd->MoveWindow(x, y);
    pWnd->SetDimention(w, iEdithHeight);
    m_mapWndList[iWndIndex++] = pWnd;
    x += w;
    auto* pCombox = new CMyComboxWnd(nullptr, this, refreshFunc);
    vector<CString> vecItems;
    vecItems.push_back(L"男");
    vecItems.push_back(L"女");
    pCombox->AddItems(vecItems);
    pCombox->MoveWindow(CPoint(x, y));
    pCombox->SetDimention(iEditWidth, iEdithHeight);
    pCombox->SetLayer(2);
    pCombox->SetExpandHeight((iEdithHeight + 10) * vecItems.size() + TextSpace* vecItems.size());
    pCombox->SetScrollbarVisible(false);
    m_mapWndList[iWndIndex++] = pCombox;
    m_pGender = pCombox;


    x = LEFT_OFFSET;
    y += iSplitter + iEdithHeight;
    pWnd = new CMyLable(nullptr, this, refreshFunc);
    pWnd->SetText(L"病人类型：", true);
    pWnd->SetAlignMode(Align::right);
    pWnd->MoveWindow(x, y);
    pWnd->SetDimention(w, iEdithHeight);
    m_mapWndList[iWndIndex++] = pWnd;

    HttpHelper httpHelper(theApp.m_spClient);
    string sType;
    auto errorCode = httpHelper.GetPatientType(sType);
    CString sTemp = CString(sType.c_str());

    auto vecTemp = SplitString(sTemp, '|');
    vector<CString> vecType;
    for (size_t i = 0; i < vecTemp.size(); i++)
    {
        auto vectemp = SplitString(vecTemp[i], ',');
        vecType.push_back(vectemp[1]);
        m_TypeID.push_back(vectemp[0]);
    }

    x += w;
    pCombox = new CMyComboxWnd(nullptr, this, refreshFunc);
    pCombox->AddItems(vecType);
    pCombox->MoveWindow(CPoint(x, y));
    pCombox->SetDimention(iEditWidth, iEdithHeight);
    pCombox->SetLayer(1);
    pCombox->SetExpandHeight(iEdithHeight* 3 + TextSpace * vecItems.size());
    m_mapWndList[iWndIndex++] = pCombox;    
    m_pType = pCombox;

    int iButtonWidth = 200;
    int iButtonHeight = iEdithHeight;
    auto pBtnOK = new CMyButton(nullptr, this, refreshFunc);
    pBtnOK->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    pBtnOK->SetText(L"确定");

    int spacex = 120;
    x = (iWidth - iButtonWidth * 2 - spacex) / 2;
    y += 120;
    pBtnOK->MoveWindow(CPoint(x, y));
    pBtnOK->SetDimention(iButtonWidth, iButtonHeight);
    pBtnOK->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnClickOk(pWnd);
    });
    m_mapWndList[iWndIndex++] = pBtnOK;

    auto pBtnCancel = new CMyButton(nullptr, this, refreshFunc);
    pBtnCancel->SetBkGroundColor(RGB(32, 41, 53), RGB(255, 91, 178));
    pBtnCancel->SetDimention(iButtonWidth, iButtonHeight);
    pBtnCancel->SetText(L"取消");
    x += iButtonWidth + spacex;
    pBtnCancel->MoveWindow(CPoint(x, y));
    pBtnCancel->RegisterClickFun([this](CMyWindow* pWnd)
    {
        OnClickCancel(pWnd);
    });
    m_mapWndList[iWndIndex++] = pBtnCancel;


    if (!m_vecItems.empty())
    {
        m_editName.SetWindowTextW(m_vecItems[1]);
        m_editAge.SetWindowTextW(m_vecItems[2]);
        m_editHospital.SetWindowTextW(m_vecItems[6]);
        m_editDescription.SetWindowTextW(m_vecItems[7]);
        if (L"男" == m_vecItems[3])
        {
            m_pGender->SetCurrentSelectIndex(0);
        }
        else
        {
            m_pGender->SetCurrentSelectIndex(1);
        }

        auto vecTypes = m_pType->GetItems();
        int i = 0;
        for (auto item : vecTypes)
        {
            if (item == m_vecItems[4])
            {
                m_pType->SetCurrentSelectIndex(i);
            }
            i++;
        }
    }
    


    CRect rect;
    GetClientRect(rect);
    for each (auto item in m_mapWndList)
    {
        auto r = item.second->GetMyRect();
        item.second->OnSize(r.Width(), r.Height());
    }
    return 0;
}

void CDataInfoRegisterDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    for each (auto item in m_mapWndList)
    {
        auto pWnd = item.second;
        if (pWnd->OnMouseMove(nFlags, point))
        {
            if (m_pCurrentMouseInWnd != nullptr && pWnd != m_pCurrentMouseInWnd)
            {
                m_pCurrentMouseInWnd->OnMouseLeave();
            }
            pWnd->OnMouseIn(nFlags, point);
            m_pCurrentMouseInWnd = pWnd;
            break;
        }
    }
    if (m_pCurrentMouseInWnd != nullptr && !m_pCurrentMouseInWnd->HitTest(point))
    {
        m_pCurrentMouseInWnd->OnMouseLeave();
        m_pCurrentMouseInWnd = nullptr;
    }
    CDialogEx::OnMouseMove(nFlags, point);
}


void CDataInfoRegisterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (nullptr == m_pCurrentMouseInWnd)
    {
        m_pGender->OnOpen(false); 
        m_pType->OnOpen(false);
        return;
    }
    if (m_pCurrentMouseInWnd == m_pGender)
    {
        m_pType->OnOpen(false);
    }
    if (m_pCurrentMouseInWnd == m_pType)
    {
        m_pGender->OnOpen(false);
    }

    m_pCurrentMouseInWnd->OnLButtonDown(nFlags, point);
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CDataInfoRegisterDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (nullptr == m_pCurrentMouseInWnd)
    {
        return;
    }
    m_pCurrentMouseInWnd->OnLButtonUp(nFlags, point);
    CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CDataInfoRegisterDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default

    return TRUE;
}


void CDataInfoRegisterDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: Add your message handler code here
                       // Do not call CDialogEx::OnPaint() for painting messages
    CRect rect;
    GetClientRect(&rect);
    CDC* pDC = GetDC();
    CDC memDC;
    CBitmap memBitmap;
    memDC.CreateCompatibleDC(NULL);
    int nWidth = rect.Width();
    int nHeight = rect.Height();
    memBitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight);
    CBitmap* pOldbit = (CBitmap*)memDC.SelectObject(&memBitmap);
    memDC.FillSolidRect(rect, RGB(25, 31, 40));

    memDC.SetBkMode(TRANSPARENT);
    memDC.SelectObject(&m_font);
    memDC.SetTextColor(RGB(255, 255, 255));
 
    vector<CMyWindow*> vecTemp;

    for each (auto item in m_mapWndList)
    {
        auto pWnd = item.second;
        vecTemp.push_back(item.second);
        //auto childRect = pWnd->GetClientRect();
        //pWnd->Refresh(&memDC, rect);
    }
    std::sort(vecTemp.begin(), vecTemp.end(), [](CMyWindow* a, CMyWindow* b)
    {
        return a->GetLayer() < b->GetLayer();
    });
    for each (auto item in vecTemp)
    {
        item->Refresh(&memDC, item->GetClientRect());
    }

    RefreshWithEdit(this, pDC, rect, m_vecEdit, memDC);
    //pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldbit);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
    ReleaseDC(pDC);

}


void CDataInfoRegisterDlg::OnOK()
{
    m_vecItems.resize(6);
    int i = 0;
    m_editName.GetWindowTextW(m_vecItems[i++]);

    m_editAge.GetWindowTextW(m_vecItems[i++]);
    m_editHospital.GetWindowTextW(m_vecItems[i++]);
 
    m_vecItems[i++] = m_pGender->GetSelectItem();
    int iCurrentSelectIndex = 0;
    iCurrentSelectIndex = m_pType->GetSelectIndex();
    m_vecItems[i++] = m_TypeID[iCurrentSelectIndex];
    m_editDescription.GetWindowTextW(m_vecItems[i++]);
    if (m_vecItems[0].IsEmpty() || m_vecItems[1].IsEmpty() || m_vecItems[2].IsEmpty() || m_vecItems[4].IsEmpty())
    {
        AfxMessageBox(L"关键字段为空!");
        return;
    }
    __super::OnOK();
}

void CDataInfoRegisterDlg::RefreshFun(CMyWindow* pWnd, CRect rect)
{
    Invalidate();
}


void CDataInfoRegisterDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);
    if (m_mapWndList.size() != 0)
    {
        for each (auto item in m_mapWndList)
        {
            item.second->OnSize(cx, cy);
        }
    }

    // TODO: Add your message handler code here
}


void CDataInfoRegisterDlg::OnClickOk(CMyWindow* pWnd)
{
    this->OnOK();
}

void CDataInfoRegisterDlg::OnClickCancel(CMyWindow* pWnd)
{
    this->OnCancel();
}

vector<CString> CDataInfoRegisterDlg::GetItems()
{
    return m_vecItems;
}

void CDataInfoRegisterDlg::SetItems(vector<CString>& vecItems)
{
    m_vecItems = vecItems;
}


BOOL CDataInfoRegisterDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    if (m_pCurrentMouseInWnd != nullptr)
    {
        m_pCurrentMouseInWnd->OnMouseWheel(nFlags, zDelta, pt);
    }
    return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}
