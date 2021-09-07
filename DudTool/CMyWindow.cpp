#include "pch.h"
#include "CMyWindow.h"
#include "DudToolClientDlg.h"
#include <iostream>
CMyWindow::CMyWindow(CMyWindow* pParent, CWnd* pRootWnd, RefreshImplFunc fun)
{
     m_pParent = pParent;
     m_pBkImage = nullptr;
     m_pBkSelectedImage = nullptr;
     m_pRootWnd = pRootWnd;
     m_textHightLigtColor = RGB(255, 255, 255);
     m_textNormalColor = RGB(128, 128, 128);
     m_refreshImpFun = fun;
     int iFontSize = 30;
     auto pParentDlg = dynamic_cast<DudToolClientDlg*>(m_pRootWnd);
     if (pParentDlg != nullptr && pParentDlg->m_bSmallFont)
     {
         iFontSize = 23;
     }
     m_font.CreateFont(iFontSize,                    //   字体的高度   
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

     if (m_EditBoxfont.m_hObject == nullptr)
     {
         m_EditBoxfont.CreateFont(20,                    //   字体的高度   
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
     }
     
}

 CMyWindow::~CMyWindow()
{
     if (m_pBkImage != nullptr)
     {
         m_pBkImage->Destroy();
         delete m_pBkImage;
         m_pBkImage = nullptr;
     }

     if (m_pBkSelectedImage != nullptr)
     {
         m_pBkSelectedImage->Destroy();
         delete m_pBkSelectedImage;
         m_pBkSelectedImage = nullptr;
     }
     m_font.DeleteObject();
     m_EditBoxfont.DeleteObject();
     if (m_pRootWnd != nullptr)
     {
         auto pParentDlg = dynamic_cast<DudToolClientDlg*>(m_pRootWnd);
         if (pParentDlg != nullptr)
         {
             pParentDlg->RemoveDirtyRect(this);
         }
     }
     m_pRootWnd = nullptr;
}
 void CMyWindow::SetParent(CMyWindow* pParent)
 {
     m_pParent = pParent;
 }

 bool CMyWindow::IsRectIntersect(CRect r1, CRect r2)
 {
     int iInstanceW = abs(r1.left - r2.left) + abs(r1.right - r2.right);
     auto iInstanceH = abs(r1.top - r2.top) + abs(r1.bottom - r2.bottom);
     if (iInstanceW < r1.Width() + r2.Width() && iInstanceH < r1.Height() + r2.Height())
     {
         return true;
     }
     return false;
 }
 void CMyWindow::SetFontHeight(int iHeight)
 {
     m_font.DeleteObject();
     m_font.CreateFont(iHeight,                                    //   字体的高度   
         0,                                          //   字体的宽度  
         0,                                          //  nEscapement 
         0,                                          //  nOrientation   
         FW_NORMAL,                                  //   nWeight   
         FALSE,                                      //   bItalic   
         FALSE,                                      //   bUnderline   
         0,                                                   //   cStrikeOut   
         GB2312_CHARSET,                             //   nCharSet   
         OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
         CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
         DEFAULT_QUALITY,                       //   nQuality   
         DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
         _T("微软雅黑"));
 }

 void CMyWindow::MoveWindow(const CPoint& pt)
{
    SetDirty(true);
    auto ptOri = GetClientRect().TopLeft();
    auto ptBR = CPoint(pt.x + m_rect.Width(), pt.y + m_rect.Height());
    m_rect.SetRect(pt, ptBR);
    auto ptNow = GetClientRect().TopLeft();
    int x = min(ptNow.x, ptOri.x);
    int y = min(ptNow.y, ptOri.y);
    auto ptOffset = ptNow - ptOri;
    int w = m_rect.Width() + abs(ptOffset.cx);
    int h = m_rect.Height() + abs(ptOffset.cy);
    CRect dirty(x, y, x + w, y + h);
    //UpdateMyWnd(dirty);
    UpdateMyWnd(GetClientRect());

}

void CMyWindow::MoveWindow(int x, int y)
{
    MoveWindow(CPoint(x, y));
}

void CMyWindow::SetDimention(int w, int h)
{
    m_rect.SetRect(m_rect.left, m_rect.top, w + m_rect.left, h + m_rect.top);
    UpdateMyWnd(GetClientRect());
}
void CMyWindow::SetSupportClick(bool bCanClick)
{
    m_bSupportClick = bCanClick;
}
void CMyWindow::OnMouseLeave()
{
    if (!m_isMouseIn)
    {
        return;
    }
    m_isMouseIn = false;
    OnMouseHover(0, CPoint(0,0));

}
bool CMyWindow::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bVisible || !m_bMouseHoverSensitive)
    {
        return false;
    }
    auto rect = GetClientRect();
    if (rect.PtInRect(point))
    {
        return true;
    }
    return false;
}
void CMyWindow::OnMouseHover(UINT nFlags, CPoint point)
{
    if (m_bMouseHoverSensitive)
    {
        UpdateMyWnd(GetDirtyRect());
    }
}
void CMyWindow::OnMouseIn(UINT nFlags, CPoint point)
{
    if (m_isMouseIn)
    {
        return;
    }
    auto rect = GetClientRect();
    if (!rect.PtInRect(point))
    {
        return;
    }

    m_isMouseIn = true;
    OnMouseHover(0, point);
}          
bool CMyWindow::IsMouseIn()
{
    return m_isMouseIn;
}

CMyWindow* CMyWindow::HitTest(CPoint pt)
{
    auto rect = GetClientRect();
    if (m_bVisible && rect.PtInRect(pt))
    {
        return this;
    }
    return nullptr;
}

void CMyWindow::SetSelectState(bool bSelect)
{
    if (!m_bSupportClick)
    {
        return;
    }
    if (m_bSelected == bSelect)
    {
        return;
    }
    m_bSelected = bSelect;
    if (bSelect)
    {//子窗口被选中那么设置父窗口被选中?
        if (m_pParent != nullptr)
        {
            m_pParent->SetSelectStateFromChild(bSelect);
        }
    }
    UpdateMyWnd(GetDirtyRect());
}

void CMyWindow::SetSelectStateFromChild(bool bSelect)
{
}

BOOL CMyWindow::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    return true;
}
bool CMyWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_isMouseIn)
    {
        return false;
    }

    if (!m_bSupportClick)
    {
        return false;
    }
    //SetSelectState(true);
   //
    if (m_pGroupManager != nullptr)
    {
        m_pGroupManager->SelectWnd(this);
    }
    if (m_clickFunc != nullptr)
    {
        m_clickFunc(this);
    }
    return true;
}
void CMyWindow::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void CMyWindow::SetBkPic(UINT nResID )
{   
    if (m_pBkImage != nullptr)
    {
        m_pBkImage->Destroy();
        delete m_pBkImage;
    }
    m_pBkImage = new CImage();
    LoadImageFromResource(m_pBkImage, nResID, _T("PNG"));

}

void CMyWindow::SetBkPic(UINT nResNormalID, UINT nResSelectID)
{
    if (nResNormalID != 0)
    {
        m_pBkImage = new CImage();
        LoadImageFromResource(m_pBkImage, nResNormalID, _T("PNG"));
    }
    if (nResSelectID != 0)
    {
        m_pBkSelectedImage = new CImage();
        LoadImageFromResource(m_pBkSelectedImage, nResSelectID, _T("PNG"));
    }

}

int CMyWindow::GetMaxlayer(CPoint pt)
{
    if (m_bVisible && GetClientRect().PtInRect(pt))
    {
        return m_iLayer;
    }
    return INT_MIN;
}

void CMyWindow::SetExtendDrawBK(bool bExtend)
{
    m_bExtendBk = bExtend;
}

void CMyWindow::SetName(const CString& sName)
{
    m_sName = sName;
}

void CMyWindow::SetBkGroundColor(COLORREF rgb)
{
    m_bSetBkGroundColor = true;
    m_bkColor = rgb;
}
void CMyWindow::SetBkGroundColor(COLORREF rgbNormal, COLORREF rgbSelect)
{
    m_bSetBkGroundColor = true;
    m_bSetHigthLightBkGroundColor = true;
    m_bkColor = rgbNormal;
    m_bkHigthLightColor = rgbSelect;
    UpdateMyWnd(GetClientRect());
}
void CMyWindow::SetTextColor(COLORREF rgbNormal, COLORREF rgbSelect)
{
    m_textHightLigtColor = rgbSelect; 
    m_textNormalColor = rgbNormal;    
}

bool CMyWindow::Refresh(CDC* pMemDC , CRect r)
{
    if (m_pRootWnd == nullptr || !m_bVisible)
    {
        return false;
    }
    auto rect = GetClientRect();
    if (!IsRectIntersect(rect, r))
    {
        return false;
    }
    if (m_bMouseHoverSensitive)
    {
        if (m_isMouseIn)
        {
            if (m_bSetHigthLightBkGroundColor)
            {
                pMemDC->FillSolidRect(&rect, m_bkHigthLightColor);
            }
            else
            {
                if (IsSetBkGroundColor())
                {
                    pMemDC->FillSolidRect(&rect, m_bkColor);
                }
            }
        } 
        else
        {
            if (IsSetBkGroundColor())
            {
                pMemDC->FillSolidRect(&rect, m_bkColor);
            }
        }

    }
    else
    {
        if (IsSetBkGroundColor())
        {
            pMemDC->FillSolidRect(&rect, m_bkColor);
        }
    }
    
   
    if (m_pBkImage != nullptr)
    {
        if (m_bExtendBk)
        {
            auto w = m_pBkImage->GetWidth();
            auto h = m_pBkImage->GetHeight();
            auto x = rect.left;
            auto y = (rect.Height() - h) / 2 + rect.top;
            int lefWidth = rect.Width();
            int _w = w;
            while (lefWidth <= rect.Width() && lefWidth > 0)
            {
                m_pBkImage->Draw(pMemDC->m_hDC, x, y, _w, h);
                x += _w;
                lefWidth -= w;
                if (lefWidth < w)
                {
                    _w = lefWidth;
                }

            }
        }
        else
        {
            auto w = m_pBkImage->GetWidth();
            auto h = m_pBkImage->GetHeight();
            auto x = (rect.Width() - w) / 2 + rect.left;
            auto y = (rect.Height() - h) / 2 + rect.top;
            m_pBkImage->Draw(pMemDC->m_hDC, x, y);
        }
        
    }
    m_bIsDirty = false;
    return true;
}
CRect CMyWindow::GetMyRect()
{
    return m_rect;
}
CSize CMyWindow::GetBkPicSize()
{
    if (m_pBkImage != nullptr)
    {
        return CSize(m_pBkImage->GetWidth(), m_pBkImage->GetHeight());
    }
    return CSize();
}
CMyWindow* CMyWindow::GetParent()
{
    return m_pParent;
}

CRect CMyWindow::GetClientRect()
{
    auto pParent = m_pParent;
    int x = m_rect.left;
    int y = m_rect.top;
    while (pParent != nullptr)
    {
        auto rect = pParent->GetMyRect();
        x += rect.left;
        y += rect.top;
        pParent = pParent->GetParent();
    }
     
    return CRect(x, y, x+ m_rect.Width(), y + m_rect.Height());
}

CRect CMyWindow::GetDirtyRect()
{
    return GetClientRect();
}
 
bool CMyWindow::IsSetBkGroundColor()
{
    return m_bSetBkGroundColor;
}
bool CMyWindow::IsSelect()
{
    return m_bSelected;
}
bool CMyWindow::IsVisible()
{
    return m_bVisible;
}
void CMyWindow::SetDirty(bool isDirty)
{
    m_bIsDirty = isDirty;
}
void CMyWindow::ForceUpdate()
{
    SetDirty(true);
    UpdateMyWnd(GetDirtyRect());
}
bool CMyWindow::IsDirty()
{
    return m_bIsDirty;
}

void CMyWindow::SetLayer(int iLayer)
{
    m_iLayer = iLayer;
    auto pParent = m_pParent;
    while (pParent != nullptr)
    {
        pParent->SetLayer(iLayer);
        pParent = pParent->GetParent();
    }
}

int CMyWindow::GetLayer()
{
    return m_iLayer;
}
            
void CMyWindow::UpdateMyWnd(CRect rect)
{
    m_bIsDirty = true;
    if (m_refreshImpFun != nullptr && rect.Width() != 0 && rect.Height() != 0)
    {
        m_refreshImpFun(this, rect);
    }
   // m_pRootWnd->SetDirtyRect()
    //m_pRootWnd->InvalidateRect(rect);
}

void CMyWindow::SetMouseHoverSensitive(bool bMouseHoverSensitive)
{
    m_bMouseHoverSensitive = bMouseHoverSensitive;
}
void CMyWindow::SetVisible(bool bVisible)
{
    m_bVisible = bVisible;
}
void CMyWindow::OnSize(int cx, int cy)
{
    if (cx == 0 || cy == 0)
    {
        return;
    }
    SetDimention(cx, cy);

}
void CMyWindow::SetGroupManager(CGroupManager* pGroupManager)
{
    m_pGroupManager = pGroupManager;
}
void CMyWindow::RegisterClickFun(ClickFunc fun)
{
    m_clickFunc = fun;
}
BOOL CMyWindow::LoadImageFromResource(CImage* pImage, UINT nResID, LPCTSTR lpTyp)
{
    if (pImage == NULL)
        return false;
    pImage->Destroy();
    // 查找资源
    HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
    if (hRsrc == NULL)
        return false;
    // 加载资源
    HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
    if (hImgData == NULL)
    {
        ::FreeResource(hImgData);
        return false;
    }
    // 锁定内存中的指定资源
    LPVOID lpVoid = ::LockResource(hImgData);
    LPSTREAM pStream = NULL;
    DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
    HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
    LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
    ::memcpy(lpByte, lpVoid, dwSize);
    // 解除内存中的指定资源
    ::GlobalUnlock(hNew);
    // 从指定内存创建流对象
    HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
    if (ht != S_OK)
    {
        GlobalFree(hNew);
    }
    else
    {
        // 加载图片
        pImage->Load(pStream);
        GlobalFree(hNew);
    }

    if (pImage->IsNull())
    {
        return false;
    }
    if (pImage->GetBPP() == 32) //确认该图像包含Alpha通道
    {
        int i;
        int j;
        for (i = 0; i < pImage->GetWidth(); i++)
        {
            for (j = 0; j < pImage->GetHeight(); j++)
            {
                byte* pByte = (byte*)pImage->GetPixelAddress(i, j);
                pByte[0] = pByte[0] * pByte[3] / 255;
                pByte[1] = pByte[1] * pByte[3] / 255;
                pByte[2] = pByte[2] * pByte[3] / 255;
            }
        }
    }

    // 释放资源
    ::FreeResource(hImgData);
    return true;
}
