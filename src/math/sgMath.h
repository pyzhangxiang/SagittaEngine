//////////////////////////////////////////////////////
// file: sgMath.h @ 2008-1-19 by Zhang Xiang
//
// common declares and defines for math lib
// 
//////////////////////////////////////////////////////
#ifndef __SGMATH_H__
#define __SGMATH_H__

// INCLUDES ////////////////////////////////////////
#include "SagittaPlatform.h"
#include "sgTypeDef.h"
#include <assert.h>
#include <ostream>
#include <limits>
#include <cmath>
#include <vector>
#include <list>
#include <string>

// DECLARES //////////////////////////////////////

// DEFINES ///////////////////////////////////////
//// pi defines
//#define PI         ((Real)3.141592654)
//#define PIX2       ((Real)6.283185307)
//#define PI_DIV_2   ((Real)1.570796327)
//#define PI_DIV_4   ((Real)0.785398163) 
//#define PI_INV     ((Real)0.318309886) 
//
//// defines for small numbers
//#define EPSILON_E3 (Real)(1E-03) 
//#define EPSILON_E4 (Real)(1E-04) 
//#define EPSILON_E5 (Real)(1E-05)
//#define EPSILON_E6 (Real)(1E-06)
//#define EPSILON_E8 (Real)(1E-08)

namespace Sagitta{

	class Degree;
	class Angle;
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix3;
	class Matrix4;
	class Ray;
	class BoundingSphere;
	class AxisAlignedBox;
	class Plane;

	/** Wrapper class which indicates a given angle value is in Radians.
		@remarks
			Radian values are interchangeable with Degree values, and conversions
			will be done automatically between them.
	*/
	class Radian
	{
		Real m_Rad;

	public:
		explicit Radian ( Real r=0 ) : m_Rad(r) {}
		Radian ( const Degree& d );
		Radian& operator = ( const Real& f ) { m_Rad = f; return *this; }
		Radian& operator = ( const Radian& r ) { m_Rad = r.m_Rad; return *this; }
		Radian& operator = ( const Degree& d );

		Real valueDegrees() const; // see bottom of this file
		Real valueRadians() const { return m_Rad; }
		Real valueAngleUnits() const;

		const Radian& operator + () const { return *this; }
		Radian operator + ( const Radian& r ) const { return Radian ( m_Rad + r.m_Rad ); }
		Radian operator + ( const Degree& d ) const;
		Radian& operator += ( const Radian& r ) { m_Rad += r.m_Rad; return *this; }
		Radian& operator += ( const Degree& d );
		Radian operator - () const { return Radian(-m_Rad); }
		Radian operator - ( const Radian& r ) const { return Radian ( m_Rad - r.m_Rad ); }
		Radian operator - ( const Degree& d ) const;
		Radian& operator -= ( const Radian& r ) { m_Rad -= r.m_Rad; return *this; }
		Radian& operator -= ( const Degree& d );
		Radian operator * ( Real f ) const { return Radian ( m_Rad * f ); }
		Radian operator * ( const Radian& f ) const { return Radian ( m_Rad * f.m_Rad ); }
		Radian& operator *= ( Real f ) { m_Rad *= f; return *this; }
		Radian operator / ( Real f ) const { return Radian ( m_Rad / f ); }
		Radian& operator /= ( Real f ) { m_Rad /= f; return *this; }

		bool operator <  ( const Radian& r ) const { return m_Rad <  r.m_Rad; }
		bool operator <= ( const Radian& r ) const { return m_Rad <= r.m_Rad; }
		bool operator == ( const Radian& r ) const { return m_Rad == r.m_Rad; }
		bool operator != ( const Radian& r ) const { return m_Rad != r.m_Rad; }
		bool operator >= ( const Radian& r ) const { return m_Rad >= r.m_Rad; }
		bool operator >  ( const Radian& r ) const { return m_Rad >  r.m_Rad; }

		operator Real() const { return m_Rad; }
	};

	/** Wrapper class which indicates a given angle value is in Degrees.
		@remarks
			Degree values are interchangeable with Radian values, and conversions
			will be done automatically between them.
	*/
	class Degree
	{
		Real m_Deg; // if you get an error here - make sure to define/typedef 'Real' first

	public:
		explicit Degree ( Real d=0 ) : m_Deg(d) {}
		Degree ( const Radian& r ) : m_Deg(r.valueDegrees()) {}
		Degree& operator = ( const Real& f ) { m_Deg = f; return *this; }
		Degree& operator = ( const Degree& d ) { m_Deg = d.m_Deg; return *this; }
		Degree& operator = ( const Radian& r ) { m_Deg = r.valueDegrees(); return *this; }

		Real valueDegrees() const { return m_Deg; }
		Real valueRadians() const; // see bottom of this file
		Real valueAngleUnits() const;

		const Degree& operator + () const { return *this; }
		Degree operator + ( const Degree& d ) const { return Degree ( m_Deg + d.m_Deg ); }
		Degree operator + ( const Radian& r ) const { return Degree ( m_Deg + r.valueDegrees() ); }
		Degree& operator += ( const Degree& d ) { m_Deg += d.m_Deg; return *this; }
		Degree& operator += ( const Radian& r ) { m_Deg += r.valueDegrees(); return *this; }
		Degree operator - () const { return Degree(-m_Deg); }
		Degree operator - ( const Degree& d ) const { return Degree ( m_Deg - d.m_Deg ); }
		Degree operator - ( const Radian& r ) const { return Degree ( m_Deg - r.valueDegrees() ); }
		Degree& operator -= ( const Degree& d ) { m_Deg -= d.m_Deg; return *this; }
		Degree& operator -= ( const Radian& r ) { m_Deg -= r.valueDegrees(); return *this; }
		Degree operator * ( Real f ) const { return Degree ( m_Deg * f ); }
		Degree operator * ( const Degree& f ) const { return Degree ( m_Deg * f.m_Deg ); }
		Degree& operator *= ( Real f ) { m_Deg *= f; return *this; }
		Degree operator / ( Real f ) const { return Degree ( m_Deg / f ); }
		Degree& operator /= ( Real f ) { m_Deg /= f; return *this; }

		bool operator <  ( const Degree& d ) const { return m_Deg <  d.m_Deg; }
		bool operator <= ( const Degree& d ) const { return m_Deg <= d.m_Deg; }
		bool operator == ( const Degree& d ) const { return m_Deg == d.m_Deg; }
		bool operator != ( const Degree& d ) const { return m_Deg != d.m_Deg; }
		bool operator >= ( const Degree& d ) const { return m_Deg >= d.m_Deg; }
		bool operator >  ( const Degree& d ) const { return m_Deg >  d.m_Deg; }

		operator Real() const { return m_Deg; }
	};

	/** Wrapper class which identifies a value as the currently default angle 
		type, as defined by Math::setAngleUnit.
		@remarks
			Angle values will be automatically converted between radians and degrees,
			as appropriate.
	*/
	class Angle
	{
		Real m_Angle;
	public:
		explicit Angle ( Real angle ) : m_Angle(angle) {}
		operator Radian() const;
		operator Degree() const;
	};

	// these functions could not be defined within the class definition of class
	// Radian because they required class Degree to be defined
	inline Radian::Radian ( const Degree& d ) : m_Rad(d.valueRadians()) {
	}
	inline Radian& Radian::operator = ( const Degree& d ) {
		m_Rad = d.valueRadians(); return *this;
	}
	inline Radian Radian::operator + ( const Degree& d ) const {
		return Radian ( m_Rad + d.valueRadians() );
	}
	inline Radian& Radian::operator += ( const Degree& d ) {
		m_Rad += d.valueRadians();
		return *this;
	}
	inline Radian Radian::operator - ( const Degree& d ) const {
		return Radian ( m_Rad - d.valueRadians() );
	}
	inline Radian& Radian::operator -= ( const Degree& d ) {
		m_Rad -= d.valueRadians();
		return *this;
	}
	
	/** class to provide access to common mathematical functions
		@remarks

		@note

		@par
	*/
	class Fixed16{
	private:
		union{
			int m_fixed;
			
			struct{
				unsigned short m_decimal;
				short m_whole;
			};	
		};
	
	// static members
	public:
		static const Fixed16 ONE_DEV_2;

	public:
		Fixed16(void) : m_fixed(0){
		}
		
		explicit Fixed16(int s) : m_fixed(s << 16){
		}
		
		explicit Fixed16(Real s) : m_whole((int)std::floor(s)), m_decimal((uShort)((s - m_whole) * 65536)){
		}

		Fixed16 &operator=(int s){ m_fixed = s << 16; return *this; }

		Fixed16 &operator=(Real s){ m_whole = (int)std::floor(s); m_decimal = (uShort)((s - m_whole) * 65536); return *this; }

		Fixed16 &operator-(void){ m_fixed = -m_fixed; return *this; }
		
		Fixed16 operator+(int rhs) const{ Fixed16 ret; ret.m_fixed = m_fixed + (rhs << 16); return ret; }
		Fixed16 operator-(int rhs) const{ Fixed16 ret; ret.m_fixed = m_fixed - (rhs << 16); return ret; }
		Fixed16 operator*(int rhs) const{ Fixed16 ret; ret.m_fixed = m_fixed * rhs; return ret; }
		Fixed16 operator/(int rhs) const{ Fixed16 ret; ret.m_fixed = m_fixed / rhs; return ret; }
		
		Fixed16 operator+(const Fixed16 &rhs) const{ Fixed16 ret; ret.m_fixed = m_fixed + rhs.m_fixed; return ret; }
		Fixed16 operator-(const Fixed16 &rhs) const{ Fixed16 ret; ret.m_fixed = m_fixed - rhs.m_fixed; return ret; }
		Fixed16 operator*(const Fixed16 &rhs) const{ Fixed16 ret; ret.m_fixed = (m_fixed >> 8) * (rhs.m_fixed >> 8); return ret; }
		Fixed16 operator/(const Fixed16 &rhs) const{ Fixed16 ret; ret.m_fixed = (m_fixed << 8) / (rhs.m_fixed >> 8); return ret; }
		
		Fixed16 operator+(Real rhs) const{ return this->operator+(Fixed16(rhs)); }
		Fixed16 operator-(Real rhs) const{ return this->operator-(Fixed16(rhs)); }
		Fixed16 operator*(Real rhs) const{ return this->operator*(Fixed16(rhs)); }
		Fixed16 operator/(Real rhs) const{ return this->operator/(Fixed16(rhs)); }
		
		Fixed16 &operator+=(int rhs){ m_fixed += (rhs << 16); return *this; }
		Fixed16 &operator-=(int rhs){ m_fixed -= (rhs << 16); return *this; }
		Fixed16 &operator*=(int rhs){ m_fixed *= rhs; return *this; }
		Fixed16 &operator/=(int rhs){ m_fixed /= rhs; return *this; }
		
		Fixed16 &operator+=(const Fixed16 &rhs){ m_fixed += rhs.m_fixed; return *this; }
		Fixed16 &operator-=(const Fixed16 &rhs){ m_fixed -= rhs.m_fixed; return *this; }
		Fixed16 &operator*=(const Fixed16 &rhs){ m_fixed = (m_fixed >> 8) * (rhs.m_fixed >> 8); return *this; }
		Fixed16 &operator/=(const Fixed16 &rhs){ m_fixed = (m_fixed << 8) / (rhs.m_fixed >> 8); return *this; }
		
		Fixed16 &operator+=(Real rhs){ return this->operator+=(Fixed16(rhs)); }
		Fixed16 &operator-=(Real rhs){ return this->operator-=(Fixed16(rhs)); }
		Fixed16 &operator*=(Real rhs){ return this->operator*=(Fixed16(rhs)); }
		Fixed16 &operator/=(Real rhs){ return this->operator/=(Fixed16(rhs)); }

		bool operator<(int rhs) const{ return m_whole < rhs; }
		bool operator>(int rhs) const{ return (m_whole > rhs) || (m_whole == rhs && m_decimal > 0); }
		bool operator==(int rhs) const{ return (m_whole == rhs) && (m_decimal == 0); }
		bool operator!=(int rhs) const{ return !(this->operator==(rhs));}
		bool operator<=(int rhs) const{ return (m_whole < rhs) || (m_whole == rhs && m_decimal == 0); }
		bool operator>=(int rhs) const{ return m_whole >= rhs; }

		bool operator<(const Fixed16 &rhs) const{ return m_fixed < rhs.m_fixed; }
		bool operator>(const Fixed16 &rhs) const{ return m_fixed > rhs.m_fixed; }
		bool operator==(const Fixed16 &rhs) const{ return m_fixed == rhs.m_fixed; }
		bool operator!=(const Fixed16 &rhs) const{ return m_fixed != rhs.m_fixed; }
		bool operator<=(const Fixed16 &rhs) const{ return m_fixed <= rhs.m_fixed; }
		bool operator>=(const Fixed16 &rhs) const{ return m_fixed >= rhs.m_fixed; }

