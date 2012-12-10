//  [4/11/2012  zhangxiang]
//  [11/19/2012 fabiozhang]

#include "sgRenderPass.h"
#include "sgRenderQueue.h"
#include "sgRenderEffect.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/common/sgLogSystem.h"
#include <string>

namespace Sagitta{

    //SG_META_DEFINE(sgRenderPass, sgObject)
    
    sgRenderPass::sgRenderPass(const sgStrHandle &queueTypeName)
    : mRenderQueue(NULL)
	, mRenderEffect(NULL)
	, mRenderTarget(NULL)
    , mUseSceneProgramOnly(false)
    {
        sgClassMeta *meta = sgMetaCenter::instance().findMeta(queueTypeName);
        if(meta && meta->isClass(sgRenderQueue::GetClassTypeName()))
        {
            mRenderQueue = (sgRenderQueue*)sgObject::createObject(queueTypeName);
        }
		if(mRenderQueue == NULL)
		{
			// use default render queue
			sgLogSystem::instance()->warning(std::string("Cannot create the specified(") + 
				std::string(queueTypeName.getStr()) +
				std::string(") render queue, use default."));

			mRenderQueue = (sgRenderQueue*)sgObject::createObject(sgRenderQueue::GetClassTypeName());
		}
    }
    
    sgRenderPass::~sgRenderPass(void)
    {
		destroyRenderEffect();

        if(mRenderQueue)
        {
            sgObject::destroyObject(mRenderQueue);
            mRenderQueue = 0;
        }
    }
    
	sgRenderEffect *sgRenderPass::createRenderEffect(const sgStrHandle &effectType)
	{
		sgClassMeta *meta = sgMetaCenter::instance().findMeta(effectType);
		if(!meta)
			return mRenderEffect;
		if(!meta->isClass(sgRenderEffect::GetClassTypeName()))
			return mRenderEffect;

		if(mRenderEffect)
		{
			// warning: the original one will be destroyed
			destroyRenderEffect();
		}
		mRenderEffect = (sgRenderEffect*)sgObject::createObject(effectType);

		return mRenderEffect;
	}

	void sgRenderPass::destroyRenderEffect(void)
	{
		if(mRenderEffect)
		{
			sgObject::destroyObject(mRenderEffect);
			mRenderEffect = NULL;
		}
	}

	void sgRenderPass::setRenderTarget( sgRenderTarget *target )
	{
		mRenderTarget = target;
	}
    
    void sgRenderPass::setUseSceneProgramOnly(bool onlyScene)
    {
        mUseSceneProgramOnly = onlyScene;
    }

	void sgRenderPass::setRenderState(const sgRenderState &rs)
    {
        mRenderState = rs;
    }

} // namespace Sagitta