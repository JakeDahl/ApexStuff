#ifndef __OVERLAY_D3D9OVERLAY_H__
#define __OVERLAY_D3D9OVERLAY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Overlay.h"

namespace Overlay
{
	class CD3D9Overlay : public IOverlay
	{
	private:
		IDirect3D9Ex		*m_pDirect3D9Ex;
		IDirect3DDevice9Ex	*m_pDirect3D9Device;
		std::string			m_strWindowTitle;
	private:
		HRESULT StartupD3D9Components(HWND hWindow);

	public:
		CD3D9Overlay(void);
		~CD3D9Overlay(void);
		bool				Create(const std::string &strWindowTitle);
		bool				Render(void);
		void				Shutdown(void);
	};
}

#endif