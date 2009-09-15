//////////////////////////////////////////////////////
// file: SBoundingSphere.h 
// created by zhangxiang on 09-06-21
// declare of the class BoundingSphere
// BoundingSphere is a class ...
//////////////////////////////////////////////////////
#ifndef __SBOUNDINGSPHERE_H__
#define __SBOUNDINGSPHERE_H__

// INCLUDES //////////////////////////////////////////
#include "SMath.h"
#include "SVector3.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class AxisAlignedBox;
	class Plane;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _MathExport BoundingSphere{
	// member variables
	private:
		Real m_Radius;
		Vector3 m_Center;

	// constructors & destructor
	public:
		/** Standard constructor - creates a unit sphere around the origin.*/
		BoundingSphere(void) : m_Radius(1.0), m_Center(Vector3::ZERO){}

		/** Constructor allowing arbitrary spheres. 
			@param center The center point of the sphere.
			@param radius The radius of the sphere.
		*/
		BoundingSphere(const Vector3 &center, Real radius)
			: m_Radius(radius), m_Center(center) {}

	// member functions
	public:
		/** Returns the radius of the sphere. */
		Real radius(void) const{ return m_Radius; }

		/** Sets the radius of the sphere. */
		void setRadius(Real radius){ m_Radius = radius; }

		/** Returns the center point of the sphere. */
		const Vector3 &center(void) const{ return m_Center; }

		/** Sets the center point of the sphere. */
		void setCenter(const Vector3 &center){ m_Center = center; }

		/** Returns whether or not this sphere interects another sphere. */
		bool intersects(const BoundingSphere &s) const{
			return (s.m_Center - m_Center).squaredLength() <=
				Math::Sqr(s.m_Radius + m_Radius);
		}
		/** Returns whether or not this sphere interects a box. */
		bool intersects(const AxisAlignedBox &box) const{
			return Math::Intersects(*this, box);
		}
		/** Returns whether or not this sphere interects a plane. */
		bool intersects(const Plane &plane) const{
			return Math::Intersects(*this, plane);
		}
		/** Returns whether or not this sphere interects a point. */
		bool intersects(const Vector3 &v) const{
			return ((v - m_Center).squaredLength() <= Math::Sqr(m_Radius));
		}

	}; //#### end class BoundingSphere

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SBOUNDINGSPHERE_H__