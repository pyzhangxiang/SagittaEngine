//////////////////////////////////////////////////////
// file: sgSceneNode.h 
// created by zhangxiang on 09-1-1
// declares of the class sgSceneNode
// sgSceneNode is a class ...
//////////////////////////////////////////////////////
#ifndef __SGSCENENODE_H__
#define __SGSCENENODE_H__

// INCLUDES //////////////////////////////////////////
#include "sgNode.h"
#include <vector>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgScene;

	//  [1/7/2009 zhangxiang]
	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport SAbstractSceneNode : public sgNode{
	// member variables
	private:
		/** scene I'm in. */
		sgScene *m_pScene;

	// constructors & destructor
	public:
		SAbstractSceneNode(sgScene *aScene = 0);
		SAbstractSceneNode(const StdString &aName, sgScene *aScene = 0);
		virtual ~SAbstractSceneNode(void) = 0;

	// member functions
	private:
		/** Overrides from sgNode, disable it.
			@remarks Throw an exception.
		*/
		void stepToParent(sgNode *aNewParent);

		/** Overrides from sgNode, disable it.
			@remarks Throw an exception.
		*/
		void addChild(sgNode* aChild);

		/** A Internal hook. 
			@remarks
				Does something when the destination node is 
				in a scene but not the same as mine.
				Default, throw a exception.
		*/
		virtual void doSthIfNotInTheSameScene(SAbstractSceneNode *aNode);

		/** A internal hook ask a Scene to do something.
			@remarks
				Post child added, let the new child ask my scene to do
				somthing for it. Default, do nothing.
				Only called by SAbstractSceneNode::addChild().
			@param
				aScene Usually the scene my parent in.
	
		*/
		virtual void askSceneToDoSth(sgScene *aScene);

	public:
		/** Disables clone method. Overrides from sgSceneObject. */
		sgNode *clone(void) const{ return 0; }

		/** Checks if i'm in a scene. */
		bool isInScene(void) const;

		/** Gets getScene method. Overrides from sgSceneObject. */
		sgScene *getScene(void) const;

		/** Internal method only called by sgScene::addSceneNode. 
			Sets my Scene.
		*/
		void _setScene(sgScene *aScene);

		/** Step myself to another parent
			@remarks
				Only Succeed when i'm not in any scene or
				the new parent is in the same scene as me.
		*/
		void stepToParent(SAbstractSceneNode *aNewParent);

		/** Adds a (precreated) child scene node to this node. If it is attached to another node,
			it must be detached first.
			@remarks Will check if the child is in the same scene.
			@param aChild The Node which is to become a child node of this one
		*/
		void addChild(SAbstractSceneNode *aChild);

	}; //#### end class AbstractSceneNode


	class sgEsse;
	class sgSceneManager;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgSceneNode : public SAbstractSceneNode{
	// type defines
	protected:
		typedef std::map<uLong, sgEsse*> EsseMap;
		typedef std::map<int, EsseMap*> TypeEsseMap;

	// member variables
	private:
		/** Esse mapped by type. */
		TypeEsseMap m_EsseMap;

		/** esse count. */
		size_t m_iEsseCount;

	// constructors & destructor
	public:
		sgSceneNode(void);
		sgSceneNode(const StdString &aName);
		virtual ~sgSceneNode(void);

	// member functions
	private:
		/** Ask a Scene to do something. Overrides from SAbstractSceneNode
			@remarks
				Internal method. Only called by SAbstractSceneNode::addChild().
			@param
				aScene Usually the scene my parent in.
	
		*/
		void askSceneToDoSth(sgScene *aScene);
		
	public:
		/** Clones this scene node. Overrides from sgSceneObject. */
		sgNode *clone(void) const;

		/** Attaches an esse to me.
			@remarks
				Add the specified esse to the scene this node is in.
		*/
		void attachEsse(sgEsse *aEsse);
		/** Detaches an esse from me by index.
			@remarks
				Will remove the specified esse from the scene it was in.
		*/
		sgEsse *detachEsse(int aType, size_t aIndex);
		/** Detaches an esse from me.
			@remarks
				Will remove the specified esse from the scene it was in.
		*/
		sgEsse *detachEsse(sgEsse *aEsse);
		/** Detaches an esse from me and attach it to another one.
			@remarks
				The other node the specified esse will attach to must be in
				the same scene as this one.
		*/
		void detachEsseTo(sgEsse *aEsse, sgSceneNode *aAnoNode);

		/** Gets an esse by name.
		*	@remarks
		*		There may be more than one esse attached to this node with deffernt type.
		*		This method only return the first one it find.
		*/
		sgEsse *getEsse(const StdString &aName) const;
		/** Gets an esse by type and name. */
		sgEsse *getEsse(int aType, const StdString &aName) const;
		/** Gets an esse by index. */
		sgEsse *getEsse(size_t aIndex) const;
		/** Gets an esse by type and index. */
		sgEsse *getEsse(int aType, size_t aIndex) const;
		/* Gets esses by name.
			@remarks Finds all esses by name.
			@param outEsses the esses found
		*/
		void getEsses(const StdString &aName, std::vector<sgEsse*> &outEsses) const;

		/** Gets specified type esse num. */
		size_t getTypedEsseNum(int aType) const;

		/** Gets esse num. */
		size_t getEsseNum(void) const;


	}; //#### end class sgSceneNode

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSCENENODE_H__

