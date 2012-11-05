/// @2012.9.27 by zhangxiang

#ifndef __SGMESHCOMPONENT_H__
#define __SGMESHCOMPONENT_H__


#include "sgComponent.h"
#include "engine/common/sgStrHandle.h"


namespace Sagitta{

	class sgMesh;

	class _SG_KernelExport sgMeshComponent : public sgComponent
	{
		SG_META_DECLARE(sgMeshComponent)

	protected:
		sgStrHandle mMeshFile;

	public:
		sgMeshComponent(void);
		virtual ~sgMeshComponent(void);

		sgStrHandle getMeshFile(void) const;
		void setMeshFile(sgStrHandle meshFile);
		sgMesh *getMesh(void) const;

	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGMESHCOMPONENT_H__
