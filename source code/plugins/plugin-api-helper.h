#pragma once

#include "plugin-api.h"

#define LOADCONFIG(cfg)	{ auto& _load_config = cfg;
#define LOADVAR(variable, varname) jsonLoad(variable, _load_config varname);
#define LOADVARDword(variable, varname)	{ string _stemp; jsonLoad(_stemp, _load_config varname); variable = STRtoDWORD(_stemp);};
#define LOADVARFunc(vartype, variable, varname, func) vartype variable; jsonLoad(variable, _load_config varname); func;
#define LOADVARDecl(vartype, variable, varname) LOADVARFunc (vartype, variable, varname, )
#define LOADEND };


//////////////////////////////////////////////////////////////////////////


#include <ddraw.h>
#include <Gdiplus.h>

#pragma comment(lib, "gdiplus")



class PluginOverlayGdiPlusHelper
{
	DWORD							m_dwBitmapWidth;
	DWORD							m_dwBitmapHeight;
	BYTE*							m_pBitmapBits;
	Gdiplus::Bitmap*				m_pBitmap;
	Gdiplus::Graphics*				m_pGraphics;
	Gdiplus::GdiplusStartupInput	m_gdiplusStartupInput;
	ULONG_PTR						m_gdiplusToken;

	void*							playclawdata;

	OVERLAY_LOCK_INFO				m_Lock;

public:
	PluginOverlayGdiPlusHelper(void* _playclawdata) : m_dwBitmapWidth(-1)
		, m_dwBitmapHeight(-1)
		, m_pBitmapBits(0)
		, m_pBitmap(0)
		, m_pGraphics(0)
		, m_gdiplusToken(0)
		, playclawdata(_playclawdata)
	{
		GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);
	}


	virtual ~PluginOverlayGdiPlusHelper()
	{
		if (m_pBitmap)
			delete m_pBitmap;

		if (m_pGraphics)
			delete m_pGraphics;

		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}

	Gdiplus::Graphics*	GetGraphics() const { return m_pGraphics; };

	OVERLAY_LOCK_INFO*	BeginFrame()
	{
		if (!GH_LockOverlayEx(playclawdata, &m_Lock))						// lock overlay buffer and get its size
			return NULL;

		if (m_Lock.dwWidth != m_dwBitmapWidth || m_Lock.dwHeight != m_dwBitmapHeight)
		{
			if (m_pBitmap)
				delete m_pBitmap;
			if (m_pGraphics)
				delete m_pGraphics;

			m_dwBitmapWidth = m_Lock.dwWidth;
			m_dwBitmapHeight = m_Lock.dwHeight;

			m_pBitmap = new Gdiplus::Bitmap(m_dwBitmapWidth, m_dwBitmapHeight, PixelFormat32bppARGB);
			m_pGraphics = Gdiplus::Graphics::FromImage(m_pBitmap);

			// fastest settings
//			pGraphics->SetCompositingMode(CompositingModeSourceCopy);
//			pGraphics->SetCompositingQuality(CompositingQualityHighSpeed);
//			pGraphics->SetPixelOffsetMode(PixelOffsetModeNone);
//			pGraphics->SetSmoothingMode(SmoothingModeNone);
//			pGraphics->SetInterpolationMode(InterpolationModeDefault);

			// fastest settings for drawing sprites
//			pGraphics->SetCompositingMode(CompositingModeSourceOver);  // 'Over for tranparency
//			pGraphics->SetCompositingQuality(CompositingQualityHighSpeed);
//			pGraphics->SetPixelOffsetMode(PixelOffsetModeHighSpeed);
//			pGraphics->SetSmoothingMode(SmoothingModeHighSpeed);
//			pGraphics->SetInterpolationMode(InterpolationModeHighQuality);
		}

		return &m_Lock;
	}

	void		EndFrame(BOOL bMarkChanged = true)
	{
		if (bMarkChanged)
		{
			// drop rendered image to shared memory
			Gdiplus::BitmapData bmp_data;
			if (m_pBitmap->LockBits(&Gdiplus::Rect(0, 0, m_dwBitmapWidth, m_dwBitmapHeight), Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmp_data) == Gdiplus::Ok)
			{
				DWORD copy_num = min(m_Lock.dwPitch, (DWORD)bmp_data.Stride);
				for (UINT y = 0; y < m_dwBitmapHeight; y++)
				{
					BYTE *pSrc = (BYTE*)bmp_data.Scan0 + y * bmp_data.Stride;
					BYTE *pDst = m_Lock.lpBuffer + y * m_Lock.dwPitch;
					memcpy(pDst, pSrc, copy_num);
				}
				m_pBitmap->UnlockBits(&bmp_data);
			}
		}

		GH_UnlockOverlayEx(playclawdata, bMarkChanged);							// unlock and mark buffer as changed
	}

	//
	//	Draw text with possible shadow
	//
	void		DrawString(const WCHAR *str, Gdiplus::Font *pFont, Gdiplus::PointF pt, Gdiplus::Brush* pMainBrush, Gdiplus::Brush* pBackBrush)
	{
		if (m_pGraphics)
		{
			size_t l = wcslen(str);
			if (pBackBrush != NULL)
				m_pGraphics->DrawString(str, l, pFont, Gdiplus::PointF(pt.X + 1, pt.Y + 1), pBackBrush);
			m_pGraphics->DrawString(str, l, pFont, Gdiplus::PointF(pt.X, pt.Y), pMainBrush);
		}
	}

	void		DrawString(const WCHAR *str, Gdiplus::Font *pFont, Gdiplus::Point pt, Gdiplus::Brush* pMainBrush, Gdiplus::Brush* pBackBrush)
	{
		DrawString(str, pFont, Gdiplus::PointF((float)pt.X, (float)pt.Y), pMainBrush, pBackBrush);
	}

	void		DrawString(const WCHAR *str, Gdiplus::Font *pFont, Gdiplus::RectF &rect, Gdiplus::Brush* pMainBrush, Gdiplus::Brush* pBackBrush, BOOL bRightAlign)
	{
		if (!bRightAlign)
		{
			DrawString(str, pFont, Gdiplus::PointF(rect.X, rect.Y), pMainBrush, pBackBrush);
		}
		else
		{
			Gdiplus::RectF ext;
			GetTextExtent(str, pFont, &ext);
			DrawString(str, pFont, Gdiplus::PointF(rect.X + rect.Width - ext.Width, rect.Y), pMainBrush, pBackBrush);
		}
	}

	void		DrawString(const WCHAR *str, Gdiplus::Font *pFont, Gdiplus::Rect &rect, Gdiplus::Brush* pMainBrush, Gdiplus::Brush* pBackBrush, BOOL bRightAlign)
	{
		DrawString(str, pFont, Gdiplus::RectF((float)rect.X, (float)rect.Y, (float)rect.Width, (float)rect.Height), pMainBrush, pBackBrush, bRightAlign);
	}

	//
	//	Get text extent size
	//
	void		GetTextExtent(const WCHAR *str, Gdiplus::Font *pFont, Gdiplus::RectF *pBoundRect)
	{
		if (m_pGraphics)
		{
			m_pGraphics->MeasureString(str, wcslen(str), pFont, Gdiplus::PointF(0, 0), pBoundRect);
		}
	}
};

