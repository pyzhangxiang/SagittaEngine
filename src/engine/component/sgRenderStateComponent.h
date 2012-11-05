/// @2012.9.27 by zhangxiang

#ifndef __SGRENDERSTATECOMPONENT_H__
#define __SGRENDERSTATECOMPONENT_H__


#include "sgComponent.h"
#include "engine/renderer/sgRenderState.h"
#include "engine/common/sgStrHandle.h"


namespace Sagitta{

	class sgMaterial;

	class _SG_KernelExport sgRenderStateComponent : public sgComponent
	{
		SG_META_DECLARE(sgRenderStateComponent)

	protected:
		sgStrHandle mMaterialFile; // material id
        
		sgRenderState mRenderState;

	public:
		sgRenderStateComponent(void);
		virtual ~sgRenderStateComponent(void);

		sgStrHandle getMaterialFile(void) const;
		void setMaterialFile(sgStrHandle materialFile);
		sgMaterial *getMaterial(void) const;

		sgRenderState getRenderState(void) const;
		void setRenderOption(const sgRenderState &ro);

	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRENDERSTATECOMPONENT_H__
