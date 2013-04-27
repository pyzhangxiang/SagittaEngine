#ifndef __SGSCENE_H__
#define __SGSCENE_H__

// INCLUDES //////////////////////////////////////////
#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"
#include "engine/common/sgStrHandle.h"
#include "math/sgColor.h"
#include "engine/renderer/sgRenderState.h"
#include <map>

namespace Sagitta{

	class sgSceneObject;
	class sgDynamicsWorld;

	class _SG_KernelExport sgScene : public sgObject
    {
		SG_META_DECLARE(sgScene)

	protected:
		//typedef sg_map(id_type, sgSceneObject*) ObjectMap;
        
    public:
		//typedef sgMapIterator<ComponentMap> ComponentIterator;

	// member variables
	protected:
		sgSceneObject *mpRoot;
        Color mAmbiantColor;
        //sgRenderState mRenderState;
        
		bool mPhysicsEnabled;
		bool mPhysicsContinuous;
		sgDynamicsWorld *mDynamicsWorld;

	// constructors & destructor
	public:
		sgScene(void);
		virtual ~sgScene(void);
        
	public:
		sgSceneObject *getRoot(void) const;
		virtual void update(Float32 deltaTime);
        
        const Color &getAmbiantColor(void) const;
        void setAmbiantColor(const Color &color);

		bool isPhysicsEnabled(void) const{ return mPhysicsEnabled; }
		void setPhysicsEnabled(bool enable);
		bool isPhysicsContinuous(void) const{ return mPhysicsContinuous; }
		void setPhysicsContinuous(bool continuous);
		void stepPhysics(Float32 deltaTime);
		sgDynamicsWorld *getDynamicsWorld(void) const{ return mDynamicsWorld; }

        //const sgRenderState &getRenderState(void) const;
        //void setRenderState(const sgRenderState &state);
        
  
	}; //#### end class sgSceneNode

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSCENE_H__

