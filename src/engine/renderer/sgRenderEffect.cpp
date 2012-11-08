//  [4/11/2012  zhangxiang]

#include "sgRenderEffect.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderQueue.h"
#include "sgGpuProgram.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/buffer/sgBuffer.h"
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
    SG_META_DEFINE_ABSTRACT(sgRenderEffect, sgObject)

	const sgStrHandle sgRenderEffect::Light0_Position("Light0_Position");
	const sgStrHandle sgRenderEffect::Light0_Ambient("Light0_Ambient");
	const sgStrHandle sgRenderEffect::Light0_Diffuse("Light0_Diffuse");
	const sgStrHandle sgRenderEffect::Light0_Specular("Light0_Specular");
	const sgStrHandle sgRenderEffect::Light1_Position("Light1_Position");
	const sgStrHandle sgRenderEffect::Light1_Ambient("Light1_Ambient");
	const sgStrHandle sgRenderEffect::Light1_Diffuse("Light1_Diffuse");
	const sgStrHandle sgRenderEffect::Light1_Specular("Light1_Specular");
	const sgStrHandle sgRenderEffect::Light2_Position("Light2_Position");
	const sgStrHandle sgRenderEffect::Light2_Ambient("Light2_Ambient");
	const sgStrHandle sgRenderEffect::Light2_Diffuse("Light2_Diffuse");
	const sgStrHandle sgRenderEffect::Light2_Specular("Light2_Specular");
	const sgStrHandle sgRenderEffect::Light3_Position("Light3_Position");
	const sgStrHandle sgRenderEffect::Light3_Ambient("Light3_Ambient");
	const sgStrHandle sgRenderEffect::Light3_Diffuse("Light3_Diffuse");
	const sgStrHandle sgRenderEffect::Light3_Specular("Light3_Specular");

    sgRenderEffect::sgRenderEffect(void)
	: sgObject()
    {
        
    }
    
    sgRenderEffect::~sgRenderEffect(void)
    {
        for(size_t i=0; i<mPassList.size(); ++i)
        {
            delete mPassList[i];
        }
        mPassList.clear();
        
        FrameUniformMap::iterator it = mFrameUniformMap.begin();
        for(; it!=mFrameUniformMap.end(); ++it)
        {
            delete it->second.data;
        }
        mFrameUniformMap.clear();
    }
    
    void sgRenderEffect::addPass(const sgStrHandle &queueType)
    {
        sgRenderPass *rp = new sgRenderPass(queueType);
        mPassList.push_back(rp);
    }
    
    sgRenderPass *sgRenderEffect::getRenderPass(size_t index) const
    {
        if(index >= mPassList.size())
            return 0;
        
        return mPassList[index];
    }

	void sgRenderEffect::addFrameUniform( const sgStrHandle &dataName, FrameUniform uniform )
	{
		if(!uniform.data)
			return ;

		sgBuffer *refdata = new sgBuffer(*(uniform.data), 0);

		FrameUniformMap::iterator it = mFrameUniformMap.find(dataName);
		if(it != mFrameUniformMap.end())
		{
			delete it->second.data;
			mFrameUniformMap.erase(it);
		}

		uniform.data = refdata;
		mFrameUniformMap.insert(std::make_pair(dataName, uniform));
	}

	void sgRenderEffect::renderObject( sg_render::CurrentRenderParam *param, sgSceneObject *object )
	{
		sgMeshComponent *meshComp = (sgMeshComponent*)(object->getComponent(sgMeshComponent::GetClassName()));
		if(!meshComp)
			return ;
		sgMesh *mesh = meshComp->getMesh();
		if(!mesh)
			return ;

		if(param->last_gpu_program != param->current_gpu_program)
		{
			param->current_gpu_program->useProgram();
			setUniformFrame(param);
		}
		setUniformObject(param, object);

		sgRenderStateComponent *renderState = (sgRenderStateComponent*)(object->getComponent(sgRenderStateComponent::GetClassName()));
		sgMaterial *material = 0;
		if(renderState)
			material = (sgMaterial*)(renderState->getMaterial());

		sgVertexData *pvb = mesh->getVertexData(); //new sgVertexData();
		sgVertexIndexBuffer *pvib = mesh->getVertexIndexBuffer(); //new sgVertexIndexBuffer(sgVertexBufferElement::ET_VERTEX);
		//	mesh->getVertexBuffer(pvb, pvib);
		const Matrix4 &modelMatrix = object->getFullTransform();

		sgGetRenderer()->renderProgramPipeline(pvb, pvib, modelMatrix, mesh->polyType());

		param->last_gpu_program = param->current_gpu_program;
	}

	void sgRenderEffect::setUniformFrame( sg_render::CurrentRenderParam *param )
	{
		if(!param)
			return ;

		FrameUniformMap::iterator it = mFrameUniformMap.begin();
		for(; it!=mFrameUniformMap.end(); ++it)
		{
			param->current_gpu_program->setParameter(it->first, it->second.extra, it->second.data);
		}
		
		// light data
		sgBuffer *lightData = new sgBuffer(sizeof(Vector4));
		Vector4 &vec4 = *((Vector4*)(lightData->data()));
		for(size_t i=0; i<param->lightlist.size() && i<4; ++i)
		{
			sgLightComponent *light = param->lightlist[i];
			if(i == 0)
			{
				vec4 = Vector4(light->getParent()->position());
				param->current_gpu_program->setParameter(Light0_Position, 1, lightData);
				
				vec4 = light->ambientColor().toGLColor();
				param->current_gpu_program->setParameter(Light0_Ambient, 1, lightData);
			}
			else if(i == 1)
			{

			}
			else if(i == 2)
			{

			}
			else if(i == 3)
			{

			}
		}
		delete lightData;
	}

	void sgRenderEffect::setUniformObject( sg_render::CurrentRenderParam *param, sgSceneObject *object )
	{
		if(!param || !object)
			return ;

	}
} // namespace Sagitta
