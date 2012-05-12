//////////////////////////////////////////////////////
// file: sgAnimationDelegate.h
// created by zhangxiang on 09-01-10
// declare of the class sgAnimationDelegate
// sgAnimationDelegate is a class ...
//////////////////////////////////////////////////////
#ifndef __SGANIMATIONDELEGATE_H__
#define __SGANIMATIONDELEGATE_H__

// INCLUDES //////////////////////////////////////////
#include "sgResourceDelegate.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgMotionClip;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgAnimationDelegate : public sgResourceDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from sgResourceDelegate. */
		void loadImpl(const StdString &aFileName, sgResource *outResource);

	}; //#### end class sgAnimationDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgMotionClipDelegate : public sgAnimationDelegate{
	// member variables

	// constructors & destructor

	// member functions
	private:
		/** Internal hook does loading. Overrides from sgResourceDelegate. */
		void loadImpl(const StdString &aFileName, sgResource *outResource);

		/** Loads animation data from .bvh file. */
		void loadMotionClip_bvh(const StdString &aFileName, sgMotionClip *outMotionClip);

	}; //#### end class sgMotionClipDelegate

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGANIMATIONDELEGATE_H__
