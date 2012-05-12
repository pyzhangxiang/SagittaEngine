//////////////////////////////////////////////////////
// file: sgLight.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgLight
// sgLight is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgLight.h"
#include "sgNode.h"
#include "../../common/utils/sgStringUtil.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	unsigned int sgLight::ms_iLightCount = 1;

	sgLight::sgLight(void)
	: sgEsse(SET_LIGHT), m_Ambient(Color::BLACK),
	m_Diffuse(Color::WHITE), m_Specular(Color::WHITE){
		StdString name = "Light" + sgStringUtil::to_string(sgLight::ms_iLightCount++);
		sgSceneObject::_setName(name);
	}

	//  [1/1/2009 zhangxiang]
	sgLight::sgLight(const StdString &aName)
	: sgEsse(aName, SET_LIGHT), m_Ambient(Color::BLACK),
	m_Diffuse(Color::WHITE), m_Specular(Color::WHITE){

	}

	sgLight::~sgLight(){

	}

	sgSceneObject *sgLight::clone(void) const{
		// for future...
		sgLight *light = new sgLight;
		return light;
	}

	//  [8/10/2008 zhangxiang]
	const Color &sgLight::ambientColor(void) const{
		return m_Ambient;
	}

	//  [8/10/2008 zhangxiang]
	void sgLight::setAmbientColor(const Color &aColor){
		m_Ambient = aColor;
	}

	//  [8/10/2008 zhangxiang]
	const Color &sgLight::diffuseColor(void) const{
		return m_Diffuse;
	}

	//  [8/10/2008 zhangxiang]
	void sgLight::setDiffuseColor(const Color &aColor){
		m_Diffuse = aColor;
	}

	//  [8/10/2008 zhangxiang]
	const Color &sgLight::specularColor(void) const{
		return m_Specular;
	}

	//  [8/10/2008 zhangxiang]
	void sgLight::setSpecularColor(const Color &aColor){
		m_Specular = aColor;
	}

	//  [8/10/2008 zhangxiang]
	//void sgLight::attachToNode(sgNode *apAttachNode){
	//	if(m_pAttachedNode){
	//		// for future... if debug then output the process
	//		detachFromNode();
	//	}
	//	apAttachNode->attachObject(this);
	//}

	////  [8/10/2008 zhangxiang]
	//void sgLight::detachFromNode(){
	//	if(m_pAttachedNode)
	//		m_pAttachedNode->detachObject(this);
	//}

} // namespace Sagitta