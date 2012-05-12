//////////////////////////////////////////////////////
// file: sgSceneNode.cpp 
// created by zhangxiang on 09-1-1
// declares of the class sgSceneNode
// sgSceneNode is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgSceneNode.h"
#include "sgEsse.h"
#include "sgScene.h"
#include "../../common/utils/sgException.h"
#include "../../common/utils/sgStringUtil.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// SAbstractSceneNode //////////////////////////////////////////////////////////////////////////

	//  [1/7/2009 zhangxiang]
	SAbstractSceneNode::SAbstractSceneNode(sgScene *aScene/* = 0 */) :
	sgNode(), m_pScene(aScene){

	}

	//  [1/7/2009 zhangxiang]
	SAbstractSceneNode::SAbstractSceneNode(const StdString &aName, sgScene *aScene/* = 0 */) :
	sgNode(aName), m_pScene(aScene){

	}

	//  [1/7/2009 zhangxiang]
	SAbstractSceneNode::~SAbstractSceneNode(void){

	}

	//  [1/9/2009 zhangxiang]
	bool SAbstractSceneNode::isInScene(void) const{
		if(m_pScene)
			return true;
		else return false;
	}

	//  [1/7/2009 zhangxiang]
	sgScene *SAbstractSceneNode::getScene(void) const{
		return m_pScene;
	}

	//  [1/1/2009 zhangxiang] -- in sgSceneNode
	//  [1/6/2009 zhangxiang] -- in sgNode
	//  [1/6/2009 zhangxiang] -- in sgSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::_setScene(sgScene *aScene){
		m_pScene = aScene;
	}

	//  [1/6/2009 zhangxiang] -- in sgSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::stepToParent(sgNode *aNewParent){
		THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
			"Could not step to non-scene node.",
			"SAbstractSceneNode::stepToParent(sgNode*)");
	}

	//  [1/6/2009 zhangxiang] -- in sgSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::addChild(sgNode* aChild){
		THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
			"Could not add non-scene node as child.",
			"SAbstractSceneNode::addChild(sgNode*)");
	}

	//  [1/12/2009 zhangxiang]
	void SAbstractSceneNode::doSthIfNotInTheSameScene(SAbstractSceneNode *aNode){
		THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
			"Node \"" + aNode->name() + "\" and node \"" +
			name() + "\" are not in the same scene.",
			"SAbstractSceneNode::doSthIfNotInTheSameScene");
	}

	//  [1/12/2009 zhangxiang]
	void SAbstractSceneNode::askSceneToDoSth(sgScene *aScene){

	}

	//  [1/6/2009 zhangxiang] -- in sgSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::stepToParent(SAbstractSceneNode *aNewParent){
		if(!aNewParent){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null new parent pointer.",
							"SAbstractSceneNode::stepToParent");
		}

		// Check if in the same scene.
		// Continue when i'm not in any scene or
		// the new parent is in the same scene as me,
		if(m_pScene){
			if(aNewParent->getScene() != m_pScene){
				THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
					"Node \"" + aNewParent->name() +
					"\" and node \"" + this->name() + 
					"\" are not in the same scene.",
					"SAbstractSceneNode::stepToParent");
			}
		}

		if(parent()){
			// if i have a parent, ask him to remove me first.
			parent()->removeChild(this);
		}

		// at last, ask the new parent to add me.
		aNewParent->addChild(this);
	}

	//  [1/6/2009 zhangxiang] -- in sgSceneNode
	void SAbstractSceneNode::addChild(SAbstractSceneNode* aChild){
		if(!aChild){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"NULL child node pointer.",
							"SAbstractSceneNode::addChild");
		}

		// Check if in the same scene.
		// Continue when the new child is not in any scene or
		// the scene it is in is the same as me.
		if(aChild->isInScene()){
			if(aChild->getScene() != m_pScene){
				/*THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
								"Node \"" + aChild->name() + "\" and node \"" +
								name() + "\" are not in the same scene.",
								"SAbstractSceneNode::addChild");*/
				doSthIfNotInTheSameScene(aChild);
			}
		}

		// Continue when the new child has no parent
		if(aChild->parent()){
			if(aChild->parent() != this){
				THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
								"Node \"" + aChild->name() +
								"\" already was a child of \"" +
								aChild->parent()->name() + "\".",
								"SAbstractSceneNode::addChild");
			}else{
				return ;
			}
		}

		m_Children.insert(std::make_pair(aChild->getNodeID(), aChild));
		aChild->setParent(this);

		// let the child ask the scene i'm in to do something to it,
		// may add it into the scene
		aChild->askSceneToDoSth(getScene());
	}

	// SAbstractSceneNode //////////////////////////////////////////////////////////////////////////


	// sgSceneNode //////////////////////////////////////////////////////////////////////////

	//  [1/1/2009 zhangxiang]
	sgSceneNode::sgSceneNode(void) :
	SAbstractSceneNode(), m_iEsseCount(0){

	}

	//  [1/1/2009 zhangxiang]
	sgSceneNode::sgSceneNode(const StdString &aName) :
	SAbstractSceneNode(aName), m_iEsseCount(0){

	}

	//  [1/1/2009 zhangxiang]
	sgSceneNode::~sgSceneNode(void){
		// notify in destructor
		// to tell esses do not detach from me
		// --- no need ----
		// because for esses, they are in scene too,
		// and cannot remove them from scene if they are
		// attached on a node.
		//	_setDisposing();

		std::vector<EsseMap*> tmpv2(m_EsseMap.size());
		size_t emi = 0;
		std::vector<sgEsse*> tmpv(m_iEsseCount);
		size_t ei = 0;

		EsseMap *esses;
		TypeEsseMap::iterator tit = m_EsseMap.begin();
		for(; tit!=m_EsseMap.end(); ++tit, ++emi){
			esses = tit->second;
			EsseMap::iterator it = esses->begin();
			for(; it!=esses->end(); ++it, ++ei){
				// store
				tmpv[ei] = it->second;
			}
			tmpv2[emi] = esses;
		}
		
		for(size_t i=0; i<ei; ++i){
			// esses will detach themselves from this node
			// and of cource will be removed from scene
			delete tmpv[i];
		}

		for(size_t i=0; i<emi; ++i){
			// delete typed esse maps
			delete tmpv2[i];
		}

		sgScene *scene = getScene();
		if(scene && !(scene->isUnloading())){
			scene->removeSceneNode(this);
		}

	}

	//  [1/7/2009 zhangxiang]
	void sgSceneNode::askSceneToDoSth(sgScene *aScene){
		if(!getScene() && aScene){
			aScene->addSceneNode(this);
		}
	}

	//  [1/2/2009 zhangxiang]
	sgNode *sgSceneNode::clone(void) const{
		sgSceneNode *ret = new sgSceneNode(this->name() + "_copy");
		ret->setActive(this->isActive());
		ret->setRelativePosition(this->relativePosition());
		ret->setRelativeOrientation(this->relativeOrentation());
		ret->setRelativeScale(this->relativeScale());
		ret->setInheritOrientation(this->inheritOrientation());
		ret->setInheritScale(this->inheritScale());

		// clone esses
		EsseMap *esses;
		TypeEsseMap::const_iterator tit = m_EsseMap.begin();
		for(; tit!=m_EsseMap.end(); ++tit){
			esses = tit->second;
			EsseMap::iterator it = esses->begin();
			for(; it!=esses->end(); ++it){
				sgSceneObject *e = it->second->clone();
				if(e){
					ret->attachEsse(static_cast<sgEsse*>(e));
				}
			}
		}

		// clone children
		sgNode::ConstChildIterator it = this->getConstChildIterator();
		for(; it.hasMoreElements(); ++it){
			sgNode *cnode = it.value()->clone();
			if(cnode){
				ret->addChild(static_cast<sgSceneNode*>(cnode));
			}
		}

		return static_cast<sgNode*>(ret);
	}

	//  [1/1/2009 zhangxiang]
	void sgSceneNode::attachEsse(sgEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL esse pointer.", "sgNode::setEsseName");
		}

		if(aEsse->isAttached()){
			if(aEsse->attachedNode() != this){
				THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Esse \"" + aEsse->name() +"\" has been attached to another node.", "sgNode::attachEsse");
			}else{
				return ;
			}
		}

		/* No need to check if in the same scene, because if an esse is not attached on a node, it isn't in any scene.
		if(aEsse->getSceneManager() && aEsse->getSceneManager() != this->getSceneManager()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Esse \"" + aEsse->name() +"\" and node \"" + name() + "\" are not in the same scene.", "sgNode::attachEsse");
		}*/

		EsseMap *esses;
		TypeEsseMap::iterator tit = m_EsseMap.find(aEsse->getType());
		if(tit == m_EsseMap.end()){
			esses = new EsseMap;
			m_EsseMap.insert(std::make_pair(aEsse->getType(), esses));
		}else{
			esses = tit->second;
		}

		if(!(esses->insert(std::make_pair(aEsse->getSObjID(), aEsse)).second)){
			// existed, do nothing
			return ;
		}
		aEsse->_notifyAttached(this);
		++m_iEsseCount;

		if(this->getScene()){
			this->getScene()->_addEsse(aEsse);
		}
	}

	//  [1/1/2009 zhangxiang]
	sgEsse *sgSceneNode::detachEsse(int aType, size_t aIndex){
		TypeEsseMap::iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "No such type esses attached on node \"" + name() + "\".", "sgSceneNode::detachEsse");
		}

		EsseMap *esses = tit->second;
		if(aIndex >= esses->size()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Index out of range.", "sgSceneNode::detachEsse");
		}

		EsseMap::iterator it = esses->begin();
		while(aIndex--) ++it;

		sgEsse *res = it->second;
		esses->erase(it);
		res->_notifyAttached(0);

		--m_iEsseCount;

		if(this->getScene()){
			this->getScene()->_removeEsse(res);
		}

		return res;
	}

	//  [1/1/2009 zhangxiang]
	sgEsse *sgSceneNode::detachEsse(sgEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, 
							"Null esse pointer.", "sgSceneNode::detachEsse");
		}

		int type = aEsse->getType();
		TypeEsseMap::iterator tit = m_EsseMap.find(type);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "No such type esses attached on Node" + sgStringUtil::to_string(this->getNodeID()) + ".", "sgSceneNode::detachEsse");
		}

		EsseMap *esses = tit->second;
		EsseMap::iterator it = esses->find(aEsse->getSObjID());
		if(it == esses->end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"There's no Esse" + sgStringUtil::to_string(aEsse->getSObjID()) +
				" attached on the Node" + sgStringUtil::to_string(this->getNodeID()) + ".", "sgSceneNode::detachEsse");
		}

		sgEsse *res = it->second;
		esses->erase(it);
		res->_notifyAttached(0);

		--m_iEsseCount;

		if(this->getScene()){
			this->getScene()->_removeEsse(res);
		}

		return res;
	}

	//  [1/2/2009 zhangxiang]
	void sgSceneNode::detachEsseTo(sgEsse *aEsse, sgSceneNode *aAnoNode){
		if(!aEsse){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, 
							"Null esse pointer.", "sgSceneNode::detachEsseTo");
		}

		if(!aAnoNode){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null another node pointer.", "sgSceneNode::detachEsseTo");
		}

		if(aAnoNode == this){
			// same parent node, do nothing
			return ;
		}

		if(this->isInScene() && this->getScene() != aAnoNode->getScene()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"The two node are not in the same scene.", "sgSceneNode::detachEsseTo");
		}

		int type = aEsse->getType();
		TypeEsseMap::iterator tit = m_EsseMap.find(type);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"No such type esses attached on Node" +
				sgStringUtil::to_string(this->getNodeID()) +
				".", "sgSceneNode::detachEsseTo");
		}

		EsseMap *esses = tit->second;
		EsseMap::iterator it = esses->find(aEsse->getSObjID());
		if(it == esses->end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"There's no Esse" + sgStringUtil::to_string(aEsse->getSObjID()) +
				" attached on the Node" + sgStringUtil::to_string(this->getNodeID()) + ".", "sgSceneNode::detachEsseTo");
		}

		sgEsse *res = it->second;
		esses->erase(it);

		--m_iEsseCount;

		aAnoNode->attachEsse(aEsse);
	}

	//  [1/1/2009 zhangxiang]
	sgEsse *sgSceneNode::getEsse(const StdString &aName) const{
		EsseMap *esses;
		TypeEsseMap::const_iterator tit = m_EsseMap.begin();
		for(; tit!=m_EsseMap.end(); ++tit){
			esses = tit->second;
			EsseMap::const_iterator it = esses->begin();
			for(; it!=esses->end(); ++it){
				if(it->second->name() == aName){
					return it->second;
				}
			}
		}

		return 0;
	}

	//  [1/1/2009 zhangxiang]
	sgEsse *sgSceneNode::getEsse(int aType, const StdString &aName) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "No such type esses attached on node \"" + name() + "\".", "sgSceneNode::getEsse");
		}

		EsseMap *esses = tit->second;
		EsseMap::const_iterator it = esses->begin();
		for(; it!=esses->end(); ++it){
			if(it->second->name() == aName){
				return it->second;
			}
		}

		return 0;
	}

	//  [1/2/2009 zhangxiang]
	sgEsse *sgSceneNode::getEsse(size_t aIndex) const{
		if(aIndex >= getEsseNum()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Esse index out of range.", "sgSceneNode::getEsse");
		}

		EsseMap *esses;
		TypeEsseMap::const_iterator tit = m_EsseMap.begin();
		for(; tit!=m_EsseMap.end(); ++tit){
			esses = tit->second;
			EsseMap::iterator it = esses->begin();
			for(; it!=esses->end(); ++it, aIndex--){
				if(!aIndex)
					return it->second;
			}
		}

		return 0;
	}

	//  [1/2/2009 zhangxiang]
	sgEsse *sgSceneNode::getEsse(int aType, size_t aIndex) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "No such type esses attached on node \"" + name() + "\".", "sgSceneNode::getEsse");
		}

		EsseMap *esses = tit->second;
		if(aIndex >= esses->size()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Index out of range.", "sgSceneNode::getEsse");
		}

		EsseMap::iterator it = esses->begin();
		while(aIndex--) ++it;

		return it->second;
	}

	//  [1/1/2009 zhangxiang]
	void sgSceneNode::getEsses(const StdString &aName, std::vector<sgEsse*> &outEsses) const{
		outEsses.clear();

		EsseMap *esses;
		TypeEsseMap::const_iterator tit = m_EsseMap.begin();
		for(; tit!=m_EsseMap.end(); ++tit){
			esses = tit->second;
			EsseMap::const_iterator it = esses->begin();
			for(; it!=esses->end(); ++it){
				if(it->second->name() == aName){
					outEsses.push_back(it->second);
				}
			}
		}
	}

	//  [1/1/2009 zhangxiang]
	size_t sgSceneNode::getTypedEsseNum(int aType) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"No such type esses attached on Node" +
				sgStringUtil::to_string(getNodeID()) + 
				".", "sgSceneNode::getSTypedEsseNum");
		}

		return tit->second->size();
	}

	//  [1/2/2009 zhangxiang]
	size_t sgSceneNode::getEsseNum(void) const{
		return m_iEsseCount;
	}

	// sgSceneNode //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta