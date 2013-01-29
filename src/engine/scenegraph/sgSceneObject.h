#ifndef __SGSCENEOBJECT_H__
#define __SGSCENEOBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "sgNode.h"
#include "engine/common/sgStlAllocator.h"
#include "engine/common/sgStrHandle.h"
#include "engine/component/sgComponent.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgScene;
    class sgSkeleton;
	class sgRagdoll;

	class _SG_KernelExport sgSceneObject : public sgNode
    {
		SG_META_DECLARE(sgSceneObject)
        
        // for the root constructed in sgScene's construction
        // to call my setScene function
        friend class sgScene;


	protected:
		typedef sg_map(sgStrHandle, sgComponent*) ComponentMap;
        
    public:
		//typedef sgMapIterator<ComponentMap> ComponentIterator;
		typedef sg_vector(sgSceneObject*) SceneObjectVec;

	// member variables
	private:
		/** Esse mapped by type. */
		ComponentMap mComponentMap;

		sgScene *mpScene;
		bool mbSceneChanged;
        
        sgSkeleton *mpSkeleton;
		sgRagdoll *mpRagdoll;
        
        bool mCastShadow;
	// constructors & destructor
	public:
		sgSceneObject(void);
		virtual ~sgSceneObject(void);

	private:
		void setScene(sgScene *pScene);
        
    protected:
        virtual void onSetParent(sgNode *aParent);
	public:
		sgScene *getScene(void) const;

        sgSkeleton *getSkeleton(void) const;
        /// the skeleton's root will become a child of the oject
        sgSkeleton *setSkeleton(sgSkeleton *pSkeleton);

		sgRagdoll *getRagdoll(void) const{ return mpRagdoll; }
		sgRagdoll *setRagdoll(sgRagdoll *ragdoll);

		/// create a component by its type(class name)
		sgComponent *createComponent(const sgStrHandle &type);
		void removeComponent(const sgStrHandle &type);
		sgComponent *getComponent(const sgStrHandle &type) const;

		void getAllObjects(SceneObjectVec &objVec);

		virtual void update(Float32 deltaTime);
        
        bool isCastShadow(void) const{ return mCastShadow; }
        void setCastShadow(bool bCastShadow);
        
    private:
        /// if this instance is a debug object
        bool mIsDebugObj;
    public:
        bool isDebugObj(void) const{ return mIsDebugObj; }
        void setIsDebugObj(bool debug);
        
    protected:
        /// the debug object attach to this instance
        //sgSceneObject *mDebugObjectToShow;
    public:
        //sgSceneObject *getDebugObjectToShow(void) const;
        //sgSceneObject *setDebugObjectToShow(sgSceneObject *obj);
        
        void showDebug(bool show);
        


	}; //#### end class sgSceneNode

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSCENEOBJECT_H__

