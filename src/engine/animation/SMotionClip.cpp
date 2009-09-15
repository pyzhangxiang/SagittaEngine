//////////////////////////////////////////////////////
// file: SMotionClip.cpp 
// created by zhangxiang on 09-01-11
// define of the class SMotionClip
// SMotionClip is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SMotionClip.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	SMotionClip::SMotionClip(SMotionClipDelegate *aDelegate /* = new SMotionClipDelegate */) :
	SAnimation(aDelegate){

	}

	//  [1/11/2009 zhangxiang]
	SMotionClip::SMotionClip(const StdString &aName, SMotionClipDelegate * aDelegate /* = new SMotionClipDelegate */) :
	SAnimation(aName, aDelegate){

	}

	//  [1/11/2009 zhangxiang]
	SMotionClip::~SMotionClip(void){

	}

} // namespace Sagitta