		friend bool operator<(int lhs, const Fixed16 &rhs);
		friend bool operator>(int lhs, const Fixed16 &rhs);
		friend bool operator==(int lhs, const Fixed16 &rhs);
		friend bool operator!=(int lhs, const Fixed16 &rhs);
		friend bool operator<=(int lhs, const Fixed16 &rhs);
		friend bool operator>=(int lhs, const Fixed16 &rhs);
		
		int whole(void) const{ return m_whole; }
		Real decimal(void) const{ return (Real)m_decimal / 65536.0f; };
		
		int toInt(void) const{ return m_whole; }
		int ceil(void) const{ return m_decimal > 0 ? m_whole + 1 : m_whole; }
		int round(void) const{ return m_decimal >= 32768 ? m_whole + 1 : m_whole; }
		int floor(void) const{ return m_whole; }
		Real toReal(void) const{ return (Real)m_fixed / 65536.0f; }
		
	}; //#### end class Fixed16

	inline bool operator<(int lhs, const Fixed16 &rhs){ return (lhs < rhs.m_whole) || (lhs == rhs.m_whole && rhs.m_decimal == 0); }
	inline bool operator>(int lhs, const Fixed16 &rhs){ return lhs > rhs.m_whole; }
	inline bool operator==(int lhs, const Fixed16 &rhs){ return rhs.operator==(lhs); }
	inline bool operator!=(int lhs, const Fixed16 &rhs){ return rhs.operator!=(lhs); }
	inline bool operator<=(int lhs, const Fixed16 &rhs){ return lhs <= rhs.m_whole; }
	inline bool operator>=(int lhs, const Fixed16 &rhs){ return (lhs > rhs.m_whole) || (lhs == rhs.m_whole && rhs.m_decimal == 0); }
	
	/** class to provide access to common mathematical functions
		@remarks

		@note

		@par

	*/
	class _SG_MathExport Math{
	public:
		// constant defines
		// pi defines
		static const Real PI;
		static const Real PI_X_2;
		static const Real PI_DIV_2;
		static const Real PI_DIV_3;
		static const Real PI_DIV_4;
		static const Real PI_INV;
		static const Real PI_DIV_180;
		
		// defines for small numbers
		static const Real EPSILON_E3;
		static const Real EPSILON_E4;
		static const Real EPSILON_E5;
		static const Real EPSILON_E6;
		static const Real EPSILON_E8;

		// defines for infinity numbers
		static const Real POS_INFINITY;
		static const Real NEG_INFINITY;

		// defines for conversion between degree and radian
		static const Real DEG_TO_RAD;
		static const Real RAD_TO_DEG;

		// defines for Color convert(d3d to gl || gl to d3d)
		static const Real COLOR_D3D_TO_GL;
		static const Real COLOR_GL_TO_D3D; 

		
		// common functions
		static bool RealEqual(Real a, Real b, Real tolerance = EPSILON_E6/*std::numeric_limits<Real>::epsilon()*/);

		static inline int IAbs(int aiValue){ return ( aiValue >= 0 ? aiValue : -aiValue ); }
		static inline int ICeil(Real afValue){ return int(ceil(afValue)); }
		static inline int IFloor(Real afValue){ return int(floor(afValue)); }
		static int ISign(int aiValue);

		static Real Abs(Real vfValue){ return Real(fabs(vfValue)); }
		static inline Degree Abs (const Degree& dValue) { return Degree(fabs(dValue.valueDegrees())); }
		static inline Radian Abs (const Radian& rValue) { return Radian(fabs(rValue.valueRadians())); }

		static Real InvSqrt(Real vfValue);
		static Real Sqr(Real vfValue){ return vfValue * vfValue; }
		static inline Real Sqrt(Real vfValue){ return Real(sqrt(vfValue)); }
		static inline Radian Sqrt(const Radian& fValue){ return Radian(sqrt(fValue.valueRadians())); }
		static inline Degree Sqrt(const Degree& fValue){ return Degree(sqrt(fValue.valueDegrees())); }

		static inline Real DegreeToRadian(Real vfValue){ return vfValue * DEG_TO_RAD; }
		static inline Real RadianToDegree(Real vfValue){ return vfValue * RAD_TO_DEG; }

