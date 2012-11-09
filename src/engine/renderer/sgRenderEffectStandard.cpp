
#include "sgRenderEffectStandard.h"
#include "sgRenderPass.h"
#include "sgRenderQueue.h"
#include "engine/buffer/sgBuffer.h"
#include "sgRenderer.h"
#include "sgGpuProgram.h"

namespace Sagitta{

    SG_META_DEFINE(sgRenderEffectStandard, sgSceneRenderEffect)

    sgRenderEffectStandard::sgRenderEffectStandard(void)
    : sgSceneRenderEffect()
    {
        addPass(sgRenderQueue::GetClassName());
    }
    
    sgRenderEffectStandard::~sgRenderEffectStandard(void)
    {
        
    }

} // namespace Sagitta
