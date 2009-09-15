//////////////////////////////////////////////////////
// file: SAnimationDelegate.h
// created by zhangxiang on 09-01-10
// declare of the class SAnimationDelegate
// SAnimationDelegate is a class ...
//////////////////////////////////////////////////////
#ifndef __SANIMATIONDELEGATE_H__
#define __SANIMATIONDELEGATE_H__

// INCLUDES //////////////////////////////////////////
#include "SResourceDelegate.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SMotionClip;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SAnimationDelegate : public SResourceDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from SResourceDelegate. */
		void loadImpl(const StdString &aFileName, SResource *outResource);

	}; //#### end class SAnimationDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SMotionClipDelegate : public SAnimationDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from SResourceDelegate. */
		void loadImpl(const StdString &aFileName, SResource *outResource);

		/** Loads animation data from .bvh file. */
		void loadMotionClip_bvh(const StdString &aFileName, SMotionClip *outMotionClip);

	}; //#### end class SMotionClipDelegate

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SANIMATIONDELEGATE_H__