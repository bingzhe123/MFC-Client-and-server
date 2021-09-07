#pragma once
#include "CMyContainerWnd.h"
#include <vector>
#include "CommonDef.h"
#include "CMyScrollWnd.h"
#include <memory>
using std::vector;
class CMyTextDisplayWnd :
    public CMyScrollWnd
{
public:
    CMyTextDisplayWnd(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun);

    void SetTextItems(const vector<CString>& vecContent);
    //virtual bool Refresh(CDC* pMemDC, CRect rect);
    //virtual void OnSize(int cx, int cy);
    void OnClickTextItem(CMyWindow* pWnd);
    void RegisterClickItemFun(ClickFunc f);
protected:
    int m_iItemHeight = 30;
    int m_iOffsety = TextSpace; //每一行之间的间隔
    ClickFunc m_pFunClickTextItem = nullptr;
 
};

using CMyTextDisplayWndPtr = std::shared_ptr<CMyTextDisplayWnd>;
