
#include "sgRenderTechniqueShadowMap.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderTargetTexture.h"
#include "sgRenderTargetWindow.h"
#include "sgRenderEffect.h"
#include "sgViewport.h"
#include "sgGpuProgram.h"
#include "engine/component/sgCameraComponent.h"

namespace Sagitta
{

    SG_META_DEFINE(sgRenderTechniqueShadowMap, sgRenderTechnique)
	
	sgRenderTechniqueShadowMap::sgRenderTechniqueShadowMap( void )
	: sgRenderTechnique()
    , mDepthTextureName("depthMap")
    , mDepthBiasMVPName("depthBiasMVP")
	{
        sgRenderPass *rpDepth = addPass();
        mDepthMap = sgGetRenderer()->_createRenderTarget(256, 256, 2, PIXEL_FORMAT_RGBA, RDT_F);
        rpDepth->setRenderTarget(mDepthMap);
        
        sgRenderPass *rp = addPass();
		mRTWindow = new sgRenderTargetWindow(1, 1);
		rp->setRenderTarget(mRTWindow);
    
	}

	sgRenderTechniqueShadowMap::~sgRenderTechniqueShadowMap( void )
	{
        sgGetRenderer()->_deleteRenderTarget(mDepthMap);
        delete mRTWindow;
	}
    
    bool sgRenderTechniqueShadowMap::preRenderPass(void)
    {
        if(getCurrentPass() == 0)
        {
            // depth pass
            
            sgRenderPass *pass = this->getRenderPass(0);
            sgRenderTarget *rt = pass->getRenderTarget();
            if(!rt)
                return false;
            
            sgViewport *viewport = rt->getViewport();
            if(!viewport)
                return false;
            
            sgCameraComponent *camera = viewport->camera();
            if(!camera)
                return false;
            
            mDepthVP = camera->getViewMatrix().transpose() * camera->getProjectionMatrix();
            
            return true;
            
        }
        
        return true;
    }
    
    void sgRenderTechniqueShadowMap::_setUniformObjectExtra(sg_render::CurrentRenderParam *param
                                                            , sgSceneObject *object)
    {
        // calc depth bais mvp
        // This is matrix transform every coordinate x,y,z
        // x = x* 0.5 + 0.5
        // y = y* 0.5 + 0.5
        // z = z* 0.5 + 0.5
        // Moving from unit cube [-1,1] to [0,1]
        static Matrix4 biasMat(
                               0.5f, 0.0f, 0.0f, 0.0f,
                               0.0f, 0.5f, 0.0f, 0.0f,
                               0.0f, 0.0f, 0.5f, 0.0f,
                               0.5f, 0.5f, 0.5f, 1.0f
                               );
        Matrix4 depthBiasMVP = mDepthVP * object->getFullTransform().transpose() * biasMat;
        param->current_gpu_program->setParameter(mDepthBiasMVPName, (1<<1)|0, depthBiasMVP.arr());
        
        size_t depthIndex = param->textures.size();
        param->current_gpu_program->setParameter(mDepthTextureName, 1, &depthIndex);
        param->textures.push_back(mDepthMap->getRtTextureId());
    }

} // namespace Sagitta
