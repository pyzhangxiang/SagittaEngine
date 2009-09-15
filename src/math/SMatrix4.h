//////////////////////////////////////////////////////
// file: SMatrix.h @ 2008-1-30 by Zhang Xiang
// declares of the class Matrix4
// Matrix4 is a class ...
//////////////////////////////////////////////////////
#ifndef __SMATRIX4_H__
#define __SMATRIX4_H__

// INCLUDES //////////////////////////////////////////
#include "SMatrix3.h"
#include "SQuaternion.h"
#include "SVector3.h"
#include "SVector4.h"
#include "SPlane.h"
#include "SMath.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class Matrix3;

	/** Class encapsulating a standard 4x4 homogenous matrix.
        @remarks
            OGRE uses column vectors when applying matrix multiplications,
            This means a vector is represented as a single column, 4-row
            matrix. This has the effect that the tranformations implemented
            by the matrices happens right-to-left e.g. if vector V is to be
            transformed by M1 then M2 then M3, the calculation would be
            M3 * M2 * M1 * V. The order that matrices are concatenated is
            vital since matrix multiplication is not cummatative, i.e. you
            can get a different result if you concatenate in the wrong order.
        @par
            The use of column vectors and right-to-left ordering is the
            standard in most mathematical texts, and id the same as used in
            OpenGL. It is, however, the opposite of Direct3D, which has
            inexplicably chosen to differ from the accepted standard and uses
            row vectors and left-to-right matrix multiplication.
        @par
            OGRE deals with the differences between D3D and OpenGL etc.
            internally when operating through different render systems. OGRE
            users only need to conform to standard maths conventions, i.e.
            right-to-left matrix multiplication, (OGRE transposes matrices it
            passes to D3D to compensate).
        @par
            The generic form M * V which shows the layout of the matrix 
            entries is shown below:
            <pre>
                [ m[0][0]  m[0][1]  m[0][2]  m[0][3] ]   {x}
                | m[1][0]  m[1][1]  m[1][2]  m[1][3] | * {y}
                | m[2][0]  m[2][1]  m[2][2]  m[2][3] |   {z}
                [ m[3][0]  m[3][1]  m[3][2]  m[3][3] ]   {1}
            </pre>
    */
	class _MathExport Matrix4{
	// member variables
	private:
		/// The matrix entries, indexed by [row][col].
		union{
			Real m_fm[4][4];
			Real _m_fm[16];

			struct{
				Real _m_fm00, _m_fm01, _m_fm02, _m_fm03;
				Real _m_fm10, _m_fm11, _m_fm12, _m_fm13;
				Real _m_fm20, _m_fm21, _m_fm22, _m_fm23;
				Real _m_fm30, _m_fm31, _m_fm32, _m_fm33;
			};
		};

	public:
		static const Matrix4 ZERO;
		static const Matrix4 IDENTITY;
		/** Useful little matrix which takes 2D clipspace {-1, 1} to {0,1}
			and inverts the Y. */
		static const Matrix4 CLIPSPACE2DTOIMAGESPACE;

	// constructors & destructor
	public:
		inline Matrix4(){

		}

		inline Matrix4(
					Real m00, Real m01, Real m02, Real m03,
					Real m10, Real m11, Real m12, Real m13,
					Real m20, Real m21, Real m22, Real m23,
					Real m30, Real m31, Real m32, Real m33 ){
			m_fm[0][0] = m00;
			m_fm[0][1] = m01;
			m_fm[0][2] = m02;
			m_fm[0][3] = m03;
			m_fm[1][0] = m10;
			m_fm[1][1] = m11;
			m_fm[1][2] = m12;
			m_fm[1][3] = m13;
			m_fm[2][0] = m20;
			m_fm[2][1] = m21;
			m_fm[2][2] = m22;
			m_fm[2][3] = m23;
			m_fm[3][0] = m30;
			m_fm[3][1] = m31;
			m_fm[3][2] = m32;
			m_fm[3][3] = m33;
		}

		inline Matrix4(const Matrix3& m3x3){
			operator=(Matrix4::IDENTITY);
			operator=(m3x3);
		}

		/** Creates a standard 4x4 transformation matrix with a zero translation part from a rotation/scaling Quaternion.
		*/
		inline Matrix4(const Quaternion& rot){
			Matrix3 m3x3;
			rot.toRotationMatrix(m3x3);
			operator=(Matrix4::IDENTITY);
			operator=(m3x3);
		}

	// member functions
	public:
		inline Real* operator [] (size_t iRow){
#ifdef _DEBUG
			assert(iRow < 4);
			// add error code...
#endif
			return m_fm[iRow];

		}

		inline const Real *const operator [] (size_t iRow) const{
#ifdef _DEBUG
			assert(iRow < 4);
			// add error code...
#endif
			return m_fm[iRow];
		}

		inline const Real* arr() const{
			return _m_fm;
		}

		inline void setValues(
							Real m00, Real m01, Real m02, Real m03,
							Real m10, Real m11, Real m12, Real m13,
							Real m20, Real m21, Real m22, Real m23,
							Real m30, Real m31, Real m32, Real m33 ){
				m_fm[0][0] = m00;
				m_fm[0][1] = m01;
				m_fm[0][2] = m02;
				m_fm[0][3] = m03;
				m_fm[1][0] = m10;
				m_fm[1][1] = m11;
				m_fm[1][2] = m12;
				m_fm[1][3] = m13;
				m_fm[2][0] = m20;
				m_fm[2][1] = m21;
				m_fm[2][2] = m22;
				m_fm[2][3] = m23;
				m_fm[3][0] = m30;
				m_fm[3][1] = m31;
				m_fm[3][2] = m32;
				m_fm[3][3] = m33;
		}

		inline Matrix4 concatenate(const Matrix4& m2) const{
			Matrix4 r;
			r.m_fm[0][0] = m_fm[0][0] * m2.m_fm[0][0] + m_fm[0][1] * m2.m_fm[1][0] + m_fm[0][2] * m2.m_fm[2][0] + m_fm[0][3] * m2.m_fm[3][0];
			r.m_fm[0][1] = m_fm[0][0] * m2.m_fm[0][1] + m_fm[0][1] * m2.m_fm[1][1] + m_fm[0][2] * m2.m_fm[2][1] + m_fm[0][3] * m2.m_fm[3][1];
			r.m_fm[0][2] = m_fm[0][0] * m2.m_fm[0][2] + m_fm[0][1] * m2.m_fm[1][2] + m_fm[0][2] * m2.m_fm[2][2] + m_fm[0][3] * m2.m_fm[3][2];
			r.m_fm[0][3] = m_fm[0][0] * m2.m_fm[0][3] + m_fm[0][1] * m2.m_fm[1][3] + m_fm[0][2] * m2.m_fm[2][3] + m_fm[0][3] * m2.m_fm[3][3];

			r.m_fm[1][0] = m_fm[1][0] * m2.m_fm[0][0] + m_fm[1][1] * m2.m_fm[1][0] + m_fm[1][2] * m2.m_fm[2][0] + m_fm[1][3] * m2.m_fm[3][0];
			r.m_fm[1][1] = m_fm[1][0] * m2.m_fm[0][1] + m_fm[1][1] * m2.m_fm[1][1] + m_fm[1][2] * m2.m_fm[2][1] + m_fm[1][3] * m2.m_fm[3][1];
			r.m_fm[1][2] = m_fm[1][0] * m2.m_fm[0][2] + m_fm[1][1] * m2.m_fm[1][2] + m_fm[1][2] * m2.m_fm[2][2] + m_fm[1][3] * m2.m_fm[3][2];
			r.m_fm[1][3] = m_fm[1][0] * m2.m_fm[0][3] + m_fm[1][1] * m2.m_fm[1][3] + m_fm[1][2] * m2.m_fm[2][3] + m_fm[1][3] * m2.m_fm[3][3];

			r.m_fm[2][0] = m_fm[2][0] * m2.m_fm[0][0] + m_fm[2][1] * m2.m_fm[1][0] + m_fm[2][2] * m2.m_fm[2][0] + m_fm[2][3] * m2.m_fm[3][0];
			r.m_fm[2][1] = m_fm[2][0] * m2.m_fm[0][1] + m_fm[2][1] * m2.m_fm[1][1] + m_fm[2][2] * m2.m_fm[2][1] + m_fm[2][3] * m2.m_fm[3][1];
			r.m_fm[2][2] = m_fm[2][0] * m2.m_fm[0][2] + m_fm[2][1] * m2.m_fm[1][2] + m_fm[2][2] * m2.m_fm[2][2] + m_fm[2][3] * m2.m_fm[3][2];
			r.m_fm[2][3] = m_fm[2][0] * m2.m_fm[0][3] + m_fm[2][1] * m2.m_fm[1][3] + m_fm[2][2] * m2.m_fm[2][3] + m_fm[2][3] * m2.m_fm[3][3];

			r.m_fm[3][0] = m_fm[3][0] * m2.m_fm[0][0] + m_fm[3][1] * m2.m_fm[1][0] + m_fm[3][2] * m2.m_fm[2][0] + m_fm[3][3] * m2.m_fm[3][0];
			r.m_fm[3][1] = m_fm[3][0] * m2.m_fm[0][1] + m_fm[3][1] * m2.m_fm[1][1] + m_fm[3][2] * m2.m_fm[2][1] + m_fm[3][3] * m2.m_fm[3][1];
			r.m_fm[3][2] = m_fm[3][0] * m2.m_fm[0][2] + m_fm[3][1] * m2.m_fm[1][2] + m_fm[3][2] * m2.m_fm[2][2] + m_fm[3][3] * m2.m_fm[3][2];
			r.m_fm[3][3] = m_fm[3][0] * m2.m_fm[0][3] + m_fm[3][1] * m2.m_fm[1][3] + m_fm[3][2] * m2.m_fm[2][3] + m_fm[3][3] * m2.m_fm[3][3];

			return r;
		}

		/** Matrix concatenation using '*'. */
		inline Matrix4 operator * (const Matrix4& m2) const{
			return concatenate(m2);
		}

		/** Vector transformation using '*'.
            @remarks
                Transforms the given 3-D vector by the matrix, projecting the 
                result back into <i>w</i> = 1.
            @note
                This means that the initial <i>w</i> is considered to be 1.0,
                and then all the tree elements of the resulting 3-D vector are
                divided by the resulting <i>w</i>.
        */
		inline Vector3 operator * (const Vector3& v) const{
			Vector3 r;

			Real fInvW = 1.0f / ( m_fm[3][0] * v.x() + m_fm[3][1] * v.y() + m_fm[3][2] * v.z() + m_fm[3][3] );

			r.setX((m_fm[0][0] * v.x() + m_fm[0][1] * v.y() + m_fm[0][2] * v.z() + m_fm[0][3]) * fInvW);
			r.setY((m_fm[1][0] * v.x() + m_fm[1][1] * v.y() + m_fm[1][2] * v.z() + m_fm[1][3]) * fInvW);
			r.setZ((m_fm[2][0] * v.x() + m_fm[2][1] * v.y() + m_fm[2][2] * v.z() + m_fm[2][3]) * fInvW);

			return r;
		}

		inline Vector4 operator * (const Vector4& v) const{
			return Vector4(
				m_fm[0][0] * v.x() + m_fm[0][1] * v.y() + m_fm[0][2] * v.z() + m_fm[0][3] * v.w(), 
				m_fm[1][0] * v.x() + m_fm[1][1] * v.y() + m_fm[1][2] * v.z() + m_fm[1][3] * v.w(),
				m_fm[2][0] * v.x() + m_fm[2][1] * v.y() + m_fm[2][2] * v.z() + m_fm[2][3] * v.w(),
				m_fm[3][0] * v.x() + m_fm[3][1] * v.y() + m_fm[3][2] * v.z() + m_fm[3][3] * v.w());
		}

		inline Plane operator * (const Plane& p) const{
			// maybe a bug...
			Plane ret;
			Matrix4 invTrans = inverse().transpose();
			Vector4 v4(p.normal().x(), p.normal().y(), p.normal().z(), p.distance());
			v4 = invTrans * v4;
			Vector3 v3(v4.x(), v4.y(), v4.z());
			ret.setNormal(v3);
			ret.setDistance(v4.w() / v3.normalise());
			// maybe a bug... ret.setDisatance(v4.w() / ret.norm().normalise()) from ogre
			ret.setNormal(v3);

			return ret;
		}

		/** matrix addition */
		inline Matrix4 operator + (const Matrix4& m2) const{
			Matrix4 r;

			r.m_fm[0][0] = m_fm[0][0] + m2.m_fm[0][0];
			r.m_fm[0][1] = m_fm[0][1] + m2.m_fm[0][1];
			r.m_fm[0][2] = m_fm[0][2] + m2.m_fm[0][2];
			r.m_fm[0][3] = m_fm[0][3] + m2.m_fm[0][3];

			r.m_fm[1][0] = m_fm[1][0] + m2.m_fm[1][0];
			r.m_fm[1][1] = m_fm[1][1] + m2.m_fm[1][1];
			r.m_fm[1][2] = m_fm[1][2] + m2.m_fm[1][2];
			r.m_fm[1][3] = m_fm[1][3] + m2.m_fm[1][3];

			r.m_fm[2][0] = m_fm[2][0] + m2.m_fm[2][0];
			r.m_fm[2][1] = m_fm[2][1] + m2.m_fm[2][1];
			r.m_fm[2][2] = m_fm[2][2] + m2.m_fm[2][2];
			r.m_fm[2][3] = m_fm[2][3] + m2.m_fm[2][3];

			r.m_fm[3][0] = m_fm[3][0] + m2.m_fm[3][0];
			r.m_fm[3][1] = m_fm[3][1] + m2.m_fm[3][1];
			r.m_fm[3][2] = m_fm[3][2] + m2.m_fm[3][2];
			r.m_fm[3][3] = m_fm[3][3] + m2.m_fm[3][3];

			return r;
		}

		/** matrix subtraction */
		inline Matrix4 operator - (const Matrix4& m2) const{
			Matrix4 r;

			r.m_fm[0][0] = m_fm[0][0] - m2.m_fm[0][0];
			r.m_fm[0][1] = m_fm[0][1] - m2.m_fm[0][1];
			r.m_fm[0][2] = m_fm[0][2] - m2.m_fm[0][2];
			r.m_fm[0][3] = m_fm[0][3] - m2.m_fm[0][3];

			r.m_fm[1][0] = m_fm[1][0] - m2.m_fm[1][0];
			r.m_fm[1][1] = m_fm[1][1] - m2.m_fm[1][1];
			r.m_fm[1][2] = m_fm[1][2] - m2.m_fm[1][2];
			r.m_fm[1][3] = m_fm[1][3] - m2.m_fm[1][3];

			r.m_fm[2][0] = m_fm[2][0] - m2.m_fm[2][0];
			r.m_fm[2][1] = m_fm[2][1] - m2.m_fm[2][1];
			r.m_fm[2][2] = m_fm[2][2] - m2.m_fm[2][2];
			r.m_fm[2][3] = m_fm[2][3] - m2.m_fm[2][3];

			r.m_fm[3][0] = m_fm[3][0] - m2.m_fm[3][0];
			r.m_fm[3][1] = m_fm[3][1] - m2.m_fm[3][1];
			r.m_fm[3][2] = m_fm[3][2] - m2.m_fm[3][2];
			r.m_fm[3][3] = m_fm[3][3] - m2.m_fm[3][3];

			return r;
		}

		/** Tests 2 matrices for equality. */
		inline bool operator == (const Matrix4& m2) const{
			/*if( 
				m_fm[0][0] != m2.m_fm[0][0] || m_fm[0][1] != m2.m_fm[0][1] || m_fm[0][2] != m2.m_fm[0][2] || m_fm[0][3] != m2.m_fm[0][3] ||
				m_fm[1][0] != m2.m_fm[1][0] || m_fm[1][1] != m2.m_fm[1][1] || m_fm[1][2] != m2.m_fm[1][2] || m_fm[1][3] != m2.m_fm[1][3] ||
				m_fm[2][0] != m2.m_fm[2][0] || m_fm[2][1] != m2.m_fm[2][1] || m_fm[2][2] != m2.m_fm[2][2] || m_fm[2][3] != m2.m_fm[2][3] ||
				m_fm[3][0] != m2.m_fm[3][0] || m_fm[3][1] != m2.m_fm[3][1] || m_fm[3][2] != m2.m_fm[3][2] || m_fm[3][3] != m2.m_fm[3][3] )
				return false;*/
			for(size_t iRow=0; iRow<4; iRow++){
				for(size_t iCol=0; iCol<4; iCol++){
					if(!Math::RealEqual(m_fm[iRow][iCol], m2.m_fm[iRow][iCol]))
						return false;
				} //#### end for iCol
			} //#### end for iRow
			return true;
		}

		/** Tests 2 matrices for inequality. */
		inline bool operator != (const Matrix4& m2) const{
			return !(*this == m2);
		}

		/** Assignment from 3x3 matrix. */
		inline void operator = (const Matrix3& mat3){
			m_fm[0][0] = mat3.m_fm[0][0]; m_fm[0][1] = mat3.m_fm[0][1]; m_fm[0][2] = mat3.m_fm[0][2];
			m_fm[1][0] = mat3.m_fm[1][0]; m_fm[1][1] = mat3.m_fm[1][1]; m_fm[1][2] = mat3.m_fm[1][2];
			m_fm[2][0] = mat3.m_fm[2][0]; m_fm[2][1] = mat3.m_fm[2][1]; m_fm[2][2] = mat3.m_fm[2][2];
		}

		inline Matrix4 transpose(void) const{
			return Matrix4(m_fm[0][0], m_fm[1][0], m_fm[2][0], m_fm[3][0],
						m_fm[0][1], m_fm[1][1], m_fm[2][1], m_fm[3][1],
						m_fm[0][2], m_fm[1][2], m_fm[2][2], m_fm[3][2],
						m_fm[0][3], m_fm[1][3], m_fm[2][3], m_fm[3][3]);
		}

		// Translation Transformation //////////////////////////////////////////////////////////////////////////
		/** Sets the translation transformation part of the matrix. */
		inline void setTrans(const Vector3& v){
			m_fm[0][3] = v.x();
			m_fm[1][3] = v.y();
			m_fm[2][3] = v.z();
		}

		/** Extracts the translation transformation part of the matrix. */
		inline Vector3 getTrans() const{
			return Vector3(m_fm[0][3], m_fm[1][3], m_fm[2][3]);
		}

		/** Builds a translation matrix. */
		inline void makeTrans(const Vector3& v){
			m_fm[0][0] = 1.0; m_fm[0][1] = 0.0; m_fm[0][2] = 0.0; m_fm[0][3] = v.x();
			m_fm[1][0] = 0.0; m_fm[1][1] = 1.0; m_fm[1][2] = 0.0; m_fm[1][3] = v.y();
			m_fm[2][0] = 0.0; m_fm[2][1] = 0.0; m_fm[2][2] = 1.0; m_fm[2][3] = v.z();
			m_fm[3][0] = 0.0; m_fm[3][1] = 0.0; m_fm[3][2] = 0.0; m_fm[3][3] = 1.0;
		}

		inline void makeTrans(Real tx, Real ty, Real tz){
			m_fm[0][0] = 1.0; m_fm[0][1] = 0.0; m_fm[0][2] = 0.0; m_fm[0][3] = tx;
			m_fm[1][0] = 0.0; m_fm[1][1] = 1.0; m_fm[1][2] = 0.0; m_fm[1][3] = ty;
			m_fm[2][0] = 0.0; m_fm[2][1] = 0.0; m_fm[2][2] = 1.0; m_fm[2][3] = tz;
			m_fm[3][0] = 0.0; m_fm[3][1] = 0.0; m_fm[3][2] = 0.0; m_fm[3][3] = 1.0;
		}

		/** Gets a translation matrix. */
		inline static Matrix4 getTrans(const Vector3& v){
			Matrix4 r;

			r.m_fm[0][0] = 1.0; r.m_fm[0][1] = 0.0; r.m_fm[0][2] = 0.0; r.m_fm[0][3] = v.x();
			r.m_fm[1][0] = 0.0; r.m_fm[1][1] = 1.0; r.m_fm[1][2] = 0.0; r.m_fm[1][3] = v.y();
			r.m_fm[2][0] = 0.0; r.m_fm[2][1] = 0.0; r.m_fm[2][2] = 1.0; r.m_fm[2][3] = v.z();
			r.m_fm[3][0] = 0.0; r.m_fm[3][1] = 0.0; r.m_fm[3][2] = 0.0; r.m_fm[3][3] = 1.0;

			return r;
		}

		/** Gets a translation matrix - variation for not using a vector. */
		inline static Matrix4 getTrans(Real tx, Real ty, Real tz){
			Matrix4 r;

			r.m_fm[0][0] = 1.0; r.m_fm[0][1] = 0.0; r.m_fm[0][2] = 0.0; r.m_fm[0][3] = tx;
			r.m_fm[1][0] = 0.0; r.m_fm[1][1] = 1.0; r.m_fm[1][2] = 0.0; r.m_fm[1][3] = ty;
			r.m_fm[2][0] = 0.0; r.m_fm[2][1] = 0.0; r.m_fm[2][2] = 1.0; r.m_fm[2][3] = tz;
			r.m_fm[3][0] = 0.0; r.m_fm[3][1] = 0.0; r.m_fm[3][2] = 0.0; r.m_fm[3][3] = 1.0;

			return r;
		}

		// Scale Transformation //////////////////////////////////////////////////////////////////////////
		/** Sets the scale part of the matrix. */
		inline void setScale(const Vector3& v){
			m_fm[0][0] = v.x();
			m_fm[1][1] = v.y();
			m_fm[2][2] = v.z();
		}

		/** Gets a scale matrix. */
		inline static Matrix4 getScale(const Vector3& v){
			Matrix4 r;

			r.m_fm[0][0] = v.x(); r.m_fm[0][1] = 0.0; r.m_fm[0][2] = 0.0; r.m_fm[0][3] = 0.0;
			r.m_fm[1][0] = 0.0; r.m_fm[1][1] = v.y(); r.m_fm[1][2] = 0.0; r.m_fm[1][3] = 0.0;
			r.m_fm[2][0] = 0.0; r.m_fm[2][1] = 0.0; r.m_fm[2][2] = v.z(); r.m_fm[2][3] = 0.0;
			r.m_fm[3][0] = 0.0; r.m_fm[3][1] = 0.0; r.m_fm[3][2] = 0.0; r.m_fm[3][3] = 1.0;

			return r;
		}

		/** Gets a scale matrix - variation for not using a vector. */
		inline static Matrix4 getScale(Real sx, Real sy, Real sz){
			Matrix4 r;

			r.m_fm[0][0] = sx; r.m_fm[0][1] = 0.0; r.m_fm[0][2] = 0.0; r.m_fm[0][3] = 0.0;
			r.m_fm[1][0] = 0.0; r.m_fm[1][1] = sy; r.m_fm[1][2] = 0.0; r.m_fm[1][3] = 0.0;
			r.m_fm[2][0] = 0.0; r.m_fm[2][1] = 0.0; r.m_fm[2][2] = sz; r.m_fm[2][3] = 0.0;
			r.m_fm[3][0] = 0.0; r.m_fm[3][1] = 0.0; r.m_fm[3][2] = 0.0; r.m_fm[3][3] = 1.0;

			return r;
		}

		/** Extracts the rotation / scaling part of the matrix as a 3x3 matrix.
			@param m3x3 Destination Matrix3
		*/
		inline void extract3x3Matrix(Matrix3& m3x3) const{
			m3x3.m_fm[0][0] = m_fm[0][0];
			m3x3.m_fm[0][1] = m_fm[0][1];
			m3x3.m_fm[0][2] = m_fm[0][2];
			m3x3.m_fm[1][0] = m_fm[1][0];
			m3x3.m_fm[1][1] = m_fm[1][1];
			m3x3.m_fm[1][2] = m_fm[1][2];
			m3x3.m_fm[2][0] = m_fm[2][0];
			m3x3.m_fm[2][1] = m_fm[2][1];
			m3x3.m_fm[2][2] = m_fm[2][2];
		}

		/** Extracts the rotation / scaling part as a quaternion from the Matrix. */
		inline Quaternion extractQuaternion() const{
			Matrix3 m3x3;
			extract3x3Matrix(m3x3);
			return Quaternion(m3x3);
		}

		inline Matrix4 operator * (Real scalar) const{
			return Matrix4(
				scalar*m_fm[0][0], scalar*m_fm[0][1], scalar*m_fm[0][2], scalar*m_fm[0][3],
				scalar*m_fm[1][0], scalar*m_fm[1][1], scalar*m_fm[1][2], scalar*m_fm[1][3],
				scalar*m_fm[2][0], scalar*m_fm[2][1], scalar*m_fm[2][2], scalar*m_fm[2][3],
				scalar*m_fm[3][0], scalar*m_fm[3][1], scalar*m_fm[3][2], scalar*m_fm[3][3]);
		}

		/** Function for writing to a stream. */
		inline _MathExport friend std::ostream& operator << (std::ostream& o, const Matrix4& m){
			o << "Matrix4(";
			for(size_t i=0; i<4; i++){
				o << " row" << (unsigned)i << "{";
				for(size_t j=0; j<4; j++){
					o << m[i][j] << " ";
				}
				o << "}";
			}
			o << ")";

			return o;
		}

		Matrix4 adjoint() const;
		Real determinant() const;
		Matrix4 inverse() const;

		/** Building a Matrix4 from orientation / scale / position.
			@remarks
				Transform is performed in the order scale, rotate, translation, i.e. translation is independent
				of orientation axes, scale does not affect size of translation, rotation and scaling are always
				centered on the origin.
        */
		void makeTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);

		/** Building an inverse Matrix4 from orientation / scale / position.
			@remarks
				As makeTransform except it build the inverse given the same data as makeTransform, so
				performing -translation, -rotate, 1/scale in that order.
        */
		void makeInverseTransform(const Vector3& position, const Vector3& scale, const Quaternion& orientation);

		/** Check whether or not the matrix is affine matrix.
            @remarks
                An affine matrix is a 4x4 matrix with row 3 equal to (0, 0, 0, 1),
                e.g. no projective coefficients.
        */
		inline bool isAffine(void) const{
			return m_fm[3][0] == 0 && m_fm[3][1] == 0 && m_fm[3][2] == 0 && m_fm[3][3] == 1;
		}

		/** Returns the inverse of the affine matrix.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
		Matrix4 inverseAffine(void) const;

		/** Concatenate two affine matrix.
            @note
                The matrices must be affine matrix. @see Matrix4::isAffine.
        */
		inline Matrix4 concatenateAffine(const Matrix4& m2) const{
#ifdef _DEBUG
			assert(isAffine() && m2.isAffine());
			// add error code...
#endif
			return Matrix4(
				m_fm[0][0] * m2.m_fm[0][0] + m_fm[0][1] * m2.m_fm[1][0] + m_fm[0][2] * m2.m_fm[2][0],
				m_fm[0][0] * m2.m_fm[0][1] + m_fm[0][1] * m2.m_fm[1][1] + m_fm[0][2] * m2.m_fm[2][1],
				m_fm[0][0] * m2.m_fm[0][2] + m_fm[0][1] * m2.m_fm[1][2] + m_fm[0][2] * m2.m_fm[2][2],
				m_fm[0][0] * m2.m_fm[0][3] + m_fm[0][1] * m2.m_fm[1][3] + m_fm[0][2] * m2.m_fm[2][3] + m_fm[0][3],

				m_fm[1][0] * m2.m_fm[0][0] + m_fm[1][1] * m2.m_fm[1][0] + m_fm[1][2] * m2.m_fm[2][0],
				m_fm[1][0] * m2.m_fm[0][1] + m_fm[1][1] * m2.m_fm[1][1] + m_fm[1][2] * m2.m_fm[2][1],
				m_fm[1][0] * m2.m_fm[0][2] + m_fm[1][1] * m2.m_fm[1][2] + m_fm[1][2] * m2.m_fm[2][2],
				m_fm[1][0] * m2.m_fm[0][3] + m_fm[1][1] * m2.m_fm[1][3] + m_fm[1][2] * m2.m_fm[2][3] + m_fm[1][3],

				m_fm[2][0] * m2.m_fm[0][0] + m_fm[2][1] * m2.m_fm[1][0] + m_fm[2][2] * m2.m_fm[2][0],
				m_fm[2][0] * m2.m_fm[0][1] + m_fm[2][1] * m2.m_fm[1][1] + m_fm[2][2] * m2.m_fm[2][1],
				m_fm[2][0] * m2.m_fm[0][2] + m_fm[2][1] * m2.m_fm[1][2] + m_fm[2][2] * m2.m_fm[2][2],
				m_fm[2][0] * m2.m_fm[0][3] + m_fm[2][1] * m2.m_fm[1][3] + m_fm[2][2] * m2.m_fm[2][3] + m_fm[2][3],

				0.0f, 0.0f, 0.0f, 1.0f);
		}

		/** 3-D Vector transformation specially for affine matrix.
            @remarks
                Transforms the given 3-D vector by the matrix, projecting the 
                result back into <i>w</i> = 1.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
		inline Vector3 transformAffine(const Vector3& v) const{
#ifdef _DEBUG
			assert(isAffine());
			// add error code...
#endif
			return Vector3(
				m_fm[0][0] * v.x() + m_fm[0][1] * v.y() + m_fm[0][2] * v.z() + m_fm[0][3], 
				m_fm[1][0] * v.x() + m_fm[1][1] * v.y() + m_fm[1][2] * v.z() + m_fm[1][3],
				m_fm[2][0] * v.x() + m_fm[2][1] * v.y() + m_fm[2][2] * v.z() + m_fm[2][3]);
		}

		/** 4-D Vector transformation specially for affine matrix.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
        */
		inline Vector4 transformAffine(const Vector4& v) const{
#ifdef _DEBUG
			assert(isAffine());
			// add error code...
#endif
			return Vector4(
				m_fm[0][0] * v.x() + m_fm[0][1] * v.y() + m_fm[0][2] * v.z() + m_fm[0][3] * v.w(), 
				m_fm[1][0] * v.x() + m_fm[1][1] * v.y() + m_fm[1][2] * v.z() + m_fm[1][3] * v.w(),
				m_fm[2][0] * v.x() + m_fm[2][1] * v.y() + m_fm[2][2] * v.z() + m_fm[2][3] * v.w(),
				v.w());
		}

	}; //#### end class Mtrix4

	/* Removed from Vector4 and made a non-member here because otherwise
       OgreMatrix4.h and OgreVector4.h have to try to include and inline each 
       other, which frankly doesn't work ;)
    */
	inline Vector4 operator * (const Vector4& v, const Matrix4& mat){
		return Vector4(
			v.x()*mat[0][0] + v.y()*mat[1][0] + v.z()*mat[2][0] + v.w()*mat[3][0],
			v.x()*mat[0][1] + v.y()*mat[1][1] + v.z()*mat[2][1] + v.w()*mat[3][1],
			v.x()*mat[0][2] + v.y()*mat[1][2] + v.z()*mat[2][2] + v.w()*mat[3][2],
			v.x()*mat[0][3] + v.y()*mat[1][3] + v.z()*mat[2][3] + v.w()*mat[3][3]
		);
	}

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SMATRIX4_H__