		static int Round(Real afValue);

		static inline Real Ceil (Real fValue) { return Real(ceil(fValue)); }
		static inline Real Floor (Real fValue) { return Real(floor(fValue)); }
		static Real Sign (Real fValue);

		static inline Radian Sign (const Radian& rValue){
			return Radian(Sign(rValue.valueRadians()));
		}

		static inline Degree Sign (const Degree& dValue){
			return Degree(Sign(dValue.valueDegrees()));
		}

		static inline Real Exp (Real fValue) { return Real(exp(fValue)); }
		static inline Real Log (Real fValue) { return Real(log(fValue)); }
		static inline Real Pow (Real fBase, Real fExponent) { return Real(pow(fBase,fExponent)); }

		// radian
		static inline Real Cos(const Radian &aAngle){ return Real(cos(aAngle.valueRadians())); }
		static inline Real Sin(const Radian &aAngle){ return Real(sin(aAngle.valueRadians())); }
		static inline Real Tan(const Radian &aAngle){ return Real(tan(aAngle.valueRadians())); }
		static inline Real Cos(const Real &aRadian){ return Real(cos(aRadian)); }
		static inline Real Sin(const Real &aRadian){ return Real(sin(aRadian)); }
		static inline Real Tan(const Real &aRadian){ return Real(tan(aRadian)); }
		
		static Radian ACos(Real vfValue);
		static Radian ASin(Real vfValue);
		static inline Radian ATan(Real vfRAngle){ return Radian(atan(vfRAngle)); }
		static inline Radian ATan2(Real vfX, Real vfY){ return Radian(atan2(vfX, vfY)); } // atan(y/x)

		// random
		static Real UnitRandom(void); // in [0, 1]
		static Real RangeRandom(Real afLow, Real afHigh); // in [afLow, afHigh]
		static Real SymmetricRandom(void); // in [-1, 1]

		/** Generates a value based on the gaussian (normal) distribution function
			with the given offset and scale parameters.
		*/
		static Real GaussianDistribution(Real x, Real offset = 0.0f, Real scale = 1.0f);


		// intersects //////////////////////////////////////////////////////////////////////////

		/** Checks whether a given point is inside a triangle, in a
            2-dimensional (Cartesian) space.
            @remarks
                The vertices of the triangle must be given in either
                trigonometrical (anticlockwise) or inverse trigonometrical
                (clockwise) order.
            @param
                p The point.
            @param
                a The triangle's first vertex.
            @param
                b The triangle's second vertex.
            @param
                c The triangle's third vertex.
            @returns
                If the point resides in the triangle, <b>true</b> is
                returned.
            @par
                If the point is outside the triangle, <b>false</b> is
                returned.
        */
        static bool PointInTri2D(const Vector2 &p, const Vector2 &a, 
			const Vector2 &b, const Vector2 &c);

		/** Checks whether a given 3D point is inside a triangle.
       @remarks
            The vertices of the triangle must be given in either
            trigonometrical (anticlockwise) or inverse trigonometrical
            (clockwise) order, and the point must be guaranteed to be in the
			same plane as the triangle
        @param
            p The point.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the callermay already have it)
        @returns
            If the point resides in the triangle, <b>true</b> is
            returned.
        @par
            If the point is outside the triangle, <b>false</b> is
            returned.
        */
        static bool PointInTri3D(const Vector3 &p, const Vector3 &a, 
			const Vector3 &b, const Vector3 &c, const Vector3 &normal);

		/** Ray / plane intersection, returns boolean result and distance. */
		static std::pair<bool, Real> Intersects(const Ray &ray, const Plane &plane);

		/** Ray / sphere intersection, returns boolean result and distance. */
		static std::pair<bool, Real> Intersects(const Ray &ray,
			const BoundingSphere &sphere, bool discardInside = true);

		/** Ray / box intersection, returns boolean result and distance. */
		static std::pair<bool, Real> Intersects(const Ray &ray, const AxisAlignedBox &box);

