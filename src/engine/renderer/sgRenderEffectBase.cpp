
#include "sgRenderEffectBase.h"
#include "sgRenderPass.h"
#include "sgRenderQueue.h"

namespace Sagitta{

    SG_META_DEFINE(sgRenderEffectBase, sgRenderEffect)

    sgRenderEffectBase::sgRenderEffectBase(void)
    : sgRenderEffect()
    {
        addPass(sgRenderQueue::GetClassName());
    }
    
    sgRenderEffectBase::~sgRenderEffectBase(void)
    {
        
    }
    
    void sgRenderEffectBase::update(Float32 deltaTime)
    {
        
    }

} // namespace Sagitta
