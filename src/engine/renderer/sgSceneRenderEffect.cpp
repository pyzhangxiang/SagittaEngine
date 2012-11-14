//  [11/6/2012 fabiozhang]

#include "sgSceneRenderEffect.h"
#include "sgObjectRenderEffect.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderQueue.h"
#include "sgGpuProgram.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/buffer/sgBuffer.h"
#include "sgViewport.h"
#include "engine/buffer/sgVertexData.h"
#include "engine/buffer/sgVertexIndexBuffer.h"
#include "engine/buffer/sgVertexBufferElement.h"
#include "engine/common/sgException.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/component/sgLightComponent.h"
#include "engine/component/sgRenderStateComponent.h"
#include "engine/component/sgMeshComponent.h"
#include "engine/resource/sgMaterial.h"
#include "engine/resource/sgMesh.h"

namespace Sagitta
{
    SG_META_DEFINE(sgSceneRenderEffect, sgRenderEffect)

    sgSceneRenderEffect::sgSceneRenderEffect(void)
	: sgRenderEffect(), mCurrentRenderParam(NULL)
    {
        
    }
    
    sgSceneRenderEffect::~sgSceneRenderEffect(void)
    {
    }
    
	void sgSceneRenderEffect::render( sg_render::CurrentRenderParam *param )
	{
		if(mPassList.empty())
			return ;

		mCurrentRenderParam = param;
		mCurrentRenderParam->last_gpu_program = NULL;

		for(size_t i=0; i<mPassList.size(); ++i)
		{
			sgRenderPass *rp = mPassList[i];
			this->setCurrentPass(i);
            mCurrentRenderParam->scene_program_only = rp->isUseSceneProgramOnly();
            this->preRenderPass();
			
			sgGpuProgram *program = rp->getGpuProgram();
			if(!program || !program->isActive())
				continue;

			mCurrentRenderParam->current_gpu_program =
				mCurrentRenderParam->scene_gpu_program = program;

			mCurrentRenderParam->resetRenderQueue(rp->getRenderQueue());
			// cull objects
			mCurrentRenderParam->cullObjects(mCurrentRenderParam->pcamera);

			// render
			const sgRenderQueue::ObjectList &objects = mCurrentRenderParam->renderqueue->getObjectList();
			for(size_t i=0; i<objects.size(); ++i)
			{
                sgSceneObject *object = objects[i];
                
                sgRenderStateComponent *renderState = (sgRenderStateComponent*)(object->getComponent(sgRenderStateComponent::GetClassName()));
                sgObjectRenderEffect *re = NULL;
                if(renderState)
                {
                    re = renderState->getRenderEffect();
                }
                
                if(re)
                {
                    re->render(mCurrentRenderParam, object);
                }
                else
                {
                    renderObject(mCurrentRenderParam, object);
                }
                
			}

            this->postRenderPass();
			
		}

		// reset render parameters
		mCurrentRenderParam = NULL;
	}

} // namespace Sagitta
