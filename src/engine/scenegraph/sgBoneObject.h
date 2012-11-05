#ifndef __SGBONEOBJECT_H__
#define __SGBONEOBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "sgSceneObject.h"
#include "engine/common/sgStlAllocator.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

    class sgSkeleton;

	class _SG_KernelExport sgBoneObject : public sgSceneObject
    {
		SG_META_DECLARE(sgBoneObject)
        
        friend class sgSkeleton;

	// member variables
	private:
        sgSkeleton *mpSkeletonBelongTo;
        
	// constructors & destructor
	public:
		sgBoneObject(void);
		virtual ~sgBoneObject(void);
        
	public:
		sgSkeleton *getSkeletonBelongTo(void) const;
        
    protected:
        virtual void onSetParent(sgNode *aParent);
        
    private:
        // for the bone root of a sgSkeleton
//        void setBoneBelongTo(sgSkeleton *pSkeleton){ mpSkeletonBelongTo = pSkeleton; }
        
	}; //#### end class sgSceneNode

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGBONEOBJECT_H__

