//////////////////////////////////////////////////////
// file: sgQuaternion.h @ 2008-1-24 by Zhang Xiang
// declares of the class SQuaternion
// SQuaternion is a class ...
//////////////////////////////////////////////////////
#ifndef __SGQUATERNION_h__
#define __SGQUATERNION_h__

// INCLUDES //////////////////////////////////////////
#include "sgMath.h"
#include "sgEuler.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** Implementation of a Quaternion, i.e. a rotation around an axis.
	@remarks

	@note

	@par

	*/
	class Matrix3;
	class Vector3;

	class _SG_MathExport Quaternion{
	// member variables
	private:
        SERIALIZE_BEGIN(Quaternion)
		union{
			Real m_faq[4]; // array indexed storage w,x,y,z

			struct{
				Real m_fw, m_fx, m_fy, m_fz;
			};
		};
        SERIALIZE_END

	public:
		// cutoff for sine near zero
		static const Real ms_fEpsilon;

		// special values
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;

	// constructors & destructor
	public:
		inline Quaternion(const Real vfw = 1.0f, const Real vfx = 0.0, const Real vfy = 0.0, const Real vfz = 0.0){
			m_fw = vfw;
			m_fx = vfx;
			m_fy = vfy;
			m_fz = vfz;
		}

		inline Quaternion(const Quaternion& vqrq){
			m_fw = vqrq.w();
			m_fx = vqrq.x();
			m_fy = vqrq.y();
			m_fz = vqrq.z();
		}

		/// Construct a quaternion from a rotation matrix
		inline Quaternion(const Matrix3& rot){
			this->fromRotationMatrix(rot);
		}

		/// Construct a quaternion from an angle / axis
		inline Quaternion(const Real& vfrAngle, const Vector3& vvrAxis){
			this->fromAngleAxis(vfrAngle, vvrAxis);
		}

		/// Construct a quaternion from 3 orthonormal local axes
		inline Quaternion(const Vector3& vvrXAxis, const Vector3& vvrYAxis, const Vector3& vvrZAxis){
			this->fromAxes(vvrXAxis, vvrYAxis, vvrZAxis);
		}

		/// Construct a quaternion from 3 orthonormal local axes
		inline Quaternion(const Vector3 *vvpAxis){
			this->fromAxes(vvpAxis);
		}

		/// Construct a quaternion from 4 manual w/x/y/z values
		inline Quaternion(Real *vfp){
			memcpy(m_faq, vfp, 4 * sizeof(Real));
		}

	// member functions
	public:
		// Get and set value of the vector.
		inline void setValues(const Real vfw, const Real vfx, const Real vfy, const Real vfz){
			m_fw = vfw; m_fx = vfx; m_fy = vfy; m_fz = vfz;
		}
		inline void setValues(const Real vfaq[4]){
			m_fw = vfaq[0]; m_fx = vfaq[1]; m_fy = vfaq[2]; m_fz = vfaq[3];
		}
		inline void setX(const Real vfValue){ m_fx = vfValue; }
		inline void setY(const Real vfValue){ m_fy = vfValue; }
		inline void setZ(const Real vfValue){ m_fz = vfValue; }
		inline void setW(const Real vfValue){ m_fw = vfValue; }
		inline Real x() const{ return m_fx; }
		inline Real y() const{ return m_fy; }
		inline Real z() const{ return m_fz; }
		inline Real w() const{ return m_fw; }

		// array accessor operator
		inline Real operator [] (const size_t vi) const{
#ifdef _DEBUG
			assert(vi < 4);
			// add error code...
#endif
			return m_faq[vi];
		}

		inline Real& operator [] (const size_t vi){
#ifdef _DEBUG
			assert(vi < 4);
			// add error code...
#endif
			return m_faq[vi];
		}

		void fromRotationMatrix(const Matrix3& vrmRot);
		void toRotationMatrix(Matrix3& rmRot) const;
		void fromAngleAxis(const Real& vfrAngle, const Vector3& vvrAxis);
		void toAngleAxis(Real& rfAngle, Vector3& rvAxis) const;
		/** Generate a quaternion from 3 Vector3s.
			@remark
				The 3 Vector3 are column vectors for
				a rotation matrix
		*/
		void fromAxes(const Vector3 *vvpAxis);
		/** Generate a quaternion from 3 Vector3.
			@remark
				The 3 Vector3 are column vectors for
				a rotation matrix
		*/
		void fromAxes(const Vector3& vvrXAxis, const Vector3& vvrYAxis, const Vector3& vvrZAxis);
		void toAxes(Vector3 *rvAxis) const;
		void toAxes(Vector3& rXAxis, Vector3& rYAxis, Vector3& rZAxis) const;

		/// Get the local x-axis
		Vector3 xAxis(void) const;
		/// Get the local y-axis
		Vector3 yAxis(void) const;
		/// Get the local z-axis
		Vector3 zAxis(void) const;

		// operator functions
		inline Quaternion& operator = (const Quaternion& vqrq){
			m_fw = vqrq.w();
			m_fx = vqrq.x();
			m_fy = vqrq.y();
			m_fz = vqrq.z();

			return *this;
		}
		Quaternion operator + (const Quaternion& vqrq) const;
		Quaternion operator - (const Quaternion& vqrq) const;
		Quaternion operator * (const Quaternion& vqrq) const;
		Quaternion operator * (const Real vfScalar) const;
		_SG_MathExport friend Quaternion operator * (const Real vfScalar, const Quaternion& vqrq);
		Quaternion operator - () const;

		inline bool operator == (const Quaternion& vqrq) const{
			return (Math::RealEqual(m_fw, vqrq.w()) &&
				Math::RealEqual(m_fx, vqrq.x()) &&
				Math::RealEqual(m_fy, vqrq.y()) &&
				Math::RealEqual(m_fz, vqrq.z()));
		}
		inline bool operator != (const Quaternion& vqrq) const{
			return !operator==(vqrq);
		}

		// functions of a quaternion
		/// dot product
		Real dotProduct(const Quaternion& vqrq) const;
		/// squred-length
		Real norm() const;
		/// squred-length
		Real squredLength() const;
		///// length
		//Real length() const;
		/// Normalises this quaternion, and returns the previous squred length
		Real normalise();

		Quaternion inverse() const; // apply to non-zero quaternion
		Quaternion unitInverse() const; // apply to unit-length quaternion
		Quaternion exp() const;
		Quaternion log() const;

		/// rotation of a vector by a quaternion
		Vector3 operator * (const Vector3& vvrv) const;

		/** Calculate the local roll element of this quaternion.
			@param 
				reprojectAxis By default the method returns the 'intuitive' result
				that is, if you projected the local Y of the quaterion onto the X and
				Y axes, the angle between them is returned. If set to false though, the
				result is the actual yaw that will be used to implement the quaternion,
				which is the shortest possible path to get to the same orientation and 
				may involve less axial rotation. 
		*/
		Real getRoll(bool reprojectAxis = true) const;

		/** Calculate the local pitch element of this quaternion
			@param 
				reprojectAxis By default the method returns the 'intuitive' result
				that is, if you projected the local Z of the quaterion onto the X and
				Y axes, the angle between them is returned. If set to true though, the
				result is the actual yaw that will be used to implement the quaternion,
				which is the shortest possible path to get to the same orientation and 
				may involve less axial rotation. 
		*/
		Real getPitch(bool reprojectAxis = true) const;

		/** Calculate the local yaw element of this quaternion
			@param 
				reprojectAxis By default the method returns the 'intuitive' result
				that is, if you projected the local Z of the quaterion onto the X and
				Z axes, the angle between them is returned. If set to true though, the
				result is the actual yaw that will be used to implement the quaternion,
				which is the shortest possible path to get to the same orientation and 
				may involve less axial rotation. 
		*/
		Real getYaw(bool reprojectAxis = true) const;

		/// Equality with tolerance (tolerance is max angle difference)
		bool equals(const Quaternion& vqrq, const Real& tolerance) const;

		// spherical linear interpolation
		static Quaternion Slerp(Real vft, const Quaternion& vqrP, const Quaternion& vqrQ, bool shortestPath = false);
		static Quaternion SlerpExtraSpins(Real vft, const Quaternion& vqrP, const Quaternion& vqrQ, int iExtraSpins);

		// setup for spherical quadratic interpolation
		static void Intermediate(const Quaternion& vqrQ0, const Quaternion& vqrQ1, const Quaternion& vqrQ2,
								Quaternion& rqA, Quaternion& rqB);
		
		// spherical quadratic interpolation
		static Quaternion Squad(Real vft, const Quaternion& vqrP,
								const Quaternion& vqrA, const Quaternion& vqrB,
								const Quaternion& vqrQ, bool shortestPath = false);

		// normalised linear interpolation - faster but less accurate (non-constant rotation velocity)
		static Quaternion Nlerp(Real vft, const Quaternion& vqrP, const Quaternion& vqrQ, bool shortestPath = false);


		//////////////////////////////////////////////////////////////////////////
		// euler rotate order is z-x-y
		void fromEuler(const Euler &_euler);
		Euler toEuler(void) const;
		//////////////////////////////////////////////////////////////////////////
		

		/** Function for writing to a stream. Outputs "Quaternion(w, x, y, z)" with w,x,y,z
            being the member values of the quaternion.
        */
		inline _SG_MathExport friend std::ostream& operator << (std::ostream& o, const Quaternion& q){
			o << "Quaternion(" << q.w() << ", " << q.x() << ", " << q.y() << ", " << q.z() << ")";
			return o;
		}

	}; //#### end class SQuaternion

}

// DEFINES ///////////////////////////////////////////

#endif // __SGQUATERNION_h__
