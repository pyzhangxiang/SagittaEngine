//////////////////////////////////////////////////////
// file: SMatrix.cpp @ 2008-1-23 by Zhang Xiang
// defines of the class SMatrix
// SMatrix is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SMatrix3.h"
#include "SMath.h"

// DECLARES //////////////////////////////////////////


// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// static constant valuables defines //////////////////////////////////////////////////////////////////////////
	const float Matrix3::EPSILON = 1e-06f;
	const Matrix3 Matrix3::ZERO(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	const Matrix3 Matrix3::IDENTITY(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	const float Matrix3::ms_fSvdEpsilon = 1e-04f;
	const unsigned int Matrix3::ms_iSvdMaxIterations = 32;


	// Get and set value of this matrix. //////////////////////////////////////////////////////////////////////////
	Vector3 Matrix3::column(size_t viCol) const{
#ifdef _DEBUG
		assert(0 <= viCol && viCol < 3);
		// add error code...
#endif
		return Vector3(m_fm[0][viCol], m_fm[1][viCol], m_fm[2][viCol]);
	}

	Vector3 Matrix3::row(size_t viRow) const{
#ifdef _DEBUG
		assert(0 <= viRow && viRow < 3);
		// add error code...
#endif
		return Vector3(m_fm[viRow][0], m_fm[viRow][1], m_fm[viRow][2]);
	}

	const float* Matrix3::arr() const{
		return m_fm[0];
	}

	void Matrix3::setColumn(size_t viCol, const Vector3& vvrv){
#ifdef _DEBUG
		assert(0 <= viCol && viCol < 3);
		// add error code...
#endif
		m_fm[0][viCol] = vvrv.x();
		m_fm[1][viCol] = vvrv.y();
		m_fm[2][viCol] = vvrv.z();
	}

	void Matrix3::setRow(size_t viRow, const Vector3& vvrv){
#ifdef _DEBUG
		assert(0 <= viRow && viRow < 3);
		// add error code...
#endif
		m_fm[viRow][0] = vvrv.x();
		m_fm[viRow][1] = vvrv.y();
		m_fm[viRow][2] = vvrv.z();
	}

	void Matrix3::fromAxes(const Vector3& vrXAxis, const Vector3& vrYAxis, const Vector3& vrZAxis){
		setColumn(0, vrXAxis);
		setColumn(1, vrYAxis);
		setColumn(2, vrZAxis);
	}

	// operator functions //////////////////////////////////////////////////////////////////////////
	bool Matrix3::operator == (const Matrix3& vmrm) const{
		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				if(!Math::RealEqual(m_fm[iRow][iCol], vmrm.m_fm[iRow][iCol]))
					return false;
			} //#### end for iCol
		} //#### end for iRow

		return true;
	}

	bool Matrix3::operator != (const Matrix3& vmrm) const{
		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				if(!Math::RealEqual(m_fm[iRow][iCol], vmrm.m_fm[iRow][iCol]))
					return true;
			} //#### end for iCol
		} //#### end for iRow

		return false;
	}

	Matrix3 Matrix3::operator + (const Matrix3& vmrm) const{
		Matrix3 mSum;

		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				mSum.m_fm[iRow][iCol] = m_fm[iRow][iCol] + vmrm.m_fm[iRow][iCol];
			} //#### end for iCol
		} //#### end for iRow

		return mSum;
	}

	Matrix3 Matrix3::operator - (const Matrix3& vmrm) const{
		Matrix3 mDiff;

		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				mDiff.m_fm[iRow][iCol] = m_fm[iRow][iCol] - vmrm.m_fm[iRow][iCol];
			} //#### end for iCol
		} //#### end for iRow

		return mDiff;
	}

	Matrix3 Matrix3::operator * (const Matrix3& vmrm) const{
		Matrix3 mProd;

		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				mProd.m_fm[iRow][iCol] = 
					m_fm[iRow][0] * vmrm.m_fm[0][iCol] + 
					m_fm[iRow][1] * vmrm.m_fm[1][iCol] +
					m_fm[iRow][2] * vmrm.m_fm[2][iCol];

			} //#### end for iCol
		} //#### end for iRow

		return mProd;
	}

	Matrix3 Matrix3::operator - () const{
		Matrix3 mNeg;

		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				mNeg.m_fm[iRow][iCol] = -m_fm[iRow][iCol];

			} //#### end for iCol
		} //#### end for iRow

		return mNeg;
	}

	// 3x3 * 3x1 = 3x1
	Vector3 Matrix3::operator * (const Vector3& vvrv) const{
		// 3x3 * 3x1 = 3x1
		Vector3 vProd;

		for(size_t iRow=0; iRow<3; iRow++){
			vProd[iRow] = 
				m_fm[iRow][0] * vvrv[0] + 
				m_fm[iRow][1] * vvrv[1] +
				m_fm[iRow][2] * vvrv[2];

		} //#### end for iRow

		return vProd;
	}

	Vector3 operator * (const Vector3& vvrv, const Matrix3& vmrm){
		// 1x3 * 3x3 = 1x3
		Vector3 vProd;

		for(size_t iCol=0; iCol<3; iCol++){
			vProd[iCol] = 
				vvrv[0] * vmrm.m_fm[0][iCol] +
				vvrv[1] * vmrm.m_fm[1][iCol] +
				vvrv[2] * vmrm.m_fm[2][iCol];

		} //#### end for iRow

		return vProd;
	}

	Matrix3 Matrix3::operator * (const float vfScalar) const{
		// matrix * scalar
		Matrix3 mProd;

		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				mProd.m_fm[iRow][iCol] =  vfScalar * m_fm[iRow][iCol];
					
			} //#### end for iCol
		} //#### end for iRow

		return mProd;
	}

	Matrix3 operator * (const float vfScalar, const Matrix3& vmrm){
		// scalar * matrix
		Matrix3 mProd;

		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				mProd.m_fm[iRow][iCol] =  vfScalar * vmrm.m_fm[iRow][iCol];

			} //#### end for iCol
		} //#### end for iRow

		return mProd;
	}

	// utilities //////////////////////////////////////////////////////////////////////////
	Matrix3 Matrix3::transpose() const{
		Matrix3 mTranspose;
		for(size_t iRow=0; iRow<3; iRow++){
			for(size_t iCol=0; iCol<3; iCol++){
				mTranspose[iRow][iCol] = m_fm[iCol][iRow];

			} //#### end for iCol
		} //#### end for iRow

		return mTranspose;
	}

	bool Matrix3::inverse(Matrix3& vmInverse, float vfTolerance /* = Math::EPSILON_E6 */) const{
		// Invert a 3x3 using cofactors.  This is about 8 times faster than
		// the Numerical Recipes code which uses Gaussian elimination.
		vmInverse[0][0] = m_fm[1][1] * m_fm[2][2] - m_fm[1][2] * m_fm[2][1];
		vmInverse[0][1] = m_fm[0][2] * m_fm[2][1] - m_fm[0][1] * m_fm[2][2];
		vmInverse[0][2] = m_fm[0][1] * m_fm[1][2] -	m_fm[0][2] * m_fm[1][1];
		vmInverse[1][0] = m_fm[1][2] * m_fm[2][0] -	m_fm[1][0] * m_fm[2][2];
		vmInverse[1][1] = m_fm[0][0] * m_fm[2][2] -	m_fm[0][2] * m_fm[2][0];
		vmInverse[1][2] = m_fm[0][2] * m_fm[1][0] -	m_fm[0][0] * m_fm[1][2];
		vmInverse[2][0] = m_fm[1][0] * m_fm[2][1] -	m_fm[1][1] * m_fm[2][0];
		vmInverse[2][1] = m_fm[0][1] * m_fm[2][0] - m_fm[0][0] * m_fm[2][1];
		vmInverse[2][2] = m_fm[0][0] * m_fm[1][1] -	m_fm[0][1] * m_fm[1][0];

		float fDet =
			m_fm[0][0] * vmInverse[0][0] +
			m_fm[0][1] * vmInverse[1][0]+
			m_fm[0][2] * vmInverse[2][0];

		if ( fabs(fDet) <= vfTolerance )
			return false;

		float fInvDet = 1.0f / fDet;
		for (size_t iRow = 0; iRow < 3; iRow++){
			for (size_t iCol = 0; iCol < 3; iCol++){
				vmInverse[iRow][iCol] *= fInvDet;

			} //#### end for iCol
		} //#### end for iRow

		return true;

	} //#### end inverse

	Matrix3 Matrix3::inverse(float vfTolerance /* = Math::EPSILON_E6 */) const{
		Matrix3 mInverse = Matrix3::ZERO;
		inverse(mInverse, vfTolerance);
		return mInverse;
	}

	float Matrix3::determinant() const{
		float fColfactor00 = m_fm[1][1] * m_fm[2][2] - m_fm[1][2] * m_fm[2][1];
		float fColfactor10 = m_fm[1][2] * m_fm[2][0] - m_fm[1][0] * m_fm[2][2];
		float fColfactor20 = m_fm[1][0] * m_fm[2][1] - m_fm[1][1] * m_fm[2][0];

		float fDet = m_fm[0][0] * fColfactor00 +
					m_fm[0][1] * fColfactor10 +
					m_fm[0][2] * fColfactor20;

		return fDet;
	}

	// support for spectral norm //////////////////////////////////////////////////////////////////////////
	void Matrix3::bidiagonalize(Matrix3& vmrA, Matrix3& vmrL, Matrix3& vmrR){
		float afV[3], afW[3];
		float fLength, fSign, fT1, fInvT1, fT2;
		bool bIdentity;

		// map first column to (*,0,0)
		fLength = sqrt(vmrA[0][0] * vmrA[0][0] + vmrA[1][0] * vmrA[1][0] + vmrA[2][0] * vmrA[2][0]);
		if(fLength > 0.0){
			fSign = (vmrA[0][0] > 0.0 ? 1.0f : -1.0f);
			fT1 = vmrA[0][0] + fSign*fLength;
			fInvT1 = 1.0f / fT1;
			afV[1] = vmrA[1][0] * fInvT1;
			afV[2] = vmrA[2][0] * fInvT1;

			fT2 = -2.0f / (1.0f + afV[1] * afV[1] + afV[2] * afV[2]);
			afW[0] = fT2 * (vmrA[0][0] + vmrA[1][0] * afV[1] + vmrA[2][0] * afV[2]);
			afW[1] = fT2 * (vmrA[0][1] + vmrA[1][1] * afV[1] + vmrA[2][1] * afV[2]);
			afW[2] = fT2 * (vmrA[0][2] + vmrA[1][2] * afV[1] + vmrA[2][2] * afV[2]);
			vmrA[0][0] += afW[0];
			vmrA[0][1] += afW[1];
			vmrA[0][2] += afW[2];
			vmrA[1][1] += afV[1] * afW[1];
			vmrA[1][2] += afV[1] * afW[2];
			vmrA[2][1] += afV[2] * afW[1];
			vmrA[2][2] += afV[2] * afW[2];

			vmrL[0][0] = 1.0f + fT2;
			vmrL[0][1] = vmrL[1][0] = fT2 * afV[1];
			vmrL[0][2] = vmrL[2][0] = fT2 * afV[2];
			vmrL[1][1] = 1.0f + fT2 * afV[1] * afV[1];
			vmrL[1][2] = vmrL[2][1] = fT2 * afV[1] * afV[2];
			vmrL[2][2] = 1.0f + fT2 * afV[2] * afV[2];
			bIdentity = false;

		}else{
			vmrL = Matrix3::IDENTITY;
			bIdentity = true;

		} //#### end if/else fLength

		// map first row to (*,*,0)
		fLength = sqrt(vmrA[0][1] * vmrA[0][1] + vmrA[0][2] * vmrA[0][2]);
		if(fLength > 0.0){
			fSign = (vmrA[0][1] > 0.0f ? 1.0f : -1.0f);
			fT1 = vmrA[0][1] + fSign*fLength;
			afV[2] = vmrA[0][2] / fT1;

			fT2 = -2.0f / (1.0f + afV[2] * afV[2]);
			afW[0] = fT2 * (vmrA[0][1] + vmrA[0][2] * afV[2]);
			afW[1] = fT2 * (vmrA[1][1] + vmrA[1][2] * afV[2]);
			afW[2] = fT2 * (vmrA[2][1] + vmrA[2][2] * afV[2]);
			vmrA[0][1] += afW[0];
			vmrA[1][1] += afW[1];
			vmrA[1][2] += afW[1] * afV[2];
			vmrA[2][1] += afW[2];
			vmrA[2][2] += afW[2] * afV[2];

			vmrR[0][0] = 1.0f;
			vmrR[0][1] = vmrR[1][0] = 0.0f;
			vmrR[0][2] = vmrR[2][0] = 0.0f;
			vmrR[1][1] = 1.0f + fT2;
			vmrR[1][2] = vmrR[2][1] = fT2 * afV[2];
			vmrR[2][2] = 1.0f + fT2 * afV[2] * afV[2];

		}else{
			vmrR = Matrix3::IDENTITY;

		} //#### end if/else fLength

		// map second column to (*,*,0)
		fLength = sqrt(vmrA[1][1] * vmrA[1][1] + vmrA[2][1] * vmrA[2][1]);
		if(fLength > 0.0){
			fSign = (vmrA[1][1] > 0.0f ? 1.0f : -1.0f);
			fT1 = vmrA[1][1] + fSign * fLength;
			afV[2] = vmrA[2][1] / fT1;

			fT2 = -2.0f / (1.0f + afV[2] * afV[2]);
			afW[1] = fT2 * (vmrA[1][1] + vmrA[2][1] * afV[2]);
			afW[2] = fT2 * (vmrA[1][2] + vmrA[2][2] * afV[2]);
			vmrA[1][1] += afW[1];
			vmrA[1][2] += afW[2];
			vmrA[2][2] += afV[2] * afW[2];

			float fA = 1.0f + fT2;
			float fB = fT2 * afV[2];
			float fC = 1.0f + fB * afV[2];

			if(bIdentity){
				vmrL[0][0] = 1.0f;
				vmrL[0][1] = vmrL[1][0] = 0.0f;
				vmrL[0][2] = vmrL[2][0] = 0.0f;
				vmrL[1][1] = fA;
				vmrL[1][2] = vmrL[2][1] = fB;
				vmrL[2][2] = fC;

			}else{
				for (int iRow = 0; iRow < 3; iRow++){
					float fTmp0 = vmrL[iRow][1];
					float fTmp1 = vmrL[iRow][2];
					vmrL[iRow][1] = fA * fTmp0 + fB*fTmp1;
					vmrL[iRow][2] = fB * fTmp0 + fC * fTmp1;

				} //#### end for iRow
			} //#### end for if/else bIdentity
		} //#### end if fLength
	} //#### end static Matrix3::bidiagonalize

	void Matrix3::golubKahanStep(Matrix3& vmrA, Matrix3& vmrL, Matrix3& vmrR){
		float fT11 = vmrA[0][1]*vmrA[0][1]+vmrA[1][1]*vmrA[1][1];
		float fT22 = vmrA[1][2]*vmrA[1][2]+vmrA[2][2]*vmrA[2][2];
		float fT12 = vmrA[1][1]*vmrA[1][2];
		float fTrace = fT11+fT22;
		float fDiff = fT11-fT22;
		float fDiscr = sqrt(fDiff*fDiff+4.0f*fT12*fT12);
		float fRoot1 = 0.5f*(fTrace+fDiscr);
		float fRoot2 = 0.5f*(fTrace-fDiscr);

		// adjust right
		float fY = vmrA[0][0] - (fabs(fRoot1-fT22) <=
			fabs(fRoot2-fT22) ? fRoot1 : fRoot2);
		float fZ = vmrA[0][1];
		float fInvLength = Math::InvSqrt(fY*fY+fZ*fZ);
		float fSin = fZ*fInvLength;
		float fCos = -fY*fInvLength;

		float fTmp0 = vmrA[0][0];
		float fTmp1 = vmrA[0][1];
		vmrA[0][0] = fCos*fTmp0-fSin*fTmp1;
		vmrA[0][1] = fSin*fTmp0+fCos*fTmp1;
		vmrA[1][0] = -fSin*vmrA[1][1];
		vmrA[1][1] *= fCos;

		size_t iRow;
		for (iRow = 0; iRow < 3; iRow++){
			fTmp0 = vmrR[0][iRow];
			fTmp1 = vmrR[1][iRow];
			vmrR[0][iRow] = fCos*fTmp0-fSin*fTmp1;
			vmrR[1][iRow] = fSin*fTmp0+fCos*fTmp1;
		}

		// adjust left
		fY = vmrA[0][0];
		fZ = vmrA[1][0];
		fInvLength = Math::InvSqrt(fY*fY+fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		vmrA[0][0] = fCos*vmrA[0][0]-fSin*vmrA[1][0];
		fTmp0 = vmrA[0][1];
		fTmp1 = vmrA[1][1];
		vmrA[0][1] = fCos*fTmp0-fSin*fTmp1;
		vmrA[1][1] = fSin*fTmp0+fCos*fTmp1;
		vmrA[0][2] = -fSin*vmrA[1][2];
		vmrA[1][2] *= fCos;

		size_t iCol;
		for (iCol = 0; iCol < 3; iCol++){
			fTmp0 = vmrL[iCol][0];
			fTmp1 = vmrL[iCol][1];
			vmrL[iCol][0] = fCos*fTmp0-fSin*fTmp1;
			vmrL[iCol][1] = fSin*fTmp0+fCos*fTmp1;
		}

		// adjust right
		fY = vmrA[0][1];
		fZ = vmrA[0][2];
		fInvLength = Math::InvSqrt(fY*fY+fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		vmrA[0][1] = fCos*vmrA[0][1]-fSin*vmrA[0][2];
		fTmp0 = vmrA[1][1];
		fTmp1 = vmrA[1][2];
		vmrA[1][1] = fCos*fTmp0-fSin*fTmp1;
		vmrA[1][2] = fSin*fTmp0+fCos*fTmp1;
		vmrA[2][1] = -fSin*vmrA[2][2];
		vmrA[2][2] *= fCos;

		for (iRow = 0; iRow < 3; iRow++){
			fTmp0 = vmrR[1][iRow];
			fTmp1 = vmrR[2][iRow];
			vmrR[1][iRow] = fCos*fTmp0-fSin*fTmp1;
			vmrR[2][iRow] = fSin*fTmp0+fCos*fTmp1;
		}

		// adjust left
		fY = vmrA[1][1];
		fZ = vmrA[2][1];
		fInvLength = Math::InvSqrt(fY*fY+fZ*fZ);
		fSin = fZ*fInvLength;
		fCos = -fY*fInvLength;

		vmrA[1][1] = fCos*vmrA[1][1]-fSin*vmrA[2][1];
		fTmp0 = vmrA[1][2];
		fTmp1 = vmrA[2][2];
		vmrA[1][2] = fCos*fTmp0-fSin*fTmp1;
		vmrA[2][2] = fSin*fTmp0+fCos*fTmp1;

		for (iCol = 0; iCol < 3; iCol++){
			fTmp0 = vmrL[iCol][1];
			fTmp1 = vmrL[iCol][2];
			vmrL[iCol][1] = fCos*fTmp0-fSin*fTmp1;
			vmrL[iCol][2] = fSin*fTmp0+fCos*fTmp1;
		}
	} //#### end static Matrix3::golubKahanStep

	void Matrix3::singularValueDecomposition(Matrix3& vmrL, Vector3& vvrS, Matrix3& vmrR) const{
		// temas: currently unused
		//const int iMax = 16;
		size_t iRow, iCol;

		Matrix3 kA = *this;
		bidiagonalize(kA,vmrL,vmrR);

		for (unsigned int i = 0; i < ms_iSvdMaxIterations; i++)
		{
			float fTmp, fTmp0, fTmp1;
			float fSin0, fCos0, fTan0;
			float fSin1, fCos1, fTan1;

			bool bTest1 = (fabs(kA[0][1]) <=
				ms_fSvdEpsilon*(fabs(kA[0][0])+fabs(kA[1][1])));
			bool bTest2 = (fabs(kA[1][2]) <=
				ms_fSvdEpsilon*(fabs(kA[1][1])+fabs(kA[2][2])));
			if (bTest1){
				if (bTest2){
					vvrS[0] = kA[0][0];
					vvrS[1] = kA[1][1];
					vvrS[2] = kA[2][2];
					break;
				}else{
					// 2x2 closed form factorization
					fTmp = (kA[1][1]*kA[1][1] - kA[2][2]*kA[2][2] +
						kA[1][2]*kA[1][2])/(kA[1][2]*kA[2][2]);
					fTan0 = 0.5f*(fTmp+sqrt(fTmp*fTmp + 4.0f));
					fCos0 = Math::InvSqrt(1.0f+fTan0*fTan0);
					fSin0 = fTan0*fCos0;

					for(iCol = 0; iCol < 3; iCol++){
						fTmp0 = vmrL[iCol][1];
						fTmp1 = vmrL[iCol][2];
						vmrL[iCol][1] = fCos0*fTmp0-fSin0*fTmp1;
						vmrL[iCol][2] = fSin0*fTmp0+fCos0*fTmp1;
					}

					fTan1 = (kA[1][2]-kA[2][2]*fTan0)/kA[1][1];
					fCos1 = Math::InvSqrt(1.0f+fTan1*fTan1);
					fSin1 = -fTan1*fCos1;

					for(iRow = 0; iRow < 3; iRow++){
						fTmp0 = vmrR[1][iRow];
						fTmp1 = vmrR[2][iRow];
						vmrR[1][iRow] = fCos1*fTmp0-fSin1*fTmp1;
						vmrR[2][iRow] = fSin1*fTmp0+fCos1*fTmp1;
					}

					vvrS[0] = kA[0][0];
					vvrS[1] = fCos0*fCos1*kA[1][1] -
						fSin1*(fCos0*kA[1][2]-fSin0*kA[2][2]);
					vvrS[2] = fSin0*fSin1*kA[1][1] +
						fCos1*(fSin0*kA[1][2]+fCos0*kA[2][2]);
					break;
				}
			}else{
				if(bTest2){
					// 2x2 closed form factorization
					fTmp = (kA[0][0]*kA[0][0] + kA[1][1]*kA[1][1] -
						kA[0][1]*kA[0][1])/(kA[0][1]*kA[1][1]);
					fTan0 = 0.5f*(-fTmp+sqrt(fTmp*fTmp + 4.0f));
					fCos0 = Math::InvSqrt(1.0f+fTan0*fTan0);
					fSin0 = fTan0*fCos0;

					for(iCol = 0; iCol < 3; iCol++){
						fTmp0 = vmrL[iCol][0];
						fTmp1 = vmrL[iCol][1];
						vmrL[iCol][0] = fCos0*fTmp0-fSin0*fTmp1;
						vmrL[iCol][1] = fSin0*fTmp0+fCos0*fTmp1;
					}

					fTan1 = (kA[0][1]-kA[1][1]*fTan0)/kA[0][0];
					fCos1 = Math::InvSqrt(1.0f+fTan1*fTan1);
					fSin1 = -fTan1*fCos1;

					for(iRow = 0; iRow < 3; iRow++){
						fTmp0 = vmrR[0][iRow];
						fTmp1 = vmrR[1][iRow];
						vmrR[0][iRow] = fCos1*fTmp0-fSin1*fTmp1;
						vmrR[1][iRow] = fSin1*fTmp0+fCos1*fTmp1;
					}

					vvrS[0] = fCos0*fCos1*kA[0][0] -
						fSin1*(fCos0*kA[0][1]-fSin0*kA[1][1]);
					vvrS[1] = fSin0*fSin1*kA[0][0] +
						fCos1*(fSin0*kA[0][1]+fCos0*kA[1][1]);
					vvrS[2] = kA[2][2];
					break;
				}else{
					golubKahanStep(kA,vmrL,vmrR);
				}
			}
		}

		// positize diagonal
		for(iRow = 0; iRow < 3; iRow++){
			if(vvrS[iRow] < 0.0){
				vvrS[iRow] = -vvrS[iRow];
				for (iCol = 0; iCol < 3; iCol++)
					vmrR[iRow][iCol] = -vmrR[iRow][iCol];
			}
		}

	} //#### end singularValueDecomposition

	void Matrix3::singularValueComposition(const Matrix3& vmrL, Vector3& vvrS, Matrix3& vmrR){
		size_t iRow, iCol;
		Matrix3 kTmp;

		// product S*R
		for(iRow = 0; iRow < 3; iRow++){
			for (iCol = 0; iCol < 3; iCol++)
				kTmp[iRow][iCol] = vvrS[iRow]*vmrR[iRow][iCol];
		}

		// product L*S*R
		for(iRow = 0; iRow < 3; iRow++){
			for(iCol = 0; iCol < 3; iCol++){
				m_fm[iRow][iCol] = 0.0;
				for(int iMid = 0; iMid < 3; iMid++)
					m_fm[iRow][iCol] += vmrL[iRow][iMid]*kTmp[iMid][iCol];
			}
		}

	} //#### end singularValueComposition

	void Matrix3::orthonormalize(){
		// Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
		// M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// where |V| indicates length of vector V and A*B indicates dot
		// product of vectors A and B.

		// compute q0
		float fInvLength = Math::InvSqrt(m_fm[0][0] * m_fm[0][0] +
										m_fm[1][0] * m_fm[1][0] +
										m_fm[2][0] * m_fm[2][0]);

		m_fm[0][0] *= fInvLength;
		m_fm[1][0] *= fInvLength;
		m_fm[2][0] *= fInvLength;

		// compute q1
		float fDot0 =
			m_fm[0][0] * m_fm[0][1] +
			m_fm[1][0] * m_fm[1][1] +
			m_fm[2][0] * m_fm[2][1];

		m_fm[0][1] -= fDot0 * m_fm[0][0];
		m_fm[1][1] -= fDot0 * m_fm[1][0];
		m_fm[2][1] -= fDot0 * m_fm[2][0];

		fInvLength = Math::InvSqrt(m_fm[0][1] * m_fm[0][1] +
								m_fm[1][1] * m_fm[1][1] +
								m_fm[2][1] * m_fm[2][1]);

		m_fm[0][1] *= fInvLength;
		m_fm[1][1] *= fInvLength;
		m_fm[2][1] *= fInvLength;

		// compute q2
		float fDot1 =
			m_fm[0][1] * m_fm[0][2] +
			m_fm[1][1] * m_fm[1][2] +
			m_fm[2][1] * m_fm[2][2];

		fDot0 =
			m_fm[0][0] * m_fm[0][2] +
			m_fm[1][0] * m_fm[1][2] +
			m_fm[2][0] * m_fm[2][2];

		m_fm[0][2] -= fDot0 * m_fm[0][0] + fDot1 * m_fm[0][1];
		m_fm[1][2] -= fDot0 * m_fm[1][0] + fDot1 * m_fm[1][1];
		m_fm[2][2] -= fDot0 * m_fm[2][0] + fDot1 * m_fm[2][1];

		fInvLength = Math::InvSqrt(m_fm[0][2] * m_fm[0][2] +
								m_fm[1][2] * m_fm[1][2] +
								m_fm[2][2] * m_fm[2][2]);

		m_fm[0][2] *= fInvLength;
		m_fm[1][2] *= fInvLength;
		m_fm[2][2] *= fInvLength;

	} //#### end orthonormalize

	void Matrix3::qduDecomposition(Matrix3& vmrQ, Vector3& vvrD, Vector3& vvrU) const{
		// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
		// and U is upper triangular with ones on its diagonal.  Algorithm uses
		// Gram-Schmidt orthogonalization (the QR algorithm).
		//
		// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
		//
		//   q0 = m0/|m0|
		//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
		//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
		//
		// where |V| indicates length of vector V and A*B indicates dot
		// product of vectors A and B.  The matrix R has entries
		//
		//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
		//   r10 = 0      r11 = q1*m1  r12 = q1*m2
		//   r20 = 0      r21 = 0      r22 = q2*m2
		//
		// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
		// u02 = r02/r00, and u12 = r12/r11.

		// Q = rotation
		// D = scaling
		// U = shear

		// D stores the three diagonal entries r00, r11, r22
		// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12

		// build orthogonal matrix Q
		float fInvLength = Math::InvSqrt(m_fm[0][0] * m_fm[0][0] +
										m_fm[1][0] * m_fm[1][0] +
										m_fm[2][0] * m_fm[2][0]);
		vmrQ[0][0] = m_fm[0][0] * fInvLength;
		vmrQ[1][0] = m_fm[1][0] * fInvLength;
		vmrQ[2][0] = m_fm[2][0] * fInvLength;

		float fDot = vmrQ[0][0] * m_fm[0][1] + 
					vmrQ[1][0] * m_fm[1][1] +
					vmrQ[2][0] * m_fm[2][1];
		vmrQ[0][1] = m_fm[0][1] - fDot * vmrQ[0][0];
		vmrQ[1][1] = m_fm[1][1] - fDot * vmrQ[1][0];
		vmrQ[2][1] = m_fm[2][1] - fDot * vmrQ[2][0];
		fInvLength = Math::InvSqrt(vmrQ[0][1] * vmrQ[0][1] +
								vmrQ[1][1] * vmrQ[1][1] +
								vmrQ[2][1] * vmrQ[2][1]);
		vmrQ[0][1] *= fInvLength;
		vmrQ[1][1] *= fInvLength;
		vmrQ[2][1] *= fInvLength;

		fDot = vmrQ[0][0] * m_fm[0][2] +
			vmrQ[1][0] * m_fm[1][2] +
			vmrQ[2][0] * m_fm[2][2];
		vmrQ[0][2] = m_fm[0][2] - fDot * vmrQ[0][0];
		vmrQ[1][2] = m_fm[1][2] - fDot * vmrQ[1][0];
		vmrQ[2][2] = m_fm[2][2] - fDot * vmrQ[2][0];
		fDot = vmrQ[0][1] * m_fm[0][2] + 
			vmrQ[1][1] * m_fm[1][2] +
			vmrQ[2][1] * m_fm[2][2];
		vmrQ[0][2] -= fDot * vmrQ[0][1];
		vmrQ[1][2] -= fDot * vmrQ[1][1];
		vmrQ[2][2] -= fDot * vmrQ[2][1];
		fInvLength = Math::InvSqrt(vmrQ[0][2] * vmrQ[0][2] + 
								vmrQ[1][2] * vmrQ[1][2] +
								vmrQ[2][2] * vmrQ[2][2]);
		vmrQ[0][2] *= fInvLength;
		vmrQ[1][2] *= fInvLength;
		vmrQ[2][2] *= fInvLength;

		// guarantee that orthogonal matrix has determinant 1 (no reflections)
		float fDet = vmrQ[0][0] * vmrQ[1][1] * vmrQ[2][2] + vmrQ[0][1] * vmrQ[1][2]*vmrQ[2][0] +
			vmrQ[0][2] * vmrQ[1][0] * vmrQ[2][1] - vmrQ[0][2] * vmrQ[1][1] * vmrQ[2][0] -
			vmrQ[0][1] * vmrQ[1][0] * vmrQ[2][2] - vmrQ[0][0] * vmrQ[1][2] * vmrQ[2][1];

		if(fDet < 0.0){
			for(size_t iRow = 0; iRow < 3; iRow++)
				for (size_t iCol = 0; iCol < 3; iCol++)
					vmrQ[iRow][iCol] = -vmrQ[iRow][iCol];
		}

		// build "right" matrix R
		Matrix3 kR;
		kR[0][0] = vmrQ[0][0] * m_fm[0][0] + 
				vmrQ[1][0] * m_fm[1][0] +
				vmrQ[2][0] * m_fm[2][0];
		kR[0][1] = vmrQ[0][0] * m_fm[0][1] + 
				vmrQ[1][0] * m_fm[1][1] +
				vmrQ[2][0] * m_fm[2][1];
		kR[1][1] = vmrQ[0][1] * m_fm[0][1] + 
				vmrQ[1][1] * m_fm[1][1] +
				vmrQ[2][1] * m_fm[2][1];
		kR[0][2] = vmrQ[0][0] * m_fm[0][2] + 
				vmrQ[1][0] * m_fm[1][2] +
				vmrQ[2][0] * m_fm[2][2];
		kR[1][2] = vmrQ[0][1] * m_fm[0][2] + 
				vmrQ[1][1] * m_fm[1][2] +
				vmrQ[2][1] * m_fm[2][2];
		kR[2][2] = vmrQ[0][2] * m_fm[0][2] + 
				vmrQ[1][2] * m_fm[1][2] +
				vmrQ[2][2] * m_fm[2][2];

		// the scaling component
		vvrD[0] = kR[0][0];
		vvrD[1] = kR[1][1];
		vvrD[2] = kR[2][2];

		// the shear component
		float fInvD0 = 1.0f / vvrD[0];
		vvrU[0] = kR[0][1] * fInvD0;
		vvrU[1] = kR[0][2] * fInvD0;
		vvrU[2] = kR[1][2] / vvrD[1];

	} //#### end qduDecomposition


	// support for spectral norm //////////////////////////////////////////////////////////////////////////
	float Matrix3::maxCubicRoot(float vfaCoeff[3]){
		// Spectral norm is for A^T*A, so characteristic polynomial
		// P(x) = c[0]+c[1]*x+c[2]*x^2+x^3 has three positive real roots.
		// This yields the assertions c[0] < 0 and c[2]*c[2] >= 3*c[1].

		// quick out for uniform scale (triple root)
		const float fOneThird = 1.0f / 3.0f;
		const float fEpsilon = 1e-06f;
		float fDiscr = vfaCoeff[2]*vfaCoeff[2] - 3.0f*vfaCoeff[1];
		if (fDiscr <= fEpsilon)
			return -fOneThird*vfaCoeff[2];

		// Compute an upper bound on roots of P(x).  This assumes that A^T*A
		// has been scaled by its largest entry.
		float fX = 1.0f;
		float fPoly = vfaCoeff[0]+fX*(vfaCoeff[1]+fX*(vfaCoeff[2]+fX));
		if(fPoly < 0.0){
			// uses a matrix norm to find an upper bound on maximum root
			fX = fabs(vfaCoeff[0]);
			float fTmp = 1.0f+fabs(vfaCoeff[1]);
			if(fTmp > fX)
				fX = fTmp;
			fTmp = 1.0f+fabs(vfaCoeff[2]);
			if(fTmp > fX)
				fX = fTmp;
		}

		// Newton's method to find root
		float fTwoC2 = 2.0f*vfaCoeff[2];
		for(int i = 0; i < 16; i++){
			fPoly = vfaCoeff[0]+fX*(vfaCoeff[1]+fX*(vfaCoeff[2]+fX));
			if(fabs(fPoly) <= fEpsilon)
				return fX;

			float fDeriv = vfaCoeff[1]+fX*(fTwoC2+3.0f*fX);
			fX -= fPoly/fDeriv;
		}

		return fX;

	} //#### end maxCubicRoot

	float Matrix3::spectralNorm() const{
		Matrix3 kP;
		size_t iRow, iCol;
		float fPmax = 0.0;
		for(iRow = 0; iRow < 3; iRow++){
			for(iCol = 0; iCol < 3; iCol++){
				kP[iRow][iCol] = 0.0;
				for(int iMid = 0; iMid < 3; iMid++){
					kP[iRow][iCol] += m_fm[iMid][iRow] * m_fm[iMid][iCol];
				}
				if(kP[iRow][iCol] > fPmax)
					fPmax = kP[iRow][iCol];
			}
		}

		float fInvPmax = 1.0f / fPmax;
		for(iRow = 0; iRow < 3; iRow++){
			for(iCol = 0; iCol < 3; iCol++)
				kP[iRow][iCol] *= fInvPmax;
		}

		float afCoeff[3];
		afCoeff[0] = -(kP[0][0]*(kP[1][1]*kP[2][2]-kP[1][2]*kP[2][1]) +
			kP[0][1]*(kP[2][0]*kP[1][2]-kP[1][0]*kP[2][2]) +
			kP[0][2]*(kP[1][0]*kP[2][1]-kP[2][0]*kP[1][1]));
		afCoeff[1] = kP[0][0]*kP[1][1]-kP[0][1]*kP[1][0] +
			kP[0][0]*kP[2][2]-kP[0][2]*kP[2][0] +
			kP[1][1]*kP[2][2]-kP[1][2]*kP[2][1];
		afCoeff[2] = -(kP[0][0]+kP[1][1]+kP[2][2]);

		float fRoot = maxCubicRoot(afCoeff);
		float fNorm = sqrt(fPmax*fRoot);

		return fNorm;

	} //#### end spectralNorm

	void Matrix3::toAxisAngleR(Vector3& vvrAxis, float& vfrAngle) const{
		// Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
		// The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
		// I is the identity and
		//
		//       +-        -+
		//   P = |  0 -z +y |
		//       | +z  0 -x |
		//       | -y +x  0 |
		//       +-        -+
		//
		// If A > 0, R represents a counterclockwise rotation about the axis in
		// the sense of looking from the tip of the axis vector towards the
		// origin.  Some algebra will show that
		//
		//   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
		//
		// In the event that A = pi, R-R^t = 0 which prevents us from extracting
		// the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
		// P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
		// z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
		// it does not matter which sign you choose on the square roots.

		float fTrace = m_fm[0][0] + m_fm[1][1] + m_fm[2][2];
		float fCos = 0.5f * (fTrace - 1.0f);
		vfrAngle = Math::ACos(fCos);  // in [0,PI]

		if(vfrAngle > 0.0){
			if(vfrAngle < Math::PI){
				vvrAxis.setX(m_fm[2][1] - m_fm[1][2]);
				vvrAxis.setY(m_fm[0][2] - m_fm[2][0]);
				vvrAxis.setZ(m_fm[1][0] - m_fm[0][1]);
				vvrAxis.normalise();
			}else{
				// angle is PI
				float fHalfInverse;
				if(m_fm[0][0] >= m_fm[1][1]){
					// r00 >= r11
					if(m_fm[0][0] >= m_fm[2][2]){
						// r00 is maximum diagonal term
						vvrAxis.setX(0.5f * Math::Sqrt(m_fm[0][0] - m_fm[1][1] - m_fm[2][2] + 1.0f));
						fHalfInverse = 0.5f / vvrAxis.x();
						vvrAxis.setY(fHalfInverse * m_fm[0][1]);
						vvrAxis.setZ(fHalfInverse * m_fm[0][2]);
					}else{
						// r22 is maximum diagonal term
						vvrAxis.setZ(0.5f * Math::Sqrt(m_fm[2][2] - m_fm[0][0] - m_fm[1][1] + 1.0f));
						fHalfInverse = 0.5f / vvrAxis.z();
						vvrAxis.setX(fHalfInverse * m_fm[0][2]);
						vvrAxis.setY(fHalfInverse * m_fm[1][2]);
					}
				}else{
					// r11 > r00
					if(m_fm[1][1] >= m_fm[2][2]){
						// r11 is maximum diagonal term
						vvrAxis.setY(0.5f * Math::Sqrt(m_fm[1][1] - m_fm[0][0] - m_fm[2][2] + 1.0f));
						fHalfInverse  = 0.5f / vvrAxis.y();
						vvrAxis.setX(fHalfInverse * m_fm[0][1]);
						vvrAxis.setZ(fHalfInverse * m_fm[1][2]);
					}else{
						// r22 is maximum diagonal term
						vvrAxis.setZ(0.5f * Math::Sqrt(m_fm[2][2] - m_fm[0][0] - m_fm[1][1] + 1.0f));
						fHalfInverse = 0.5f / vvrAxis.z();
						vvrAxis.setX(fHalfInverse * m_fm[0][2]);
						vvrAxis.setY(fHalfInverse * m_fm[1][2]);
					}
				}
			}
		}else{
			// The angle is 0 and the matrix is the identity.  Any axis will
			// work, so just use the x-axis.
			vvrAxis.setX(1.0f);
			vvrAxis.setY(0.0f);
			vvrAxis.setZ(0.0f);
		}

	} //#### end toAxisAngleR

	void Matrix3::fromAxisAngle(const Vector3& vvrAxis, const float vfrAngle){
		float fCos = Math::Cos(vfrAngle);
		float fSin = Math::Sin(vfrAngle);
		float fOneMinusCos = 1.0f - fCos;
		float fX2 = vvrAxis.x() * vvrAxis.x();
		float fY2 = vvrAxis.y() * vvrAxis.y();
		float fZ2 = vvrAxis.z() * vvrAxis.z();
		float fXYM = vvrAxis.x() * vvrAxis.y() * fOneMinusCos;
		float fXZM = vvrAxis.x() * vvrAxis.z() * fOneMinusCos;
		float fYZM = vvrAxis.y() * vvrAxis.z() * fOneMinusCos;
		float fXSin = vvrAxis.x() * fSin;
		float fYSin = vvrAxis.y() * fSin;
		float fZSin = vvrAxis.z() * fSin;

		m_fm[0][0] = fX2 * fOneMinusCos + fCos;
		m_fm[0][1] = fXYM - fZSin;
		m_fm[0][2] = fXZM + fYSin;
		m_fm[1][0] = fXYM + fZSin;
		m_fm[1][1] = fY2 * fOneMinusCos + fCos;
		m_fm[1][2] = fYZM - fXSin;
		m_fm[2][0] = fXZM - fYSin;
		m_fm[2][1] = fYZM + fXSin;
		m_fm[2][2] = fZ2 * fOneMinusCos + fCos;

	} //#### fromAxisAngle

	bool Matrix3::toEulerAnglesXYZ(float &vfrYAngle, float &vfrPAngle, float &vfrRAngle) const{
		// rot =  cy*cz          -cy*sz           sy
		//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
		//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

		vfrPAngle = Math::ASin(m_fm[0][2]);
		if(vfrPAngle < Math::PI_DIV_2){
			if( vfrPAngle > -Math::PI_DIV_2){
				vfrYAngle = Math::ATan2(-m_fm[1][2], m_fm[2][2]);
				vfrRAngle = Math::ATan2(-m_fm[0][1], m_fm[0][0]);
				return true;
			}else{
				// WARNING.  Not a unique solution.
				float fRmY = Math::ATan2(m_fm[1][0], m_fm[1][1]);
				vfrRAngle = 0.0;  // any angle works
				vfrYAngle = vfrRAngle - fRmY;
				return false;
			}
		}else{
			// WARNING.  Not a unique solution.
			float fRpY = Math::ATan2(m_fm[1][0], m_fm[1][1]);
			vfrRAngle = 0.0;  // any angle works
			vfrYAngle = fRpY - vfrRAngle;
			return false;
		}

	} //#### end toEulerAnglesXYZ

	bool Matrix3::toEulerAnglesXZY(float &vfrYAngle, float &vfrPAngle, float &vfrRAngle) const{
		// rot =  cy*cz          -sz              cz*sy
		//        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
		//       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

		vfrPAngle = Math::ASin(-m_fm[0][1]);
		if(vfrPAngle < Math::PI_DIV_2){
			if( vfrPAngle > -Math::PI_DIV_2){
				vfrYAngle = Math::ATan2(m_fm[2][1], m_fm[1][1]);
				vfrRAngle = Math::ATan2(m_fm[0][2], m_fm[0][0]);
				return true;
			}else{
				// WARNING.  Not a unique solution.
				float fRmY = Math::ATan2(-m_fm[2][0], m_fm[2][2]);
				vfrRAngle = 0.0;  // any angle works
				vfrYAngle = vfrRAngle - fRmY;
				return false;
			}
		}else{
			// WARNING.  Not a unique solution.
			float fRpY = Math::ATan2(-m_fm[2][0], m_fm[2][2]);
			vfrRAngle = 0.0;  // any angle works
			vfrYAngle = fRpY - vfrRAngle;
			return false;
		}

	} //#### end toEulerAnglesXZY

	bool Matrix3::toEulerAnglesYXZ(float &vfrYAngle, float &vfrPAngle, float &vfrRAngle) const{
		// rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
		//        cx*sz           cx*cz          -sx
		//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

		vfrPAngle = Math::ASin(-m_fm[1][2]);
		if(vfrPAngle < Math::PI_DIV_2){
			if( vfrPAngle > -Math::PI_DIV_2){
				vfrYAngle = Math::ATan2(m_fm[0][2], m_fm[2][2]);
				vfrRAngle = Math::ATan2(m_fm[1][0], m_fm[1][1]);
				return true;
			}else{
				// WARNING.  Not a unique solution.
				float fRmY = Math::ATan2(-m_fm[0][1], m_fm[0][0]);
				vfrRAngle = 0.0;  // any angle works
				vfrYAngle = vfrRAngle - fRmY;
				return false;
			}
		}else{
			// WARNING.  Not a unique solution.
			float fRpY = Math::ATan2(-m_fm[0][1], m_fm[0][0]);
			vfrRAngle = 0.0;  // any angle works
			vfrYAngle = fRpY - vfrRAngle;
			return false;
		}

	} //#### end toEulerAnglesYXZ

	bool Matrix3::toEulerAnglesYZX(float &vfrYAngle, float &vfrPAngle, float &vfrRAngle) const{
		// rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
		//        sz              cx*cz          -cz*sx
		//       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

		vfrPAngle = Math::ASin(m_fm[1][0]);
		if(vfrPAngle < Math::PI_DIV_2){
			if( vfrPAngle > -Math::PI_DIV_2){
				vfrYAngle = Math::ATan2(-m_fm[2][0], m_fm[0][0]);
				vfrRAngle = Math::ATan2(-m_fm[1][2], m_fm[1][1]);
				return true;
			}else{
				// WARNING.  Not a unique solution.
				float fRmY = Math::ATan2(m_fm[2][1], m_fm[2][2]);
				vfrRAngle = 0.0;  // any angle works
				vfrYAngle = vfrRAngle - fRmY;
				return false;
			}
		}else{
			// WARNING.  Not a unique solution.
			float fRpY = Math::ATan2(m_fm[2][1], m_fm[2][2]);
			vfrRAngle = 0.0;  // any angle works
			vfrYAngle = fRpY - vfrRAngle;
			return false;
		}

	} //#### end toEulerAnglesYZX

	bool Matrix3::toEulerAnglesZXY(float &vfrYAngle, float &vfrPAngle, float &vfrRAngle) const{
		// rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
		//        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
		//       -cx*sy           sx              cx*cy

		vfrPAngle = Math::ASin(m_fm[2][1]);
		if(vfrPAngle < Math::PI_DIV_2){
			if( vfrPAngle > -Math::PI_DIV_2){
				vfrYAngle = Math::ATan2(-m_fm[0][1], m_fm[1][1]);
				vfrRAngle = Math::ATan2(-m_fm[2][0], m_fm[2][2]);
				return true;
			}else{
				// WARNING.  Not a unique solution.
				float fRmY = Math::ATan2(m_fm[0][2], m_fm[0][0]);
				vfrRAngle = 0.0;  // any angle works
				vfrYAngle = vfrRAngle - fRmY;
				return false;
			}
		}else{
			// WARNING.  Not a unique solution.
			float fRpY = Math::ATan2(m_fm[0][2], m_fm[0][0]);
			vfrRAngle = 0.0;  // any angle works
			vfrYAngle = fRpY - vfrRAngle;
			return false;
		}

	} //#### end toEulerAnglesZXY

	bool Matrix3::toEulerAnglesZYX(float &vfrYAngle, float &vfrPAngle, float &vfrRAngle) const{
		// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
		//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
		//       -sy              cy*sx           cx*cy

		vfrPAngle = Math::ASin(-m_fm[2][0]);
		if(vfrPAngle < Math::PI_DIV_2){
			if( vfrPAngle > -Math::PI_DIV_2){
				vfrYAngle = Math::ATan2(m_fm[1][0], m_fm[0][0]);
				vfrRAngle = Math::ATan2(m_fm[2][1], m_fm[2][2]);
				return true;
			}else{
				// WARNING.  Not a unique solution.
				float fRmY = Math::ATan2(-m_fm[0][1], m_fm[0][2]);
				vfrRAngle = 0.0;  // any angle works
				vfrYAngle = vfrRAngle - fRmY;
				return false;
			}
		}else{
			// WARNING.  Not a unique solution.
			float fRpY = Math::ATan2(-m_fm[0][1], m_fm[0][2]);
			vfrRAngle = 0.0;  // any angle works
			vfrYAngle = fRpY - vfrRAngle;
			return false;
		}

	} //#### end toEulerAnglesZYX

	void Matrix3::fromEulerAnglesXYZ(const float &vfrYAngle, const float &vfrPAngle, const float &vfrRAngle){
		float fCos, fSin;

		fCos = Math::Cos(vfrYAngle);
		fSin = Math::Sin(vfrYAngle);
		Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = Math::Cos(vfrPAngle);
		fSin = Math::Sin(vfrPAngle);
		Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = Math::Cos(vfrRAngle);
		fSin = Math::Sin(vfrRAngle);
		Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		*this = kXMat*(kYMat*kZMat);

	} //#### end fromEulerAnglesXYZ

	void Matrix3::fromEulerAnglesXZY(const float &vfrYAngle, const float &vfrPAngle, const float &vfrRAngle){
		float fCos, fSin;

		fCos = Math::Cos(vfrYAngle);
		fSin = Math::Sin(vfrYAngle);
		Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = Math::Cos(vfrPAngle);
		fSin = Math::Sin(vfrPAngle);
		Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = Math::Cos(vfrRAngle);
		fSin = Math::Sin(vfrRAngle);
		Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		*this = kXMat*(kZMat*kYMat);

	} //#### end fromEulerAnglesXZY

	void Matrix3::fromEulerAnglesYXZ(const float &vfrYAngle, const float &vfrPAngle, const float &vfrRAngle){
		float fCos, fSin;

		fCos = Math::Cos(vfrYAngle);
		fSin = Math::Sin(vfrYAngle);
		Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = Math::Cos(vfrPAngle);
		fSin = Math::Sin(vfrPAngle);
		Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = Math::Cos(vfrRAngle);
		fSin = Math::Sin(vfrRAngle);
		Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		*this = kYMat*(kXMat*kZMat);

	} //#### end fromEulerAnglesYXZ

	void Matrix3::fromEulerAnglesYZX(const float &vfrYAngle, const float &vfrPAngle, const float &vfrRAngle){
		float fCos, fSin;

		fCos = Math::Cos(vfrYAngle);
		fSin = Math::Sin(vfrYAngle);
		Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = Math::Cos(vfrPAngle);
		fSin = Math::Sin(vfrPAngle);
		Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = Math::Cos(vfrRAngle);
		fSin = Math::Sin(vfrRAngle);
		Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		*this = kYMat*(kZMat*kXMat);

	} //#### end fromEulerAnglesYZX

	void Matrix3::fromEulerAnglesZXY(const float &vfrYAngle, const float &vfrPAngle, const float &vfrRAngle){
		float fCos, fSin;

		fCos = Math::Cos(vfrYAngle);
		fSin = Math::Sin(vfrYAngle);
		Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = Math::Cos(vfrPAngle);
		fSin = Math::Sin(vfrPAngle);
		Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		fCos = Math::Cos(vfrRAngle);
		fSin = Math::Sin(vfrRAngle);
		Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		*this = kZMat*(kXMat*kYMat);

	} //#### end fromEulerAnglesZXY

	void Matrix3::fromEulerAnglesZYX(const float &vfrYAngle, const float &vfrPAngle, const float &vfrRAngle){
		float fCos, fSin;

		fCos = Math::Cos(vfrYAngle);
		fSin = Math::Sin(vfrYAngle);
		Matrix3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

		fCos = Math::Cos(vfrPAngle);
		fSin = Math::Sin(vfrPAngle);
		Matrix3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

		fCos = Math::Cos(vfrRAngle);
		fSin = Math::Sin(vfrRAngle);
		Matrix3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

		*this = kZMat*(kYMat*kXMat);

	} //#### end fromEulerAnglesZYX

	// support for eigensolver //////////////////////////////////////////////////////////////////////////
	void Matrix3::_tridiagonal(float vfDiag[3], float vfSubDiag[3]){
		// Householder reduction T = Q^t M Q
		//   Input:
		//     mat, symmetric 3x3 matrix M
		//   Output:
		//     mat, orthogonal matrix Q
		//     diag, diagonal entries of T
		//     subd, subdiagonal entries of T (T is symmetric)

		float fA = m_fm[0][0];
		float fB = m_fm[0][1];
		float fC = m_fm[0][2];
		float fD = m_fm[1][1];
		float fE = m_fm[1][2];
		float fF = m_fm[2][2];

		vfDiag[0] = fA;
		vfSubDiag[2] = 0.0;
		if( Math::Abs(fC) >= EPSILON ){
			float fLength = Math::Sqrt(fB*fB+fC*fC);
			float fInvLength = 1.0f / fLength;
			fB *= fInvLength;
			fC *= fInvLength;
			float fQ = 2.0f * fB * fE + fC * (fF - fD);
			vfDiag[1] = fD+fC*fQ;
			vfDiag[2] = fF-fC*fQ;
			vfSubDiag[0] = fLength;
			vfSubDiag[1] = fE-fB*fQ;
			m_fm[0][0] = 1.0f;
			m_fm[0][1] = 0.0;
			m_fm[0][2] = 0.0;
			m_fm[1][0] = 0.0;
			m_fm[1][1] = fB;
			m_fm[1][2] = fC;
			m_fm[2][0] = 0.0;
			m_fm[2][1] = fC;
			m_fm[2][2] = -fB;
		}else{
			vfDiag[1] = fD;
			vfDiag[2] = fF;
			vfSubDiag[0] = fB;
			vfSubDiag[1] = fE;
			m_fm[0][0] = 1.0f;
			m_fm[0][1] = 0.0;
			m_fm[0][2] = 0.0;
			m_fm[1][0] = 0.0;
			m_fm[1][1] = 1.0f;
			m_fm[1][2] = 0.0;
			m_fm[2][0] = 0.0;
			m_fm[2][1] = 0.0;
			m_fm[2][2] = 1.0f;
		}

	} //#### end _tridiagonal

	bool Matrix3::_qlAlgorithm(float vfDiag[3], float vfSubDiag[3]){
		// QL iteration with implicit shifting to reduce matrix from tridiagonal
		// to diagonal

		for(int i0 = 0; i0 < 3; i0++){
			const unsigned int iMaxIter = 32;
			unsigned int iIter;
			for (iIter = 0; iIter < iMaxIter; iIter++){
				int i1;
				for(i1 = i0; i1 <= 1; i1++){
					float fSum = Math::Abs(vfDiag[i1]) +
						Math::Abs(vfDiag[i1+1]);
					if ( Math::Abs(vfSubDiag[i1]) + fSum == fSum )
						break;
				}
				if( i1 == i0 )
					break;

				float fTmp0 = (vfDiag[i0+1]-vfDiag[i0])/(2.0f*vfSubDiag[i0]);
				float fTmp1 = Math::Sqrt(fTmp0*fTmp0+1.0f);
				if ( fTmp0 < 0.0 )
					fTmp0 = vfDiag[i1]-vfDiag[i0]+vfSubDiag[i0]/(fTmp0-fTmp1);
				else
					fTmp0 = vfDiag[i1]-vfDiag[i0]+vfSubDiag[i0]/(fTmp0+fTmp1);
				float fSin = 1.0f;
				float fCos = 1.0f;
				float fTmp2 = 0.0;
				for (int i2 = i1-1; i2 >= i0; i2--){
					float fTmp3 = fSin*vfSubDiag[i2];
					float fTmp4 = fCos*vfSubDiag[i2];
					if( Math::Abs(fTmp3) >= Math::Abs(fTmp0) ){
						fCos = fTmp0/fTmp3;
						fTmp1 = Math::Sqrt(fCos*fCos+1.0f);
						vfSubDiag[i2+1] = fTmp3*fTmp1;
						fSin = 1.0f/fTmp1;
						fCos *= fSin;
					}else{
						fSin = fTmp3/fTmp0;
						fTmp1 = Math::Sqrt(fSin*fSin+1.0f);
						vfSubDiag[i2+1] = fTmp0*fTmp1;
						fCos = 1.0f/fTmp1;
						fSin *= fCos;
					}
					fTmp0 = vfDiag[i2+1]-fTmp2;
					fTmp1 = (vfDiag[i2]-fTmp0)*fSin+2.0f*fTmp4*fCos;
					fTmp2 = fSin*fTmp1;
					vfDiag[i2+1] = fTmp0+fTmp2;
					fTmp0 = fCos*fTmp1-fTmp4;

					for(int iRow = 0; iRow < 3; iRow++){
						fTmp3 = m_fm[iRow][i2+1];
						m_fm[iRow][i2+1] = fSin*m_fm[iRow][i2] + fCos*fTmp3;
						m_fm[iRow][i2] = fCos*m_fm[iRow][i2] - fSin*fTmp3;
					}
				}
				vfDiag[i0] -= fTmp2;
				vfSubDiag[i0] = fTmp0;
				vfSubDiag[i1] = 0.0;
			}

			if ( iIter == iMaxIter ){
				// should not get here under normal circumstances
				return false;
			}
		}

		return true;

	} //#### end _qlAlgorithm

	void Matrix3::eigenSolveSymmetric(float vfEigenvalue[3], Vector3 vvEigenvector[3]) const{
		Matrix3 kMatrix = *this;
		float afSubDiag[3];
		kMatrix._tridiagonal(vfEigenvalue,afSubDiag);
		kMatrix._qlAlgorithm(vfEigenvalue,afSubDiag);

		for (size_t i = 0; i < 3; i++)
		{
			vvEigenvector[i][0] = kMatrix[0][i];
			vvEigenvector[i][1] = kMatrix[1][i];
			vvEigenvector[i][2] = kMatrix[2][i];
		}

		// make eigenvectors form a right--handed system
		Vector3 kCross = vvEigenvector[1].crossProduct(vvEigenvector[2]);
		float fDet = vvEigenvector[0].dotProduct(kCross);
		if ( fDet < 0.0 )
		{
			vvEigenvector[2][0] = - vvEigenvector[2][0];
			vvEigenvector[2][1] = - vvEigenvector[2][1];
			vvEigenvector[2][2] = - vvEigenvector[2][2];
		}

	} //#### end eigenSolveSymmetric

	void Matrix3::tensorProduct(const Vector3 &vvrU, const Vector3 &vvrV, Matrix3 &vmrProduct){
		for(size_t iRow = 0; iRow < 3; iRow++){
			for(size_t iCol = 0; iCol < 3; iCol++)
				vmrProduct[iRow][iCol] = vvrU[iRow]*vvrV[iCol];
		}

	} //#### end tensorProduct

}