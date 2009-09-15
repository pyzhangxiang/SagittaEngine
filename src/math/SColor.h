//////////////////////////////////////////////////////
// file: SColor.h @ 2008-10-6 by Zhang Xiang
// declares of the class Color
// Color is a class ...
//////////////////////////////////////////////////////
#ifndef __COLOR_H__
#define __COLOR_H__

// INCLUDES //////////////////////////////////////////
#include "SMath.h"
#include "SVector4.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _MathExport Color{
	public:
		typedef uInt Rgba;
		typedef uInt Argb;
		typedef Vector4 GLColor;
		typedef Argb D3DColor;

		static const Color BLACK;
		static const Color WHITE;
		static const Color DARKGRAY;
		static const Color GRAY;
		static const Color LIGHTGRAY;
		static const Color RED;
		static const Color GREEN;
		static const Color BLUE;
		static const Color CYAN;
		static const Color MAGENTA;
		static const Color YELLOW;

		static int red(Argb argb){                // get red part of RGB
			return ((argb >> 16) & 0xff);
		}

		static int green(Argb argb){                // get green part of RGB
			return ((argb >> 8) & 0xff);
		}

		static int blue(Argb argb){                // get blue part of RGB
			return (argb & 0xff);
		}

		static int alpha(Argb argb){                // get alpha part of RGBA
			return ((argb >> 24) & 0xff);
		}

		static Argb rgb(int r, int g, int b){			// set RGB value
			return (0xff << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
		}

		static Argb argb(int a, int r, int g, int b){	// set RGBA value
			return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
		}

		static Rgba rgba(int r, int g, int b, int a){	// set RGBA value
			return ((r & 0xff) << 24) | ((g & 0xff) << 16) | ((b & 0xff) << 8) | (a & 0xff);
		}

		static int rgbLimit(int part){
			return (part > 255) ? 255 : ((part < 0) ? 0 : part);
		}

		static Real rgbLimit(Real part){
			return (part > 1.0) ? 1.0 : ((part < 0.0) ? 0.0 : part);
		}

		static void rgbLimit(GLColor &aColor){
			aColor.setValues(rgbLimit(aColor.x()),
							rgbLimit(aColor.y()),
							rgbLimit(aColor.z()),
							rgbLimit(aColor.w()));
		}

	// member variables
	private:
		uByte _m_iblue;
		uByte _m_igreen;
		uByte _m_ired;
		uByte _m_ialpha;

	// constructors & destructor
	public:
		Color();
		explicit Color(int r, int g, int b, int a = 255);
		explicit Color(Real r, Real g, Real b, Real a = 1.0);
		Color(const GLColor &color);
		explicit Color(Argb argb);
		Color(const Color &color);
		~Color(){}

	// member functions
	public:
		Color &operator = (const Color &color);

		int alpha() const{ return _m_ialpha; }
		void setAlpha(int alpha){ _m_ialpha = rgbLimit(alpha); }

		Real alphaF() const{ return static_cast<Real>(_m_ialpha) * Math::COLOR_D3D_TO_GL; }
		void setAlphaF(Real alpha){ _m_ialpha = Math::Round(rgbLimit(alpha) * Math::COLOR_GL_TO_D3D); }

		int red() const{ return _m_ired; }
		int green() const{ return _m_igreen; }
		int blue() const{ return _m_iblue; }
		void setRed(int red){ _m_ired = rgbLimit(red); }
		void setGreen(int green){ _m_igreen = rgbLimit(green); }
		void setBlue(int blue){ _m_iblue = rgbLimit(blue); }

		Real redF() const{ return static_cast<Real>(_m_ired) * Math::COLOR_D3D_TO_GL; }
		Real greenF() const{ return static_cast<Real>(_m_igreen) * Math::COLOR_D3D_TO_GL; }
		Real blueF() const{ return static_cast<Real>(_m_iblue) * Math::COLOR_D3D_TO_GL; }
		void setRedF(Real red){ _m_ired = Math::Round(rgbLimit(red) * Math::COLOR_GL_TO_D3D); }
		void setGreenF(Real green){ _m_igreen = Math::Round(rgbLimit(green) * Math::COLOR_GL_TO_D3D); }
		void setBlueF(Real blue){ _m_iblue = Math::Round(rgbLimit(blue) * Math::COLOR_GL_TO_D3D); }

		void getRgb(int *r, int *g, int *b, int *a = 0) const;
		void setRgb(int r, int g, int b, int a = 255);

		void getRgbF(Real *r, Real *g, Real *b, Real *a = 0) const;
		void setRgbF(Real r, Real g, Real b, Real a = 1.0);

		Argb toRgb() const;
		void setRgb(Argb rgb);

		Argb toArgb(void) const;
		void setArgb(Argb argb);

		Rgba toRgba() const;
		void setRgba(Rgba rgba);

		GLColor toGLColor() const{ return GLColor(redF(), greenF(), blueF(), alphaF()); }
		D3DColor toD3DColor(void) const{ return toArgb(); }

		Color operator+(const Color &color) const;
		Color operator*(const Color &color) const;
		Color &operator+=(const Color &color);
		Color &operator*=(const Color &color);

		bool operator == (const Color &color) const;
		bool operator != (const Color &color) const;

	}; //#### end class Color

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __COLOR_H__