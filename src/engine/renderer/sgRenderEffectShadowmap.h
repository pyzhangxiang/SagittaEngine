//  [4/11/2012  zhangxiang]

#ifndef __SGRENDEREFFECTSHADOWMAP_H__
#define __SGRENDEREFFECTSHADOWMAP_H__

#include "sgSceneRenderEffect.h"

namespace Sagitta{

    class sgRenderTarget;
    
	class _SG_KernelExport sgRenderEffectShadowMap : public sgSceneRenderEffect
	{
        SG_META_DECLARE(sgRenderEffectShadowMap)
        
    protected:
        sgRenderTarget *mDepthRT;
        
    public:
        sgRenderEffectShadowMap(void);
        virtual ~sgRenderEffectShadowMap(void);
        
        virtual void update(Float32 deltaTime);
        
    protected:
        virtual void setUniformObjectExtra(sg_render::CurrentRenderParam *param, sgSceneObject *object);
        
        virtual void preRenderPass(void);
        virtual void postRenderPass(void);
	};

} // namespace Sagitta


#endif // __SGRENDEREFFECTSHADOWMAP_H__

