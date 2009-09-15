//////////////////////////////////////////////////////
// file: SMaterial.cpp @ 2008-8-11 by Zhang Xiang
// defines of the class SMaterial
// SMaterial is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SMaterial.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	//  [8/11/2008 zhangxiang]
	SMaterial::SMaterial(void){
		m_AmbientColor = Color(Color::GLColor(0.2, 0.2, 0.2, 1.0));
		m_DiffuseColor = Color(Color::GLColor(0.2, 0.2, 0.2, 1.0));
		m_SpecularColor = Color(Color::GLColor(1.0, 1.0, 1.0, 1.0));
		m_EmissionColor = Color(Color::GLColor(0.0, 0.0, 0.0, 1.0));

		m_Shininess = 0.0;
		m_SpecularAmount = 0.5;
		m_ReflectFraction = 0.0;
	}

	//  [8/11/2008 zhangxiang]
	SMaterial::SMaterial(const Color &aAmbientColor, const Color &aDiffuseColor, const Color &aSpecularColor, const Color &aEmissionColor,
						Real aShininess, Real aSpecularAmount, Real aReflectFraction) :
					m_AmbientColor(aAmbientColor), m_DiffuseColor(aDiffuseColor), m_SpecularColor(aSpecularColor), m_EmissionColor(aEmissionColor),
					m_Shininess(aShininess), m_SpecularAmount(aSpecularAmount), m_ReflectFraction(aReflectFraction){

	}

	//  [8/11/2008 zhangxiang]
	SMaterial::SMaterial(const SMaterial &aMaterial) :
						m_AmbientColor(aMaterial.ambientColor()), m_DiffuseColor(aMaterial.diffuseColor()), m_SpecularColor(aMaterial.specularColor()), m_EmissionColor(aMaterial.emissionColor()),
						m_Shininess(aMaterial.shininess()), m_SpecularAmount(aMaterial.specularAmount()), m_ReflectFraction(aMaterial.reflectFraction()){

	}

	//  [8/11/2008 zhangxiang]
	SMaterial::~SMaterial(){

	}

	//  [8/11/2008 zhangxiang]
	const Color &SMaterial::ambientColor(void) const{
		return m_AmbientColor;
	}
	
	//  [8/11/2008 zhangxiang]
	void SMaterial::setAmbientColor(const Color &aAmbientColor){
		m_AmbientColor = aAmbientColor;
	}

	//  [8/11/2008 zhangxiang]
	const Color &SMaterial::diffuseColor(void) const{
		return m_DiffuseColor;
	}

	//  [8/11/2008 zhangxiang]
	void SMaterial::setDiffuseColor(const Color &aDiffuseColor){
		m_DiffuseColor = aDiffuseColor;
	}

	//  [8/11/2008 zhangxiang]
	const Color &SMaterial::specularColor(void) const{
		return m_SpecularColor;
	}
	
	//  [8/11/2008 zhangxiang]
	void SMaterial::setSpecularColor(const Color &aSpecularColor){
		m_SpecularColor = aSpecularColor;
	}

	//  [8/11/2008 zhangxiang]
	const Color &SMaterial::emissionColor(void) const{
		return m_EmissionColor;
	}
	
	//  [8/11/2008 zhangxiang]
	void SMaterial::setEmissionColor(const Color &aEmissionColor){
		m_EmissionColor = aEmissionColor;
	}

	//  [8/11/2008 zhangxiang]
	Real SMaterial::shininess(void) const{
		return m_Shininess;
	}
	
	//  [8/11/2008 zhangxiang]
	void SMaterial::setShininess(Real aShininess){
		m_Shininess = aShininess;
	}

	//  [8/11/2008 zhangxiang]
	Real SMaterial::specularAmount(void) const{
		return m_SpecularAmount;
	}
	
	//  [8/11/2008 zhangxiang]
	void SMaterial::setSpecularAmount(Real aSpecularAmount){
		m_SpecularAmount = aSpecularAmount;
	}

	//  [8/11/2008 zhangxiang]
	Real SMaterial::reflectFraction(void) const{
		return m_ReflectFraction;
	}

	//  [8/11/2008 zhangxiang]
	void SMaterial::setReflectFraction(Real aReflectFraction){
		m_ReflectFraction = aReflectFraction;
	}

	//  [8/11/2008 zhangxiang]
	SMaterial& SMaterial::operator = (const SMaterial &aMaterial){
		m_AmbientColor = aMaterial.ambientColor();
		m_DiffuseColor = aMaterial.diffuseColor();
		m_SpecularColor = aMaterial.specularColor();
		m_EmissionColor = aMaterial.emissionColor();
		m_Shininess = aMaterial.shininess();
		m_SpecularAmount = aMaterial.specularAmount();
		m_ReflectFraction = aMaterial.reflectFraction();

		return *this;
	}

} // namespace Sagitta