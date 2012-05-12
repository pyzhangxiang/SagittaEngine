//////////////////////////////////////////////////////
// file: sgPlaneBoundedVolume.h 
// created by zhangxiang on 09-06-21
// declare of the class PlaneBoundedVolume
// PlaneBoundedVolume is a class ...
//////////////////////////////////////////////////////
#ifndef __SGPLANEBOUNDEDVOLUME_H__
#define __SGPLANEBOUNDEDVOLUME_H__

// INCLUDES //////////////////////////////////////////
#include "sgMath.h"
#include "sgPlane.h"
#include "sgBoundingSphere.h"
#include "sgAxisAlignedBox.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_MathExport PlaneBoundedVolume{
	// type defines
	protected:
		typedef std::vector<Plane> PlaneList;

	// member variables
	public:
		PlaneList m_Planes;
		Plane::Side m_Outside;

	// constructors & destructor
	public:
		PlaneBoundedVolume(void) : m_Outside(Plane::NEGATIVE_SIDE){}
		/** Constructor, determines which side is deemed to be 'outside' */
		PlaneBoundedVolume(Plane::Side theOutside) : m_Outside(theOutside){}

	// member functions
	public:
		/** Intersection test with AABB
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const AxisAlignedBox &box) const{
            if(box.isNull()) return false;
            if(box.isInfinite()) return true;

            // Get centre of the box
            Vector3 centre = box.getCenter();
            // Get the half-size of the box
            Vector3 halfSize = box.getHalfSize();
            
            PlaneList::const_iterator i, iend;
            iend = m_Planes.end();
            for(i=m_Planes.begin(); i!=iend; ++i){
                const Plane& plane = *i;

                Plane::Side side = plane.getSide(centre, halfSize);
                if(side == m_Outside){
                    // Found a splitting plane therefore return not intersecting
                    return false;
                }
            }

            // couldn't find a splitting plane, assume intersecting
            return true;
        }

        /** Intersection test with Sphere
        @remarks May return false positives but will never miss an intersection.
        */
        inline bool intersects(const BoundingSphere &sphere) const{
            PlaneList::const_iterator i, iend;
            iend = m_Planes.end();
            for(i=m_Planes.begin(); i!=iend; ++i){
                const Plane& plane = *i;

                // Test which side of the plane the sphere is
                Real d = plane.getDistance(sphere.center());
                // Negate d if planes point inwards
                if(m_Outside == Plane::NEGATIVE_SIDE) d = -d;

                if((d - sphere.radius()) > 0)
                    return false;
            }

            return true;
        }

        /** Intersection test with a Ray
        @returns std::pair of hit (bool) and distance
        @remarks May return false positives but will never miss an intersection.
        */
        inline std::pair<bool, Real> intersects(const Ray &ray){
            return Math::Intersects(ray, m_Planes, m_Outside == Plane::POSITIVE_SIDE);
        }

	}; //#### end class PlaneBoundedVolume

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGPLANEBOUNDEDVOLUME_H__
