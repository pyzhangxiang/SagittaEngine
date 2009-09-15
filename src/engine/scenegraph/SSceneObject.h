//////////////////////////////////////////////////////
// file: SSceneObject.h @ 2008-1-31 by Zhang Xiang
// declares of the class SSceneObject
// SSceneObject is a class ...
//////////////////////////////////////////////////////
#ifndef __SSCENEOBJECT_H__
#define __SSCENEOBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"
#include "../../math/SMathHeader.h"
#include <vector>

// DECLARES //////////////////////////////////////////




namespace Sagitta{
	
	// TYPE DEFINE
//	typedef Vector4 Color;

	class SNode;
	class SScene;
	class SSceneManager;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SSceneObject : public SObject{
	// static members
	private:
		static uLong ms_iSceneObjectCount;

	// member variables
	private:
		/// object id
		const uLong mc_iSceneObjID;

		/// object name
		StdString m_sName;

		/// whether this object is active
		bool m_bActive;

	// constructors & destructor
	public:
		SSceneObject();
		SSceneObject(const std::string &aName);
		virtual ~SSceneObject(void);

	// member functions
	public:
		/** Clone this scene object - should be re-implemented by subclass*/
		virtual SSceneObject *clone(void) const = 0;

		/** Gets id. */
		uLong getSObjID(void) const;

		/** Gets my name. */
		const StdString &name(void) const;

		/** Sets my name. */
		void _setName(const StdString &aName);

		/** Check if I'm active. */
		virtual bool isActive(void) const;

		/** Sets active property. */
		virtual void setActive(bool aActive);

		/** Gets the position of this object in world space. */
		virtual Vector3 position(void) const = 0;
		
		/** Gets the SceneManager myself belong to. */
	//	virtual SSceneManager *getSceneManager(void) const = 0;

		/** Gets the scene i'm in. */
		virtual SScene *getScene(void) const = 0;

		/** Checks if i'm in a scene. */
		bool isInScene(void) const;

	}; //#### end class SSceneObject

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SSCENEOBJECT_H__