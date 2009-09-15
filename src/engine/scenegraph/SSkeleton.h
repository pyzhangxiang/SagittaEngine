//////////////////////////////////////////////////////
// file: SSkeleton.h @ 2008-8-28 by Zhang Xiang
// declares of the class SSkeleton
// SSkeleton is a class ...
//////////////////////////////////////////////////////
#ifndef __SSKELETON_H__
#define __SSKELETON_H__

// INCLUDES //////////////////////////////////////////
#include "SSubScene.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SSkeletonDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SSkeleton : public SSubScene{
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
		SSkeleton(void);
		SSkeleton(const StdString &aName);
		SSkeleton(SSkeletonDelegate *aDelegate);
		SSkeleton(const StdString &aName, SSkeletonDelegate *aDelegate);
		~SSkeleton(void);

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
	
	}; //#### end class SSkeleton

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SSKELETON_H__