#include "D3D9Overlay.h"
#include "D3D9Surface.h"

using namespace Overlay;

HRESULT CD3D9Overlay::StartupD3D9Components(HWND hWindow)
{
	auto hResult = Direct3DCreate9Ex(D3D_SDK_VERSION, &this->m_pDirect3D9Ex);

	if (SUCCEEDED(hResult))
	{
		D3DPRESENT_PARAMETERS params;
		ZeroMemory(&params, sizeof(D3DPRESENT_PARAMETERS));

		params.Windowed = TRUE;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.BackBufferFormat = D3DFMT_A8R8G8B8;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D16;
		params.MultiSampleType = D3DMULTISAMPLE_NONE;
		params.PresentationInterval = 0x80000000L;

		DWORD dwMSQAAQuality = 0;
		if (SUCCEEDED(this->m_pDirect3D9Ex->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, true, D3DMULTISAMPLE_NONMASKABLE, &dwMSQAAQuality)))
		{
			params.MultiSampleType = D3DMULTISAMPLE_NONMASKABLE;
			params.MultiSampleQuality = dwMSQAAQuality - 1;
		}

		hResult = this->m_pDirect3D9Ex->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, NULL, &this->m_pDirect3D9Device);

		if (SUCCEEDED(hResult))
		{
			auto &pSurface = this->GetSurface();
			pSurface->SetDevice(this->m_pDirect3D9Device);
			hResult = pSurface->CreateSurfaceFonts(this->m_pDirect3D9Device);
		}
	}

	return hResult;
}

CD3D9Overlay::CD3D9Overlay(void) : IOverlay(), m_pDirect3D9Device(nullptr), m_pDirect3D9Ex(nullptr)
{
	this->GetSurface() = std::make_shared< CD3D9Surface >(nullptr);
}

CD3D9Overlay::~CD3D9Overlay(void)
{

}

bool CD3D9Overlay::Create(const std::string &strWindowTitle)
{
	if (strWindowTitle.empty())
		return false;

	if (!(this->GetTargetWindow() = FindWindowA(NULL, strWindowTitle.c_str())))
		return false;
	this->m_strWindowTitle = strWindowTitle;

	BOOL bComposition = FALSE;
	if (FAILED(DwmIsCompositionEnabled(&bComposition)))
		return false;

	static auto strOverlayClass = this->GetRandomString(rand() % 32 + 12);
	static auto strOverlayTitle = this->GetRandomString(rand() % 32 + 12);

#ifdef _UNICODE
	static auto wstrOverlayClass = this->ConvertStringToWString(strOverlayClass);
	static auto wstrOverlayTitle = this->ConvertStringToWString(strOverlayTitle);
#endif

	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		0,
		IOverlay::WndProc,
		0,
		0,
		NULL,
		LoadIcon(NULL, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		0,
		0,
#ifdef _UNICODE
		wstrOverlayClass.c_str(),
#else
		strOverlayClass.c_str(),
#endif
		LoadIcon(0, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wc))
		return false;

	auto &hWndOverlay = this->GetOverlayWindow();

	if (!(hWndOverlay = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
#ifdef _UNICODE
		wstrOverlayClass.c_str(),
		wstrOverlayTitle.c_str(),
#else
		strOverlayClass.c_str(),
		strOverlayTitle.c_str(),
#endif
		WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, NULL, 0)))
		return false;

	if (!SetLayeredWindowAttributes(hWndOverlay, RGB(NULL, NULL, NULL), 255, ULW_COLORKEY | LWA_ALPHA))
		return false;

	static MARGINS  DWMMargins = { -1, -1, -1, -1 };
	if (FAILED(DwmExtendFrameIntoClientArea(hWndOverlay, &DWMMargins)))
		return false;

	ShowWindow(hWndOverlay, SW_SHOWDEFAULT);
	if (!UpdateWindow(hWndOverlay))
		return false;

	this->ScaleOverlayWindow();

	return (bool)SUCCEEDED(this->StartupD3D9Components(hWndOverlay));
}

bool CD3D9Overlay::Render(void)
{
	if (!this->UpdateOverlay())
		return false;

	static int iFrameCounter = 1;
	if ((iFrameCounter % 10) == 0)
	{
		if (!FindWindowA(NULL, this->m_strWindowTitle.c_str()))
			return false;

		iFrameCounter = 0;
	}

	if (SUCCEEDED(this->m_pDirect3D9Device->Clear(0, nullptr, D3DCLEAR_TARGET, 0x00000000, 1.f, 0)))
	{
		if (this->GetTargetWindow() == GetForegroundWindow())
		{
			if (SUCCEEDED(this->m_pDirect3D9Device->BeginScene()))
			{
				for (auto callback_function : this->GetAllCallbackFunctions())
				{
					callback_function(this, this->GetSurface());
				}

				this->m_pDirect3D9Device->EndScene();
			}
		}

		this->m_pDirect3D9Device->PresentEx(nullptr, nullptr, NULL, nullptr, 0);
	}

	iFrameCounter++;

	return true;
}

void CD3D9Overlay::Shutdown(void)
{
	if (this->m_pDirect3D9Ex)
	{
		this->m_pDirect3D9Ex->Release();
		this->m_pDirect3D9Ex = NULL;
	}

	if (this->m_pDirect3D9Device)
	{
		this->m_pDirect3D9Device->Release();
		this->m_pDirect3D9Device = NULL;
	}

	this->GetAllCallbackFunctions().clear();

	auto &hWndOverlay = this->GetOverlayWindow();
	DestroyWindow(hWndOverlay);
	hWndOverlay = NULL;

	this->GetTargetWindow() = NULL;
}