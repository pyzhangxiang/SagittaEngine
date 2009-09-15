//////////////////////////////////////////////////////
// file: SEsse.h 
// created by zhangxiang on 09-1-1
// declares of the class SEsse
// SEsse is a class ...
//////////////////////////////////////////////////////
#ifndef __SESSE_H__
#define __SESSE_H__

// INCLUDES //////////////////////////////////////////
#include "SSceneObject.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SScene;
	class SSceneNode;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SEsse : public SSceneObject{
	// enum defines
	public:
		/** scene object type */
		enum SEsseType{ SET_UNKNOWN = 0 };

	// member variables
	private:
		/** scene object type */
		const int mc_iType;
		/** scene object type by string */
		const StdString mc_sType;
		/** the node I attached */
		SSceneNode *m_pAttachedNode;

	// constructors & destructor
	public:
		SEsse(int aiType = SET_UNKNOWN/*, const StdString &asType = "unknown"*/);
		SEsse(const StdString &aName, int aiType = SET_UNKNOWN/*, const StdString &asType = "unknown"*/);
		virtual ~SEsse(void);

	// member functions
	public:
		/** Checks if I'm active. Overrides from SSceneObject.
			@remarks Only the case when I'm attached on a node and it is active and I'm active too, it returns true.
		*/
		bool isActive(void) const;

		/** Gets my type by integer. */
		int getType(void) const;

		/** Gets the position of this esse in world space. Overrides from SSceneObject.
		*/
		virtual Vector3 position(void) const;

		/** Gets the scene manager myself belong to. Overrides from SSceneObject. */
		SScene *getScene(void) const;

		/** Gets the node this object attached. */
		SSceneNode *attachedNode(void) const;

		/** Checks if attached to a node. */
		bool isAttached(void) const;

		/** Attach myself to a node. */
		void attachToNode(SSceneNode *aNode);

		/** Detach myself from the node it attached.
		*	@remarks Will set m_pAttachedNode to be 0.
		*/
		void detachFromNode(void);

		/** Internal method called to notify the object that it has been attached to a node. */
		virtual void _notifyAttached(SSceneNode *aNode);

	}; //#### end class SEsse

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SESSE_H__