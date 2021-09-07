#pragma once
#include "CMyContainerWnd.h"
#include "CMyCheckBox.h"
#include "CMyScrollbarWnd.h"
#include "CMyCheckBox.h"
#include <map>
#include <vector>
#include "CMyScrollbarWnd.h"
#include "CMyScrollWnd.h"
#include "CMyBoderWnd.h"

using std::map;
using std::vector;
//默认是展开的没有收缩功能

struct RoleInfo
{
    CString id;
    CString roleName;
    bool isSelected;
};
using ItemSelectFunc = std::function<void(vector<std::pair<CMyCheckBox*, RoleInfo>> vecRoleInfo)>;

class CMyMutiSelectItemTitle :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    virtual bool Refresh(CDC* pMemDC, CRect rect);
    void SetText(const CString& sText);
private:
    CString m_sText;
};
class CMyCheckedScrollWnd :
    public CMyScrollWnd
{
public:
    using  CMyScrollWnd::CMyScrollWnd;
    void Initialze(vector< RoleInfo>& vecRoleInfo);
    //virtual void OnSize(int cx, int cy);
    //virtual bool Refresh(CDC* pMemDC, CRect rect);
    //virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    ////fCurrentPosDelat:当前窗口所在的比例
    //void OnScrollWnd(float fCurrentPosDelat);
    void RegisterItemSelectFunc(ItemSelectFunc f);
    vector<RoleInfo> GetAllSelectRole();

private:
    vector<std::pair<CMyCheckBox*, RoleInfo>> m_vecRoleInfoItems;
    ItemSelectFunc m_pItemSelectFun = nullptr;

};

class CMyMutiSelectItemWnd :
    public CMyContainerWnd
{
public:
    using CMyContainerWnd::CMyContainerWnd;
    ~CMyMutiSelectItemWnd();
    void Initialize(vector< RoleInfo> &vecRoleInfo);
    virtual void OnSize(int cx, int cy);
    virtual bool OnMouseMove(UINT nFlags, CPoint point);
    virtual BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    void OnItemSelect(vector<std::pair<CMyCheckBox*, RoleInfo>> vecRoleInfo);
    vector<RoleInfo> GetAllSelectRole();
private:
    CMyMutiSelectItemTitle* m_pTitle = nullptr;
    CMyBoderWnd* m_pBoderWnd = nullptr;
    CMyCheckedScrollWnd* m_pScrollWnd = nullptr;

};

