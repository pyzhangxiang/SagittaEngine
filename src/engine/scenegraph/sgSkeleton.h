//////////////////////////////////////////////////////
// file: sgSkeleton.h @ 2008-8-28 by Zhang Xiang
// declares of the class sgSkeleton
// sgSkeleton is a class ...
//////////////////////////////////////////////////////
#ifndef __SGSKELETON_H__
#define __SGSKELETON_H__

// INCLUDES //////////////////////////////////////////
#include "sgSubScene.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SSkeletonDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgSkeleton : public sgSubScene{
	// enum declares
	public:
		// scene type
		enum SceneType{ ST_SKELETON = 1 };

	// member variables
	private:
		/// altitude of this skeleton
//		Real m_fAltitude;
		/// the lowest position of this skeleton
//		Real m_fGround;
	
	// constructors & destructor
	public:
		sgSkeleton(void);
		sgSkeleton(const StdString &aName);
		sgSkeleton(SSkeletonDelegate *aDelegate);
		sgSkeleton(const StdString &aName, SSkeletonDelegate *aDelegate);
		~sgSkeleton(void);

	// member functions
	public:
		/** Gets altitude. */
	//	Real altitude(void) const;

		/** Gets ground. */
//		Real ground(void) const;

		/** Sets altitude.
			@remarks Internal method. Called by SSkeletonDelegate.
		*/
//		void _setAltitude(Real aAltitude);

		/** Sets ground.
			@remarks Internal method. Called by SSkeletonDelegate.
		*/
//		void _setGround(Real aGround);
	
	}; //#### end class sgSkeleton

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSKELETON_H__


