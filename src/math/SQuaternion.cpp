//////////////////////////////////////////////////////
// file: SQuaternion.cpp @ 2008-1-24 by Zhang Xiang
// defines of the class SQuaternion
// SQuaternion is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SQuaternion.h"
#include "SMatrix3.h"
#include "SVector3.h"
#include "SMath.h"

// DECLARES //////////////////////////////////////////


// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	const float Quaternion::ms_fEpsilon = 1e-03f;
	const Quaternion Quaternion::ZERO(0.0, 0.0, 0.0, 0.0);
	const Quaternion Quaternion::IDENTITY(1.0f, 0.0, 0.0, 0.0);

	void Quaternion::fromRotationMatrix(const Matrix3& vrmRot){
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		float fTrace = vrmRot[0][0]+vrmRot[1][1]+vrmRot[2][2];
		float fRoot;

		if( fTrace > 0.0 ){
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = Math::Sqrt(fTrace + 1.0f);  // 2w
			m_fw = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;  // 1/(4w)
			m_fx = (vrmRot[2][1]-vrmRot[1][2])*fRoot;
			m_fy = (vrmRot[0][2]-vrmRot[2][0])*fRoot;
			m_fz = (vrmRot[1][0]-vrmRot[0][1])*fRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if( vrmRot[1][1] > vrmRot[0][0] )
				i = 1;
			if( vrmRot[2][2] > vrmRot[i][i] )
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			fRoot = Math::Sqrt(vrmRot[i][i]-vrmRot[j][j]-vrmRot[k][k] + 1.0f);
			float* apkQuat[3] = { &m_fx, &m_fy, &m_fz };
			*apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;
			m_fw = (vrmRot[k][j]-vrmRot[j][k])*fRoot;
			*apkQuat[j] = (vrmRot[j][i]+vrmRot[i][j])*fRoot;
			*apkQuat[k] = (vrmRot[k][i]+vrmRot[i][k])*fRoot;
		}

	} //#### end fromRotationMatrix

	void Quaternion::toRotationMatrix(Matrix3& rmRot) const{
		float fTx  = 2.0f * m_fx;
		float fTy  = 2.0f * m_fy;
		float fTz  = 2.0f * m_fz;
		float fTwx = fTx * m_fw;
		float fTwy = fTy * m_fw;
		float fTwz = fTz * m_fw;
		float fTxx = fTx * m_fx;
		float fTxy = fTy * m_fx;
		float fTxz = fTz * m_fx;
		float fTyy = fTy * m_fy;
		float fTyz = fTz * m_fy;
		float fTzz = fTz * m_fz;

		rmRot[0][0] = 1.0f - (fTyy + fTzz);
		rmRot[0][1] = fTxy - fTwz;
		rmRot[0][2] = fTxz + fTwy;
		rmRot[1][0] = fTxy + fTwz;
		rmRot[1][1] = 1.0f - (fTxx + fTzz);
		rmRot[1][2] = fTyz - fTwx;
		rmRot[2][0] = fTxz - fTwy;
		rmRot[2][1] = fTyz + fTwx;
		rmRot[2][2] = 1.0f - (fTxx + fTyy);

	} //#### end toRotationMatrix

	void Quaternion::fromAngleAxis(const float& vfrAngle, const Vector3& vvrAxis){
		// assert:  axis[] is unit length
		//
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		float fHalfAngle = 0.5f * vfrAngle;
		float fSin = Math::Sin(fHalfAngle);
		m_fw = Math::Cos(fHalfAngle);
		m_fx = fSin * vvrAxis.x();
		m_fy = fSin * vvrAxis.y();
		m_fz = fSin * vvrAxis.z();

	} //#### end fromAngleAxis

	void Quaternion::toAngleAxis(float& rfAngle, Vector3& rvAxis) const{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		float fSqrLength = m_fx * m_fx + m_fy * m_fy + m_fz * m_fz;
		if(fSqrLength > 0.0){
			rfAngle = 2.0f * Math::ACos(m_fw);
			float fInvLength = Math::InvSqrt(fSqrLength);
			rvAxis.setX(m_fx * fInvLength);
			rvAxis.setY(m_fy * fInvLength);
			rvAxis.setZ(m_fz * fInvLength);
		}else{
			// angle is 0 (mod 2*pi), so any axis will do
			rfAngle = 0.0f;
			rvAxis.setX(1.0);
			rvAxis.setY(0.0);
			rvAxis.setZ(0.0);
		}

	} //#### end toAngleAxis

	void Quaternion::fromAxes(const Vector3 *vvpAxis){
		Matrix3 mRot;

		for(size_t iCol=0; iCol<3; iCol++){
			mRot[0][iCol] = vvpAxis[iCol].x();
			mRot[1][iCol] = vvpAxis[iCol].y();
			mRot[2][iCol] = vvpAxis[iCol].z();
		}

		fromRotationMatrix(mRot);

	} //#### end fromAxes

	void Quaternion::fromAxes(const Vector3& vvrXAxis, const Vector3& vvrYAxis, const Vector3& vvrZAxis){
		Matrix3 mRot;

		mRot[0][0] = vvrXAxis.x();
		mRot[1][0] = vvrXAxis.y();
		mRot[2][0] = vvrXAxis.z();

		mRot[0][1] = vvrYAxis.x();
		mRot[1][1] = vvrYAxis.y();
		mRot[2][1] = vvrYAxis.z();

		mRot[0][2] = vvrZAxis.x();
		mRot[1][2] = vvrZAxis.y();
		mRot[2][2] = vvrZAxis.z();

		fromRotationMatrix(mRot);

	} //#### end fromAxes

	void Quaternion::toAxes(Vector3 *rvAxis) const{
		Matrix3 mRot;

		toRotationMatrix(mRot);

		for(size_t iCol=0; iCol<3; iCol++){
			rvAxis[iCol].setX(mRot[0][iCol]);
			rvAxis[iCol].setY(mRot[1][iCol]);
			rvAxis[iCol].setZ(mRot[2][iCol]);
		}

	} //#### end toAxes

	Vector3 Quaternion::xAxis(void) const{
		//float fTx  = 2.0f * m_fx;
		float fTy  = 2.0f * m_fy;
		float fTz  = 2.0f * m_fz;
		float fTwy = fTy * m_fw;
		float fTwz = fTz * m_fw;
		float fTxy = fTy * m_fx;
		float fTxz = fTz * m_fx;
		float fTyy = fTy * m_fy;
		float fTzz = fTz * m_fz;

		return Vector3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

	} //#### end xAxis

	Vector3 Quaternion::yAxis(void) const{
		float fTx  = 2.0f * m_fx;
		float fTy  = 2.0f * m_fy;
		float fTz  = 2.0f * m_fz;
		float fTwx = fTx * m_fw;
		float fTwz = fTz * m_fw;
		float fTxx = fTx * m_fx;
		float fTxy = fTy * m_fx;
		float fTyz = fTz * m_fy;
		float fTzz = fTz * m_fz;

		return Vector3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);

	} //#### end yAxis

	Vector3 Quaternion::zAxis(void) const{
		float fTx  = 2.0f * m_fx;
		float fTy  = 2.0f * m_fy;
		float fTz  = 2.0f * m_fz;
		float fTwx = fTx * m_fw;
		float fTwy = fTy * m_fw;
		float fTxx = fTx * m_fx;
		float fTxz = fTz * m_fx;
		float fTyy = fTy * m_fy;
		float fTyz = fTz * m_fy;

		return Vector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));

	} //#### end zAxis

	void Quaternion::toAxes(Vector3& rXAxis, Vector3& rYAxis, Vector3& rZAxis) const{
		Matrix3 mRot;

		toRotationMatrix(mRot);

		rXAxis.setX(mRot[0][0]);
		rXAxis.setY(mRot[1][0]);
		rXAxis.setZ(mRot[2][0]);

		rYAxis.setX(mRot[0][1]);
		rYAxis.setY(mRot[1][1]);
		rYAxis.setZ(mRot[2][1]);

		rZAxis.setX(mRot[0][2]);
		rZAxis.setY(mRot[1][2]);
		rZAxis.setZ(mRot[2][2]);

	} //#### end toAxes

	Quaternion Quaternion::operator + (const Quaternion& vqrq) const{
		return Quaternion(m_fw + vqrq.w(), m_fx + vqrq.x(), m_fy + vqrq.y(), m_fz + vqrq.z());
	}

	Quaternion Quaternion::operator - (const Quaternion& vqrq) const{
		return Quaternion(m_fw - vqrq.w(), m_fx - vqrq.x(), m_fy - vqrq.y(), m_fz - vqrq.z());
	}

	Quaternion Quaternion::operator * (const Quaternion& vqrq) const{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.

		return Quaternion(m_fw * vqrq.w() - m_fx * vqrq.x() - m_fy * vqrq.y() - m_fz * vqrq.z(),
						m_fw * vqrq.x() + m_fx * vqrq.w() + m_fy * vqrq.z() - m_fz * vqrq.y(),
						m_fw * vqrq.y() + m_fy * vqrq.w() + m_fz * vqrq.x() - m_fx * vqrq.z(),
						m_fw * vqrq.z() + m_fz * vqrq.w() + m_fx * vqrq.y() - m_fy * vqrq.x());
	}

	Quaternion Quaternion::operator * (const float vfScalar) const{
		return Quaternion(vfScalar * m_fw, vfScalar * m_fx, vfScalar * m_fy, vfScalar * m_fz);
	}

	Quaternion operator * (const float vfScalar, const Quaternion& vqrq){
		return Quaternion(vfScalar * vqrq.w(), vfScalar * vqrq.x(), vfScalar * vqrq.y(), vfScalar * vqrq.z());
	}

	Quaternion Quaternion::operator - () const{
		return Quaternion(-m_fw, -m_fx, -m_fy, -m_fz);
	}

	float Quaternion::dotProduct(const Quaternion& vqrq) const{
		return (m_fw * vqrq.w() + m_fx * vqrq.x() + m_fy * vqrq.y() + m_fz * vqrq.z());
	}

	float Quaternion::norm() const{
		return (m_fw * m_fw + m_fx * m_fx + m_fy * m_fy + m_fz * m_fz);
	}

	float Quaternion::squredLength() const{
		return (m_fw * m_fw + m_fx * m_fx + m_fy * m_fy + m_fz * m_fz);
	}

	//float Quaternion::length() const{
	//	return Math::Sqrt(m_fw * m_fw + m_fx * m_fx + m_fy * m_fy + m_fz * m_fz);
	//}

	float Quaternion::normalise(){
		float squredLength = norm();
		float factor = 1.0f / Math::Sqrt(squredLength);
		*this = *this * factor;

		return squredLength;
	}

	Quaternion Quaternion::inverse() const{
		float fNorm = m_fw * m_fw + m_fx * m_fx + m_fy * m_fy + m_fz * m_fz;
		if(fNorm > 0.0){
			float fInvNorm = 1.0f / fNorm;
			return Quaternion(m_fw*fInvNorm, -m_fx*fInvNorm, -m_fy*fInvNorm, -m_fz*fInvNorm);
		}else{
			// return an invalid result to flag the error
			return Quaternion::ZERO;
		}
	} //#### end inverse

	Quaternion Quaternion::unitInverse() const{
		// assert: 'this' is unit length
		return Quaternion(m_fw, -m_fx, -m_fy, -m_fz);
	}

	Quaternion Quaternion::exp() const{
		// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
		// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

		float fAngle = Math::Sqrt(m_fx*m_fx + m_fy*m_fy + m_fz*m_fz);
		float fSin = Math::Sin(fAngle);

		Quaternion kResult;
		kResult.setW(Math::Cos(fAngle));

		if( Math::Abs(fSin) >= ms_fEpsilon ){
			float fCoeff = fSin/fAngle;
			kResult.setX(fCoeff*m_fx);
			kResult.setY(fCoeff*m_fy);
			kResult.setZ(fCoeff*m_fz);
		}else{
			kResult.setX(m_fx);
			kResult.setY(m_fy);
			kResult.setZ(m_fz);
		}

		return kResult;

	} //#### end exp

	Quaternion Quaternion::log() const{
		// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
		// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

		Quaternion kResult;
		kResult.setW(0.0);

		if( Math::Abs(m_fw) < 1.0 ){
			float fAngle =  Math::ACos(m_fw);
			float fSin = Math::Sin(fAngle);
			if( Math::Abs(fSin) >= ms_fEpsilon ){
				float fCoeff = fAngle/fSin;
				kResult.setX(fCoeff*m_fx);
				kResult.setY(fCoeff*m_fy);
				kResult.setZ(fCoeff*m_fz);
				return kResult;
			}
		}

		kResult.setX(m_fx);
		kResult.setY(m_fy);
		kResult.setZ(m_fz);

		return kResult;

	} //#### end log

	Vector3 Quaternion::operator * (const Vector3& vvrv) const{
		// nVidia SDK implementation
		Vector3 uv, uuv;
		Vector3 qvec(m_fx, m_fy, m_fz);
		uv = qvec.crossProduct(vvrv);
		uuv = qvec.crossProduct(uv);
		uv *= (2.0f * m_fw);
		uuv *= 2.0f;

		return vvrv + uv + uuv;
	}

	bool Quaternion::equals(const Quaternion& vqrq, const float& tolerance) const{
		float fCos = dotProduct(vqrq);
		float angle = Math::ACos(fCos);

		return (Math::Abs(angle) <= tolerance)
			|| Math::RealEqual(angle, Math::PI, tolerance);
	}

	Quaternion Quaternion::Slerp(float vft, const Quaternion& vqrP, const Quaternion& vqrQ, bool shortestPath /* = false */){
		float fCos = vqrP.dotProduct(vqrQ);
		Quaternion rkT;

		// Do we need to invert rotation?
		if(fCos < 0.0f && shortestPath){
			fCos = -fCos;
			rkT = -vqrQ;
		}else{
			rkT = vqrQ;
		}

		if(Math::Abs(fCos) < 1 - ms_fEpsilon){
			// Standard case (slerp)
			float fSin = Math::Sqrt(1 - Math::Sqr(fCos));
			float fAngle = Math::ATan2(fSin, fCos);
			float fInvSin = 1.0f / fSin;
			float fCoeff0 = Math::Sin((1.0f - vft) * fAngle) * fInvSin;
			float fCoeff1 = Math::Sin(vft * fAngle) * fInvSin;
			return fCoeff0 * vqrP + fCoeff1 * rkT;
		}else{
			// There are two situations:
			// 1. "vqrP" and "vqrQ" are very close (fCos ~= +1), so we can do a linear
			//    interpolation safely.
			// 2. "vqrP" and "vqrQ" are almost inverse of each other (fCos ~= -1), there
			//    are an infinite number of possibilities interpolation. but we haven't
			//    have method to fix this case, so just use linear interpolation here.
			Quaternion t = (1.0f - vft) * vqrP + vft * rkT;
			// taking the complement requires renormalisation
			t.normalise();
			return t;
		}

	} //#### end Slerp

	Quaternion Quaternion::SlerpExtraSpins(float vft, const Quaternion& vqrP, const Quaternion& vqrQ, int iExtraSpins){
		float fCos = vqrP.dotProduct(vqrQ);
		float fAngle = Math::ACos(fCos);

		if ( Math::Abs(fAngle) < ms_fEpsilon )
			return vqrP;

		float fSin = Math::Sin(fAngle);
		float fPhase = Math::PI*iExtraSpins*vft;
		float fInvSin = 1.0f/fSin;
		float fCoeff0 = Math::Sin((1.0f-vft)*fAngle - fPhase)*fInvSin;
		float fCoeff1 = Math::Sin(vft*fAngle + fPhase)*fInvSin;
		return fCoeff0*vqrP + fCoeff1*vqrQ;

	} //#### end SlerpExtraSpins

	void Quaternion::Intermediate(const Quaternion& vqrQ0, const Quaternion& vqrQ1, const Quaternion& vqrQ2, Quaternion& rqA, Quaternion& rqB){
		// assert:  q0, q1, q2 are unit quaternions

		Quaternion kQ0inv = vqrQ0.unitInverse();
		Quaternion kQ1inv = vqrQ1.unitInverse();
		Quaternion rkP0 = kQ0inv*vqrQ1;
		Quaternion rkP1 = kQ1inv*vqrQ2;
		Quaternion kArg = 0.25*(rkP0.log()-rkP1.log());
		Quaternion kMinusArg = -kArg;

		rqA = vqrQ1*kArg.exp();
		rqB = vqrQ1*kMinusArg.exp();

	} //#### end Intermediate

	Quaternion Quaternion::Squad(float vft, const Quaternion& vqrP, const Quaternion& vqrA, const Quaternion& vqrB, const Quaternion& vqrQ, bool shortestPath /* = false */){
		float fSlerpT = 2.0f*vft*(1.0f-vft);
		Quaternion kSlerpP = Slerp(vft, vqrP, vqrQ, shortestPath);
		Quaternion kSlerpQ = Slerp(vft, vqrA, vqrB);
		return Slerp(fSlerpT, kSlerpP ,kSlerpQ);

	} //#### end Squad

	float Quaternion::getRoll(bool reprojectAxis /* = true */) const{
		if(reprojectAxis){
			// roll = atan2(localx.y, localx.x)
			// pick parts of xAxis() implementation that we need
			float fTx  = 2.0f*m_fx;
			float fTy  = 2.0f*m_fy;
			float fTz  = 2.0f*m_fz;
			float fTwz = fTz*m_fw;
			float fTxy = fTy*m_fx;
			float fTyy = fTy*m_fy;
			float fTzz = fTz*m_fz;

			// Vector3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

			return Math::ATan2(fTxy+fTwz, 1.0f-(fTyy+fTzz));

		}else{
			return Math::ATan2(2*(m_fx*m_fy + m_fw*m_fz), m_fw*m_fw + m_fx*m_fx - m_fy*m_fy - m_fz*m_fz);
		}

	} //#### end getRoll

	float Quaternion::getPitch(bool reprojectAxis /* = true */) const{
		if(reprojectAxis){
			// pitch = atan2(localy.z, localy.y)
			// pick parts of yAxis() implementation that we need
			float fTx  = 2.0f*m_fx;
			float fTy  = 2.0f*m_fy;
			float fTz  = 2.0f*m_fz;
			float fTwx = fTx*m_fw;
			float fTxx = fTx*m_fx;
			float fTyz = fTz*m_fy;
			float fTzz = fTz*m_fz;

			// Vector3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
			return Math::ATan2(fTyz+fTwx, 1.0f-(fTxx+fTzz));
		}else{
			// internal version
			return Math::ATan2(2*(m_fy*m_fz + m_fw*m_fx), m_fw*m_fw - m_fx*m_fx - m_fy*m_fy + m_fz*m_fz);
		}

	} //#### end getPitch

	float Quaternion::getYaw(bool reprojectAxis /* = true */) const{
		if(reprojectAxis){
			// yaw = atan2(localz.x, localz.z)
			// pick parts of zAxis() implementation that we need
			float fTx  = 2.0f*m_fx;
			float fTy  = 2.0f*m_fy;
			float fTz  = 2.0f*m_fz;
			float fTwy = fTy*m_fw;
			float fTxx = fTx*m_fx;
			float fTxz = fTz*m_fx;
			float fTyy = fTy*m_fy;

			// Vector3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

			return Math::ATan2(fTxz+fTwy, 1.0f-(fTxx+fTyy));

		}else{
			// internal version
			return Math::ASin(-2*(m_fx*m_fz - m_fw*m_fy));
		}

	} //#### end getYaw

	Quaternion Quaternion::Nlerp(float vft, const Quaternion& vqrP, const Quaternion& vqrQ, bool shortestPath /* = false */){
		Quaternion result;
		float fCos = vqrP.dotProduct(vqrQ);
		if (fCos < 0.0f && shortestPath){
			result = vqrP + vft * ((-vqrQ) - vqrP);
		}else{
			result = vqrP + vft * (vqrQ - vqrP);
		}
		result.normalise();
		return result;

	} //#### end Nlerp

	void Quaternion::fromEuler(const Euler& aEuler){
		Quaternion qx;
		Quaternion qy;
		Quaternion qz;

		qx.fromAngleAxis(aEuler.x(), Vector3(1.0, 0.0, 0.0));
		qy.fromAngleAxis(aEuler.y(), Vector3(0.0, 1.0, 0.0));
		qz.fromAngleAxis(aEuler.z(), Vector3(0.0, 0.0, 1.0));

		*this = qz * qx * qy;
	}

	Euler Quaternion::toEuler(void) const{
		Euler aEuler;

		aEuler.setX(Math::ASin( 2 * m_fw * m_fx + 2 * m_fy * m_fz));
		aEuler.setY(-Math::ATan2(2 * m_fx * m_fz - 2 * m_fw * m_fy , 1 - 2 * m_fx * m_fx - 2 * m_fy * m_fy));
		aEuler.setZ(-Math::ATan2(2 * m_fx * m_fy - 2 * m_fw * m_fz , 1 - 2 * m_fx * m_fx - 2 * m_fz * m_fz));	

		return aEuler;
	}
}