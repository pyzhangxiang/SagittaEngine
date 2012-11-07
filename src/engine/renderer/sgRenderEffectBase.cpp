
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

		scale = new sgBuffer(sizeof(Real));
		addData("scale", scale);
    }
    
    sgRenderEffectBase::~sgRenderEffectBase(void)
    {
        delete scale;
    }
    
    void sgRenderEffectBase::update(Float32 deltaTime)
    {
        *((Real*)(scale->data())) = deltaTime;
    }

	void sgRenderEffectBase::setUniformScene( void )
	{
		
	}

	void sgRenderEffectBase::setUniformObject( sgSceneObject *object )
	{
		DataMap::iterator it = mDataMap.begin();
		for(; it!=mDataMap.end(); ++it)
		{
			mCurrentRenderParam->current_gpu_program->setParameter(it->first, it->second);
		}
	}

} // namespace Sagitta
