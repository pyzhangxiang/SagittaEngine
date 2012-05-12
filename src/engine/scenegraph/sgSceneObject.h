//////////////////////////////////////////////////////
// file: sgSceneObject.h @ 2008-1-31 by Zhang Xiang
// declares of the class sgSceneObject
// sgSceneObject is a class ...
//////////////////////////////////////////////////////
#ifndef __SGSCENEOBJECT_H__
#define __SGSCENEOBJECT_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/sgObject.h"
#include "../../math/sgMathHeader.h"
#include <vector>

// DECLARES //////////////////////////////////////////




namespace Sagitta{
	
	// TYPE DEFINE
//	typedef Vector4 Color;

	class sgNode;
	class sgScene;
	class sgSceneManager;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgSceneObject : public sgObject{
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
		sgSceneObject();
		sgSceneObject(const std::string &aName);
		virtual ~sgSceneObject(void);

	// member functions
	public:
		/** Clone this scene object - should be re-implemented by subclass*/
		virtual sgSceneObject *clone(void) const = 0;

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
	//	virtual sgSceneManager *getSceneManager(void) const = 0;

		/** Gets the scene i'm in. */
		virtual sgScene *getScene(void) const = 0;

		/** Checks if i'm in a scene. */
		bool isInScene(void) const;

	}; //#### end class sgSceneObject

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSCENEOBJECT_H__


