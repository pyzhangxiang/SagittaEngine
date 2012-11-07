//  [4/11/2012  zhangxiang]

#include "sgRenderEffect.h"
#include "sgRenderPass.h"
#include "sgRenderer.h"
#include "sgRenderQueue.h"
#include "engine/scenegraph/sgSceneObject.h"
#include "engine/buffer/sgBuffer.h"

namespace Sagitta
{
    SG_META_DEFINE_ABSTRACT(sgRenderEffect, sgObject)

    sgRenderEffect::sgRenderEffect(void)
	: mCurrentRenderParam(NULL)
    {
        
    }
    
    sgRenderEffect::~sgRenderEffect(void)
    {
        for(size_t i=0; i<mPassList.size(); ++i)
        {
            delete mPassList[i];
        }
        mPassList.clear();
        
        DataMap::iterator it = mDataMap.begin();
        for(; it!=mDataMap.end(); ++it)
        {
            delete it->second;
        }
        mDataMap.clear();
    }
    
    void sgRenderEffect::addPass(const sgStrHandle &queueType)
    {
        sgRenderPass *rp = new sgRenderPass(queueType);
        mPassList.push_back(rp);
    }
    
    sgRenderPass *sgRenderEffect::getRenderPass(size_t index) const
    {
        if(index >= mPassList.size())
            return 0;
        
        return mPassList[index];
    }

	void sgRenderEffect::addData( const sgStrHandle &dataName, sgBuffer *data )
	{
		if(!data)
			return ;

		sgBuffer *refdata = new sgBuffer(*data, 0);

		DataMap::iterator it = mDataMap.find(dataName);
		if(it != mDataMap.end())
		{
			delete it->second;
			mDataMap.erase(it);
		}

		mDataMap.insert(std::make_pair(dataName, refdata));
	}
} // namespace Sagitta
