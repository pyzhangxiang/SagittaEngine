//  [25/11/2012  zhangxiang]

#include "sgRenderQueueCastShadow.h"
#include "engine/scenegraph/sgSceneObject.h"

namespace Sagitta
{
    SG_META_DEFINE(sgRenderQueueCastShadow, sgRenderQueue)

    sgRenderQueueCastShadow::sgRenderQueueCastShadow(void)
    {
        
    }
    
    sgRenderQueueCastShadow::~sgRenderQueueCastShadow(void)
    {
        
    }
        
    void sgRenderQueueCastShadow::sort()
    {
        ObjectList::iterator it = mObjectList.begin();
        for(; it!=mObjectList.end(); )
        {
            if(!(*it)->isCastShadow())
            {
                it = mObjectList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    

} // namespace Sagitta
