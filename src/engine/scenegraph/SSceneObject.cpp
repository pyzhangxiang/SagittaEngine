//////////////////////////////////////////////////////
// file: SSceneObject.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class SSceneObject
// SSceneObject is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SSceneObject.h"
#include "SNode.h"
#include "../../common/utils/SString.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong SSceneObject::ms_iSceneObjectCount = 1;

	//  [7/30/2008 zhangxiang]
	SSceneObject::SSceneObject() : mc_iSceneObjID(ms_iSceneObjectCount), m_bActive(true){
		m_sName = "SceneObject" + SString::to_string(SSceneObject::ms_iSceneObjectCount++);
	}

	//  [7/30/2008 zhangxiang]
	SSceneObject::SSceneObject(const std::string &aName) : mc_iSceneObjID(ms_iSceneObjectCount), m_sName(aName), m_bActive(true){
		SSceneObject::ms_iSceneObjectCount++;
	}

	//  [7/30/2008 zhangxiang]
	SSceneObject::~SSceneObject(){
	
	}

	//  [1/1/2009 zhangxiang]
	uLong SSceneObject::getSObjID(void) const{
		return mc_iSceneObjID;
	}

	//  [7/30/2008 zhangxiang]
	const StdString &SSceneObject::name(void) const{
		return m_sName;
	}

	//  [12/23/2008 zhangxiang]
	void SSceneObject::_setName(const StdString &aName){
		m_sName = aName;
	}

	//  [1/2/2009 zhangxiang]
	bool SSceneObject::isActive(void) const{
		return m_bActive;
	}

	//  [1/2/2009 zhangxiang]
	void SSceneObject::setActive(bool aActive){
		m_bActive = aActive;
	}

	//  [1/6/2009 zhangxiang]
	bool SSceneObject::isInScene(void) const{
		if(getScene())
			return true;
		else return false;
	}

} // namespace Sagitta