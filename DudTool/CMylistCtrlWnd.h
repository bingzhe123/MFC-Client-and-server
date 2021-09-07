#pragma once
#include "CMyContainerWnd.h"
#include "CMyLable.h"
#include "CMyButton.h"
#include <map>
using std::map;

class CMyListHeaderWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual ~CMyListHeaderWnd();
    virtual void OnSize(int cx, int cy);
    CMyLable* CreateLable(int x, int y, int w, int h, const CString& sContent);
    //vecColumnRatio：每一列占的宽度比例
    void Initialize(vector<CString> vecHeader, vector<float>& vecColumnRatio);

private:
    vector<float> m_vecRatio;
};

class CMyListItemWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual ~CMyListItemWnd();
    virtual bool Refresh(CDC* pMemDC , CRect rect);
    virtual void OnSize(int cx, int cy);

    CMyLable* CreateLable(int x, int y, int w, int h, const CString& sContent);
    void SetBkGroundColor(COLORREF rgbNormal, COLORREF rgbSelect);
    //以逗号分隔一条信息
    //每条数据的x位置由Header中的位置决定，只要传入偏移y即可
    //
    void AddItem(const vector<CString>& vecItemContent, CMyListHeaderWnd* pHeadWnd, int y, int h);
    //某一列增加一排按钮
    void AddColumnWnd(int iColumn, vector<CMyWindow*> vecButton);
    vector<CMyWindow*> GetColumnWnd(int iColunm);
    void RemoveColumnWnd(int iColum);

    void ModifyColumnText(int iColumn, const CString& sText);

private:
    int m_iMinWidth;//窗口的最小宽度，如果字符串大于宽度要换行显示
    COLORREF m_bkSelectColor;
    CMyContainerWnd* m_pHeaderWnd = nullptr;
    map<int, vector<CMyWindow*>> m_mapClolumnWnd;
    map<int, CMyLable*> m_mapItem; //key:columnIndex
};