//  [4/11/2012  zhangxiang]

#include "sgRenderQueue.h"
#include "engine/scenegraph/sgSceneObject.h"

namespace Sagitta
{
    SG_META_DEFINE(sgRenderQueue, sgObject)

    sgRenderQueue::sgRenderQueue(void)
    {
        
    }
    
    sgRenderQueue::~sgRenderQueue(void)
    {
        
    }
    
    void sgRenderQueue::push_back(sgSceneObject *object)
    {
		if(!filter(object))
			return ;

        mObjectList.push_back(object);
    }
    
    void sgRenderQueue::clear(void)
    {
        mObjectList.clear();
    }
    

} // namespace Sagitta
