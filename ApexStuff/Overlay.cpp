#include "Overlay.h"

using namespace Overlay;

IOverlay::IOverlay(void) : m_hWndOverlay(NULL), m_hWndTarget(NULL)
{

}

IOverlay::~IOverlay(void)
{

}

bool IOverlay::UpdateOverlay(void)
{
	static MSG uMsg;

	if (PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE))
	{
		if (uMsg.message == WM_QUIT)
			return false;

		TranslateMessage(&uMsg);
		DispatchMessage(&uMsg);
	}


	return true;
}

HWND &IOverlay::GetOverlayWindow(void)
{
	return this->m_hWndOverlay;
}

HWND &IOverlay::GetTargetWindow(void)
{
	return this->m_hWndTarget;
}

int	IOverlay::GetWidth(void) const
{
	return this->m_iWidth;
}

int IOverlay::GetHeight(void) const
{
	return this->m_iHeight;
}

std::shared_ptr< ISurface > &IOverlay::GetSurface(void)
{
	return this->m_pSurface;
}

std::string IOverlay::GetRandomString(const std::size_t len)
{
	static std::string strAlphaNumericChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::string strRandomString("", len);
	for (std::size_t i = 0; i < len; i++)
		strRandomString[i] = strAlphaNumericChars[rand() % strAlphaNumericChars.size()];

	return strRandomString;
}

std::vector< RenderCallbackFn > &IOverlay::GetAllCallbackFunctions(void)
{
	return this->m_vRenderCallbacks;
}

std::wstring IOverlay::ConvertStringToWString(const std::string &strInput)
{
	if (strInput.empty())
		return L"";

	auto nSize = strInput.length() + 1;
	std::wstring wstrOutput(nSize, L'\x23');

	size_t nCharsConverted = 0;
	mbstowcs_s(&nCharsConverted, &wstrOutput[0], nSize, strInput.c_str(), _TRUNCATE);

	return wstrOutput;
}

void IOverlay::AddToCallback(RenderCallbackFn callback_function)
{
	this->m_vRenderCallbacks.push_back(callback_function);
}

void IOverlay::ScaleOverlayWindow(void)
{
	static RECT rect_client;
	GetClientRect(this->m_hWndTarget, &rect_client);

	this->m_iWidth = rect_client.right;
	this->m_iHeight = rect_client.bottom;

	static RECT rect_window;
	GetWindowRect(this->m_hWndTarget, &rect_window);

	static POINT ptDifference;
	ptDifference.x = ptDifference.y = 0;

	ClientToScreen(this->m_hWndTarget, &ptDifference);

	auto x = rect_window.left + (ptDifference.x - rect_window.left);
	auto y = rect_window.top + (ptDifference.y - rect_window.top);

	if (x == 0)
	{
		x--;
		m_iWidth++;
	}

	if (y == 0)
	{
		y--;
		m_iHeight++;
	}

	MoveWindow(this->m_hWndOverlay, x, y, m_iWidth, m_iHeight, TRUE);
}

LRESULT CALLBACK IOverlay::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		return 0;
	case WM_ERASEBKGND:
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		return TRUE;
	case WM_PAINT:
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

std::shared_ptr< IOverlay > &IOverlay::GetInstance(void)
{
	static std::shared_ptr< IOverlay > pOverlay = nullptr;
	return pOverlay;
}