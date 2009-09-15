//////////////////////////////////////////////////////
// file: SSceneNode.cpp 
// created by zhangxiang on 09-1-1
// declares of the class SSceneNode
// SSceneNode is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SSceneNode.h"
#include "SEsse.h"
#include "SScene.h"
#include "../../common/utils/SException.h"
#include "../../common/utils/SString.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// SAbstractSceneNode //////////////////////////////////////////////////////////////////////////

	//  [1/7/2009 zhangxiang]
	SAbstractSceneNode::SAbstractSceneNode(SScene *aScene/* = 0 */) :
	SNode(), m_pScene(aScene){

	}

	//  [1/7/2009 zhangxiang]
	SAbstractSceneNode::SAbstractSceneNode(const StdString &aName, SScene *aScene/* = 0 */) :
	SNode(aName), m_pScene(aScene){

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
	SScene *SAbstractSceneNode::getScene(void) const{
		return m_pScene;
	}

	//  [1/1/2009 zhangxiang] -- in SSceneNode
	//  [1/6/2009 zhangxiang] -- in SNode
	//  [1/6/2009 zhangxiang] -- in SSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::_setScene(SScene *aScene){
		m_pScene = aScene;
	}

	//  [1/6/2009 zhangxiang] -- in SSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::stepToParent(SNode *aNewParent){
		THROW_SAGI_EXCEPT(SException::ERR_NOT_IMPLEMENTED,
			"Could not step to non-scene node.",
			"SAbstractSceneNode::stepToParent(SNode*)");
	}

	//  [1/6/2009 zhangxiang] -- in SSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::addChild(SNode* aChild){
		THROW_SAGI_EXCEPT(SException::ERR_NOT_IMPLEMENTED,
			"Could not add non-scene node as child.",
			"SAbstractSceneNode::addChild(SNode*)");
	}

	//  [1/12/2009 zhangxiang]
	void SAbstractSceneNode::doSthIfNotInTheSameScene(SAbstractSceneNode *aNode){
		THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
			"Node \"" + aNode->name() + "\" and node \"" +
			name() + "\" are not in the same scene.",
			"SAbstractSceneNode::doSthIfNotInTheSameScene");
	}

	//  [1/12/2009 zhangxiang]
	void SAbstractSceneNode::askSceneToDoSth(SScene *aScene){

	}

	//  [1/6/2009 zhangxiang] -- in SSceneNode
	//  [1/7/2009 zhangxiang]
	void SAbstractSceneNode::stepToParent(SAbstractSceneNode *aNewParent){
		if(!aNewParent){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null new parent pointer.",
							"SAbstractSceneNode::stepToParent");
		}

		// Check if in the same scene.
		// Continue when i'm not in any scene or
		// the new parent is in the same scene as me,
		if(m_pScene){
			if(aNewParent->getScene() != m_pScene){
				THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
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

	//  [1/6/2009 zhangxiang] -- in SSceneNode
	void SAbstractSceneNode::addChild(SAbstractSceneNode* aChild){
		if(!aChild){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"NULL child node pointer.",
							"SAbstractSceneNode::addChild");
		}

		// Check if in the same scene.
		// Continue when the new child is not in any scene or
		// the scene it is in is the same as me.
		if(aChild->isInScene()){
			if(aChild->getScene() != m_pScene){
				/*THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
								"Node \"" + aChild->name() + "\" and node \"" +
								name() + "\" are not in the same scene.",
								"SAbstractSceneNode::addChild");*/
				doSthIfNotInTheSameScene(aChild);
			}
		}

		// Continue when the new child has no parent
		if(aChild->parent()){
			if(aChild->parent() != this){
				THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
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


	// SSceneNode //////////////////////////////////////////////////////////////////////////

	//  [1/1/2009 zhangxiang]
	SSceneNode::SSceneNode(void) :
	SAbstractSceneNode(), m_iEsseCount(0){

	}

	//  [1/1/2009 zhangxiang]
	SSceneNode::SSceneNode(const StdString &aName) :
	SAbstractSceneNode(aName), m_iEsseCount(0){

	}

	//  [1/1/2009 zhangxiang]
	SSceneNode::~SSceneNode(void){
		// notify in destructor
		// to tell esses do not detach from me
		// --- no need ----
		// because for esses, they are in scene too,
		// and cannot remove them from scene if they are
		// attached on a node.
		//	_setDisposing();

		std::vector<EsseMap*> tmpv2(m_EsseMap.size());
		size_t emi = 0;
		std::vector<SEsse*> tmpv(m_iEsseCount);
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

		SScene *scene = getScene();
		if(scene && !(scene->isUnloading())){
			scene->removeSceneNode(this);
		}

	}

	//  [1/7/2009 zhangxiang]
	void SSceneNode::askSceneToDoSth(SScene *aScene){
		if(!getScene() && aScene){
			aScene->addSceneNode(this);
		}
	}

	//  [1/2/2009 zhangxiang]
	SNode *SSceneNode::clone(void) const{
		SSceneNode *ret = new SSceneNode(this->name() + "_copy");
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
				SSceneObject *e = it->second->clone();
				if(e){
					ret->attachEsse(static_cast<SEsse*>(e));
				}
			}
		}

		// clone children
		SNode::ConstChildIterator it = this->getConstChildIterator();
		for(; it.hasMoreElements(); ++it){
			SNode *cnode = it.value()->clone();
			if(cnode){
				ret->addChild(static_cast<SSceneNode*>(cnode));
			}
		}

		return static_cast<SNode*>(ret);
	}

	//  [1/1/2009 zhangxiang]
	void SSceneNode::attachEsse(SEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "NULL esse pointer.", "SNode::setEsseName");
		}

		if(aEsse->isAttached()){
			if(aEsse->attachedNode() != this){
				THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Esse \"" + aEsse->name() +"\" has been attached to another node.", "SNode::attachEsse");
			}else{
				return ;
			}
		}

		/* No need to check if in the same scene, because if an esse is not attached on a node, it isn't in any scene.
		if(aEsse->getSceneManager() && aEsse->getSceneManager() != this->getSceneManager()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Esse \"" + aEsse->name() +"\" and node \"" + name() + "\" are not in the same scene.", "SNode::attachEsse");
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
	SEsse *SSceneNode::detachEsse(int aType, size_t aIndex){
		TypeEsseMap::iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "No such type esses attached on node \"" + name() + "\".", "SSceneNode::detachEsse");
		}

		EsseMap *esses = tit->second;
		if(aIndex >= esses->size()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Index out of range.", "SSceneNode::detachEsse");
		}

		EsseMap::iterator it = esses->begin();
		while(aIndex--) ++it;

		SEsse *res = it->second;
		esses->erase(it);
		res->_notifyAttached(0);

		--m_iEsseCount;

		if(this->getScene()){
			this->getScene()->_removeEsse(res);
		}

		return res;
	}

	//  [1/1/2009 zhangxiang]
	SEsse *SSceneNode::detachEsse(SEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, 
							"Null esse pointer.", "SSceneNode::detachEsse");
		}

		int type = aEsse->getType();
		TypeEsseMap::iterator tit = m_EsseMap.find(type);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "No such type esses attached on Node" + SString::to_string(this->getNodeID()) + ".", "SSceneNode::detachEsse");
		}

		EsseMap *esses = tit->second;
		EsseMap::iterator it = esses->find(aEsse->getSObjID());
		if(it == esses->end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"There's no Esse" + SString::to_string(aEsse->getSObjID()) +
				" attached on the Node" + SString::to_string(this->getNodeID()) + ".", "SSceneNode::detachEsse");
		}

		SEsse *res = it->second;
		esses->erase(it);
		res->_notifyAttached(0);

		--m_iEsseCount;

		if(this->getScene()){
			this->getScene()->_removeEsse(res);
		}

		return res;
	}

	//  [1/2/2009 zhangxiang]
	void SSceneNode::detachEsseTo(SEsse *aEsse, SSceneNode *aAnoNode){
		if(!aEsse){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, 
							"Null esse pointer.", "SSceneNode::detachEsseTo");
		}

		if(!aAnoNode){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null another node pointer.", "SSceneNode::detachEsseTo");
		}

		if(aAnoNode == this){
			// same parent node, do nothing
			return ;
		}

		if(this->isInScene() && this->getScene() != aAnoNode->getScene()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"The two node are not in the same scene.", "SSceneNode::detachEsseTo");
		}

		int type = aEsse->getType();
		TypeEsseMap::iterator tit = m_EsseMap.find(type);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"No such type esses attached on Node" +
				SString::to_string(this->getNodeID()) +
				".", "SSceneNode::detachEsseTo");
		}

		EsseMap *esses = tit->second;
		EsseMap::iterator it = esses->find(aEsse->getSObjID());
		if(it == esses->end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"There's no Esse" + SString::to_string(aEsse->getSObjID()) +
				" attached on the Node" + SString::to_string(this->getNodeID()) + ".", "SSceneNode::detachEsseTo");
		}

		SEsse *res = it->second;
		esses->erase(it);

		--m_iEsseCount;

		aAnoNode->attachEsse(aEsse);
	}

	//  [1/1/2009 zhangxiang]
	SEsse *SSceneNode::getEsse(const StdString &aName) const{
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
	SEsse *SSceneNode::getEsse(int aType, const StdString &aName) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "No such type esses attached on node \"" + name() + "\".", "SSceneNode::getEsse");
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
	SEsse *SSceneNode::getEsse(size_t aIndex) const{
		if(aIndex >= getEsseNum()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Esse index out of range.", "SSceneNode::getEsse");
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
	SEsse *SSceneNode::getEsse(int aType, size_t aIndex) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "No such type esses attached on node \"" + name() + "\".", "SSceneNode::getEsse");
		}

		EsseMap *esses = tit->second;
		if(aIndex >= esses->size()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Index out of range.", "SSceneNode::getEsse");
		}

		EsseMap::iterator it = esses->begin();
		while(aIndex--) ++it;

		return it->second;
	}

	//  [1/1/2009 zhangxiang]
	void SSceneNode::getEsses(const StdString &aName, std::vector<SEsse*> &outEsses) const{
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
	size_t SSceneNode::getTypedEsseNum(int aType) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"No such type esses attached on Node" +
				SString::to_string(getNodeID()) + 
				".", "SSceneNode::getSTypedEsseNum");
		}

		return tit->second->size();
	}

	//  [1/2/2009 zhangxiang]
	size_t SSceneNode::getEsseNum(void) const{
		return m_iEsseCount;
	}

	// SSceneNode //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta