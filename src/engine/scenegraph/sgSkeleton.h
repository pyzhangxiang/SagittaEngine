#ifndef __SGSKELETON_H__
#define __SGSKELETON_H__

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
	class sgRagdoll;
    
	class _SG_KernelExport sgSkeleton : public sgObject
    {
		SG_META_DECLARE(sgSkeleton)
        
        // for add and remove bone object in sgBoneObject::onSetParent
        friend class sgBoneObject;
        
        // for setBelongTo in sgSceneObject::setSkeleton
        friend class sgSceneObject;
        
	protected:
		typedef sg_map(std::string, sgBoneObject*) BoneNodeMap;
        
    public:
		//typedef sgMapIterator<BoneNodeMap> BoneNodeMapIterator;
        
        // member variables
	protected:
		sgSceneObject *mpRoot;
        sgBoneObject *mpBoneRoot;
        BoneNodeMap mBoneNodeMap;

		sgRagdoll *mpRagdoll;

        // constructors & destructor
	public:
		sgSkeleton(void);
		virtual ~sgSkeleton(void);
        
	public:
		sgSceneObject *getRoot(void) const;
        sgBoneObject *getBoneRoot(void) const;
		virtual void update(Float32 deltaTime);

		sgBoneObject *getBoneNode(const std::string &bonename) const;
		size_t getBoneNum(void) const{ return mBoneNodeMap.size(); }
        
        sgSceneObject *parent(void) const;

		sgRagdoll *getRagdoll(void) const{ return mpRagdoll; }
		sgRagdoll *setRagdoll(sgRagdoll *ragdoll);

    protected:
        // for sgBoneObject
		bool addBoneObject(sgBoneObject *node);
        void removeBoneObject(sgBoneObject *node);
        void removeBoneObject(const std::string &name);
    
    protected:
        // for sgSceneObject
        void setBelongTo(sgSceneObject *parent);
        
    public:
        virtual void showDebug(bool show);
    
	}; //#### end class sgSceneNode
    
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSKELETON_H__

