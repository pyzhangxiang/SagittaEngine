//////////////////////////////////////////////////////
// file: SLight.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class SLight
// SLight is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SLight.h"
#include "SNode.h"
#include "../../common/utils/SString.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	unsigned int SLight::ms_iLightCount = 1;

	SLight::SLight(void)
	: SEsse(SET_LIGHT), m_Ambient(Color::BLACK),
	m_Diffuse(Color::WHITE), m_Specular(Color::WHITE){
		StdString name = "Light" + SString::to_string(SLight::ms_iLightCount++);
		SSceneObject::_setName(name);
	}

	//  [1/1/2009 zhangxiang]
	SLight::SLight(const StdString &aName)
	: SEsse(aName, SET_LIGHT), m_Ambient(Color::BLACK),
	m_Diffuse(Color::WHITE), m_Specular(Color::WHITE){

	}

	SLight::~SLight(){

	}

	SSceneObject *SLight::clone(void) const{
		// for future...
		SLight *light = new SLight;
		return light;
	}

	//  [8/10/2008 zhangxiang]
	const Color &SLight::ambientColor(void) const{
		return m_Ambient;
	}

	//  [8/10/2008 zhangxiang]
	void SLight::setAmbientColor(const Color &aColor){
		m_Ambient = aColor;
	}

	//  [8/10/2008 zhangxiang]
	const Color &SLight::diffuseColor(void) const{
		return m_Diffuse;
	}

	//  [8/10/2008 zhangxiang]
	void SLight::setDiffuseColor(const Color &aColor){
		m_Diffuse = aColor;
	}

	//  [8/10/2008 zhangxiang]
	const Color &SLight::specularColor(void) const{
		return m_Specular;
	}

	//  [8/10/2008 zhangxiang]
	void SLight::setSpecularColor(const Color &aColor){
		m_Specular = aColor;
	}

	//  [8/10/2008 zhangxiang]
	//void SLight::attachToNode(SNode *apAttachNode){
	//	if(m_pAttachedNode){
	//		// for future... if debug then output the process
	//		detachFromNode();
	//	}
	//	apAttachNode->attachObject(this);
	//}

	////  [8/10/2008 zhangxiang]
	//void SLight::detachFromNode(){
	//	if(m_pAttachedNode)
	//		m_pAttachedNode->detachObject(this);
	//}

} // namespace Sagitta