//////////////////////////////////////////////////////
// file: SColor.cpp @ 2008-10-6 by Zhang Xiang
// defines of the class Color
// Color is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SColor.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	const Color Color::BLACK(0, 0, 0);
	const Color Color::WHITE(255, 255, 255);
	const Color Color::DARKGRAY(128, 128, 128);
	const Color Color::GRAY(160, 160, 164);
	const Color Color::LIGHTGRAY(192, 192, 192);
	const Color Color::RED(255, 0, 0);
	const Color Color::GREEN(0, 255, 0);
	const Color Color::BLUE(0, 0, 255);
	const Color Color::CYAN(0, 255, 255);
	const Color Color::MAGENTA(255, 0, 255);
	const Color Color::YELLOW(255, 255, 0);
	
	Color::Color(void) :
		_m_ialpha(255), _m_ired(0), _m_igreen(0), _m_iblue(0){

	}

	Color::Color(int r, int g, int b, int a /* = 255 */){	
		setRgb(r, g, b, a);
	}

	Color::Color(Real r, Real g, Real b, Real a /* = 1.0 */){
		setRgbF(r, g, b, a);
	}

	Color::Color(const Color::GLColor &color){
		setRgbF(color.x(), color.y(), color.z(), color.w());
	}

	Color::Color(Argb argb) :
		_m_ialpha(alpha(argb)), _m_ired(red(argb)), _m_igreen(green(argb)), _m_iblue(blue(argb)){

	}

	Color::Color(const Color &color) :
		_m_ialpha(color._m_ialpha), _m_ired(color._m_ired), _m_igreen(color._m_igreen), _m_iblue(color._m_iblue){

	}

	Color &Color::operator = (const Color &color){
		_m_ired = color._m_ired;
		_m_igreen = color._m_igreen;
		_m_iblue = color._m_iblue;
		_m_ialpha = color._m_ialpha;

		return *this;
	}

	void Color::getRgb(int *r, int *g, int *b, int *a /* = 0 */) const{
		if(!r || !g || !b)
			return;

		*r = _m_ired;
		*g = _m_igreen;
		*b = _m_iblue;

		if(a)
			*a = _m_ialpha;
	}

	void Color::setRgb(int r, int g, int b, int a /* = 255 */){
		_m_ired = rgbLimit(r);
		_m_igreen = rgbLimit(g);
		_m_iblue = rgbLimit(b);
		_m_ialpha = rgbLimit(a);
	}

	void Color::getRgbF(Real *r, Real *g, Real *b, Real *a /* = 0 */) const{
		if(!r || !g || !b)
			return;

		*r = static_cast<Real>(_m_ired) * Math::COLOR_D3D_TO_GL;
		*g = static_cast<Real>(_m_igreen) * Math::COLOR_D3D_TO_GL;
		*b = static_cast<Real>(_m_iblue) * Math::COLOR_D3D_TO_GL;

		if(a)
			*a = static_cast<Real>(_m_ialpha) * Math::COLOR_D3D_TO_GL;
	}

	void Color::setRgbF(Real r, Real g, Real b, Real a /* = 1.0 */){
		_m_ired = Math::Round(rgbLimit(r) * Math::COLOR_GL_TO_D3D);
		_m_igreen = Math::Round(rgbLimit(g) * Math::COLOR_GL_TO_D3D);
		_m_iblue = Math::Round(rgbLimit(b) * Math::COLOR_GL_TO_D3D);
		_m_ialpha = Math::Round(rgbLimit(a) * Math::COLOR_GL_TO_D3D);
	}

	Color::Argb Color::toRgb() const{
		return Color::rgb(_m_ired, _m_igreen, _m_iblue);
	}

	void Color::setRgb(Color::Argb rgb){
		_m_ired = rgbLimit(red(rgb));
		_m_igreen = rgbLimit(green(rgb));
		_m_iblue = rgbLimit(blue(rgb));
		_m_ialpha = 255;
	}

	//  [1/15/2009 zhangxiang]
	Color::Argb Color::toArgb(void) const{
		return Color::argb(_m_ialpha, _m_ired, _m_igreen, _m_iblue);
	}

	//  [1/15/2009 zhangxiang]
	void Color::setArgb(Argb argb){
		_m_ired = rgbLimit(red(argb));
		_m_igreen = rgbLimit(green(argb));
		_m_iblue = rgbLimit(blue(argb));
		_m_ialpha = rgbLimit(alpha(argb));
	}

	Color::Rgba Color::toRgba() const{
		return Color::rgba(_m_ired, _m_igreen, _m_iblue, _m_ialpha);
	}

	void Color::setRgba(Color::Rgba rgba){
		_m_ired = rgbLimit((int)((rgba >> 24) & 0xff));
		_m_igreen = rgbLimit((int)((rgba >> 16) & 0xff));
		_m_iblue = rgbLimit((int)((rgba >> 8) & 0xff));
		_m_ialpha = rgbLimit((int)(rgba & 0xff));
	}

	//  [1/24/2009 zhangxiang]
	Color Color::operator+(const Color &color) const{
		return Color(_m_ired + color._m_ired,
					_m_igreen + color._m_igreen,
					_m_iblue + color._m_iblue,
					_m_ialpha + color._m_ialpha);
	}

	//  [1/24/2009 zhangxiang]
	Color Color::operator*(const Color &color) const{
		return Color(_m_ired * color._m_ired / 255,
			_m_igreen * color._m_igreen / 255,
			_m_iblue * color._m_iblue / 255,
			_m_ialpha * color._m_ialpha / 255);
	}

	//  [1/24/2009 zhangxiang]
	Color &Color::operator+=(const Color &color){
		_m_ired = rgbLimit(_m_ired + color._m_ired);
		_m_igreen = rgbLimit(_m_igreen + color._m_igreen);
		_m_iblue = rgbLimit(_m_iblue + color._m_iblue);
		_m_ialpha = rgbLimit(_m_ialpha + color._m_ialpha);

		return *this;
	}

	//  [1/24/2009 zhangxiang]
	Color &Color::operator*=(const Color &color){
		_m_ired *= color._m_ired / 255;
		_m_igreen *= color._m_igreen / 255;
		_m_iblue *= color._m_iblue / 255;
		_m_ialpha *= color._m_ialpha / 255;

		return *this;
	}

	bool Color::operator == (const Sagitta::Color &color) const{
		return (_m_ired == color._m_ired &&
				_m_igreen == color._m_igreen &&
				_m_iblue == color._m_iblue &&
				_m_ialpha == color._m_ialpha);
	}

	bool Color::operator !=(const Sagitta::Color &color) const{
		return !operator==(color);
	}

} // namespace Sagitta