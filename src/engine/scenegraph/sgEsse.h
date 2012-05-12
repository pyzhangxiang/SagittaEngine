//////////////////////////////////////////////////////
// file: sgEsse.h 
// created by zhangxiang on 09-1-1
// declares of the class sgEsse
// sgEsse is a class ...
//////////////////////////////////////////////////////
#ifndef __SGESSE_H__
#define __SGESSE_H__

// INCLUDES //////////////////////////////////////////
#include "sgSceneObject.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgScene;
	class sgSceneNode;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgEsse : public sgSceneObject{
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
		sgSceneNode *m_pAttachedNode;

	// constructors & destructor
	public:
		sgEsse(int aiType = SET_UNKNOWN/*, const StdString &asType = "unknown"*/);
		sgEsse(const StdString &aName, int aiType = SET_UNKNOWN/*, const StdString &asType = "unknown"*/);
		virtual ~sgEsse(void);

	// member functions
	public:
		/** Checks if I'm active. Overrides from sgSceneObject.
			@remarks Only the case when I'm attached on a node and it is active and I'm active too, it returns true.
		*/
		bool isActive(void) const;

		/** Gets my type by integer. */
		int getType(void) const;

		/** Gets the position of this esse in world space. Overrides from sgSceneObject.
		*/
		virtual Vector3 position(void) const;

		/** Gets the scene manager myself belong to. Overrides from sgSceneObject. */
		sgScene *getScene(void) const;

		/** Gets the node this object attached. */
		sgSceneNode *attachedNode(void) const;

		/** Checks if attached to a node. */
		bool isAttached(void) const;

		/** Attach myself to a node. */
		void attachToNode(sgSceneNode *aNode);

		/** Detach myself from the node it attached.
		*	@remarks Will set m_pAttachedNode to be 0.
		*/
		void detachFromNode(void);

		/** Internal method called to notify the object that it has been attached to a node. */
		virtual void _notifyAttached(sgSceneNode *aNode);

	}; //#### end class sgEsse

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGESSE_H__

