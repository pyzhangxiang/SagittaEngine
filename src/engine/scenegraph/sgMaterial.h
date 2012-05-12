//////////////////////////////////////////////////////
// file: sgMaterial.h @ 2008-8-11 by Zhang Xiang
// declares of the class sgMaterial
// sgMaterial is a class ...
//////////////////////////////////////////////////////
#ifndef __SGMATERIAL_H__
#define __SGMATERIAL_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/sgObject.h"
#include "sgSceneObject.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgMaterial : public sgObject{
		// member variables
	protected:
		Color m_AmbientColor;
		Color m_DiffuseColor;
		Color m_SpecularColor;
		Color m_EmissionColor;
		Real m_Shininess;	//(0, infinity). for phong lighting  a.k.a shineness. 
		Real m_SpecularAmount;	//[0,infinity]. 0 = matt
		Real m_ReflectFraction; // [0,1]. 1 == total reflection, i.e. a perfect mirror

		// constructors & destructor
	public:
		sgMaterial(void);
		sgMaterial(const Color &aAmbientColor, const Color &aDiffuseColor, const Color &aSpecularColor, const Color &aEmissionColor,
				Real aShininess, Real aSpecularAmount, Real aReflectFraction);
		sgMaterial(const sgMaterial &aMaterial);
		virtual ~sgMaterial(void);

		// member functions
	public:
		/** Gets the ambient color of this material. */
		virtual const Color &ambientColor(void) const;
		/** Sets the ambient color of this material. */
		virtual void setAmbientColor(const Color &aAmbientColor);

		/** Gets the diffuse color of this material. */
		virtual const Color &diffuseColor(void) const;
		/** Sets the diffuse color of this material. */
		virtual void setDiffuseColor(const Color &aDiffuseColor);

		/** Gets the specular color of this material. */
		virtual const Color &specularColor(void) const;
		/** Sets the specular color of this material. */
		virtual void setSpecularColor(const Color &aSpecularColor);

		/** Gets the emission color of this material. */
		virtual const Color &emissionColor(void) const;
		/** Sets the emission color of this material. */
		virtual void setEmissionColor(const Color &aEmissionColor);

		/** Gets the shininess of this material. */
		virtual Real shininess(void) const;
		/** Sets the shininess of this material. */
		virtual void setShininess(Real aShininess);

		/** Gets the specular amount of this material. */
		virtual Real specularAmount(void) const;
		/** Sets the specular amount of this material. */
		virtual void setSpecularAmount(Real aSpecularAmount);

		/** Gets the reflect fraction of this material. */
		virtual Real reflectFraction(void) const;
		/** Sets the reflect fraction of this material. */
		virtual void setReflectFraction(Real aReflectFraction);

		virtual sgMaterial& operator = (const sgMaterial &aMaterial);

	}; //#### end class sgMaterial

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGMATERIAL_H__

