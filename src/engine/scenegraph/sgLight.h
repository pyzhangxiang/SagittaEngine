//////////////////////////////////////////////////////
// file: sgLight.h @ 2008-1-31 by Zhang Xiang
// declares of the class sgLight
// sgLight is a class ...
//////////////////////////////////////////////////////
#ifndef __SGLIGHT_H__
#define __SGLIGHT_H__

// INCLUDES //////////////////////////////////////////
#include "sgEsse.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgNode;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgLight : public sgEsse{
	// enum defines
	public:
		enum SEsseType{ SET_LIGHT = 3 };

	// member variables
	protected:
		Color m_Ambient;
		Color m_Diffuse;
		Color m_Specular;

		static unsigned int ms_iLightCount;

	// constructors & destructor
	public:
		sgLight(void);
		sgLight(const StdString &aName);
		~sgLight(void);

	// member functions
	public:
		virtual sgSceneObject *clone(void) const;

		/** Gets the ambient color of this light. */
		virtual const Color &ambientColor(void) const;

		/** Sets the ambient color of this light. */
		virtual void setAmbientColor(const Color &aColor);

		/** Gets the diffuse color of this light. */
		virtual const Color &diffuseColor(void) const;

		/** Sets the diffuse color of this light. */
		virtual void setDiffuseColor(const Color &aColor);

		/** Gets the specular color of this light. */
		virtual const Color &specularColor(void) const;

		/** Sets the specular color of this light. */
		virtual void setSpecularColor(const Color &aColor);

		/** Attachs this object to a node. */
		//virtual void attachToNode(sgNode *apAttachNode);

		///** Detach this object from the node it attached. */
		//virtual void detachFromNode();

	}; //#### end class sgLight

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGLIGHT_H__

