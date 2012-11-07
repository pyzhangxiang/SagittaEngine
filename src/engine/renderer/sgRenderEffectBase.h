//  [4/11/2012  zhangxiang]

#ifndef __SGRENDEREFFECTBASE_H__
#define __SGRENDEREFFECTBASE_H__

#include "sgSceneRenderEffect.h"


namespace Sagitta{
    class sgBuffer;
	class _SG_KernelExport sgRenderEffectBase : public sgSceneRenderEffect
	{
        SG_META_DECLARE(sgRenderEffectBase)
        sgBuffer *scale;
    public:
        sgRenderEffectBase(void);
        virtual ~sgRenderEffectBase(void);
        
        virtual void update(Float32 deltaTime);
      
		// test uniform
		virtual void setUniformScene(void);
		virtual void setUniformObject(sgSceneObject *object);
	};
    
} // namespace Sagitta

#endif