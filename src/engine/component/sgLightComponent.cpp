
#include "sgMeshComponent.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/resource/sgMesh.h"
#include "sgLightComponent.h"


namespace Sagitta
{
	SG_META_DEFINE(sgLightComponent, sgComponent)

	sgLightComponent::sgLightComponent( void )
	:  m_Ambient(Color::BLACK),
	m_Diffuse(Color::WHITE), m_Specular(Color::WHITE)
	{

	}

	sgLightComponent::~sgLightComponent( void )
	{

	}

	//  [8/10/2008 zhangxiang]
	const Color &sgLightComponent::ambientColor(void) const{
		return m_Ambient;
	}

	//  [8/10/2008 zhangxiang]
	void sgLightComponent::setAmbientColor(const Color &aColor){
		m_Ambient = aColor;
	}

	//  [8/10/2008 zhangxiang]
	const Color &sgLightComponent::diffuseColor(void) const{
		return m_Diffuse;
	}

	//  [8/10/2008 zhangxiang]
	void sgLightComponent::setDiffuseColor(const Color &aColor){
		m_Diffuse = aColor;
	}

	//  [8/10/2008 zhangxiang]
	const Color &sgLightComponent::specularColor(void) const{
		return m_Specular;
	}

	//  [8/10/2008 zhangxiang]
	void sgLightComponent::setSpecularColor(const Color &aColor){
		m_Specular = aColor;
	}
}
