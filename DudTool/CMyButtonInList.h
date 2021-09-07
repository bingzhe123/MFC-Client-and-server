#pragma once
#include "CMyButton.h"
class CMyButtonInList :
    public CMyButton
{
public:
    using CMyButton::CMyButton;
    void SetThisButtonIndex(int iIndex);
    int GetThisButtonIndex();
private:
    int m_iIndex;
};

