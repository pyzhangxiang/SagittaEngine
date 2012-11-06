//  [4/11/2012  zhangxiang]

#ifndef __SGRENDEREFFECT_H__
#define __SGRENDEREFFECT_H__

#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"


namespace Sagitta{
    
    class sgRenderPass;
    class sgBuffer;
	class sgSceneObject;

	namespace sg_render
	{
		struct CurrentRenderParam;
	};

	class _SG_KernelExport sgRenderEffect : public sgObject
	{
        SG_META_DECLARE_ABSTRACT(sgRenderEffect)
        
    protected:
        typedef sg_vector(sgRenderPass*) PassList;
        PassList mPassList;
        
        typedef sg_map(std::string, sgBuffer*) DataMap;
        DataMap mDataMap;
        
		sg_render::CurrentRenderParam *mCurrentRenderParam;

    protected:
        void addPass(const sgStrHandle &queueType = sgStrHandle::EmptyString);
        
    public:
        sgRenderEffect(void);
        virtual ~sgRenderEffect(void) = 0;
        
        bool emptyRenderPass(void){ return mPassList.empty(); }
        size_t getRenderPassNum(void) const{ return mPassList.size(); }
        sgRenderPass *getRenderPass(size_t index) const;
        
        virtual void update(Float32 deltaTime){}

		virtual void render(sg_render::CurrentRenderParam *param, sgSceneObject *object) = 0;

		virtual void setSceneUniforms(void){}
		virtual void setObjectUniforms(void){}
	};

} // namespace Sagitta


#endif // __SGRENDEREFFECT_H__

