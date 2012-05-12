//////////////////////////////////////////////////////
// file: sgMatrix.h @ 2008-1-20 by Zhang Xiang
// declares of the class Matrix3
// Matrix3 is a class ...
//////////////////////////////////////////////////////
#ifndef __SGMATRIX3_H__
#define __SGMATRIX3_H__

// INCLUDES //////////////////////////////////////////
#include "sgVector3.h"
#include "sgMath.h"

// NB All code adapted from Wild Magic 0.2 Matrix math (free source code)
// http://www.geometrictools.com/

// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** A 3x3 matrix which can represent rotations around axes.
		@note
			<b>All the code is adapted from the Wild Magic 0.2 Matrix
            library (http://www.geometrictools.com/).</b>
		@par
			The coordinate system is assumed to be <b>right-handed</b>.
	*/
	class _SG_MathExport Matrix3{
	// member variables
	private:
		union{
			Real m_fm[3][3];	// indexed by [row][col]
			Real _m_fm[9];
			
			struct{
				Real _m_fm00, _m_fm01, _m_fm02;
				Real _m_fm10, _m_fm11, _m_fm12;
				Real _m_fm20, _m_fm21, _m_fm22;
			};
		};

	// constructors & destructor
	public:
		inline Matrix3(void){}
		inline explicit Matrix3(const Real vfam[3][3]){
			memcpy(m_fm, vfam, 9 * sizeof(Real));
		}
		inline explicit Matrix3(const Real vfam[9]){
			memcpy(_m_fm, vfam, 9 * sizeof(Real));
		}
		inline explicit Matrix3(const Real vf00, const Real vf01, const Real vf02,
								const Real vf10, const Real vf11, const Real vf12,
								const Real vf20, const Real vf21, const Real vf22){
			_m_fm00 = vf00; _m_fm01 = vf01; _m_fm02 = vf02;
			_m_fm10 = vf10; _m_fm11 = vf11; _m_fm12 = vf12;
			_m_fm20 = vf20; _m_fm21 = vf21; _m_fm22 = vf22;
		}
		inline Matrix3(const Matrix3& vmrm){
			memcpy(m_fm, vmrm.m_fm, 9 * sizeof(Real));
		}

	// member functions
	public:
		// Get and set value of this matrix.
		Vector3 column(size_t viCol) const;
		Vector3 row(size_t viRow) const;
		const Real* arr(void) const;
		void setColumn(size_t viCol, const Vector3& vvrv);
		void setRow(size_t viRow, const Vector3& vvrv);
		void fromAxes(const Vector3& vrXAxis, const Vector3& vrYAxis, const Vector3& vrZAxis);
		inline Real* operator [] (size_t viRow) const{
			return (Real*)m_fm[viRow];
		}

		// operator functions
	public:
		inline Matrix3& operator = (const Matrix3& vmrm){
			memcpy(m_fm, vmrm.m_fm, 9 * sizeof(Real));

			return *this;
		}

		bool operator == (const Matrix3& vmrm) const;
		bool operator != (const Matrix3& vmrm) const;

		// arithmetic operations
		Matrix3 operator + (const Matrix3& vmrm) const;
		Matrix3 operator - (const Matrix3& vmrm) const;
		Matrix3 operator * (const Matrix3& vmrm) const;
		Matrix3 operator - () const;
		inline const Matrix3& operator + () const{
			return *this;
		}

		/// matrix * vector (3x3 * 3x1 = 3x1)
		Vector3 operator * (const Vector3& vvrv) const;

		/// vector * Matrix (1x3 * 3x3 = 1x3)
		_SG_MathExport friend Vector3 operator * (const Vector3& vvrv, const Matrix3& vmrm);

		/// matrix * scalar
		Matrix3 operator * (const Real vfScalar) const;

		/// scalar * matrix
		_SG_MathExport friend Matrix3 operator * (const Real vfScalar, const Matrix3& vmrm);

		// utilities
		Matrix3 transpose() const;
		bool inverse(Matrix3& vmInverse, Real vfTolerance = Math::EPSILON_E6) const;
		Matrix3 inverse(Real vfTolerance = Math::EPSILON_E6) const;
		/// ÷»
		Real determinant() const;

		// singular value decomposition
		void singularValueDecomposition(Matrix3& vmrL, Vector3& vvrS, Matrix3& vmrR) const;
		void singularValueComposition(const Matrix3& vmrL, Vector3& vvrS, Matrix3& vmrR);

		// Gram-Schmidt orhtonormalization(applied to columns of rotation matrix)
		void orthonormalize();

		// orthogonal Q, diagonal D, upper triangular U stroed as(u01, u02, u12)
		void qduDecomposition(Matrix3& vmrQ, Vector3& vvrD, Vector3& vvrU) const;
		Real spectralNorm() const;

		// matrix must be orthonormal
		/** Generate a Vector3 of rotate axis and an radian angle from a Matrix3
			@param vvrAxis the rotate axis derived from this matrix
			@param vfrAngle the radian angle derived from this matrix
		*/
		void toAxisAngleR(Vector3& vvrAxis, Real& vfrAngle) const;

		/** Generate a Vector3 of rotate axis and an degree angle from a Matrix3
			@param vvrAxis the rotate axis derived from this matrix
			@param vfrAngle the degree angle derived from this matrix
		*/
		void toAxisAngleD(Vector3& vvrAxis, Real& vfdAngle) const{
			Real fRAngle;
			toAxisAngleR(vvrAxis, fRAngle);
			vfdAngle = Math::RadianToDegree(fRAngle);
		}

		void fromAxisAngle(const Vector3& vvrAxis, const Real vfrAngle);

		// The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
		// where yaw is rotation about the Up vector, pitch is rotation about the
		// Right axis, and roll is rotation about the Direction axis.
		bool toEulerAnglesXYZ(Real &aRXAngle, Real &aRYAngle, Real &aRZAngle) const;
		bool toEulerAnglesXZY(Real &aRXAngle, Real &aRZAngle, Real &aRYAngle) const;
		bool toEulerAnglesYXZ(Real &aRYAngle, Real &aRXAngle, Real &aRZAngle) const;
		bool toEulerAnglesYZX(Real &aRYAngle, Real &aRZAngle, Real &aRXAngle) const;
		bool toEulerAnglesZXY(Real &aRZAngle, Real &aRXAngle, Real &aRYAngle) const;
		bool toEulerAnglesZYX(Real &aRZAngle, Real &aRYAngle, Real &aRXAngle) const;

		void fromEulerAnglesXYZ(const Real &aRXAngle, const Real &aRYAngle, const Real &aRZAngle);
		void fromEulerAnglesXZY(const Real &aRXAngle, const Real &aRZAngle, const Real &aRYAngle);
		void fromEulerAnglesYXZ(const Real &aRYAngle, const Real &aRXAngle, const Real &aRZAngle);
		void fromEulerAnglesYZX(const Real &aRYAngle, const Real &aRZAngle, const Real &aRXAngle);
		void fromEulerAnglesZXY(const Real &aRZAngle, const Real &aRXAngle, const Real &aRYAngle);
		void fromEulerAnglesZYX(const Real &aRZAngle, const Real &aRYAngle, const Real &aRXAngle);

		// eigensolver, matrix must be symmetric
		void eigenSolveSymmetric(Real vfEigenvalue[3], Vector3 vvEigenvector[3]) const;

		static void tensorProduct(const Vector3& vvrU, const Vector3& vvrV, Matrix3& vmrProduct);

		static const Real EPSILON;
		static const Matrix3 ZERO;
		static const Matrix3 IDENTITY;

	private:
		// support for eigensolver
		void _tridiagonal(Real vfDiag[3], Real vfSubDiag[3]);
		bool _qlAlgorithm(Real vfDiag[3], Real vfSubDiag[3]);

		// support for singular value decomposition
		static const Real ms_fSvdEpsilon;
		static const unsigned int ms_iSvdMaxIterations;
		static void bidiagonalize(Matrix3& vmrA, Matrix3& vmrL, Matrix3& vmrR);
		static void golubKahanStep(Matrix3& vmrA, Matrix3& vmrL, Matrix3& vmrR);

		// support for spectral norm
		static Real maxCubicRoot(Real vfaCoeff[3]);

		// for faster access
		friend class Matrix4;

	}; //#### end class SMenu

}

// DEFINES ///////////////////////////////////////////

#endif // __SGMATRIX3_H__
