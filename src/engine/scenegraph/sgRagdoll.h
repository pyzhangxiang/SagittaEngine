#ifndef __SGRAGDOLL_H__
#define __SGRAGDOLL_H__

// INCLUDES //////////////////////////////////////////
#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"
#include "engine/common/sgStrHandle.h"
#include "math/sgVector3.h"
#include <map>

// DECLARES //////////////////////////////////////////

class btRigidBody;
class btGeneric6DofConstraint;

namespace Sagitta{
    
	class sgBoneObject;
    class sgSceneObject;
	class sgScene;
	class sgSkeleton;
    

	class _SG_KernelExport sgRagdoll : public sgObject
    {
		SG_META_DECLARE(sgRagdoll)
		
		// for setParent in sgSceneObject::setRagdoll
		friend class sgSkeleton;

	private:
		sgSkeleton *mSkeleton;
		sgStrHandle mRagdollConfig;

		struct Body
		{
			std::string jointName;
			btRigidBody *body;
			btGeneric6DofConstraint *constraint;
			sgSceneObject *object;
			Vector3 offset;
			bool ifCollideWithParent;
		};
		typedef sg_map(std::string, Body*) BodyMap;
		BodyMap mBodyMap;
		BodyMap mBodyMapByJointName;
		
		bool mVisible;

	private:
		// on skeleton changed
		void _resetTransform(void);
		//void _onSceneChanged(void);

	public:
		sgRagdoll(void);
		virtual ~sgRagdoll(void);

		void setRagdollConfig(const sgStrHandle &config);
		void addToScene(void);
		void removeFromScene(void);
		
		

		void resetTransform(void);
		void syncTransformToSkeleton(void);

		void update(Float32 deltaTime);

		//sgSceneObject *parent(void) const{ return mParentObject; }
		sgSkeleton *getSkeleton(void) const{ return mSkeleton; }

		sgScene *getScene(void) const;

		bool isVisible(void) const{ return mVisible; }
		void setVisible(bool visible);

	protected:
		void setSkeleton(sgSkeleton *parent);
    
	private:
		void release(void);
    
	}; //#### end class sgSceneNode
    
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRAGDOLL_H__

