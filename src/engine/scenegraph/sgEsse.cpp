//////////////////////////////////////////////////////
// file: sgEsse.cpp 
// created by zhangxiang on 09-1-1
// declares of the class sgEsse
// sgEsse is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgEsse.h"
#include "sgScene.h"
#include "sgSceneNode.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/1/2009 zhangxiang]
	sgEsse::sgEsse(int aType /* = SET_UNKNOWN */)
	: sgSceneObject(), mc_iType(aType), m_pAttachedNode(0){

	}

	//  [1/1/2009 zhangxiang]
	sgEsse::sgEsse(const StdString &aName, int aType /* = SET_UNKNOWN */)
	: sgSceneObject(aName), mc_iType(aType), m_pAttachedNode(0){

	}

	//  [1/1/2009 zhangxiang]
	sgEsse::~sgEsse(void){
		if(m_pAttachedNode){
			m_pAttachedNode->detachEsse(this);
		}
	}

	//  [1/2/2009 zhangxiang]
	bool sgEsse::isActive(void) const{
		if(m_pAttachedNode && m_pAttachedNode->isActive() &&
			getScene()->getSceneManager() && sgSceneObject::isActive()){
			return true;
		}else{
			return false;
		}
	}

	//  [1/1/2009 zhangxiang]
	int sgEsse::getType(void) const{
		return mc_iType;
	}

	//  [1/1/2009 zhangxiang]
	Vector3 sgEsse::position(void) const{
		if(m_pAttachedNode){
			return m_pAttachedNode->position();
		}else{
			return Vector3::ZERO;
		}
	}

	//  [1/1/2009 zhangxiang]
	sgScene *sgEsse::getScene(void) const{
		if(m_pAttachedNode)
			return m_pAttachedNode->getScene();

		return 0;
	}

	//  [1/1/2009 zhangxiang]
	sgSceneNode *sgEsse::attachedNode(void) const{
		return m_pAttachedNode;
	}

	//  [1/1/2009 zhangxiang]
	bool sgEsse::isAttached(void) const{
		if(m_pAttachedNode)
			return true;
		else return false;
	}

	//  [1/1/2009 zhangxiang]
	void sgEsse::attachToNode(sgSceneNode *aNode){
		if(m_pAttachedNode == aNode)
			return ;

		if(m_pAttachedNode){
			m_pAttachedNode->detachEsse(this);
		}

		if(aNode){
			aNode->attachEsse(this);
		}

		m_pAttachedNode = aNode;
	}

	//  [1/1/2009 zhangxiang]
	void sgEsse::detachFromNode(void){
		if(m_pAttachedNode){
			m_pAttachedNode->detachEsse(this);
		}

		m_pAttachedNode = 0;
	}

	//  [1/1/2009 zhangxiang]
	void sgEsse::_notifyAttached(sgSceneNode *aNode){
		m_pAttachedNode = aNode;
	}

} // namespace Sagitta