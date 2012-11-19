
#ifndef __SGRENDERTECHNIQUE_H__
#define __SGRENDERTECHNIQUE_H__

#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"

namespace Sagitta
{
    class sgRenderPass;

	class _SG_KernelExport sgRenderTechnique : public sgObject
	{
        SG_META_DECLARE_ABSTRACT(sgRenderTechnique)
        
    protected:
        typedef sg_vector(sgRenderPass*) PassList;
		PassList mPassList;
        
    public:
        sgRenderTechnique(void);
        virtual ~sgRenderTechnique(void) = 0;
        
    private:
		size_t mCurrentPass;
	protected:
		// for subclasses
		size_t getCurrentPass(void) const{ return mCurrentPass; }
		void setCurrentPass(size_t pass){ mCurrentPass = pass; }
        
    public:
        sgRenderPass *addPass(const sgStrHandle &queueType = sgStrHandle::EmptyString);
        
        bool emptyRenderPass(void){ return mPassList.empty(); }
        size_t getRenderPassNum(void) const{ return mPassList.size(); }
        sgRenderPass *getRenderPass(size_t index) const;
        
        void render(void);
	};

} // namespace Sagitta


#endif // __SGRENDERTECHNIQUE_H__

