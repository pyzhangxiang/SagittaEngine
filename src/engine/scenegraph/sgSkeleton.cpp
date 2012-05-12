//////////////////////////////////////////////////////
// file: sgSkeleton.cpp @ 2008-8-28 by Zhang Xiang
// defines of the class sgSkeleton
// sgSkeleton is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgSkeleton.h"
#include "../resource/sgSceneDelegate.h"
#include "../../common/utils/sgStringUtil.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	sgSkeleton::sgSkeleton(void) :
	sgSubScene(new SSkeletonDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	sgSkeleton::sgSkeleton(const StdString &aName) :
	sgSubScene(aName, new SSkeletonDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	sgSkeleton::sgSkeleton(SSkeletonDelegate *aDelegate) :
	sgSubScene(aDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	sgSkeleton::sgSkeleton(const StdString &aName, SSkeletonDelegate *aDelegate) :
	sgSubScene(aName, aDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	sgSkeleton::~sgSkeleton(void){

	}

	//  [1/12/2009 zhangxiang]
	/*Real sgSkeleton::altitude(void) const{
		return m_fAltitude;
	}*/

	//  [1/12/2009 zhangxiang]
	/*Real sgSkeleton::ground(void) const{
		return m_fGround;
	}*/

	//  [1/12/2009 zhangxiang]
	/*void sgSkeleton::_setAltitude(Real aAltitude){
		m_fAltitude = aAltitude;
	}*/

	//  [1/12/2009 zhangxiang]
	/*void sgSkeleton::_setGround(Real aGround){
		m_fGround = aGround;
	}*/

} // namespace Sagitta