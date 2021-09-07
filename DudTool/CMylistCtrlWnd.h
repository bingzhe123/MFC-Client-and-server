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
    //vecColumnRatio��ÿһ��ռ�Ŀ�ȱ���
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
    //�Զ��ŷָ�һ����Ϣ
    //ÿ�����ݵ�xλ����Header�е�λ�þ�����ֻҪ����ƫ��y����
    //
    void AddItem(const vector<CString>& vecItemContent, CMyListHeaderWnd* pHeadWnd, int y, int h);
    //ĳһ������һ�Ű�ť
    void AddColumnWnd(int iColumn, vector<CMyWindow*> vecButton);
    vector<CMyWindow*> GetColumnWnd(int iColunm);
    void RemoveColumnWnd(int iColum);

    void ModifyColumnText(int iColumn, const CString& sText);

private:
    int m_iMinWidth;//���ڵ���С��ȣ�����ַ������ڿ��Ҫ������ʾ
    COLORREF m_bkSelectColor;
    CMyContainerWnd* m_pHeaderWnd = nullptr;
    map<int, vector<CMyWindow*>> m_mapClolumnWnd;
    map<int, CMyLable*> m_mapItem; //key:columnIndex
};