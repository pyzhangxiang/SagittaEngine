/// @2012.9.27 by zhangxiang

#ifndef __SGLIGHTCOMPONENT_H__
#define __SGLIGHTCOMPONENT_H__


#include "sgComponent.h"
#include "math/sgColor.h"


namespace Sagitta{

	class sgMesh;

	class _SG_KernelExport sgLightComponent : public sgComponent
	{
		SG_META_DECLARE(sgLightComponent)

	protected:
		Color m_Ambient;
		Color m_Diffuse;
		Color m_Specular;
		Real mIntensity;

	public:
		sgLightComponent(void);
		virtual ~sgLightComponent(void);

		/** Gets the ambient color of this light. */
		const Color &ambientColor(void) const;

		/** Sets the ambient color of this light. */
		void setAmbientColor(const Color &aColor);

		/** Gets the diffuse color of this light. */
		const Color &diffuseColor(void) const;

		/** Sets the diffuse color of this light. */
		void setDiffuseColor(const Color &aColor);

		/** Gets the specular color of this light. */
		const Color &specularColor(void) const;

		/** Sets the specular color of this light. */
		void setSpecularColor(const Color &aColor);

		Real getIntensity(void) const{ return mIntensity; }
		void setIntensity(Real i){ mIntensity = i; }

	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGLIGHTCOMPONENT_H__

