
#include "sgRenderTechniqueShadowMap.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderTargetTexture.h"
#include "sgRenderTargetWindow.h"
#include "sgRenderEffect.h"
#include "sgViewport.h"
#include "sgGpuProgram.h"
#include "sgRenderQueueCastShadow.h"
#include "engine/component/sgCameraComponent.h"

namespace Sagitta
{

    SG_META_DEFINE(sgRenderTechniqueShadowMap, sgRenderTechnique)
	
	sgRenderTechniqueShadowMap::sgRenderTechniqueShadowMap( void )
	: sgRenderTechnique()
    , mDepthTextureName("depthMap")
    , mDepthMVPName("depthMVP")
	{
        sgRenderPass *rpDepth = addPass(sgRenderQueueCastShadow::GetClassName());
        mDepthMap = sgGetRenderer()->_createRenderTarget(1024, 1024, 4, PIXEL_FORMAT_RGBA, RDT_UBYTE);
        rpDepth->setRenderTarget(mDepthMap);
        rpDepth->setUseSceneProgramOnly(true);
        //sgRenderState &rs = rpDepth->getRenderState();
        //rs.setFaceCullingEnable(true);
        //rs.setFaceToCull(sgRenderState::FTC_FRONT);
        
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
		if(getCurrentPass() == 1)
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
			Matrix4 depthMVP = object->getFullTransform().transpose() * mDepthVP/* * biasMat*/;
			param->current_gpu_program->setParameter(mDepthMVPName, (1<<1)|0, depthMVP.arr());
        
			size_t depthIndex = param->textures.size();
			param->current_gpu_program->setParameter(mDepthTextureName, 1, &depthIndex);
			param->textures.push_back(mDepthMap->getRtTextureId());
		}
    }

} // namespace Sagitta
