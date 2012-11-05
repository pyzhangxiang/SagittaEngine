//  [4/11/2012  zhangxiang]

#ifndef __SGRENDERPASS_H__
#define __SGRENDERPASS_H__

#include "engine/common/sgObject.h"

namespace Sagitta{
    
    class sgGpuProgram;
    class sgRenderQueue;

	class _SG_KernelExport sgRenderPass : public sgMemObject
	{
    //    SG_META_DECLARE(sgRenderPass)
        
	private:
        sgGpuProgram *mGpuProgram;
        
        // only for scene effect, like shadow map
        sgRenderQueue *mRenderQueue;
        
	public:
        sgRenderPass(const sgStrHandle &queueTypeName = sgStrHandle::EmptyString);
		virtual ~sgRenderPass(void);
        
        sgRenderQueue *getRenderQueue(void) const{ return mRenderQueue; }
        
        sgGpuProgram *getGpuProgram(void) const{ return mGpuProgram; }
        void setProgram(sgGpuProgram *program);


	};

} // namespace Sagitta


#endif // __SGRENDERPASS_H__

