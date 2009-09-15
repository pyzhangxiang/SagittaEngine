//////////////////////////////////////////////////////
// file: SRay.h 
// created by zhangxiang on 09-06-21
// declare of the class Ray
// Ray is a class ...
//////////////////////////////////////////////////////
#ifndef __SRAY_H__
#define __SRAY_H__

// INCLUDES //////////////////////////////////////////
#include "SMath.h"
#include "SVector3.h"
#include "SPlaneBoundedVolume.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class Plane;
	class BoundingSphere;
	class AxisAlignedBox;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _MathExport Ray{
	// member variables
	private:
		Vector3 m_Origin;
		Vector3 m_Direction;

	// constructors & destructor
	public:
		Ray(void) : m_Origin(Vector3::ZERO), m_Direction(Vector3::UNIT_Z){}
		Ray(const Vector3 &origin, const Vector3 &direction)
		: m_Origin(origin), m_Direction(direction){

		}

	// member functions
	public:
		/** Sets the origin of the ray. */
		void setOrigin(const Vector3 &origin){ m_Origin = origin; } 
		/** Gets the origin of the ray. */
		const Vector3 &origin(void) const{ return m_Origin; } 

		/** Sets the direction of the ray. */
		void setDirection(const Vector3 &dir){ m_Direction = dir; } 
		/** Gets the direction of the ray. */
		const Vector3 &direction(void) const{ return m_Direction; }

		/** Gets the position of a point t units along the ray. */
		Vector3 getPoint(Real t) const{ 
			return Vector3(m_Origin + (m_Direction * t));
		}

		/** Gets the position of a point t units along the ray. */
		Vector3 operator *(Real t) const{ 
			return getPoint(t);
		};

		/** Tests whether this ray intersects the given plane. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, Real> intersects(const Plane& p) const{
			return Math::Intersects(*this, p);
		}
        /** Tests whether this ray intersects the given plane bounded volume. 
        @returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
        */
        std::pair<bool, Real> intersects(const PlaneBoundedVolume& p) const{
            return Math::Intersects(*this, p.m_Planes, p.m_Outside == Plane::POSITIVE_SIDE);
        }
		/** Tests whether this ray intersects the given sphere. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, Real> intersects(const BoundingSphere& s) const{
			return Math::Intersects(*this, s);
		}
		/** Tests whether this ray intersects the given box. 
		@returns A pair structure where the first element indicates whether
			an intersection occurs, and if true, the second element will
			indicate the distance along the ray at which it intersects. 
			This can be converted to a point in space by calling getPoint().
		*/
		std::pair<bool, Real> intersects(const AxisAlignedBox& box) const{
			return Math::Intersects(*this, box);
		}

	}; //#### end class Ray

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRAY_H__