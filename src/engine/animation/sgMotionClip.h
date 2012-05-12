//////////////////////////////////////////////////////
// file: sgMotionClip.h 
// created by zhangxiang on 09-01-11
// declare of the class sgMotionClip
// sgMotionClip is a class ...
//////////////////////////////////////////////////////
#ifndef __SGMOTIONCLIP_H__
#define __SGMOTIONCLIP_H__

// INCLUDES //////////////////////////////////////////
#include "sgAnimation.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgMotionClipDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgMotionClip : public sgAnimation{
	// member variables

	// constructors & destructor
	public:
		sgMotionClip(sgMotionClipDelegate *aDelegate = new sgMotionClipDelegate);
		sgMotionClip(const StdString &aName, sgMotionClipDelegate * aDelegate = new sgMotionClipDelegate);
		~sgMotionClip(void);

	// member functions

	}; //#### end class sgMotionClip

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGMOTIONCLIP_H__
