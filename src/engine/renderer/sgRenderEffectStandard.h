//  [4/11/2012  zhangxiang]

#ifndef __SGRENDEREFFECTSTANDARD_H__
#define __SGRENDEREFFECTSTANDARD_H__

#include "sgSceneRenderEffect.h"


namespace Sagitta{
    class sgBuffer;
	class _SG_KernelExport sgRenderEffectStandard : public sgSceneRenderEffect
	{
        SG_META_DECLARE(sgRenderEffectStandard)
        
    public:
        sgRenderEffectStandard(void);
        virtual ~sgRenderEffectStandard(void);
     
	};
    
} // namespace Sagitta

#endif // __SGRENDEREFFECTSTANDARD_H__
