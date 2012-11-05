//  [4/11/2012  zhangxiang]

#include "sgRenderPass.h"
#include "sgRenderQueue.h"
#include "sgGpuProgram.h"
#include "engine/scenegraph/sgSceneObject.h"

namespace Sagitta{

    //SG_META_DEFINE(sgRenderPass, sgObject)
    
    sgRenderPass::sgRenderPass(const sgStrHandle &queueTypeName)
    : mGpuProgram(NULL)
    , mRenderQueue(NULL)
    {
        sgClassMeta *meta = sgMetaCenter::instance().findMeta(queueTypeName);
        if(meta && meta->isClass(sgRenderQueue::GetClassName()))
        {
            mRenderQueue = (sgRenderQueue*)sgObject::createObject(queueTypeName);
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
    
    void sgRenderPass::setProgram(sgGpuProgram *program)
    {
        mGpuProgram = program;
    }
    
} // namespace Sagitta