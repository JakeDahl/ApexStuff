#include "Color.h"

using namespace Overlay;

Color::Color(void)
{
	this->Set(0xFF000000);
}

Color::Color(int r, int g, int b, int a)
{
	this->Set(r, g, b, a);
}

Color::Color(unsigned long hex_color)
{
	this->Set(hex_color);
}

Color Color::operator = (const Color &color)
{
	this->Set(color.GetHex());
	return *this;
}

int &Color::operator [] (int iIndex)
{
	switch (iIndex)
	{
	case 1:
		return this->m_Color[1];
		break;
	case 2:
		return this->m_Color[2];
		break;
	case 3:
		return this->m_Color[3];
		break;
	default:
		break;
	}
	return this->m_Color[0];
}

int &Color::operator () (int iIndex)
{
	switch (iIndex)
	{
	case 1:
		return this->m_Color[1];
		break;
	case 2:
		return this->m_Color[2];
		break;
	case 3:
		return this->m_Color[3];
		break;
	default:
		break;
	}
	return this->m_Color[0];
}

int Color::r(void) const
{
	return this->m_Color[0];
}

int Color::g(void) const
{
	return this->m_Color[1];
}

int Color::b(void) const
{
	return this->m_Color[2];
}

int Color::a(void) const
{
	return this->m_Color[3];
}

unsigned long Color::GetHex(void) const
{
	return (unsigned long)(
		(unsigned long)this->m_Color[3] << 24) |
		((unsigned long)this->m_Color[0] << 16) |
		((unsigned long)this->m_Color[1] << 8) |
		((unsigned long)this->m_Color[2]);
}

void Color::Set(int r, int g, int b, int a)
{
	this->m_Color[0] = r;
	this->m_Color[1] = g;
	this->m_Color[2] = b;
	this->m_Color[3] = a;
}

void Color::Set(unsigned long hex_color)
{
	this->m_Color[0] = (int)((hex_color >> 16) & 0xFF);
	this->m_Color[1] = (int)((hex_color >> 8) & 0xFF);
	this->m_Color[3] = (int)((hex_color >> 24) & 0xFF);
	this->m_Color[2] = (int)(hex_color & 0xFF);
}