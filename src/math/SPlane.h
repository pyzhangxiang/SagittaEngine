//////////////////////////////////////////////////////
// file: SPlane.h @ 2008-1-28 by Zhang Xiang
// declares of the class SPlane
// SPlane is a class ...
//////////////////////////////////////////////////////
#ifndef __SPLANE_H__
#define __SPLANE_H__

// INCLUDES //////////////////////////////////////////
#include "SVector3.h"
#include "SMath.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class  AxisAlignedBox;

	/** Defines a plane in 3D space.
        @remarks
            A plane is defined in 3D space by the equation
            Ax + By + Cz + D = 0
        @par
            This equates to a vector (the normal of the plane, whose x, y
            and z components equate to the coefficients A, B and C
            respectively), and a constant (D) which is the distance along
            the normal you have to go to move the plane back to the origin.
     */
	class _SagittaExport Plane{
	// enum declares
	public:
		enum Side{
			NO_SIDE,
			POSITIVE_SIDE,
			NEGATIVE_SIDE,
			BOTH_SIDE
		};
	// member variables
	private:
		Real m_fd;
		Vector3 m_vNormal;

	// constructors & destructor
	public:
		Plane();
		Plane(const Plane& vprp);
		/** Construct a plane through a normal, and a distance to move the plane along the normal. */
		Plane(const Vector3& vvrNormal, Real vfConstant);
		Plane(const Vector3& vvrNormal, const Vector3& vvrPoint);
		Plane(const Vector3& vvrPoint0, const Vector3& vvrPiont1, const Vector3& vvrPoint2);
		
		// member functions
	public:
		/// get the distance of the plane
		inline Real distance() const{
			// maybe a bug...
			return m_fd;
		}
		/// get the normal of the plane
		inline Vector3 normal() const{
			// maybe a bug...
			return m_vNormal;
		}

		inline void setDistance(Real fd){
			m_fd = fd;
		}

		inline void setNormal(const Vector3& v){
			m_vNormal = v;
		}

		Side getSide(const Vector3& vvrPoint) const;
		/**
			returns the side where the aligneBox is. the flag BOTH_SIDE indicates an intersecting box.
			one corner ON the plane is sufficient to consider the box and the plane intersecting.
		*/
		Side getSide(const AxisAlignedBox& varBox) const;
		
		/** Returns which side of the plane that the given box lies on.
            The box is defined as centre/half-size pairs for effectively.
			@param centre The centre of the box.
			@param halfSize The half-size of the box.
			@returns
				POSITIVE_SIDE if the box complete lies on the "positive side" of the plane,
				NEGATIVE_SIDE if the box complete lies on the "negative side" of the plane,
				and BOTH_SIDE if the box intersects the plane.
        */
		Side getSide(const Vector3& centre, const Vector3& halfSize) const;

		/** This is a pseudodistance. The sign of the return value is
            positive if the point is on the positive side of the plane,
            negative if the point is on the negative side, and zero if the
            point is on the plane.
            @par
				The absolute value of the return value is the true distance only
				when the plane normal is a unit length vector.
        */
		Real getDistance(const Vector3& vrvPoint) const;

		/** Redefine this plane based on 3 points. */
		void redefine(const Vector3& vvrPoint0, const Vector3& vvrPiont1, const Vector3& vvrPoint2);

		/** Redefine this plane based on a normal and a point. */
		void redefine(const Vector3& vvrNormal, const Vector3& vvrPoint);

		/** Project a vector onto the plane. 
			@remarks This gives you the element of the input vector that is perpendicular 
				to the normal of the plane. You can get the element which is parallel
				to the normal of the plane by subtracting the result of this method
				from the original vector, since parallel + perpendicular = original.
			@param v The input vector
		*/
		Vector3 projectVector(const Vector3& p);

		/** Normalises the plane.
            @remarks
                This method normalises the plane's normal and the length scale of d
                is as well.
            @note
                This function will not crash for zero-sized vectors, but there
                will be no changes made to their components.
            @returns The previous length of the plane's normal.
        */
		Real normalise(void);

		/// comparison operator
		bool operator == (const Plane& vrps) const{
			// maybe a bug...
			return (m_fd == vrps.m_fd && m_vNormal == vrps.m_vNormal);
		}
		bool operator != (const Plane& vrps) const{
			// maybe a bug...
			return (vrps.m_fd != m_fd && vrps.m_vNormal != m_vNormal);
		}


		_SagittaExport friend std::ostream& operator << (std::ostream& o, const Plane& p);

	}; //#### end class Plane

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SPLANE_H__