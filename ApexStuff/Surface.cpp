#include "Surface.h"

using namespace Overlay;

ISurface::SurfaceFont::SurfaceFont(void) : m_strSurfaceFontName(""), m_strWindowsFontName(""), m_iSize(0), m_iWeight(0), m_iStyle(0), m_iOpt1(0), m_iOpt2(0), m_iOpt3(0), m_iOpt4(0), m_pSurfaceFont(0)
{

}

ISurface::SurfaceFont::SurfaceFont(const std::string &strSurfaceFontName, const std::string &strWindowsFontName, int iSize, int iWeight, int iStyle, int iOpt1, int iOpt2, int iOpt3, int iOpt4) :
	m_strSurfaceFontName(strSurfaceFontName),
	m_strWindowsFontName(strWindowsFontName),
	m_iSize(iSize),
	m_iWeight(iWeight),
	m_iStyle(iStyle),
	m_iOpt1(iOpt1),
	m_iOpt2(iOpt2),
	m_iOpt3(iOpt3),
	m_iOpt4(iOpt4),
	m_pSurfaceFont(0)
{

}

ISurface::SurfaceFont::~SurfaceFont(void)
{

}

ISurface::ISurface(void)
{

}

ISurface::ISurface(PVOID pSurface)
{

}

PVOID &ISurface::GetDevice(void)
{
	return this->m_pDevice;
}

std::shared_ptr< ISurface::SurfaceFont > ISurface::GetSurfaceFontData(const std::string &strSurfaceFontName)
{
	if (strSurfaceFontName.empty())
		return nullptr;

	for (auto &pSurfaceFont : this->m_vpSurfaceFonts)
		if (!pSurfaceFont.m_strSurfaceFontName.compare(strSurfaceFontName))
			return std::make_shared< SurfaceFont >(pSurfaceFont);

	return nullptr;
}

std::vector< ISurface::SurfaceFont > &ISurface::GetAllSurfaceFonts(void)
{
	return this->m_vpSurfaceFonts;
}

void ISurface::PrepareFont(const std::string &strSurfaceFontName, const std::string &strWindowsFontName, int iSize, int iWeight, int iStyle, int iOpt1, int iOpt2, int iOpt3, int iOpt4)
{
	if (strSurfaceFontName.empty() || strWindowsFontName.empty())
		return;

	if (this->GetSurfaceFontData(strSurfaceFontName) != nullptr)
		return;

	this->m_vpSurfaceFonts.push_back({ strSurfaceFontName, strWindowsFontName, iSize, iWeight, iStyle, iOpt1, iOpt2, iOpt3, iOpt4 });
}

void ISurface::SetDevice(PVOID pDevice)
{
	this->m_pDevice = pDevice;
}