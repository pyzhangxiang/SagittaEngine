//  [4/11/2012  zhangxiang]

#ifndef __SGRENDERPASS_H__
#define __SGRENDERPASS_H__

#include "engine/common/sgObject.h"

namespace Sagitta{
    
    class sgGpuProgram;
    class sgRenderQueue;
	class sgRenderEffect;
	class sgRenderTarget;

	class _SG_KernelExport sgRenderPass : public sgMemObject
	{
    //    SG_META_DECLARE(sgRenderPass)
        
	private:
        // only for scene effect, like shadow map
        sgRenderQueue *mRenderQueue;

		sgRenderEffect *mRenderEffect;
		sgRenderTarget *mRenderTarget;
        
        bool mUseSceneProgramOnly;
        
	public:
        sgRenderPass(const sgStrHandle &queueTypeName = sgStrHandle::EmptyString);
		virtual ~sgRenderPass(void);
        
        sgRenderQueue *getRenderQueue(void) const{ return mRenderQueue; }

		sgRenderEffect *createRenderEffect(const sgStrHandle &effectType);
		void destroyRenderEffect(void);
		sgRenderEffect *getRenderEffect(void) const{ return mRenderEffect; }

		sgRenderTarget *getRenderTarget(void) const{ return mRenderTarget; }
		void setRenderTarget(sgRenderTarget *target);
        
        bool isUseSceneProgramOnly(void) const{ return mUseSceneProgramOnly; }
        void setUseSceneProgramOnly(bool onlyScene);
	};

} // namespace Sagitta


#endif // __SGRENDERPASS_H__

