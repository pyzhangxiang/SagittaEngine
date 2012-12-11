#ifndef __SGRAGDOLL_H__
#define __SGRAGDOLL_H__

// INCLUDES //////////////////////////////////////////
#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"
#include "engine/common/sgStrHandle.h"
#include "math/sgColor.h"
#include "engine/renderer/sgRenderState.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{
    
	class sgBoneObject;
    class sgSceneObject;
    
	class _SG_KernelExport sgRagdoll : public sgObject
    {
		SG_META_DECLARE(sgRagdoll)
        
	public:
		sgRagdoll(void);
		virtual ~sgRagdoll(void);
    
    
	}; //#### end class sgSceneNode
    
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRAGDOLL_H__

