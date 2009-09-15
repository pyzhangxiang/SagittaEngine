//////////////////////////////////////////////////////
// file: SMotionClip.h 
// created by zhangxiang on 09-01-11
// declare of the class SMotionClip
// SMotionClip is a class ...
//////////////////////////////////////////////////////
#ifndef __SMOTIONCLIP_H__
#define __SMOTIONCLIP_H__

// INCLUDES //////////////////////////////////////////
#include "SAnimation.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SMotionClipDelegate;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SMotionClip : public SAnimation{
	// member variables

	// constructors & destructor
	public:
		SMotionClip(SMotionClipDelegate *aDelegate = new SMotionClipDelegate);
		SMotionClip(const StdString &aName, SMotionClipDelegate * aDelegate = new SMotionClipDelegate);
		~SMotionClip(void);

	// member functions

	}; //#### end class SMotionClip

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SMOTIONCLIP_H__