#ifndef __OVERLAY_D3D9SURFACE_H__
#define __OVERLAY_D3D9SURFACE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Surface.h"

namespace Overlay
{
	// Function: PrepareFont
	//	Opt1: Width
	//	Opt2: Italic
	//	Opt3: MipLevels
	class CD3D9Surface : public ISurface
	{
	private:
		LPD3DXLINE			m_pD3DXLine;
	public:
		CD3D9Surface(PVOID pDevice);
		~CD3D9Surface(void);
		HRESULT				CreateSurfaceFonts(PVOID pFontDevice);
		LPDIRECT3DDEVICE9	GetD3D9Device(void);
		void				BorderBox(int x, int y, int w, int h, int thickness, Color color);
		void				BorderBoxOutlined(int x, int y, int w, int h, int thickness, int border_thickness, Color color, Color outlined);
		void 				Line(int x, int y, int xx, int yy, Color color, float flThickness = 1.f);
		void 				Rect(int x, int y, int w, int h, Color color);
		void 				RectOutlined(int x, int y, int w, int h, int border_thickness, Color color, Color outlined);
		void 				String(int x, int y, const std::string &strSurfaceFontName, Color color, const char *pszString, ...);
	};
}

#endif