#ifndef __OVERLAY_OVERLAY_H__
#define __OVERLAY_OVERLAY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Surface.h"

namespace Overlay
{
	class IOverlay;
	class CD3D9Overlay;
	class Color;
	class CD3D9Surface;

	using RenderCallbackFn = void(*)(IOverlay *thisptr, std::shared_ptr< ISurface > pSurface);

	class IOverlay
	{
	private:
		HWND							m_hWndOverlay;
		HWND							m_hWndTarget;
		int								m_iWidth;
		int								m_iHeight;
		std::shared_ptr< ISurface >		m_pSurface;
		std::vector< RenderCallbackFn >	m_vRenderCallbacks;

	public:
		IOverlay(void);
		~IOverlay(void);
		bool							UpdateOverlay(void);
		virtual	bool					Create(const std::string &strWindowTitle) = 0;
		virtual bool					Render(void) = 0;
		HWND							&GetOverlayWindow(void);
		HWND							&GetTargetWindow(void);
		int	GetWidth(void) const;
		int GetHeight(void) const;
		std::shared_ptr< ISurface >		&GetSurface(void);
		std::string						GetRandomString(const std::size_t len);
		std::vector< RenderCallbackFn >	&GetAllCallbackFunctions(void);
		std::wstring					ConvertStringToWString(const std::string &strInput);
		void							AddToCallback(RenderCallbackFn callback_function);
		void							ScaleOverlayWindow(void);
		virtual void					Shutdown(void) = 0;
	public:
		static LRESULT CALLBACK			WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static std::shared_ptr< IOverlay > &GetInstance(void);

	};
}

#endif