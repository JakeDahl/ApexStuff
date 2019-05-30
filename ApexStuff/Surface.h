#ifndef __OVERLAY_SURFACE_H__
#define __OVERLAY_SURFACE_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Color.h"

namespace Overlay
{
	class ISurface
	{
	public:
		class SurfaceFont
		{
		public:
			std::string		m_strSurfaceFontName;
			std::string		m_strWindowsFontName;
			int				m_iSize;
			int				m_iWeight;
			int				m_iStyle;
			int				m_iOpt1;
			int				m_iOpt2;
			int				m_iOpt3;
			int				m_iOpt4;
			PVOID			m_pSurfaceFont;

		public:
			SurfaceFont(void);
			SurfaceFont(const std::string &strSurfaceFontName, const std::string &strWindowsFontName, int iSize, int iWeight, int iStyle, int iOpt1 = 0, int iOpt2 = 0, int iOpt3 = 0, int iOpt4 = 0);
			~SurfaceFont(void);
		};

	private:
		PVOID						m_pDevice;
		std::vector<  SurfaceFont >	m_vpSurfaceFonts;

	public:
		ISurface(void);
		ISurface(PVOID pSurface);
		virtual HRESULT					CreateSurfaceFonts(PVOID pFontDevice) = 0;
		PVOID							&GetDevice(void);
		std::shared_ptr< SurfaceFont >	GetSurfaceFontData(const std::string &strSurfaceFontName);
		std::vector< SurfaceFont >		&GetAllSurfaceFonts(void);
		void							PrepareFont(const std::string &strSurfaceFontName, const std::string &strWindowsFontName, int iSize, int iWeight, int iStyle, int iOpt1 = 0, int iOpt2 = 0, int iOpt3 = 0, int iOpt4 = 0);
		void							SetDevice(PVOID pDevice);
		virtual void					BorderBox(int x, int y, int w, int h, int thickness, Color color) = 0;
		virtual void					BorderBoxOutlined(int x, int y, int w, int h, int thickness, int border_thickness, Color color, Color outlined) = 0;
		virtual void					Line(int x, int y, int xx, int yy, Color color, float flThickness = 1.f) = 0;
		virtual void					Rect(int x, int y, int w, int h, Color color) = 0;
		virtual void					RectOutlined(int x, int y, int w, int h, int border_thickness, Color color, Color outlined) = 0;
		virtual void					String(int x, int y, const std::string &strSurfaceFontName, Color color, const char *pszString, ...) = 0;
	};
}

#endif