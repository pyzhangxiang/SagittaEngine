//////////////////////////////////////////////////////
// file: sgSceneObject.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgSceneObject
// sgSceneObject is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgSceneObject.h"
#include "sgNode.h"
#include "../../common/utils/sgStringUtil.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong sgSceneObject::ms_iSceneObjectCount = 1;

	//  [7/30/2008 zhangxiang]
	sgSceneObject::sgSceneObject() : mc_iSceneObjID(ms_iSceneObjectCount), m_bActive(true){
		m_sName = "SceneObject" + sgStringUtil::to_string(sgSceneObject::ms_iSceneObjectCount++);
	}

	//  [7/30/2008 zhangxiang]
	sgSceneObject::sgSceneObject(const std::string &aName) : mc_iSceneObjID(ms_iSceneObjectCount), m_sName(aName), m_bActive(true){
		sgSceneObject::ms_iSceneObjectCount++;
	}

	//  [7/30/2008 zhangxiang]
	sgSceneObject::~sgSceneObject(){
	
	}

	//  [1/1/2009 zhangxiang]
	uLong sgSceneObject::getSObjID(void) const{
		return mc_iSceneObjID;
	}

	//  [7/30/2008 zhangxiang]
	const StdString &sgSceneObject::name(void) const{
		return m_sName;
	}

	//  [12/23/2008 zhangxiang]
	void sgSceneObject::_setName(const StdString &aName){
		m_sName = aName;
	}

	//  [1/2/2009 zhangxiang]
	bool sgSceneObject::isActive(void) const{
		return m_bActive;
	}

	//  [1/2/2009 zhangxiang]
	void sgSceneObject::setActive(bool aActive){
		m_bActive = aActive;
	}

	//  [1/6/2009 zhangxiang]
	bool sgSceneObject::isInScene(void) const{
		if(getScene())
			return true;
		else return false;
	}

} // namespace Sagitta