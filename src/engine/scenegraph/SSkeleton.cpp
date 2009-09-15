//////////////////////////////////////////////////////
// file: SSkeleton.cpp @ 2008-8-28 by Zhang Xiang
// defines of the class SSkeleton
// SSkeleton is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SSkeleton.h"
#include "../resource/SSceneDelegate.h"
#include "../../common/utils/SString.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	SSkeleton::SSkeleton(void) :
	SSubScene(new SSkeletonDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	SSkeleton::SSkeleton(const StdString &aName) :
	SSubScene(aName, new SSkeletonDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	SSkeleton::SSkeleton(SSkeletonDelegate *aDelegate) :
	SSubScene(aDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	SSkeleton::SSkeleton(const StdString &aName, SSkeletonDelegate *aDelegate) :
	SSubScene(aName, aDelegate, ST_SKELETON)/*,
	m_fAltitude(0.0), m_fGround(0.0)*/{

	}

	//  [1/11/2009 zhangxiang]
	SSkeleton::~SSkeleton(void){

	}

	//  [1/12/2009 zhangxiang]
	/*Real SSkeleton::altitude(void) const{
		return m_fAltitude;
	}*/

	//  [1/12/2009 zhangxiang]
	/*Real SSkeleton::ground(void) const{
		return m_fGround;
	}*/

	//  [1/12/2009 zhangxiang]
	/*void SSkeleton::_setAltitude(Real aAltitude){
		m_fAltitude = aAltitude;
	}*/

	//  [1/12/2009 zhangxiang]
	/*void SSkeleton::_setGround(Real aGround){
		m_fGround = aGround;
	}*/

} // namespace Sagitta