//////////////////////////////////////////////////////
// file: SEuler.cpp @ 2008-8-28 by Zhang Xiang
// defines of the class SEuler
// SEuler is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SEuler.h"
#include "SMatrix3.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	const Euler Euler::ZERO(0.0, 0.0, 0.0);

	void Euler::fromRotateMatrix(const Matrix3& rotMat){
		m_fx = Math::ASin(rotMat.arr()[7]);
		m_fy = Math::ATan2(-rotMat.arr()[6], rotMat.arr()[8]);
		m_fz = Math::ATan2(-rotMat.arr()[1], rotMat.arr()[4]);
	}

	void Euler::toRotateMatrix(Matrix3& rmRot) const{
		/*double cosx = Math::Cos(Math::DegreeToRadian(x));
		double cosy = Math::Cos(Math::DegreeToRadian(y));
		double cosz = Math::Cos(Math::DegreeToRadian(z));
		double sinx = Math::Sin(Math::DegreeToRadian(x));
		double siny = Math::Sin(Math::DegreeToRadian(y));
		double sinz = Math::Sin(Math::DegreeToRadian(z));

		/*Real *data = rotMat.arr();
		*data = cosy * cosz - sinx * siny * sinz;
		*(data + 1) = - cosx * sinz;
		*(data + 2) = sinx * cosy * sinz + siny * cosz;
		*(data + 3) = cosy * sinz + sinx * siny * cosz;
		*(data + 4) = cosx * cosz;
		*(data + 5) = siny * sinz - sinx * cosy * cosz;
		*(data + 6) = - cosx * siny;
		*(data + 7) = sinx;
		*(data + 8) = cosx * cosy;*/

		rmRot.fromEulerAnglesZXY(m_fy, m_fx, m_fz);
	}

} // namespace Sagitta