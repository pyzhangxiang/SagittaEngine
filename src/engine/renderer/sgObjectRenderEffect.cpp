//  [4/11/2012  zhangxiang]

#include "sgObjectRenderEffect.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderQueue.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/buffer/sgBuffer.h"

namespace Sagitta
{
    SG_META_DEFINE_ABSTRACT(sgObjectRenderEffect, sgRenderEffect)

    sgObjectRenderEffect::sgObjectRenderEffect(void)
	: sgRenderEffect()
    {
        
    }
    
    sgObjectRenderEffect::~sgObjectRenderEffect(void)
    {
    }

	void sgObjectRenderEffect::render( sg_render::CurrentRenderParam *param, sgSceneObject *object )
	{

	}

    

} // namespace Sagitta
