/// @2012.11.27 by zhangxiang

#ifndef __SGMESHCOMPONENT_H__
#define __SGMESHCOMPONENT_H__


#include "sgComponent.h"
#include "engine/common/sgStrHandle.h"


namespace Sagitta{

	class sgMesh;

	class _SG_KernelExport sgRigidBodyComponent : public sgComponent
	{
		SG_META_DECLARE(sgRigidBodyComponent)

	protected:

	public:
		sgRigidBodyComponent(void);
		virtual ~sgRigidBodyComponent(void);


	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGMESHCOMPONENT_H__
