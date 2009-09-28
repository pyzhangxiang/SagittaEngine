//////////////////////////////////////////////////////
// file: SEsse.cpp 
// created by zhangxiang on 09-1-1
// declares of the class SEsse
// SEsse is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SEsse.h"
#include "SScene.h"
#include "SSceneNode.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/1/2009 zhangxiang]
	SEsse::SEsse(int aType /* = SET_UNKNOWN */)
	: SSceneObject(), mc_iType(aType), m_pAttachedNode(0){

	}

	//  [1/1/2009 zhangxiang]
	SEsse::SEsse(const StdString &aName, int aType /* = SET_UNKNOWN */)
	: SSceneObject(aName), mc_iType(aType), m_pAttachedNode(0){

	}

	//  [1/1/2009 zhangxiang]
	SEsse::~SEsse(void){
		if(m_pAttachedNode){
			m_pAttachedNode->detachEsse(this);
		}
	}

	//  [1/2/2009 zhangxiang]
	bool SEsse::isActive(void) const{
		if(m_pAttachedNode && m_pAttachedNode->isActive() &&
			getScene()->getSceneManager() && SSceneObject::isActive()){
			return true;
		}else{
			return false;
		}
	}

	//  [1/1/2009 zhangxiang]
	int SEsse::getType(void) const{
		return mc_iType;
	}

	//  [1/1/2009 zhangxiang]
	Vector3 SEsse::position(void) const{
		if(m_pAttachedNode){
			return m_pAttachedNode->position();
		}else{
			return Vector3::ZERO;
		}
	}

	//  [1/1/2009 zhangxiang]
	SScene *SEsse::getScene(void) const{
		if(m_pAttachedNode)
			return m_pAttachedNode->getScene();

		return 0;
	}

	//  [1/1/2009 zhangxiang]
	SSceneNode *SEsse::attachedNode(void) const{
		return m_pAttachedNode;
	}

	//  [1/1/2009 zhangxiang]
	bool SEsse::isAttached(void) const{
		if(m_pAttachedNode)
			return true;
		else return false;
	}

	//  [1/1/2009 zhangxiang]
	void SEsse::attachToNode(SSceneNode *aNode){
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
	void SEsse::detachFromNode(void){
		if(m_pAttachedNode){
			m_pAttachedNode->detachEsse(this);
		}

		m_pAttachedNode = 0;
	}

	//  [1/1/2009 zhangxiang]
	void SEsse::_notifyAttached(SSceneNode *aNode){
		m_pAttachedNode = aNode;
	}

} // namespace Sagitta