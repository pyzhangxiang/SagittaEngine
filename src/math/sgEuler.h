//////////////////////////////////////////////////////
// file: sgEuler.h @ 2008-8-28 by Zhang Xiang
// declares of the class Euler
// Euler is a class ...
//////////////////////////////////////////////////////
#ifndef __SGEULER_H__
#define __SGEULER_H__

// INCLUDES //////////////////////////////////////////
#include "sgMath.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class Matrix3;
	class Vector3;

	/** Implementation of Euler, z-x-y(roll-pitch-yaw) rotate order
	@remarks

	@note

	@par

	*/
	class _SG_MathExport Euler{
	// static members
	public:
		static const Euler ZERO;

	// member variables
	private:
        SERIALIZE_BEGIN(Euler)
		/// Radian value of angles
		Real m_fx, m_fy, m_fz;
        SERIALIZE_END

	// constructors & destructor
	public:
		Euler() : m_fx(0), m_fy(0), m_fz(0){

		}

		/** Constructor. 
			@param
				_x
			@param
				_y
			@param
				_z Radian value angle
		*/
		Euler(Real _x, Real _y, Real _z)
		:m_fx(_x), m_fy(_y), m_fz(_z){

		}

		Euler(Real arrayData[3])
		:m_fx(arrayData[0]), m_fy(arrayData[1]), m_fz(arrayData[2]){

		}

		Euler(const Matrix3 &rotMat){
			fromRotateMatrix(rotMat);
		}

		Euler(const Euler &rhe) :
		m_fx(rhe.m_fx), m_fy(rhe.m_fy), m_fz(rhe.m_fz){

		}

	// member functions
	public:
		Real x(void) const{ return m_fx; }
		Real y(void) const{ return m_fy; }
		Real z(void) const{ return m_fz; }
		void setX(Real _x){ m_fx = _x; }
		void setY(Real _y){ m_fy = _y; }
		void setZ(Real _z){ m_fz = _z; }
		void setValues(Real _x, Real _y, Real _z){
			m_fx = _x;
			m_fy = _y;
			m_fz = _z;
		};
		void fromRotateMatrix(const Matrix3 &rotMat);
		void toRotateMatrix(Matrix3 &rmRot) const;

	}; //#### end class Euler

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGEULER_H__
