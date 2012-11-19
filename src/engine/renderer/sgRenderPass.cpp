//  [4/11/2012  zhangxiang]
//  [11/19/2012 fabiozhang]

#include "sgRenderPass.h"
#include "sgRenderQueue.h"
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
        if(meta && meta->isClass(sgRenderQueue::GetClassName()))
        {
            mRenderQueue = (sgRenderQueue*)sgObject::createObject(queueTypeName);
        }
		if(mRenderQueue == NULL)
		{
			// use default render queue
			sgLogSystem::instance()->warning(std::string("Cannot create the specified(") + 
				std::string(queueTypeName.getStr()) +
				std::string(") render queue, use default."));

			mRenderQueue = (sgRenderQueue*)sgObject::createObject(sgRenderQueue::GetClassName());
		}
    }
    
    sgRenderPass::~sgRenderPass(void)
    {
        if(mRenderQueue)
        {
            sgObject::destroyObject(mRenderQueue);
            mRenderQueue = 0;
        }
    }
    
    void sgRenderPass::setRenderEffect(sgRenderEffect *effect)
    {
        mRenderEffect = effect;
    }

	void sgRenderPass::setRenderTarget( sgRenderTarget *target )
	{
		mRenderTarget = target;
	}
    
    void sgRenderPass::setUseSceneProgramOnly(bool onlyScene)
    {
        mUseSceneProgramOnly = onlyScene;
    }

	

} // namespace Sagitta