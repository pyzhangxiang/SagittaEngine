#ifndef __SGRAGDOLL_H__
#define __SGRAGDOLL_H__

// INCLUDES //////////////////////////////////////////
#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"
#include "engine/common/sgStrHandle.h"
#include <map>

// DECLARES //////////////////////////////////////////

class btRigidBody;
class btTypedConstraint;

namespace Sagitta{
    
	class sgBoneObject;
    class sgSceneObject;
	class sgScene;
    

	class _SG_KernelExport sgRagdoll : public sgObject
    {
		SG_META_DECLARE(sgRagdoll)
		
		// for setParent in sgSceneObject::setRagdoll
		friend class sgSceneObject;

	private:
		sgSceneObject *mParentObject;
		sgStrHandle mRagdollConfig;

		struct Body
		{
			std::string jointName;
			btRigidBody *body;
			btTypedConstraint *constraint;
			sgSceneObject *object;
		};
		typedef sg_map(std::string, Body) BodyMap;
		BodyMap mBodyMap;
		BodyMap mBodyMapByJointName;
		
		bool mVisible;

	public:
		sgRagdoll(void);
		virtual ~sgRagdoll(void);

		void setRagdollConfig(const sgStrHandle &config);
		void addToScene(void);
		void removeFromScene(void);

		void resetTransform(void);
		void syncTransformToSkeleton(void);

		void update(Float32 deltaTime);

		sgSceneObject *parent(void) const{ return mParentObject; }

		sgScene *getScene(void) const;

		bool isVisible(void) const{ return mVisible; }
		void setVisible(bool visible);

	protected:
		void setParent(sgSceneObject *parent);
    
	private:
		void release(void);
    
	}; //#### end class sgSceneNode
    
} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGRAGDOLL_H__

