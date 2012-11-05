
#include "sgEngine.h"
#include "common/sgMalloc.h"
#include "common/sgStrHandle.h"
#include "common/sgObjectCenter.h"
#include "common/sgLogSystem.h"
#include "renderer/sgRenderer.h"
#include "resource/sgResourceCenter.h"
#include "resource/sgSimpleMeshes.h"

namespace Sagitta
{

    void sgEngineInit(const sgStrHandle &rendererType,
                      const std::string &assetRootDir)
    {
        // first create memory
        sgCreateAllocator();
		sgLogSystem::CreateInstance();
        sgObjectCenter::CreateInstance();
        sgResourceCenter::CreateInstance();
        sgResourceCenter::instance()->setRootDir(assetRootDir);
//        sgLoadInternalMeshes();
        sgCreateRenderer(rendererType);
    }
    
    void sgEngineShutdown(void)
    {
        sgDestroyRenderer();
        sgResourceCenter::DestroyInstance();
        sgObjectCenter::DestroyInstance();
		sgStrHandleCenter::instance().release();
		sgLogSystem::DestroyInstance();
        sgDestroyAllocator();
    }

}