		/** Ray / box intersection, returns boolean result and two intersection distance.
        @param
            ray The ray.
        @param
            box The box.
        @param
            d1 A real pointer to retrieve the near intersection distance
                from the ray origin, maybe <b>null</b> which means don't care
                about the near intersection distance.
        @param
            d2 A real pointer to retrieve the far intersection distance
                from the ray origin, maybe <b>null</b> which means don't care
                about the far intersection distance.
        @returns
            If the ray is intersects the box, <b>true</b> is returned, and
            the near intersection distance is return by <i>d1</i>, the
            far intersection distance is return by <i>d2</i>. Guarantee
            <b>0</b> <= <i>d1</i> <= <i>d2</i>.
        @par
            If the ray isn't intersects the box, <b>false</b> is returned, and
            <i>d1</i> and <i>d2</i> is unmodified.
        */
        static bool Intersects(const Ray &ray, const AxisAlignedBox &box, Real *d1, Real *d2);

		/** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
		@param 
			normal The triangle plane's normal (passed in rather than calculated
				on demand since the callermay already have it), doesn't need
                normalised since we don't care.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        static std::pair<bool, Real> Intersects(const Ray &ray, const Vector3 &a,
            const Vector3 &b, const Vector3 &c, const Vector3 &normal,
            bool positiveSide = true, bool negativeSide = true);

		/** Ray / triangle intersection, returns boolean result and distance.
        @param
            ray The ray.
        @param
            a The triangle's first vertex.
        @param
            b The triangle's second vertex.
        @param
            c The triangle's third vertex.
        @param
            positiveSide Intersect with "positive side" of the triangle
        @param
            negativeSide Intersect with "negative side" of the triangle
        @returns
            If the ray is intersects the triangle, a pair of <b>true</b> and the
            distance between intersection point and ray origin returned.
        @par
            If the ray isn't intersects the triangle, a pair of <b>false</b> and
            <b>0</b> returned.
        */
        static std::pair<bool, Real> Intersects(const Ray &ray, const Vector3 &a,
            const Vector3 &b, const Vector3 &c,
            bool positiveSide = true, bool negativeSide = true);

		/** Sphere / box intersection test. */
		static bool Intersects(const BoundingSphere &sphere, const AxisAlignedBox &box);

		/** Plane / box intersection test. */
		static bool Intersects(const Plane &plane, const AxisAlignedBox &box);

		/** Ray / convex plane list intersection test. 
		@param ray The ray to test with
		@param plaeList List of planes which form a convex volume
		@param normalIsOutside Does the normal point outside the volume
		*/
		static std::pair<bool, Real> Intersects(
			const Ray &ray, const std::vector<Plane> &planeList, 
			bool normalIsOutside);
		
		/** Ray / convex plane list intersection test. 
		@param ray The ray to test with
		@param plaeList List of planes which form a convex volume
		@param normalIsOutside Does the normal point outside the volume
		*/
		static std::pair<bool, Real> Intersects(
			const Ray &ray, const std::list<Plane> &planeList, 
			bool normalIsOutside);

		/** Sphere / plane intersection test. 
		@remarks NB just do a plane.getDistance(sphere.getCenter()) for more detail!
		*/
		static bool Intersects(const BoundingSphere &sphere, const Plane &plane);

		// others //////////////////////////////////////////////////////////////////////////

		/** Calculates the tangent space vector for a given set of positions / texture coords. */
		static Vector3 CalculateTangentSpaceVector(
			const Vector3 &position1, const Vector3 &position2, const Vector3 &position3,
			Real u1, Real v1, Real u2, Real v2, Real u3, Real v3);

		/** Build a reflection matrix for the passed in plane. */
		static Matrix4 BuildReflectionMatrix(const Plane &p);
		/** Calculate a face normal, including the w component which is the offset from the origin. */
		static Vector4 CalculateFaceNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
		/** Calculate a face normal, no w-information. */
		static Vector3 CalculateBasicFaceNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
		/** Calculate a face normal without normalize, including the w component which is the offset from the origin. */
		static Vector4 CalculateFaceNormalWithoutNormalize(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
		/** Calculate a face normal without normalize, no w-information. */
		static Vector3 CalculateBasicFaceNormalWithoutNormalize(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
		
	}; //#### end class Math

	//  [6/22/2009 zhangxiang]
	inline Real Radian::valueDegrees(void) const{
		return Math::RadianToDegree(m_Rad);
	}

	//  [6/22/2009 zhangxiang]
	inline Real Degree::valueRadians(void) const{
		return Math::DegreeToRadian(m_Deg);
	}

};

#endif // __SGMATH_H__
