//////////////////////////////////////////////////////
// file: SLight.h @ 2008-1-31 by Zhang Xiang
// declares of the class SLight
// SLight is a class ...
//////////////////////////////////////////////////////
#ifndef __SLIGHT_H__
#define __SLIGHT_H__

// INCLUDES //////////////////////////////////////////
#include "SEsse.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SNode;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SLight : public SEsse{
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
		SLight(void);
		SLight(const StdString &aName);
		~SLight(void);

	// member functions
	public:
		virtual SSceneObject *clone(void) const;

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
		//virtual void attachToNode(SNode *apAttachNode);

		///** Detach this object from the node it attached. */
		//virtual void detachFromNode();

	}; //#### end class SLight

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SLIGHT_H__