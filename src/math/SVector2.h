//////////////////////////////////////////////////////
// file: SVector2.h @ 2008-1-20 by Zhang Xiang
// declares of the class Vector2
// Vector2 is a class ...
//////////////////////////////////////////////////////
#ifndef __SVECTOR2_H__
#define __SVECTOR2_H__

// INCLUDES //////////////////////////////////////////
#include "SMath.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
		@remarks
			A direction in 2D space represented as distances along the 2
            orthoganal axes (x, y). Note that positions, directions and
            scaling factors can be represented by a vector, depending on how
            you interpret the values.
		@note

		@par

	*/
	class _MathExport Vector2{
	// member variables
	private:
		union{
			Real m_fa[2];	// array indexed storage

			struct{
				Real m_fx, m_fy; // explicit names
			}; //#### end struct
		}; //#### end union

	public:
		/** special points */
		static const Vector2 ZERO;
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
		static const Vector2 UNIT_SCALE;

	// constructors & destructor
	public:
		inline Vector2() : m_fx(0.0), m_fy(0.0){}
		inline Vector2(const Real vfx, const Real vfy) : m_fx(vfx), m_fy(vfy){}
		inline explicit Vector2(const Real vfScaler) : m_fx(vfScaler), m_fy(vfScaler){}
		inline explicit Vector2(const Real vfaCoordinate[2]) : m_fx(vfaCoordinate[0]), m_fy(vfaCoordinate[1]){}
		inline explicit Vector2(const int vfaCoordinate[2]) : m_fx(Real(vfaCoordinate[0])), m_fy(Real(vfaCoordinate[1])){}
		inline explicit Vector2(Real *vfpCoordinate) : m_fx(vfpCoordinate[0]), m_fy(vfpCoordinate[1]){}
		inline Vector2(const Vector2 &vvrVector){ m_fx = vvrVector.x(); m_fy = vvrVector.y(); }
		inline ~Vector2(){}

	// member functions
	public:
		// Get and set value of the vector.
		inline void setValues(const Real vfx, const Real vfy){ m_fx = vfx; m_fy = vfy; }
		inline void setValues(const Real vfaCoordinate[2]){ m_fx = vfaCoordinate[0]; m_fy = vfaCoordinate[1]; }
		inline void setValues(const Real vfScalar){ m_fx = vfScalar; m_fy = vfScalar; }
		inline void setX(const Real vfValue){ m_fx = vfValue; }
		inline void setY(const Real vfValue){ m_fy = vfValue; }
		inline Real x() const{ return m_fx; }
		inline Real y() const{ return m_fy; }
		inline const Real *data() const{ return m_fa; }
		inline Real operator [] (const size_t viIndex) const{
#ifdef _DEBUG
			assert(viIndex < 2);
			// add error code...
#endif
			return m_fa[viIndex];
		}
		inline Real& operator [] (const size_t viIndex){
#ifdef _DEBUG
			assert(viIndex < 2);
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
		inline Vector2& operator = (const Vector2 &vvrVector){
			m_fx = vvrVector.x();
			m_fy = vvrVector.y();

			return *this;
		}

		/** Assigns the value of a scalar, so let x equals y.
			@param
				vfScalar the scalar value
		*/
		inline Vector2& operator = (const Real vfScalar){
			m_fx = vfScalar;
			m_fy = vfScalar;

			return *this;
		}

		inline bool operator == (const Vector2 &vvrVector) const{
			return (Math::RealEqual(m_fx, vvrVector.x()) &&
				Math::RealEqual(m_fy, vvrVector.y()));
		}

		inline bool operator != (const Vector2 &vvrVector) const{
			return (!Math::RealEqual(m_fx, vvrVector.x()) ||
				!Math::RealEqual(m_fy, vvrVector.y()));
		}

		// arithmetic operations
		inline Vector2 operator + (const Vector2 &vvrVector) const{
			return Vector2(m_fx + vvrVector.x(), m_fy + vvrVector.y());
		}

		inline Vector2 operator - (const Vector2 &vvrVector) const{
			return Vector2(m_fx - vvrVector.x(), m_fy - vvrVector.y());
		}

		inline Vector2 operator * (const Real vfScalar) const{
			return Vector2(m_fx * vfScalar, m_fy * vfScalar);
		}

		inline Vector2 operator * (const Vector2 &vvrVector) const{
			return Vector2(m_fx * vvrVector.x(), m_fy * vvrVector.y());
		}

		inline Vector2 operator / (const Real vfScalar) const{
#ifdef _DEBUG
			assert(vfScalar != 0.0);
			// add error code...
#endif
			Real fReciprocal = 1.0f / vfScalar;

			return Vector2(m_fx * fReciprocal, m_fy * fReciprocal);
		}

		inline Vector2 operator / (const Vector2 &vvrVector) const{
#ifdef _DEBUG
			assert(vvrVector.x() != 0.0 && vvrVector.y() != 0.0);
			// add error code...
#endif
			return Vector2(m_fx / vvrVector.x(), m_fy / vvrVector.y()); 
		}

		inline const Vector2& operator + () const{
			return *this;
		}

		inline Vector2 operator - () const{
			return Vector2(-m_fx, -m_fy);
		}

		// overloaded operators to help Vector2
		inline friend Vector2 operator * (const Real vfScalar, const Vector2 &vvrVector){
			return Vector2(vfScalar * vvrVector.x(), vfScalar * vvrVector.y());
		}

		inline friend Vector2 operator / (const Real vfScalar, const Vector2 &vvrVector){
#ifdef _DEBUG
			assert(vvrVector.x() != 0.0 && vvrVector.y() != 0.0);
			// add error code...
#endif
			return Vector2(vfScalar / vvrVector.x(), vfScalar / vvrVector.y());
		}

		// arithmetic updates
		inline Vector2& operator += (const Vector2 &vvrVector){
			m_fx += vvrVector.x();
			m_fy += vvrVector.y();

			return *this;
		}

		inline Vector2& operator += (const Real vfScalar){
			m_fx += vfScalar;
			m_fy += vfScalar;

			return *this;
		}

		inline Vector2& operator -= (const Vector2 &vvrVector){
			m_fx -= vvrVector.x();
			m_fy -= vvrVector.y();

			return *this;
		}

		inline Vector2& operator -= (const Real vfScalar){
			m_fx -= vfScalar;
			m_fy -= vfScalar;

			return *this;
		}

		inline Vector2& operator *= (const Vector2 &vvrVector){
			m_fx *= vvrVector.x();
			m_fy *= vvrVector.y();

			return *this;
		}

		inline Vector2& operator *= (const Real vfScalar){
			m_fx *= vfScalar;
			m_fy *= vfScalar;

			return *this;
		}

		inline Vector2& operator /= (const Vector2 &vvrVector){
#ifdef _DEBUG
			assert(vvrVector.x() != 0.0 && vvrVector.y() != 0.0);
			// add error code...
#endif
			m_fx /= vvrVector.x();
			m_fy /= vvrVector.y();

			return *this;
		}

		inline Vector2& operator /= (const Real vfScalar){
#ifdef _DEBUG
			assert(vfScalar != 0.0);
			// add error code...
#endif
			Real fReciprocal = 1.0f / vfScalar;

			m_fx *= fReciprocal;
			m_fy *= fReciprocal;

			return *this;
		}

		/** Returns the square of the length(magnitude) of the vector.
			@remarks
				This  method is for efficiency - calculating the actual
				length of a vector requires a square root, which is expensive
				in terms of the operations required. This method returns the
				square of the length of the vector, i.e. the same as the
				length but before the square root is taken. Use this if you
				want to find the longest / shortest vector without incurring
				the square root.
		*/
		inline Real squaredLength() const{
			return ((m_fx * m_fx) + (m_fy * m_fy));
		}

		/** Returns the length of the vector. */
		inline Real length() const{
			return sqrt((m_fx * m_fx) + (m_fy * m_fy));
		}

		/** Returns the distance to another vector.
            @warning
                This operation requires a square root and is expensive in
                terms of CPU operations. If you don't need to know the exact
                distance (e.g. for just comparing distances) use squaredDistance()
                instead.
        */
		inline Real distance(const Vector2 &vvrVector) const{
			return (*this - vvrVector).length();
		}

		/** Returns the square of the distance to another vector.
            @remarks
                This method is for efficiency - calculating the actual
                distance to another vector requires a square root, which is
                expensive in terms of the operations required. This method
                returns the square of the distance to another vector, i.e.
                the same as the distance but before the square root is taken.
                Use this if you want to find the longest / shortest distance
                without incurring the square root.
        */
		inline Real squareDistance(const Vector2 &vvrVector) const{
			return (*this - vvrVector).squaredLength();
		}

		/** Calculates the dot (scalar) product of this vector with another.
            @remarks
                The dot product can be used to calculate the angle between 2
                vectors. If both are unit vectors, the dot product is the
                cosine of the angle; otherwise the dot product must be
                divided by the product of the lengths of both vectors to get
                the cosine of the angle. This result can further be used to
                calculate the distance of a point from a plane.
            @param
                vec Vector with which to calculate the dot product (together
                with this one).
            @returns
                A Real representing the dot product value.
        */
		inline Real dotProduct(const Vector2 &vvrVector) const{
			return ((m_fx * vvrVector.x()) + (m_fy * vvrVector.y()));
		}

		/** Normalises the vector.
            @remarks
                This method normalises the vector such that it's
                length / magnitude is 1. The result is called a unit vector.
            @note
                This function will not crash for zero-sized vectors, but there
                will be no changes made to their components.
            @returns The previous length of the vector.
        */
		inline Real normalise(void){
			Real fLength = sqrt(squaredLength());

			// Will also work for zero-sized vectors, but will change nothing.
			if(fLength > Math::EPSILON_E8){
				Real fInvLength = 1.0f / fLength;

				m_fx *= fInvLength;
				m_fy *= fInvLength;
			}

			return fLength;
		}

		/** Returns a vector at a point half way between this and the passed
            in vector.
        */
		inline Vector2 midPoint(const Vector2 &vvrVector) const{
			return Vector2((m_fx + vvrVector.x()) * 0.5f, (m_fy + vvrVector.y()) * 0.5f);
		}

		/** Returns true if the vector's scalar components are all greater
            that the ones of the vector it is compared against.
        */
		inline bool operator < (const Vector2 &vvrVector) const{
			if(m_fx < vvrVector.x() && m_fy < vvrVector.y())
				return true;
			return false;
		}

		/** Returns true if the vector's scalar components are all smaller
            that the ones of the vector it is compared against.
        */
		inline bool operator > (const Vector2 &vvrVector) const{
			if(m_fx > vvrVector.x() && m_fy > vvrVector.y())
				return true;
			return false;
		}

		/** Sets this vector's components to the minimum of its own and the
            ones of the passed in vector.
            @remarks
                'Minimum' in this case means the combination of the lowest
                value of x, y and z from both vectors. Lowest is taken just
                numerically, not magnitude, so -1 < 0.
        */
		inline void makeFloor(const Vector2 &vvrVector){
			if(m_fx > vvrVector.x()) m_fx = vvrVector.x();
			if(m_fy > vvrVector.y()) m_fy = vvrVector.y();
		}

		/** Sets this vector's components to the maximum of its own and the
            ones of the passed in vector.
            @remarks
                'Maximum' in this case means the combination of the highest
                value of x, y and z from both vectors. Highest is taken just
                numerically, not magnitude, so 1 > -3.
        */
		inline void makeCeil(const Vector2 &vvrVector){
			if(m_fx < vvrVector.x()) m_fx = vvrVector.x();
			if(m_fy < vvrVector.y()) m_fy = vvrVector.y();
		}

		/** Generates a vector perpendicular to this vector (eg an 'up' vector).
            @remarks
                This method will return a vector which is perpendicular to this
                vector. There are an infinite number of possibilities but this
                method will guarantee to generate one of them. If you need more
                control you should use the Quaternion class.
        */
		inline Vector2 perpendicular(void) const{
			return Vector2(-m_fy, m_fx);
		}

		/** Calculates the 2 dimensional cross-product of 2 vectors, which results
			in a single Realing point value which is 2 times the area of the triangle.
        */
		inline Real crossProduct(const Vector2 &vvrVector) const{
			return ((m_fx * vvrVector.y()) - (m_fy * vvrVector.x()));
		}

		/** Returns true if this vector is zero length. */
		inline bool isZeroLength(void) const{
			Real fsqlen = (m_fx * m_fx) + (m_fy * m_fy);
			return (fsqlen < (Math::EPSILON_E6 * Math::EPSILON_E6));
		}

		/** As normalise, except that this vector is unaffected and the
		*	normalised vector is returned as a copy. 
		*/
		inline Vector2 normalisedCopy(void) const{
			Vector2 vret = *this;
			vret.normalise();
			return vret;
		}

		/** Calculates a reflection vector to the plane with the given normal .
			@remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
		*/
		inline Vector2 reflect(const Vector2 &normal) const{
			return Vector2(*this - (2 * this->dotProduct(normal) * normal));
		}

		/** function for writing to a stream */
		inline _MathExport friend std::ostream& operator << (std::ostream &o, const Vector2 &v){
			o << "Vector2(" << v.x() << "," << v.y() << ")";
			return o;
		}

	}; //#### end class Vector2

};

// DEFINES ///////////////////////////////////////////

#endif // __SVECTOR2_H__