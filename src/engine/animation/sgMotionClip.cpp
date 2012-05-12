//////////////////////////////////////////////////////
// file: sgMotionClip.cpp 
// created by zhangxiang on 09-01-11
// define of the class sgMotionClip
// sgMotionClip is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgMotionClip.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	sgMotionClip::sgMotionClip(sgMotionClipDelegate *aDelegate /* = new sgMotionClipDelegate */) :
	sgAnimation(aDelegate){

	}

	//  [1/11/2009 zhangxiang]
	sgMotionClip::sgMotionClip(const StdString &aName, sgMotionClipDelegate * aDelegate /* = new sgMotionClipDelegate */) :
	sgAnimation(aName, aDelegate){

	}

	//  [1/11/2009 zhangxiang]
	sgMotionClip::~sgMotionClip(void){

	}

} // namespace Sagitta