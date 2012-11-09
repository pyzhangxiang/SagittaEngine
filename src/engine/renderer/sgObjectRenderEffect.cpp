//  [4/11/2012  zhangxiang]

#include "sgObjectRenderEffect.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderQueue.h"
#include "sgGpuProgram.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/buffer/sgBuffer.h"

namespace Sagitta
{
    SG_META_DEFINE(sgObjectRenderEffect, sgRenderEffect)

    sgObjectRenderEffect::sgObjectRenderEffect(void)
	: sgRenderEffect()
    {
        
    }
    
    sgObjectRenderEffect::~sgObjectRenderEffect(void)
    {
    }

	void sgObjectRenderEffect::render( sg_render::CurrentRenderParam *param, sgSceneObject *object )
	{
		for(size_t i=0; i<mPassList.size(); ++i)
		{
			sgRenderPass *rp = mPassList[i];

			this->setCurrentPass(i);

			sgGpuProgram *program = rp->getGpuProgram();
			if(!program || !program->isActive())
				continue;

			param->current_gpu_program = program;
			renderObject(param, object);
		}
	}

    

} // namespace Sagitta
