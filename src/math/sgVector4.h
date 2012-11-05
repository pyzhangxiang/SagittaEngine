//////////////////////////////////////////////////////
// file: sgVector4.h @ 2008-1-23 by Zhang Xiang
// declares of the class Vector4
// Vector4 is a class ...
//////////////////////////////////////////////////////
#ifndef __SGVECTOR4_H__
#define __SGVECTOR4_H__

// INCLUDES //////////////////////////////////////////
#include "sgMath.h"
#include "sgVector3.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** 4-dimensional homogenous vector
	@remarks

	@note

	@par

	*/
	class _SG_MathExport Vector4{
		// member variables
	private:
        SERIALIZE_BEGIN(Vector4)
		union{
			Real m_fa[4];	// array indexed storage

			struct{
				Real m_fx, m_fy, m_fz, m_fw; // explicit names
			}; //#### end struct
		}; //#### end union
        SERIALIZE_END

	public:
		/** special points */
		static const Vector4 ZERO;
		static const Vector4 ZERO3D;
		static const Vector4 UNIT_SCALE;

		// constructors & destructor
	public:
		inline Vector4() : m_fx(0.0), m_fy(0.0), m_fz(0.0), m_fw(0.0){}
		inline Vector4(const Real vfx, const Real vfy, const Real vfz, const Real vfw)
			: m_fx(vfx), m_fy(vfy), m_fz(vfz), m_fw(vfw){}
		inline explicit Vector4(const Real vfScaler)
			: m_fx(vfScaler), m_fy(vfScaler), m_fz(vfScaler), m_fw(vfScaler){}
		inline explicit Vector4(const Real vfaCoordinate[4])
			: m_fx(vfaCoordinate[0]), m_fy(vfaCoordinate[1]), m_fz(vfaCoordinate[2]), m_fw(vfaCoordinate[3]){}
		inline explicit Vector4(const int vfaCoordinate[4])
			: m_fx(Real(vfaCoordinate[0])), m_fy(Real(vfaCoordinate[1])), m_fz(Real(vfaCoordinate[2])), m_fw(Real(vfaCoordinate[3])){}
		inline explicit Vector4(Real *vfpCoordinate)
			: m_fx(vfpCoordinate[0]), m_fy(vfpCoordinate[1]), m_fz(vfpCoordinate[2]), m_fw(vfpCoordinate[4]){}
		inline Vector4(const Vector4 &vvrVector){
			m_fx = vvrVector.x(); m_fy = vvrVector.y(); m_fz = vvrVector.z(); m_fw = vvrVector.w();
		}
		inline Vector4(const Vector3 &vvrVector){
			m_fx = vvrVector.x(); m_fy = vvrVector.y(); m_fz = vvrVector.z(); m_fw = 1;
		}
		inline ~Vector4(){}

		// member functions
	public:
		// Get and set value of the vector.
		inline void setValues(const Real vfx, const Real vfy, const Real vfz, const Real vfw){
			m_fx = vfx; m_fy = vfy; m_fz = vfz; m_fw = vfw;
		}
		inline void setValues(const Real vfaCoordinate[4]){
			m_fx = vfaCoordinate[0]; m_fy = vfaCoordinate[1]; m_fz = vfaCoordinate[2]; m_fw = vfaCoordinate[3];
		}
		inline void setValues(const Real vfScalar){
			m_fx = vfScalar; m_fy = vfScalar; m_fz = vfScalar; m_fw = vfScalar; 
		}
		inline void setX(const Real vfValue){ m_fx = vfValue; }
		inline void setY(const Real vfValue){ m_fy = vfValue; }
		inline void setZ(const Real vfValue){ m_fz = vfValue; }
		inline void setW(const Real vfValue){ m_fw = vfValue; }
		inline Real x() const{ return m_fx; }
		inline Real y() const{ return m_fy; }
		inline Real z() const{ return m_fz; }
		inline Real w() const{ return m_fw; }
		inline const Real *data() const{ return m_fa; }
		inline Real operator [] (const size_t viIndex) const{
#ifdef _DEBUG
			assert(viIndex < 4);
			// add error code...
#endif
			return m_fa[viIndex];
		}
		inline Real& operator [] (const size_t viIndex){
#ifdef _DEBUG
			assert(viIndex < 4);
			// add error code...
#endif
			return m_fa[viIndex];
		}

		// operator functions
	public:
		/** Assigns the value of the other vector.
			@param
			vvrVector the other vector
		*/
		inline Vector4& operator = (const Vector4 &vvrVector){
			m_fx = vvrVector.x();
			m_fy = vvrVector.y();
			m_fz = vvrVector.z();
			m_fw = vvrVector.w();

			return *this;
		}

		/** Assigns the value of a scalar, so let x equals y.
			@param
			vfScalar the scalar value
		*/
		inline Vector4& operator = (const Real vfScalar){
			m_fx = vfScalar;
			m_fy = vfScalar;
			m_fz = vfScalar;
			m_fw = vfScalar;

			return *this;
		}

		/** Assigns the value of a Vector3, and let the value of w be 1.0.
			@param
			vvrVector the other vector which is a 3-dimensional vector
		*/
		inline Vector4& operator = (const Vector3 &vvrVector){
			m_fx = vvrVector.x();
			m_fy = vvrVector.y();
			m_fz = vvrVector.z();
			m_fw = 1.0f;

			return *this;
		}

		/** Conversion operator for Vector4 to Vector3
		*/
		inline operator Vector3() const{
			return Vector3(m_fx / m_fw, m_fy / m_fw, m_fz / m_fw);
		}

		inline bool operator == (const Vector4 &vvrVector) const{
			return (Math::RealEqual(m_fx, vvrVector.x()) &&
				Math::RealEqual(m_fy, vvrVector.y()) &&
				Math::RealEqual(m_fz, vvrVector.z()) &&
				Math::RealEqual(m_fw, vvrVector.w()));
		}

		inline bool operator != (const Vector4 &vvrVector) const{
			return (!Math::RealEqual(m_fx, vvrVector.x()) ||
				!Math::RealEqual(m_fy, vvrVector.y()) ||
				!Math::RealEqual(m_fz, vvrVector.z()) ||
				!Math::RealEqual(m_fw, vvrVector.w()));
		}

		// arithmetic operations
		inline Vector4 operator + (const Vector4 &vvrVector) const{
			return Vector4(m_fx + vvrVector.x(), m_fy + vvrVector.y(), m_fz + vvrVector.z(), m_fw + vvrVector.w());
		}

		inline Vector4 operator - (const Vector4 &vvrVector) const{
			return Vector4(m_fx - vvrVector.x(), m_fy - vvrVector.y(), m_fz - vvrVector.z(), m_fw - vvrVector.w());
		}

		inline Vector4 operator * (const Real vfScalar) const{
			return Vector4(m_fx * vfScalar, m_fy * vfScalar, m_fz * vfScalar, m_fw * vfScalar);
		}

		inline Vector4 operator * (const Vector4 &vvrVector) const{
			return Vector4(m_fx * vvrVector.x(), m_fy * vvrVector.y(), m_fz * vvrVector.z(), m_fw * vvrVector.w());
		}

		inline Vector4 operator / (const Real vfScalar) const{
#ifdef _DEBUG
			assert(vfScalar != 0.0);
			// add error code...
#endif
			Real fInv = 1.0f / vfScalar;

			return Vector4(m_fx * fInv, m_fy * fInv, m_fz * fInv, m_fw * fInv);
		}

		inline Vector4 operator / (const Vector4 &vvrVector) const{
#ifdef _DEBUG
			assert(vvrVector.x() != 0.0 && vvrVector.y() != 0.0 && vvrVector.z() != 0.0 && vvrVector.w() != 0.0);
			// add error code...
#endif
			return Vector4(m_fx / vvrVector.x(), m_fy / vvrVector.y(), m_fz / vvrVector.z(), m_fw / vvrVector.w()); 
		}

		inline const Vector4& operator + () const{
			return *this;
		}

		inline Vector4 operator - () const{
			return Vector4(-m_fx, -m_fy, -m_fz, -m_fw);
		}

		// overloaded operators to help Vector4
		inline friend Vector4 operator * (const Real vfScalar, const Vector4 &vvrVector){
			return Vector4(vfScalar * vvrVector.x(), vfScalar * vvrVector.y(), vfScalar * vvrVector.z(), vfScalar * vvrVector.w());
		}

		inline friend Vector4 operator / (const Real vfScalar, const Vector4 &vvrVector){
#ifdef _DEBUG
			assert(vvrVector.x() != 0.0 && vvrVector.y() != 0.0 && vvrVector.z() != 0.0 && vvrVector.w() != 0.0);
			// add error code...
#endif
			return Vector4(vfScalar / vvrVector.x(), vfScalar / vvrVector.y(), vfScalar / vvrVector.z(), vfScalar / vvrVector.w());
		}

		// arithmetic updates
		inline Vector4& operator += (const Vector4 &vvrVector){
			m_fx += vvrVector.x();
			m_fy += vvrVector.y();
			m_fz += vvrVector.z();
			m_fw += vvrVector.w();

			return *this;
		}

		inline Vector4& operator += (const Real vfScalar){
			m_fx += vfScalar;
			m_fy += vfScalar;
			m_fz += vfScalar;
			m_fw += vfScalar;

			return *this;
		}

		inline Vector4& operator -= (const Vector4 &vvrVector){
			m_fx -= vvrVector.x();
			m_fy -= vvrVector.y();
			m_fz -= vvrVector.z();
			m_fw -= vvrVector.w();

			return *this;
		}

		inline Vector4& operator -= (const Real vfScalar){
			m_fx -= vfScalar;
			m_fy -= vfScalar;
			m_fz -= vfScalar;
			m_fw -= vfScalar;

			return *this;
		}

		inline Vector4& operator *= (const Vector4 &vvrVector){
			m_fx *= vvrVector.x();
			m_fy *= vvrVector.y();
			m_fz *= vvrVector.z();
			m_fw *= vvrVector.w();

			return *this;
		}

		inline Vector4& operator *= (const Real vfScalar){
			m_fx *= vfScalar;
			m_fy *= vfScalar;
			m_fz *= vfScalar;
			m_fw *= vfScalar;

			return *this;
		}

		inline Vector4& operator /= (const Vector4 &vvrVector){
#ifdef _DEBUG
			assert(vvrVector.x() != 0.0 && vvrVector.y() != 0.0 && vvrVector.z() != 0.0 && vvrVector.w() != 0.0);
			// add error code...
#endif
			m_fx /= vvrVector.x();
			m_fy /= vvrVector.y();
			m_fz /= vvrVector.z();
			m_fw /= vvrVector.w();

			return *this;
		}

		inline Vector4& operator /= (const Real vfScalar){
#ifdef _DEBUG
			assert(vfScalar != 0.0);
			// add error code...
#endif
			Real fInv = 1.0f / vfScalar;

			m_fx *= fInv;
			m_fy *= fInv;
			m_fz *= fInv;
			m_fw *= fInv;

			return *this;
		}

		///** Returns the square of the length(magnitude) of the vector.
		//@remarks
		//This  method is for efficiency - calculating the actual
		//length of a vector requires a square root, which is expensive
		//in terms of the operations required. This method returns the
		//square of the length of the vector, i.e. the same as the
		//length but before the square root is taken. Use this if you
		//want to find the longest / shortest vector without incurring
		//the square root.
		//*/
		//inline Real squaredLength() const{
		//	return ((m_fx * m_fx) + (m_fy * m_fy) + (m_fz * m_fz));
		//}

		///** Returns the length of the vector. 
		//	@warning
  //              This operation requires a square root and is expensive in
  //              terms of CPU operations. If you don't need to know the exact
  //              length (e.g. for just comparing lengths) use squaredLength()
  //              instead.
		//*/
		//inline Real length() const{
		//	return sqrt((m_fx * m_fx) + (m_fy * m_fy) + (m_fz * m_fz));
		//}

		///** Returns the distance to another vector.
  //          @warning
  //              This operation requires a square root and is expensive in
  //              terms of CPU operations. If you don't need to know the exact
  //              distance (e.g. for just comparing distances) use squaredDistance()
  //              instead.
  //      */
		//inline Real distance(const Vector4 &vvrVector) const{
		//	return (*this - vvrVector).length();
		//}

		///** Returns the square of the distance to another vector.
  //          @remarks
  //              This method is for efficiency - calculating the actual
  //              distance to another vector requires a square root, which is
  //              expensive in terms of the operations required. This method
  //              returns the square of the distance to another vector, i.e.
  //              the same as the distance but before the square root is taken.
  //              Use this if you want to find the longest / shortest distance
  //              without incurring the square root.
  //      */
		//inline Real squareDistance(const Vector4 &vvrVector) const{
		//	return (*this - vvrVector).squaredLength();
		//}

		/** Calculates the dot (scalar) product of this vector with another.
			@remarks
				The dot product can be used to calculate the angle between 2
				vectors. If both are unit vectors, the dot product is the
				cosine of the angle; otherwise the dot product must be
				divided by the product of the lengths of both vectors to get
				the cosine of the angle. This result can further be used to
				calculate the distance of a point from a plane.
			@param
				vvrVector Vector with which to calculate the dot product (together
				with this one).
			@returns
				A Real representing the dot product value.
		*/
		inline Real dotProduct(const Vector4 &vvrVector) const{
			return ((m_fx * vvrVector.x()) + (m_fy * vvrVector.y()) + (m_fz * vvrVector.z()) + (m_fw * vvrVector.w()));
		}

		///** Calculates the absolute dot (scalar) product of this vector with another.
  //          @remarks
  //              This function work similar dotProduct, except it use absolute value
  //              of each component of the vector to computing.
  //          @param
  //              vec Vector with which to calculate the absolute dot product (together
  //              with this one).
  //          @returns
  //              A Real representing the absolute dot product value.
  //      */
		//inline Real absDotProduct(const Vector4 &vvrVector) const{
		//	return fabs(m_fx * vvrVector.x()) + fabs(m_fy * vvrVector.y()) + fabs(m_fz * vvrVector.z());
		//}

		///** Normalises the vector.
		//	@remarks
		//		This method normalises the vector such that it's
		//		length / magnitude is 1. The result is called a unit vector.
		//	@note
		//		This function will not crash for zero-sized vectors, but there
		//		will be no changes made to their components.
		//	@returns The previous length of the vector.
		//*/
		//inline Real normalise(void){
		//	Real fLength = sqrt((m_fx * m_fx) + (m_fy * m_fy) + (m_fz * m_fz));

		//	// Will also work for zero-sized vectors, but will change nothing.
		//	if(fLength > EPSILON_E8){
		//		Real fInvLength = 1.0f / fLength;

		//		m_fx *= fInvLength;
		//		m_fy *= fInvLength;
		//		m_fz *= fInvLength;
		//	}

		//	return fLength;
		//}

		///** Calculates the cross-product of 2 vectors, i.e. the vector that
  //          lies perpendicular to them both.
  //          @remarks
  //              The cross-product is normally used to calculate the normal
  //              vector of a plane, by calculating the cross-product of 2
  //              non-equivalent vectors which lie on the plane (e.g. 2 edges
  //              of a triangle).
  //          @param
  //              vec Vector which, together with this one, will be used to
  //              calculate the cross-product.
  //          @returns
  //              A vector which is the result of the cross-product. This
  //              vector will <b>NOT</b> be normalised, to maximise efficiency
  //              - call Vector4::normalise on the result if you wish this to
  //              be done. As for which side the resultant vector will be on, the
  //              returned vector will be on the side from which the arc from 'this'
  //              to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
  //              = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
		//		This is because OGRE uses a right-handed coordinate system.
  //          @par
  //              For a clearer explanation, look a the left and the bottom edges
  //              of your monitor's screen. Assume that the first vector is the
  //              left edge and the second vector is the bottom edge, both of
  //              them starting from the lower-left corner of the screen. The
  //              resulting vector is going to be perpendicular to both of them
  //              and will go <i>inside</i> the screen, towards the cathode tube
  //              (assuming you're using a CRT monitor, of course).
  //      */
		//inline Vector4 crossProduct(const Vector4 &vvrVector) const{
		//	return Vector4(
		//		m_fy * vvrVector.z() - m_fz * vvrVector.y(),
		//		m_fz * vvrVector.x() - m_fx * vvrVector.z(),
		//		m_fx * vvrVector.y() - m_fy * vvrVector.x());
		//}

		///** Returns a vector at a point half way between this and the passed
		//	in vector.
		//*/
		//inline Vector4 midPoint(const Vector4 &vvrVector) const{
		//	return Vector4((m_fx + vvrVector.x()) * 0.5f, (m_fy + vvrVector.y()) * 0.5f, (m_fz + vvrVector.z()) * 0.5f);
		//}

		///** Returns true if the vector's scalar components are all greater
		//	that the ones of the vector it is compared against.
		//*/
		//inline bool operator < (const Vector4 &vvrVector) const{
		//	if(m_fx < vvrVector.x() && m_fy < vvrVector.y() && m_fz < vvrVector.z())
		//		return true;
		//	return false;
		//}

		///** Returns true if the vector's scalar components are all smaller
		//	that the ones of the vector it is compared against.
		//*/
		//inline bool operator > (const Vector4 &vvrVector) const{
		//	if(m_fx > vvrVector.x() && m_fy > vvrVector.y() && m_fz > vvrVector.z())
		//		return true;
		//	return false;
		//}

		///** Sets this vector's components to the minimum of its own and the
		//	ones of the passed in vector.
		//	@remarks
		//		'Minimum' in this case means the combination of the lowest
		//		value of x, y and z from both vectors. Lowest is taken just
		//		numerically, not magnitude, so -1 < 0.
		//*/
		//inline void makeFloor(const Vector4 &vvrVector){
		//	if(m_fx > vvrVector.x()) m_fx = vvrVector.x();
		//	if(m_fy > vvrVector.y()) m_fy = vvrVector.y();
		//	if(m_fz > vvrVector.z()) m_fz = vvrVector.z();
		//}

		///** Sets this vector's components to the maximum of its own and the
		//	ones of the passed in vector.
		//	@remarks
		//		'Maximum' in this case means the combination of the highest
		//		value of x, y and z from both vectors. Highest is taken just
		//		numerically, not magnitude, so 1 > -3.
		//*/
		//inline void makeCeil(const Vector4 &vvrVector){
		//	if(m_fx < vvrVector.x()) m_fx = vvrVector.x();
		//	if(m_fy < vvrVector.y()) m_fy = vvrVector.y();
		//	if(m_fz < vvrVector.z()) m_fz = vvrVector.z();
		//}

		///** Generates a vector perpendicular to this vector (eg an 'up' vector).
		//	@remarks
		//		This method will return a vector which is perpendicular to this
		//		vector. There are an infinite number of possibilities but this
		//		method will guarantee to generate one of them. If you need more
		//		control you should use the Quaternion class.
		//*/
		////inline Vector4 perpendicular(void) const{
		////	Vector4 perp = this->crossProduct(Vector4::UNIT_X);

		////	// Check length.
		////	if(perp.squaredLength() < EPSILON_E6*EPSILON_E6){
		////		// This vector is the Y axis multiplied by a scalar, so we have
		////		// to use another axis.
		////		perp = this->crossProduct(Vector4::UNIT_Y);
		////	}

		////	return perp;
		////}

		///** Returns true if this vector is zero length. */
		//inline bool isZeroLength(void) const{
		//	Real fsqlen = (m_fx * m_fx) + (m_fy * m_fy) + (m_fz * m_fz);
		//	return (fsqlen < (EPSILON_E6 * EPSILON_E6));
		//}

		///** As normalise, except that this vector is unaffected and the
		//*	normalised vector is returned as a copy. 
		//*/
		//inline Vector4 normalisedCopy(void) const{
		//	Vector4 vret = *this;
		//	vret.normalise();
		//	return vret;
		//}

		///** Calculates a reflection vector to the plane with the given normal .
		//	@remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		//*/
		//inline Vector4 reflect(const Vector4 &normal) const{
		//	return Vector4(*this - (2 * this->dotProduct(normal) * normal));
		//}

		// /** Gets the shortest arc quaternion to rotate this vector to the destination
  //          vector.
		//	@remarks
		//		If you call this with a dest vector that is close to the inverse
		//		of this vector, we will rotate 180 degrees around the 'fallbackAxis'
		//		(if specified, or a generated axis if not) since in this case
		//		ANY axis of rotation is valid.
  //      */
		///* for future...
		//Quaternion getRotationTo(const Vector4 &dest, const Vector4 &fallbackAxis = Vector4::ZERO) const{

		//} */

		///** Returns whether this vector is within a positional tolerance
		//	of another vector.
		//	@param rhs The vector to compare with
		//	@param tolerance The amount that each element of the vector may vary by
		//	and still be considered equal
		//*/
		//inline bool positionEquals(const Vector4 &vvrv, Real tolerance = EPSILON_E3) const{
		//	return (Math::RealEqual(m_fx, vvrv.x(), tolerance) &&
		//		Math::RealEqual(m_fy, vvrv.y(), tolerance) &&
		//		Math::RealEqual(m_fz, vvrv.z(), tolerance));
		//}

		///** Returns whether this vector is within a positional tolerance
		//	of another vector, also take scale of the vectors into account.
		//	@param rhs The vector to compare with
		//	@param tolerance The amount (related to the scale of vectors) that distance
  //          of the vector may vary by and still be considered close
		//*/
		//inline bool positionCloses(const Vector4 &vvrv, Real tolerance = EPSILON_E3) const{
		//	return (squareDistance(vvrv) <= (squaredLength() + vvrv.squaredLength()) * tolerance);
		//}

		///** Returns whether this vector is within a directional tolerance
		//	of another vector.
		//	@param vvrv The vector to compare with
		//	@param frATolerance The maximum radian angle by which the vectors may vary and
		//	still be considered equal
		//	@note Both vectors should be normalised.
		//*/
		//inline bool dirctionEquals(const Vector4 &vvrv, const Real frATolerance = EPSILON_E5){
		//	Real fdot = dotProduct(vvrv);
		//	Real frAngle = acos(fdot);

		//	return fabs(frAngle) <= frATolerance;
		//}

		/** function for writing to a stream */
		inline _SG_MathExport friend std::ostream& operator << (std::ostream &o, const Vector4 &v){
			o << "Vector4(" << v.x() << "," << v.y() << "," << v.z() <<"," << v.w() << ")";
			return o;
		}

	}; //#### end class Vector4

};

// DEFINES ///////////////////////////////////////////

#endif // __SGVECTOR4_H__
