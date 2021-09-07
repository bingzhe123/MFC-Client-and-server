#pragma once
#include "CMyWindow.h"
#include <vector>

//��Ϊcomboxչ����ʱ���細�ڣ�����Ҫ���⴦��
class CComboxWndManger
{
public:
    static CComboxWndManger& Instance();
    void AddWnd(CMyWindow* pWnd);
    vector<CMyWindow*> GetWnd();
private:
    vector<CMyWindow*> m_vecWnd;

};

//��Ϊcomboxչ����ʱ���細�ڣ�����Ҫ���⴦��
class CCrosWndManger :public CComboxWndManger
{
public:
    static CCrosWndManger& Instance();
};
