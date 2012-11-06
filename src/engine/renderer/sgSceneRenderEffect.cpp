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
    SG_META_DEFINE_ABSTRACT(sgSceneRenderEffect, sgRenderEffect)

    sgSceneRenderEffect::sgSceneRenderEffect(void)
	: sgRenderEffect(), mCurrentPass(0)
    {
        
    }
    
    sgSceneRenderEffect::~sgSceneRenderEffect(void)
    {
    }
    
	void sgSceneRenderEffect::renderScene( sg_render::CurrentRenderParam *param )
	{
		if(mPassList.empty())
			return ;

		mCurrentRenderParam = param;

		for(mCurrentPass=0; mCurrentPass<mPassList.size(); ++mCurrentPass)
		{
			sgRenderPass *rp = mPassList[mCurrentPass];
			
			mCurrentRenderParam->resetRenderQueue(rp->getRenderQueue());
			// cull objects
			mCurrentRenderParam->cullObjects(mCurrentRenderParam->pcamera);

			mCurrentRenderParam->last_gpu_program = NULL;
			mCurrentRenderParam->current_gpu_program =
				mCurrentRenderParam->scene_gpu_program = rp->getGpuProgram();

			setUniformScene();

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
                    re->renderObject(mCurrentRenderParam, object);
                }
                else
                {
                    renderObject(object);
                }
                
			}

			
		}

		// reset render parameters
		mCurrentRenderParam = NULL;
	}

	void sgSceneRenderEffect::renderObject( sgSceneObject *object )
	{
        sgMeshComponent *meshComp = (sgMeshComponent*)(object->getComponent(sgMeshComponent::GetClassName()));
        if(!meshComp)
            return ;
        sgMesh *mesh = meshComp->getMesh();
        if(!mesh)
            return ;
        
        if(mCurrentRenderParam->last_gpu_program != mCurrentRenderParam->scene_gpu_program)
        {
            mCurrentRenderParam->current_gpu_program = mCurrentRenderParam->scene_gpu_program;
            mCurrentRenderParam->current_gpu_program->useProgram();
            setUniformScene();
        }
        setUniformObject(object);

        sgRenderStateComponent *renderState = (sgRenderStateComponent*)(object->getComponent(sgRenderStateComponent::GetClassName()));
        sgMaterial *material = 0;
        if(renderState)
            material = (sgMaterial*)(renderState->getMaterial());
        
        sgVertexData *pvb = mesh->getVertexData(); //new sgVertexData();
		sgVertexIndexBuffer *pvib = mesh->getVertexIndexBuffer(); //new sgVertexIndexBuffer(sgVertexBufferElement::ET_VERTEX);
        //	mesh->getVertexBuffer(pvb, pvib);
        const Matrix4 &modelMatrix = object->getFullTransform();
        
        sgGetRenderer()->renderProgramPipeline(pvb, pvib, modelMatrix, mesh->polyType());
        
        mCurrentRenderParam->last_gpu_program = mCurrentRenderParam->current_gpu_program;
	}

} // namespace Sagitta
