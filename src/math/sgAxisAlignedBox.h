//////////////////////////////////////////////////////
// file: sgAxisAlignedBox.h @ 2008-1-28 by Zhang Xiang
// declares of the class AxisAlignedBox
// AxisAlignedBox is a class ...
//////////////////////////////////////////////////////
#ifndef __SGAXISALIGNEDBOX_H__
#define __SGAXISALIGNEDBOX_H__

// INCLUDES //////////////////////////////////////////
#include "sgVector3.h"
#include "sgMatrix4.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class Plane;
	class BoundingSphere;

	/** A 3D box aligned with the x/y/z axes.
		@remarks
			This class represents a simple box which is aligned with the
			axes. Internally it only stores 2 points as the extremeties of
			the box, one which is the minima of all 3 axes, and the other
			which is the maxima of all 3 axes. This class is typically used
			for an axis-aligned bounding box (AABB) for collision and
			visibility determination.
	*/
	class _SG_MathExport AxisAlignedBox{
	// enum declares
	protected:
		enum Extent{
			EXTENT_NULL,
			EXTENT_FINITE,
			EXTENT_INFINITE
		};

	public:
		/**
		   1-----2
		  /|    /|
		 / |   / |
		5-----4  |
		|  0--|--3
		| /   | /
		|/    |/
		6-----7
		*/
		typedef enum{
			FAR_LEFT_BOTTOM = 0,
			FAR_LEFT_TOP = 1,
			FAR_RIGHT_TOP = 2,
			FAR_RIGHT_BOTTOM = 3,
			NEAR_RIGHT_BOTTOM = 7,
			NEAR_LEFT_BOTTOM = 6,
			NEAR_LEFT_TOP = 5,
			NEAR_RIGHT_TOP = 4
		} CornerEnum;

	// member variables
	protected:
		Vector3 m_vMinimum;
		Vector3 m_vMaximum;
		Extent m_eExtent;
		mutable Vector3 *m_pvCorners;

	// constructors & destructor
	public:
		inline AxisAlignedBox() : m_pvCorners(0){
			// Default to a null box
			setMinimum(-0.5f, -0.5f, -0.5f);
			setMaximum(0.5f, 0.5f, 0.5f);
			m_eExtent = EXTENT_NULL;
		}

		inline AxisAlignedBox(const AxisAlignedBox& vraBox) : m_pvCorners(0){
			if(vraBox.isNull())
				setNull();
			else if(vraBox.isInfinite())
				setInfinite();
			else{
				// maybe a bug...
				setExtents(vraBox.m_vMinimum, vraBox.m_vMaximum);
			}
		}

		inline AxisAlignedBox(const Vector3& min, const Vector3& max) : m_pvCorners(0){
			setExtents(min, max);
		}

		inline AxisAlignedBox(Real mx, Real my, Real mz,
			Real Mx, Real My, Real Mz) : m_pvCorners(0){
				setExtents(mx, my, mz, Mx, My, Mz);
		}

		inline AxisAlignedBox& operator = (const AxisAlignedBox& vras){
			// Specifically override to avoid copying mpCorners
			if(vras.isNull())
				setNull();
			else if(vras.isInfinite())
				setInfinite();
			else
				setExtents(vras.m_vMinimum, vras.m_vMaximum);

			return *this;
		}

		~AxisAlignedBox(){
			if(m_pvCorners)
				delete []m_pvCorners;
		}

		// member functions
	public:
		/** Gets the minimum corner of the box. */							
		inline const Vector3& getMinimum(void) const{						
			return m_vMinimum;												
		}																	
																			
		/** Gets a modifiable version of the minimum corner of the box. */	
		inline Vector3& getMinimum(void){									
			return m_vMinimum;												
		}																	

		/** Gets the maximum corner of the box. */
		inline const Vector3& getMaximum(void) const{
			return m_vMaximum;
		}

		/** Gets a modifiable version of the maximum corner of the box. */
		inline Vector3& getMaximum(void){
			return m_vMaximum;
		}

		/** Sets the minimum corner of the box. */
		inline void setMinimum(const Vector3& vrvc){
			m_eExtent = EXTENT_FINITE;
			m_vMinimum = vrvc;
		}

		inline void setMinimum(Real x, Real y, Real z){
			m_eExtent = EXTENT_FINITE;
			m_vMinimum.setX(x);
			m_vMinimum.setY(y);
			m_vMinimum.setZ(z);
		}

		/** Changes one of the components of the minimum corner of the box
			used to resize only one dimension of the box. */
		inline void setMinimumX(Real x){
			m_vMinimum.setX(x);
		}

		inline void setMinimumY(Real y){
			m_vMinimum.setY(y);
		}

		inline void setMinimumZ(Real z){
			m_vMinimum.setZ(z);
		}

		/** Sets the maximum corner of the box. */
		inline void setMaximum(const Vector3& vrvc){
			m_eExtent = EXTENT_FINITE;
			m_vMaximum = vrvc;
		}

		inline void setMaximum(Real x, Real y, Real z){
			m_eExtent = EXTENT_FINITE;
			m_vMaximum.setX(x);
			m_vMaximum.setY(y);
			m_vMaximum.setZ(z);
		}

		/** Changes one of the components of the maximum corner of the box
			used to resize only one dimension of the box. */
		inline void setMaximumX(Real x){
			m_vMaximum.setX(x);
		}

		inline void setMaximumY(Real y){
			m_vMaximum.setY(y);
		}

		inline void setMaximumZ(Real z){
			m_vMaximum.setZ(z);
		}

		/** Sets both minimum and maximum extents at once. */
		inline void setExtents(const Vector3& min, const Vector3& max){
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
			assert((min.x() <= max.x() && min.y() <= max.y() && min.z() <= max.z()) &&								///
				"The minimum corner of the box must be less than or equal to maximum corner");			///
			// add error code...																		///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
			m_eExtent = EXTENT_FINITE;
			m_vMinimum = min;
			m_vMaximum = max;
		}

		inline void setExtents(Real mx, Real my, Real mz,
			Real Mx, Real My, Real Mz){
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
			assert((mx <= Mx && my <= My && mz <= Mz) &&												///
				"The minimum corner of the box must be less than or equal to maximum corner");			///
			// add error code...																		///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
			m_eExtent = EXTENT_FINITE;

			m_vMinimum.setX(mx);
			m_vMinimum.setY(my);
			m_vMinimum.setZ(mz);

			m_vMaximum.setX(Mx);
			m_vMaximum.setY(My);
			m_vMaximum.setZ(Mz);

		}

		/** Returns a pointer to an array of 8 corner points, useful for
			collision vs. non-aligned objects.
			@remarks
				If the order of these corners is important, they are as
				follows: The 4 points of the minimum Z face (note that
				because Ogre uses right-handed coordinates, the minimum Z is
				at the 'back' of the box) starting with the minimum point of
				all, then anticlockwise around this face (if you are looking
				onto the face from outside the box). Then the 4 points of the
				maximum Z face, starting with maximum point of all, then
				anticlockwise around this face (looking onto the face from
				outside the box). Like this:
				<pre>
				   1-----2
				  /|    /|
				 / |   / |
				5-----4  |
				|  0--|--3
				| /   | /
				|/    |/
				6-----7
				</pre>
			@remarks as this implementation uses a static member, make sure to use your own copy !
		*/
		inline const Vector3* getAllCorners(void) const{
#ifdef _DEBUG
			// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
			assert((m_eExtent == EXTENT_FINITE) &&																	///
				"Can't get corners of a null or infinite AAB");														///
			// add error code...																					///
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
			// The order of these items is, using right-handed co-ordinates:
			// Minimum Z face, starting with Min(all), then anticlockwise
			//   around face (looking onto the face)
			// Maximum Z face, starting with Max(all), then anticlockwise
			//   around face (looking onto the face)
			// Only for optimization/compatibility.
			if(!m_pvCorners)
				m_pvCorners = new Vector3[8];

			m_pvCorners[0] = m_vMinimum;
			m_pvCorners[1].setX(m_vMinimum.x()); m_pvCorners[1].setY(m_vMaximum.y()); m_pvCorners[1].setZ(m_vMinimum.z());
			m_pvCorners[2].setX(m_vMaximum.x()); m_pvCorners[2].setY(m_vMaximum.y()); m_pvCorners[2].setZ(m_vMinimum.z());
			m_pvCorners[3].setX(m_vMaximum.x()); m_pvCorners[3].setY(m_vMinimum.y()); m_pvCorners[3].setZ(m_vMinimum.z());

			m_pvCorners[4] = m_vMaximum;
			m_pvCorners[5].setX(m_vMinimum.x()); m_pvCorners[5].setY(m_vMaximum.y()); m_pvCorners[5].setZ(m_vMaximum.z());
			m_pvCorners[6].setX(m_vMinimum.x()); m_pvCorners[6].setY(m_vMinimum.y()); m_pvCorners[6].setZ(m_vMaximum.z());
			m_pvCorners[7].setX(m_vMaximum.x()); m_pvCorners[7].setY(m_vMinimum.y()); m_pvCorners[7].setZ(m_vMaximum.z());

			return m_pvCorners;
		}

		/** Gets the position of one of the corners. */
		Vector3 getCorner(CornerEnum cornerToGet) const{
			switch(cornerToGet){
				case FAR_LEFT_BOTTOM:
					return m_vMinimum;
				case FAR_LEFT_TOP:
					return Vector3(m_vMinimum.x(), m_vMaximum.y(), m_vMinimum.z());
				case FAR_RIGHT_TOP:
					return Vector3(m_vMaximum.x(), m_vMaximum.y(), m_vMinimum.z());
				case FAR_RIGHT_BOTTOM:
					return Vector3(m_vMaximum.x(), m_vMinimum.y(), m_vMinimum.z());
				case NEAR_RIGHT_BOTTOM:
					return Vector3(m_vMaximum.x(), m_vMinimum.y(), m_vMaximum.z());
				case NEAR_LEFT_BOTTOM:
					return Vector3(m_vMinimum.x(), m_vMinimum.y(), m_vMaximum.z());
				case NEAR_LEFT_TOP:
					return Vector3(m_vMinimum.x(), m_vMaximum.y(), m_vMaximum.z());
				case NEAR_RIGHT_TOP:
					return m_vMaximum;
				default:
					return Vector3();
			}
		}

		_SG_MathExport friend std::ostream& operator<<(std::ostream& o, const AxisAlignedBox& aab){
			switch(aab.m_eExtent){
				case EXTENT_NULL:
					o << "AxisAlignedBox(null)";
					return o;
				case EXTENT_FINITE:
					o << "AxisAlignedBox(min=" <<aab.m_vMinimum << ", max=" << aab.m_vMaximum << ")";
					return o;
				case EXTENT_INFINITE:
					o << "AxisAlignedBox(infinite)";
					return o;
				default:	// shut up compiler
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
					assert(false && "Never reached");													///
					// add error code...																///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
					return o;
			}
		}

		/** Merges the passed in box into the current box. The result is the
			box which encompasses both. */
		inline void merge(const AxisAlignedBox& vras){
			if(vras.m_eExtent == EXTENT_NULL || m_eExtent == EXTENT_INFINITE){
				// Do nothing if vras null, or this is infinite.
				return ;

			}else if(vras.m_eExtent == EXTENT_INFINITE){
				// Otherwise if vras is infinite, make this infinite, too.
				m_eExtent = EXTENT_INFINITE;

			}else if(m_eExtent == EXTENT_NULL){
				// Otherwise if current null, just take vras.
				setExtents(vras.getMinimum(), vras.getMaximum());

			}else{
				// Otherwise merge.
				Vector3 min = m_vMinimum;
				Vector3 max = m_vMaximum;
				max.makeCeil(vras.getMaximum());
				min.makeFloor(vras.getMinimum());

				setExtents(min, max);
			}
		}

		/** Extends the box to ecompass the specified point(if needed). */
		inline void merge(const Vector3& point){
			switch(m_eExtent){
				case EXTENT_NULL: // if null, use this point
					setExtents(point, point);
					return ;

				case EXTENT_FINITE:
					m_vMaximum.makeCeil(point);
					m_vMinimum.makeFloor(point);
					return ;

				case EXTENT_INFINITE: // if infinite, makes no difference
					return ;
				
				default:
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
					assert(false && "Never reached");													///
					// add error code...																///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
					return ;
			}
		}

		/** Transforms the box according to the matrix supplied.
			@remarks
				By calling this method you get the axis-aligned box which
				surrounds the transformed version of this box. Therefore each
				corner of the box is transformed by the matrix, then the
				extents are mapped back onto the axes to produce another
				AABB. Useful when you have a local AABB for an object which
				is then transformed.
		*/
		inline void transform(const Matrix4& matrix){
			// Do nothing if current null or infinite
			if( m_eExtent != EXTENT_FINITE )
				return;

			Vector3 oldMin, oldMax, currentCorner;

			// Getting the old values so that we can use the existing merge method.
			oldMin = m_vMinimum;
			oldMax = m_vMaximum;

			// We sequentially compute the corners in the following order :
			// 0, 6, 5, 1, 2, 4 ,7 , 3
			// This sequence allows us to only change one member at a time to get at all corners.

			// For each one, we transform it using the matrix
			// Which gives the resulting point and merge the resulting point.

			// First corner 
			// min min min
			currentCorner = oldMin;
			merge(matrix * currentCorner);

			// min,min,max
			currentCorner.setZ(oldMax.z());
			merge(matrix * currentCorner);

			// min max max
			currentCorner.setY(oldMax.y());
			merge(matrix * currentCorner);

			// min max min
			currentCorner.setZ(oldMin.z());
			merge(matrix * currentCorner);

			// max max min
			currentCorner.setX(oldMax.x());
			merge(matrix * currentCorner);

			// max max max
			currentCorner.setZ(oldMax.z());
			merge(matrix * currentCorner);

			// max min max
			currentCorner.setY(oldMin.y());
			merge(matrix * currentCorner);

			// max min min
			currentCorner.setZ(oldMin.z());
			merge(matrix * currentCorner); 
		}
		

		/** Transforms the box according to the affine matrix supplied.
			@remarks
				By calling this method you get the axis-aligned box which
				surrounds the transformed version of this box. Therefore each
				corner of the box is transformed by the matrix, then the
				extents are mapped back onto the axes to produce another
				AABB. Useful when you have a local AABB for an object which
				is then transformed.
			@note
				The matrix must be an affine matrix. @see Matrix4::isAffine.
		*/
		inline void transformAffine(const Matrix4& m){
			assert(m.isAffine());

			// Do nothing if current null or infinite
			if( m_eExtent != EXTENT_FINITE )
				return;

			Vector3 centre = getCenter();
			Vector3 halfSize = getHalfSize();

			Vector3 newCentre = m.transformAffine(centre);
			Vector3 newHalfSize(
				Math::Abs(m[0][0]) * halfSize.x() + Math::Abs(m[0][1]) * halfSize.y() + Math::Abs(m[0][2]) * halfSize.z(), 
				Math::Abs(m[1][0]) * halfSize.x() + Math::Abs(m[1][1]) * halfSize.y() + Math::Abs(m[1][2]) * halfSize.z(),
				Math::Abs(m[2][0]) * halfSize.x() + Math::Abs(m[2][1]) * halfSize.y() + Math::Abs(m[2][2]) * halfSize.z());

			setExtents(newCentre - newHalfSize, newCentre + newHalfSize);
		}
		

		/** Sets the box to a "null" value i.e. not a box. */
		inline void setNull(){
			m_eExtent = EXTENT_NULL;
		}

		/** Returns true if the box is null i.e. empty. */
		inline bool isNull(void) const{
			return (m_eExtent == EXTENT_NULL);
		}

		/** Returns true if the box is finite. */
		inline bool isFinite(void) const{
			return (m_eExtent == EXTENT_FINITE);
		}

		/** Returns true if the box is finite. */
		inline bool ifFinite(void) const{
			return (m_eExtent == EXTENT_FINITE);
		}

		/** Sets the box to "infinite". */
		inline void setInfinite(){
			m_eExtent = EXTENT_INFINITE;
		}

		/** Returns true if the box is infinite. */
		inline bool isInfinite(void) const{
			return (m_eExtent == EXTENT_INFINITE);
		}

		/** Return whether or not this box intersects another. */
		inline bool intersects(const AxisAlignedBox& b2) const{
			// Early-fail for nulls
			if(this->isNull() || b2.isNull())
				return false;

			// Early-success for infinites
			if(this->isInfinite() || b2.isInfinite())
				return true;

			// Use up to 6 separating planes.
			if(m_vMaximum.x() < b2.m_vMinimum.x())
				return false;
			if(m_vMaximum.y() < b2.m_vMinimum.y())
				return false;
			if(m_vMaximum.z() < b2. m_vMinimum.z())
				return false;

			if(m_vMinimum.x() > b2.m_vMaximum.x())
				return false;
			if(m_vMinimum.y() > b2.m_vMaximum.y())
				return false;
			if(m_vMinimum.z() > b2.m_vMaximum.z())
				return false;

			// otherwise, must be intersecting
			return true;
		}

		/** Calculate the area of intersection of this box and another. */
		inline AxisAlignedBox intersection(const AxisAlignedBox& b2) const{
			if(this->isNull() || b2.isNull()){
				return AxisAlignedBox();
			}else if(this->isInfinite()){
				return b2;
			}else if(b2.isInfinite()){
				return *this;
			}

			Vector3 intMin = m_vMinimum;
			Vector3 intMax = m_vMaximum;

			intMin.makeCeil(b2.getMinimum());
			intMax.makeFloor(b2.getMaximum());

			// Check intersection isn't null
			if(intMin.x() < intMax.x() && intMin.y() < intMax.y() && intMin.z() < intMax.z()){
				return AxisAlignedBox(intMin, intMax);
			}

			return AxisAlignedBox();
		}

		/** Calculate the volume of this box. */
		Real volume(void) const{
			Vector3 diff = m_vMaximum - m_vMinimum;
			switch(m_eExtent){
				case EXTENT_NULL:
					return 0.0f;

				case EXTENT_FINITE:
					return diff.x() * diff.y() * diff.z();

				case EXTENT_INFINITE:
					return Math::POS_INFINITY;

				default: // shut up compiler
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
					assert(false && "Never reached");													///
					// add error code...																///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
					return 0.0f;
			}
		}

		/** Scales the AABB by vector given. */
		inline void scale(const Vector3& s){
			// Do nothing if current null or infinite
			if(m_eExtent != EXTENT_FINITE)
				return ;

			// NB assumes centered on origin.
			Vector3 min = m_vMinimum * s;
			Vector3 max = m_vMaximum * s;
			setExtents(min, max);
		}

		/// Tests whether this box inersects a sphere.
		inline bool intersects(const BoundingSphere& s) const{
			return Math::Intersects(s, *this);
		}

		/// Tests whether this box intersects a plane.
		inline bool intersects(const Plane& p) const{
			return Math::Intersects(p, *this);
		}

		/** Tests whether the vector point is within this box. */
		inline bool intersects(const Vector3& v) const{
			switch(m_eExtent){
				case EXTENT_NULL:
					return false;

				case EXTENT_FINITE:
					return (v.x() >= m_vMinimum.x() && v.x() <= m_vMaximum.x() &&
						v.y() >= m_vMinimum.y() && v.y() <= m_vMaximum.y() &&
						v.z() >= m_vMinimum.z() && v.z() <= m_vMaximum.z());

				case EXTENT_INFINITE:
					return true;

				default:
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
					assert(false && "Never reached");													///
					// add error code...																///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
					return false;
			}
		}

		/** Gets the centre of the box. */
		inline Vector3 getCenter(void) const{
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
			assert((m_eExtent == EXTENT_FINITE) && "Can't get center of a null or infinite AAB");		///													///
			// add error code...																		///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
			return Vector3((m_vMaximum.x() + m_vMinimum.x()) * 0.5f,
				(m_vMaximum.y() + m_vMinimum.y()) * 0.5f,
				(m_vMaximum.z() + m_vMinimum.z()) * 0.5f);
		}

		/** Gets the size of the box. */
		inline Vector3 getSize(void) const{
			switch(m_eExtent){
				case EXTENT_NULL:
					return Vector3::ZERO;

				case EXTENT_FINITE:
					return m_vMaximum - m_vMinimum;

				case EXTENT_INFINITE:
					return Vector3(Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);

				default: // shut up compiler
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
					assert(false && "Never reached");													///
					// add error code...																///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
					return Vector3::ZERO;
			}
		}

		/** Gets the half-size of the box. */
		inline Vector3 getHalfSize(void) const{
			switch(m_eExtent){
				case EXTENT_NULL:
					return Vector3::ZERO;

				case EXTENT_FINITE:
					return (m_vMaximum - m_vMinimum) * 0.5f;

				case EXTENT_INFINITE:
					return Vector3(Math::POS_INFINITY, Math::POS_INFINITY, Math::POS_INFINITY);

				default:
#ifdef _DEBUG
// ???... /////////////////////////////////////////////////////////////////////////////////////////////////
					assert(false && "Never reached");													///
					// add error code...																///
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
					return Vector3::ZERO;
			}
		}

		/** Tests whether the given point contained by this box. */
		inline bool contains(const Vector3& v) const{
			if(isNull())
				return false;
			if(isInfinite())
				return true;

			return (v.x() >= m_vMinimum.x() && v.x() <= m_vMaximum.x() &&
				v.y() >= m_vMinimum.y() && v.y() <= m_vMaximum.y() &&
				v.z() >= m_vMinimum.z() && v.z() <= m_vMaximum.z());
		}

		/** Tests whether another box contained by this box. */
		inline bool contains(const AxisAlignedBox& other) const{
			if(other.isNull() || this->isInfinite())
				return true;

			if(this->isNull() || other.isInfinite())
				return false;

			return (this->m_vMinimum.x() <= other.m_vMinimum.x() &&
				   this->m_vMinimum.y() <= other.m_vMinimum.y() &&
				   this->m_vMinimum.z() <= other.m_vMinimum.z() &&
				   other.m_vMaximum.x() <= this->m_vMaximum.x() &&
				   other.m_vMaximum.y() <= this->m_vMaximum.y() &&
				   other.m_vMaximum.z() <= this->m_vMaximum.z());
		}

		/** Tests 2 boxes for equality. */
		inline bool operator == (const AxisAlignedBox& vras) const{
			if(this->m_eExtent != vras.m_eExtent)
				return false;

			if(!this->ifFinite())
				return true;

			return this->m_vMinimum == vras.m_vMinimum &&
					this->m_vMaximum == vras.m_vMaximum;
		}

		/** Tests 2 boxes for inequality. */
		inline bool operator != (const AxisAlignedBox& vras) const{
			return !(*this == vras);
		}

	}; //#### end class AxisAlignedBox

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGAXISALIGNEDBOX_H__
