#include "D3D9Surface.h"

using namespace Overlay;

CD3D9Surface::CD3D9Surface(PVOID pDevice) : ISurface(pDevice)
{

}

CD3D9Surface::~CD3D9Surface(void)
{

}

HRESULT CD3D9Surface::CreateSurfaceFonts(PVOID pFontDevice)
{
	auto hResult = S_OK;
	for (auto &pSurfaceFont : this->GetAllSurfaceFonts())
	{
		hResult = D3DXCreateFontA((LPDIRECT3DDEVICE9)pFontDevice,
			pSurfaceFont.m_iSize,
			(UINT)pSurfaceFont.m_iOpt1,
			(UINT)pSurfaceFont.m_iWeight,
			pSurfaceFont.m_iOpt2,
			pSurfaceFont.m_iOpt3,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			pSurfaceFont.m_strWindowsFontName.c_str(),
			(LPD3DXFONT *)&pSurfaceFont.m_pSurfaceFont);

		if (FAILED(hResult))
			break;
	}

	return hResult;
}

LPDIRECT3DDEVICE9 CD3D9Surface::GetD3D9Device(void)
{
	return (LPDIRECT3DDEVICE9)this->GetDevice();
}

void CD3D9Surface::BorderBox(int x, int y, int w, int h, int thickness, Color color)
{
	this->Rect(x, y, w, thickness, color);
	this->Rect(x, y, thickness, h, color);
	this->Rect(x + w, y, thickness, h, color);
	this->Rect(x, y + h, w + thickness, thickness, color);
}

void CD3D9Surface::BorderBoxOutlined(int x, int y, int w, int h, int thickness, int border_thickness, Color color, Color outlined)
{
	this->BorderBox(x, y, w, h, thickness, color);
	this->BorderBox(x - border_thickness, y - border_thickness, w + border_thickness + thickness, h + border_thickness + thickness, border_thickness, outlined);
	this->BorderBox(x + thickness, y + thickness, w - border_thickness - thickness, h - border_thickness - thickness, border_thickness, outlined);
}

void CD3D9Surface::Line(int x, int y, int xx, int yy, Color color, float flThickness)
{
	auto pDevice = this->GetD3D9Device();
	if (!pDevice)
		return;

	if (!m_pD3DXLine && !(*(DWORD*)m_pD3DXLine))
	{
		if (FAILED(D3DXCreateLine(pDevice, &m_pD3DXLine)))
			return;

		if (!m_pD3DXLine)
			return;

		m_pD3DXLine->SetWidth(1.f);
		m_pD3DXLine->SetPattern(0xFFFFFFFF);
		m_pD3DXLine->SetAntialias(TRUE);
	}

	m_pD3DXLine->SetWidth(flThickness);
	D3DXVECTOR2 points[2];
	points[0].x = (FLOAT)x;
	points[0].y = (FLOAT)y;
	points[1].x = (FLOAT)xx;
	points[1].y = (FLOAT)yy;
	m_pD3DXLine->Draw(points, 2, color.GetHex());
}

void CD3D9Surface::Rect(int x, int y, int w, int h, Color color)
{
	auto pDevice = this->GetD3D9Device();
	if (!pDevice)
		return;

	static D3DRECT rect_angle;
	rect_angle = { x,y, x + w, y + h };

	pDevice->Clear(1, &rect_angle, D3DCLEAR_TARGET, color.GetHex(), 1.f, 0);
}

void CD3D9Surface::RectOutlined(int x, int y, int w, int h, int border_thickness, Color color, Color outlined)
{
	this->Rect(x, y, w, h, color);
	this->BorderBox(x - border_thickness, y - border_thickness, w + border_thickness, h + border_thickness, border_thickness, outlined);
}

void CD3D9Surface::String(int x, int y, const std::string &strSurfaceFontName, Color color, const char *pszString, ...)
{
	auto pSurfaceFontData = this->GetSurfaceFontData(strSurfaceFontName);
	if (!pSurfaceFontData)
		return;

	static char pszBuffer[0x400];

	va_list args;
	va_start(args, pszString);
	vsprintf_s(pszBuffer, pszString, args);
	va_end(args);


	static RECT rect_angle;
	rect_angle = { x, y, x + 500, y + 200 };

	((LPD3DXFONT)pSurfaceFontData->m_pSurfaceFont)->DrawTextA(nullptr, pszBuffer, -1, &rect_angle, DT_TOP, color.GetHex());
}