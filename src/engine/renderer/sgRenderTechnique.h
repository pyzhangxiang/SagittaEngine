
#ifndef __SGRENDERTECHNIQUE_H__
#define __SGRENDERTECHNIQUE_H__

#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"
#include "sgRenderer.h"

namespace Sagitta
{
    class sgRenderPass;
	class sgRenderTarget;

	class _SG_KernelExport sgRenderTechnique : public sgObject
	{
        SG_META_DECLARE_ABSTRACT(sgRenderTechnique)
        
    protected:
        typedef sg_vector(sgRenderPass*) PassList;
		PassList mPassList;
        
    public:
        sgRenderTechnique(void);
        virtual ~sgRenderTechnique(void);
        
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
        
		/// do something before and after the render pass
		virtual bool preRenderPass(void){ return true; }
		virtual void postRenderPass(void){}

        void render(void);
		void resize(UInt32 width, UInt32 height);
		void update(Float32 deltaTime);

		/// called by rendereffect, after sgRenderEffect::setUniformFrame
        virtual void _setUniformFrameExtra(sg_render::CurrentRenderParam *param){}
		virtual void _setUniformObjectExtra(sg_render::CurrentRenderParam *param
                                            , sgSceneObject *object){}
	};

	//////////////////////////////////////////////////////////////////////////
	class _SG_KernelExport sgRenderTechniqueBase : public sgRenderTechnique
	{
		SG_META_DECLARE(sgRenderTechniqueBase)

	private:
		sgRenderTarget *mRTWindow;

	public:
		sgRenderTechniqueBase(void);
		virtual ~sgRenderTechniqueBase(void);

	};

} // namespace Sagitta


#endif // __SGRENDERTECHNIQUE_H__

