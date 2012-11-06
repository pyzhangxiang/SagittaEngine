//  [11/6/2012 fabiozhang]

#include "sgSceneRenderEffect.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderQueue.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/buffer/sgBuffer.h"

namespace Sagitta
{
    SG_META_DEFINE_ABSTRACT(sgSceneRenderEffect, sgRenderEffect)

    sgSceneRenderEffect::sgSceneRenderEffect(void)
	: sgRenderEffect()
    {
        
    }
    
    sgSceneRenderEffect::~sgSceneRenderEffect(void)
    {
    }
    
	void sgSceneRenderEffect::render( sg_render::CurrentRenderParam *param, sgSceneObject *object )
	{
		if(mPassList.empty())
			return ;

		mCurrentRenderParam = param;

		for(size_t i=0; i<mPassList.size(); ++i)
		{
			sgRenderPass *rp = mPassList[i];
			
			mCurrentRenderParam->resetRenderQueue(rp->getRenderQueue());
			// cull objects
			mCurrentRenderParam->cullObjects(mCurrentRenderParam->pcamera);

			mCurrentRenderParam->last_gpu_program = NULL;
			mCurrentRenderParam->current_gpu_program =
				mCurrentRenderParam->scene_gpu_program = rp->getGpuProgram();

			setSceneUniforms();

			// render
			const sgRenderQueue::ObjectList &objects = mCurrentRenderParam->renderqueue->getObjectList();
			for(size_t i=0; i<objects.size(); ++i)
			{
				renderObject(objects[i]);
			}

			
		}

		// reset render parameters
		mCurrentRenderParam = NULL;
	}

	void sgSceneRenderEffect::renderObject( sgSceneObject *object )
	{

		mCurrentRenderParam->last_gpu_program = mCurrentRenderParam->current_gpu_program;
	}

} // namespace Sagitta
