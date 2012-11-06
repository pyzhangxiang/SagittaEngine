//  [4/11/2012  zhangxiang]

#ifndef __SGRENDEREFFECTSHADOWMAP_H__
#define __SGRENDEREFFECTSHADOWMAP_H__

#include "sgSceneRenderEffect.h"

namespace Sagitta{

	class _SG_KernelExport sgRenderEffectShadowmap : public sgSceneRenderEffect
	{
        SG_META_DECLARE(sgRenderEffectShadowmap)
        
    public:
        sgRenderEffectShadowmap(void);
        virtual ~sgRenderEffectShadowmap(void);
        
        virtual void update(Float32 deltaTime);
	};

} // namespace Sagitta


#endif // __SGRENDEREFFECTSHADOWMAP_H__

