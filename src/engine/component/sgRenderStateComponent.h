/// @2012.9.27 by zhangxiang

#ifndef __SGRENDERSTATECOMPONENT_H__
#define __SGRENDERSTATECOMPONENT_H__


#include "sgComponent.h"
#include "engine/renderer/sgRenderState.h"
#include "engine/common/sgStrHandle.h"


namespace Sagitta{

	class sgMaterial;
    class sgRenderEffect;
    class sgTexture;

	class _SG_KernelExport sgRenderStateComponent : public sgComponent
	{
		SG_META_DECLARE(sgRenderStateComponent)

	protected:
		sgStrHandle mMaterialFile; // material id
        
        typedef sg_vector(sgStrHandle) TextureList;
        TextureList mTextureList;
        
		sgRenderState mRenderState;
        
        sgRenderEffect *mRenderEffect;

	public:
		sgRenderStateComponent(void);
		virtual ~sgRenderStateComponent(void);

		sgStrHandle getMaterialFile(void) const;
		void setMaterialFile(sgStrHandle materialFile);
		sgMaterial *getMaterial(void) const;

		sgRenderState getRenderState(void) const;
		void setRenderOption(const sgRenderState &ro);
        
        void addTexture(const sgStrHandle &filename);
        void clearTexture(void);
        size_t getTextureNum(void) const{ return mTextureList.size(); }
        sgTexture *getTexture(size_t index) const;

        sgRenderEffect *createRenderEffect(const sgStrHandle &effectType);
        void destroyRenderEffect(void);
        sgRenderEffect *getRenderEffect(void) const{ return mRenderEffect; }
	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRENDERSTATECOMPONENT_H__
