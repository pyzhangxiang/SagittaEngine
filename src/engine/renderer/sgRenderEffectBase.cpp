
#include "sgRenderEffectBase.h"
#include "sgRenderPass.h"
#include "sgRenderQueue.h"
#include "engine/buffer/sgBuffer.h"
#include "sgRenderer.h"
#include "sgGpuProgram.h"

namespace Sagitta{

    SG_META_DEFINE(sgRenderEffectBase, sgSceneRenderEffect)

    sgRenderEffectBase::sgRenderEffectBase(void)
    : sgSceneRenderEffect()
    {
        addPass(sgRenderQueue::GetClassName());
    }
    
    sgRenderEffectBase::~sgRenderEffectBase(void)
    {
        
    }

} // namespace Sagitta
