//  [25/11/2012  zhangxiang]

#include "sgRenderQueueCastShadow.h"
#include "engine/scenegraph/sgSceneObject.h"

namespace Sagitta
{
    SG_META_DEFINE(sgRenderQueueCastShadow, sgRenderQueue)

    sgRenderQueueCastShadow::sgRenderQueueCastShadow(void)
    {
        
    }
    
    sgRenderQueueCastShadow::~sgRenderQueueCastShadow(void)
    {
        
    }
        
	bool sgRenderQueueCastShadow::filter( sgSceneObject *object )
	{
		if(object->isCastShadow())
			return true;
		else
			return false;
	}


} // namespace Sagitta
