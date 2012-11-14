
#include "sgRenderEffectShadowMap.h"
#include "sgRenderPass.h"
#include "sgRenderQueue.h"
#include "sgRenderer.h"
#include "sgRenderTarget.h"
#include "sgGpuProgram.h"

namespace Sagitta{

    SG_META_DEFINE(sgRenderEffectShadowMap, sgSceneRenderEffect)

    sgRenderEffectShadowMap::sgRenderEffectShadowMap(void)
    : sgSceneRenderEffect(), mDepthRT(NULL)
    {
        addPass(sgRenderQueue::GetClassName());
        addPass(sgRenderQueue::GetClassName());
        
        mDepthRT = sgGetRenderer()->createRenderTarget(512, 512, 2, PIXEL_FORMAT_RG, RDT_F);
    }
    
    sgRenderEffectShadowMap::~sgRenderEffectShadowMap(void)
    {
        
    }
    
    void sgRenderEffectShadowMap::update(Float32 deltaTime)
    {
        
    }
    
    void sgRenderEffectShadowMap::setUniformObjectExtra(sg_render::CurrentRenderParam *param, sgSceneObject *object)
    {
        if(this->getCurrentPass() == 0)
            return ;
        
        if(mDepthRT == NULL)
            return ;
        
        int tindex = (int)(param->textures.size());
        param->textures.push_back(mDepthRT->getRtTextureId());
        
        if(tindex == 0)
        {
            param->current_gpu_program->setParameter(Texture0, 1, &tindex);
        }
        else if(tindex == 1)
        {
            param->current_gpu_program->setParameter(Texture1, 1, &tindex);
        }
        else if(tindex == 2)
        {
            param->current_gpu_program->setParameter(Texture2, 1, &tindex);
        }
        else if(tindex == 3)
        {
            param->current_gpu_program->setParameter(Texture3, 1, &tindex);
        }
    }
    
    void sgRenderEffectShadowMap::preRenderPass(void)
    {
        if(getCurrentPass() == 0)
            sgGetRenderer()->beginRenderTarget(mDepthRT);
    }
    
    void sgRenderEffectShadowMap::postRenderPass(void)
    {
        if(getCurrentPass() == 0)
            sgGetRenderer()->endRenderTarget();
    }

} // namespace Sagitta
