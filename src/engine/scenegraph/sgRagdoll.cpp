
#include "sgRagdoll.h"
#include "sgSceneObject.h"
#include "sgBoneObject.h"
#include "engine/component/sgAnimationComponent.h"
#include "engine/resource/sgAnimation.h"
#include "engine/resource/sgAnimationJoint.h"

namespace Sagitta{
    
	
	SG_META_DEFINE(sgRagdoll, sgObject)
    
	//  [1/1/2009 zhangxiang]
	sgRagdoll::sgRagdoll(void)
	: sgObject()
    {
		
	}
    
    
	//  [1/1/2009 zhangxiang]
	sgRagdoll::~sgRagdoll(void)
    {
        
	}
    
	
} // namespace Sagitta
