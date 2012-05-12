//////////////////////////////////////////////////////
// file: sgScene.h @ 2008-7-30 by Zhang Xiang
// declares of the class sgScene
// sgScene is a class ...
//////////////////////////////////////////////////////
#ifndef __SGSCENE_H__
#define __SGSCENE_H__

// INCLUDES //////////////////////////////////////////
#include "sgSceneNode.h"
#include "../resource/sgResource.h"
#include "../resource/sgSceneDelegate.h"
#include "../../common/utils/sgIterator.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgEsse;
	class sgNode;
	class sgSceneNode;
	class sgSubScene;
	class sgSceneManager;
	class sgAnimation;

	/** Binding Map. */
	class _SG_KernelExport BindingMap{
	// type defines
	private:
		typedef std::map<uLong, uLong> IDMap;

	// member variables
	private:
		IDMap m_BindingMap;

	// constructors & destructor
	
	// member functions
	public:
		/** add a binding info. */
		void addBindingInfo(uLong aSceneNodeID, uLong aAnimaJointID);

		/** remove a binding info. */
		void removeBindingInfo(uLong aSceneNodeID);

		/** find a binded AnimaJoint id.
			@return
				If found, will return a positive number;
				Or, will return -1
		*/
		int findAnimaJoint(uLong aSceneNodeID) const;

	}; //#### end class BindingMap

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgScene : public sgResource{
	// inner class declares
	protected:
		/** Used to joint sgSubScene and sgSceneManager. */
		class _SG_KernelExport SSceneJoint : public SAbstractSceneNode{
		// friend classes
		public:
			friend class sgScene;
			friend class sgSceneManager;

		// member variables

		// constructors & destructor
		public:
			SSceneJoint(sgScene *aScene) : SAbstractSceneNode(aScene){}
			SSceneJoint(const StdString &aName, sgScene *aScene) : SAbstractSceneNode(aName, aScene){}
		protected:
			/** Only sgScene can destruct SSceneJoint. */
			~SSceneJoint(void){}

		// member functions
		private:
			/** Notifies i'm a root node in a scene.
				@remarks Called by sgScene::sgScene
			*/
			void notifyRoot(void){ setActive(true); }

			/** A Internal hook. Overrides from SAbstractSceneNode.
			@remarks
				Does something when the destination Node is 
				in a scene but not the same as mine.
				If the destination Node is not in any SceneManager,
				and I'm in a SceneManager, that mean a new SubScene who
				contains the destination Node is like to be added to the
				SceneManager I'm belong in.
			*/
			void doSthIfNotInTheSameScene(SAbstractSceneNode *aNode);


		}; //#### end class SJointNode

	// type defines
	protected:
		// key_value is node's scene object id
		typedef std::map<uLong, sgSceneNode*> SceneNodeMap;
		// key_value is esse's scene object id
		typedef std::map<uLong, sgEsse*> EsseMap;
		// key_value is esse's type
		typedef std::map<int, EsseMap*> TypeEsseMap;

	public:
		typedef sgConstMapIterator<EsseMap> ConstTypedEsseIterator;
		typedef sgConstMapIterator<SceneNodeMap> ConstSceneNodeIterator;

	// static members
	private:
		static uLong ms_iSceneCount;

	// member variables
	private:
		/// the root scene node
		SSceneJoint *m_pRoot;

		/// scene node mapped by id
		SceneNodeMap m_SceneNodeMap;

		/// esse map by type
		TypeEsseMap m_EsseMap;

		/// animation data
		sgAnimation *m_pAnimation;

		/// binding map
		BindingMap m_BindingMap;

		/** the scene manager i'm in
			if i'm not in any scene manager, the value is 0
		*/
		sgSceneManager *m_pSceneManager;

		/// visible property
		bool m_bVisible;

		/// total frame num, get value from Animation
	//	size_t m_iFrameNum;

		/// current frame
		int m_iCurrentFrame;

		/// if true, this scene could repeat self
		bool m_bLoopFrame;

	// constructors & destructor
	public:
		sgScene(sgSceneDelegate *aDelegate = new sgSceneDelegate);
		sgScene(const StdString &aName, sgSceneDelegate *aDelegate = new sgSceneDelegate);
		virtual ~sgScene(void) = 0;

	// member functions
	private:
		/** Internal hook does unloading. Overrides from sgResource. */
		void unLoadImpl(void);

	public:
		/** Gets my root node. */
		SSceneJoint *root(void) const;

		/** Adds new child node to root. */
		void addChildToRoot(sgSceneNode *aChild);
		
		/** Returns if this scene is visible. */
		bool isVisible(void) const;
		/** Sets visble property. */
		void setVisible(bool aVisible);

		/** Gets Animation data. */
		sgAnimation *getAnimation(void) const;
		/** Sets Animation data.
			@return The original Animation.
		*/
		sgAnimation *setAnimation(sgAnimation *aAnimation);

		/** Gets BindingMap. */
		const BindingMap &bindingMap(void) const;
		/** Gets BindingMap. */
		BindingMap &bindingMap(void);

		/** Gets the scene manager i'm in.
			If i'm not in any scene, return 0.
		*/
		sgSceneManager *getSceneManager(void) const;

		/** Sets the SceneManager i'm in.
			@remarks
				Internal method. Only called by sgSceneManager::addSubScene() and
				sgSceneManager::removeSubScene().
		*/
		void _setSceneManager(sgSceneManager *aSM);

		/** Creates a new node.
			@remark
				If I have root(always have expect initialization of myself),
				make the new node to be root's child.
			@par
				key word "virtual" let sub-classes have the ability to 
				disable this method(etc. sgSkeleton should not can create sgSceneNode but
				SBoneNode.
		*/
		virtual sgSceneNode *createSceneNode(void);

		/** Creates a new node by name.
			@remark
				If I have root(always have expect initialization of myself),
				make the new node to be root's child.
			@par
				key word "virtual" let sub-classes have the ability to 
				disable this method(etc. sgSkeleton should not can create sgSceneNode but
				SBoneNode.
		*/
		virtual sgSceneNode *createSceneNode(const StdString &asName);

		/** Adds a new node to this SceneManager.
		*/
		void addSceneNode(sgSceneNode *aNode);

		/** Removes a scene node from this scene.
			@remarks Will remove all esses attached on the specified node and all children of it.
		*/
		void removeSceneNode(sgSceneNode *aNode);

		/** Adds an esse to the scene. Internal method. */
		void _addEsse(sgEsse *aEsse);

		/** Remove an esse from the scene. Internal method. */
		void _removeEsse(sgEsse *aEsse);

		/** Gets specified esse by index. */
		sgEsse *getTypedEsse(int aType, size_t aIndex) const;

		/** Gets specified type esse num. */
		size_t getTypedEsseNum(int aType) const;

		/** Gets esse num. */
		size_t getEsseNum(void) const;

		/** Merge the specified scene.
			@remark
				Moves all element of the source scene to me,
				and delete the source scene.
		*/
		void merge(sgSubScene *aScene);

		/** Gets Esse iterator by type. */
		ConstTypedEsseIterator getConstEsseIterator(int aType) const;

		/** Gets const SceneNode iterator. */
		ConstSceneNodeIterator getConstSceneNodeIterator(void) const;

		/** Check if this scene is loop frame. */
		bool isLoopFrame(void) const;
		/** Sets the property of loop frame. */
		void setLoopFrame(bool aLoop);

		/** Gets total frame num. */
		size_t frameNum(void) const;

		/** Gets current frame. */
		int currentFrame(void) const;

		/** Sets current frame. */
		virtual void setCurrentFrame(size_t aFrame);

		/** Step up or down current frame.
			@remarks
				If the current frame is out of range,
				it will be intercepted into 0 - m_iFrameNum.
		*/
		virtual void stepFrame(int aSteps);

		/** Update all SceneNode's from Animation by current frame.
			@par
				Renderer will call this method before rendering.
				So you need not call it.
				
		*/
		virtual void _updateFromAnimation(void);

	}; //#### end class SMenu

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSCENE_H__

