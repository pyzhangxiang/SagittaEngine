//////////////////////////////////////////////////////
// file: SScene.cpp @ 2008-7-30 by Zhang Xiang
// defines of the class SScene
// SScene is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SScene.h"
#include "SSubScene.h"
#include "SSceneNode.h"
#include "SEsse.h"
#include "SEntity.h"
#include "SSceneManager.h"
#include "../animation/SAnimation.h"
#include "../animation/SAnimaJoint.h"
#include "../buffer/SVertexData.h"
#include "../buffer/SVertexBufferElement.h"
#include "../buffer/SVertexIndexBuffer.h"
#include "../io/SFile.h"
#include "../../common/utils/SException.h"
#include "../../common/utils/SString.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// BindingMap //////////////////////////////////////////////////////////////////////////

	//  [1/11/2009 zhangxiang]
	void BindingMap::addBindingInfo(uLong aSceneNodeID, uLong aAnimaJointID){
		m_BindingMap.insert(std::make_pair(aSceneNodeID, aAnimaJointID));
	}

	//  [1/11/2009 zhangxiang]
	void BindingMap::removeBindingInfo(uLong aSceneNodeID){
		IDMap::iterator it = m_BindingMap.find(aSceneNodeID);
		if(it == m_BindingMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_ITEM_NOT_FOUND,
				"No binding info for SceneNode with id " + SString::to_string(aSceneNodeID),
				"BindingMap::removeBindingInfo");
		}

		m_BindingMap.erase(it);
	}

	//  [1/11/2009 zhangxiang]
	int BindingMap::findAnimaJoint(uLong aSceneNodeID) const{
		IDMap::const_iterator it = m_BindingMap.find(aSceneNodeID);
		if(it == m_BindingMap.end()){
			return -1;
		}
		return static_cast<int>(it->second);
	}

	// BindingMap //////////////////////////////////////////////////////////////////////////

	// SSceneJoint //////////////////////////////////////////////////////////////////////////

	//  [1/12/2009 zhangxiang]
	void SScene::SSceneJoint::doSthIfNotInTheSameScene(SAbstractSceneNode *aNode){
		if(!(this->getScene()) || !(this->getScene()->getSceneManager())){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Node \"" + name() + "\" is not in any SceneManager, "
				"so cannot add any Node to be child that in other scenes.",
				"SSceneJoint::doSthIfNotInTheSameScene");
		}
		if(aNode->getScene()->getSceneManager()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
				"Node \"" + aNode->name() + "\" has been in a SceneManager",
				"SSceneJoint::doSthIfNotInTheSameScene");
		}
		// ok, go on...
	}

	// SSceneJoint //////////////////////////////////////////////////////////////////////////

	// SScene //////////////////////////////////////////////////////////////////////////

	uLong SScene::ms_iSceneCount = 1;

	//  [8/18/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	SScene::SScene(SSceneDelegate *aDelegate/* = new SSceneDelegate */) :
	SResource(aDelegate), m_pRoot(0), m_pAnimation(0), m_pSceneManager(0),
	m_bVisible(true), m_iCurrentFrame(0), m_bLoopFrame(false){
		StdString name = "Scene" + SString::to_string(SScene::ms_iSceneCount++);
		setName(name);

//		m_pRoot = new SSceneNode("SceneRoot");
//		addSceneNode(m_pRoot);
		m_pRoot = new SSceneJoint("SceneRoot", this);
		m_pRoot->notifyRoot();

		// esse map
		EsseMap *unknown = new EsseMap;
		m_EsseMap.insert(std::make_pair(SEsse::SET_UNKNOWN, unknown));
	}

	//  [8/18/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	SScene::SScene(const StdString &aName, SSceneDelegate *aDelegate/* = new SSceneDelegate */) :
	SResource(aName, aDelegate), m_pRoot(0), m_pAnimation(0), m_pSceneManager(0),
	m_bVisible(true), m_iCurrentFrame(0), m_bLoopFrame(false){
		SScene::ms_iSceneCount++;

	//	m_pRoot = new SSceneNode("SceneRoot");
	//	addSceneNode(m_pRoot);
		m_pRoot = new SSceneJoint("SceneRoot", this);
		m_pRoot->notifyRoot();

		// esse map
		EsseMap *unknown = new EsseMap;
		m_EsseMap.insert(std::make_pair(SEsse::SET_UNKNOWN, unknown));
	}

	//  [8/18/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	SScene::~SScene(void){
		// notify scene nodes don't
		// remove from me because i'm in
		// destructor
		_setDisposing();

		unLoad();

		// at last, delete root
		delete m_pRoot;
	}

	void SScene::unLoadImpl(void){
		m_pRoot->removeAllChildren();
		// i have the ownership
		SceneNodeMap::iterator sit = m_SceneNodeMap.begin();
		SceneNodeMap::iterator send = m_SceneNodeMap.end();
		while(sit != send){
			delete sit->second;
			++sit;
		}
		m_SceneNodeMap.clear();

		// Yes, now there should be no esse in this scene
		// remove all esse map
		TypeEsseMap::iterator teit = m_EsseMap.begin();
		TypeEsseMap::iterator teeit = m_EsseMap.end();
		for(; teit!=teeit; ++teit){
			delete teit->second;
		}
		m_EsseMap.clear();
	}

	//  [1/7/2009 zhangxiang]
	void SScene::_setSceneManager(SSceneManager *aSM){
		m_pSceneManager = aSM;
	}

	//  [1/6/2009 zhangxiang]
	SScene::SSceneJoint *SScene::root(void) const{
		return m_pRoot;
	}

	//  [1/6/2009 zhangxiang]
	void SScene::addChildToRoot(SSceneNode *aChild){
		m_pRoot->addChild(aChild);
	}

	//  [1/8/2009 zhangxiang]
	bool SScene::isVisible(void) const{
		return m_bVisible;
	}

	//  [1/8/2009 zhangxiang]
	void SScene::setVisible(bool aVisible){
		m_bVisible = aVisible;
	}

	//  [1/11/2009 zhangxiang]
	SAnimation *SScene::getAnimation(void) const{
		return m_pAnimation;
	}

	//  [1/11/2009 zhangxiang]
	SAnimation *SScene::setAnimation(SAnimation *aAnimation){
		SAnimation *ret = m_pAnimation;
		m_pAnimation = aAnimation;
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	const BindingMap &SScene::bindingMap(void) const{
		return m_BindingMap;
	}

	//  [1/11/2009 zhangxiang]
	BindingMap &SScene::bindingMap(void){
		return m_BindingMap;
	}

	//  [8/18/2008 zhangxiang]
	SSceneManager *SScene::getSceneManager(void) const{
		return m_pSceneManager;
	}

	//  [1/6/2009 zhangxiang]
	SSceneNode *SScene::createSceneNode(void){
		SSceneNode *ret = new SSceneNode();
		m_pRoot->addChild(ret);

		return ret;
	}

	//  [1/6/2009 zhangxiang]
	SSceneNode *SScene::createSceneNode(const StdString &asName){
		SSceneNode *ret = new SSceneNode(asName);
		m_pRoot->addChild(ret);

		return ret;
	}

	//  [1/6/2009 zhangxiang]
	void SScene::addSceneNode(SSceneNode *aNode){
		if(!aNode){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "NULL node pointer.", "SScene::addSceneNode");
		}

		if(aNode->getScene()){
			if(aNode->getScene() != this){
				THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "This node is not in the scene.", "SScene::addSceneNode");
			}else{
				return ;
			}
		}

		m_SceneNodeMap.insert(std::make_pair(aNode->getNodeID(), aNode));
		aNode->_setScene(this);

		// add esses
		size_t esseNum = aNode->getEsseNum();
		for(size_t i=0; i<esseNum; ++i){
			_addEsse(aNode->getEsse(i));
		}

		// add child nodes
		size_t childnum = aNode->childNum();
		for(size_t i=0; i<childnum; ++i){
			addSceneNode(static_cast<SSceneNode*>(aNode->getChild(i)));
		}
	}

	//  [1/2/2009 zhangxiang] -- in SSceneManager
	//  [1/6/2009 zhangxiang]
	void SScene::removeSceneNode(SSceneNode *aNode){
		if(!aNode){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "NULL node pointer.", "SScene::removeSceneNode");
		}

		if(aNode->parent()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "This node has parent, cannot be removed from a scene.", "SScene::removeSceneNode");
		}

		SceneNodeMap::iterator it = m_SceneNodeMap.find(aNode->getNodeID());
		if(it == m_SceneNodeMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "The specified node is not in this scene.", "SScene::removeSceneNode");
		}

		m_SceneNodeMap.erase(it);
		aNode->_setScene(0);

		// remove esses
		size_t esseNum = aNode->getEsseNum();
		for(uInt i=0; i<esseNum; ++i){
			_removeEsse(aNode->getEsse(i));
		}

		// remove child nodes
		size_t childnum = aNode->childNum();
		for(uInt i=0; i<childnum; ++i){
			removeSceneNode(static_cast<SSceneNode*>(aNode->getChild(i)));
		}
	}

	//  [1/2/2009 zhangxiang] -- in SSceneManager
	//  [1/6/2009 zhangxiang]
	void SScene::_addEsse(SEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "NULL esse pointer.", "SSceneManager::addEsse");
		}

		// esse should have been attached to a node
		if(!aEsse->isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
							"Esse \"" + aEsse->name() +
							"\" is not attached on a node.",
							"SScene::_addEsse");
		}

		if(aEsse->getScene()){
			if(aEsse->getScene() != this){
				THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Esse \"" + aEsse->name() +"\" is in another scene.", "SSceneManager::addEsse");
			}
		}

		EsseMap *esses;
		TypeEsseMap::iterator tit = m_EsseMap.find(aEsse->getType());
		if(tit == m_EsseMap.end()){
			esses = new EsseMap;
			m_EsseMap.insert(make_pair(aEsse->getType(), esses));
		}else{
			esses = tit->second;
		}

		esses->insert(std::make_pair(aEsse->getSObjID(), aEsse));
	}

	//  [1/2/2009 zhangxiang] -- in SSceneManager
	//  [1/6/2009 zhangxiang]
	void SScene::_removeEsse(SEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "NULL esse pointer.", "SSceneManager::removeEsse");
		}

		// esse should have been detached from node
		if(aEsse->isAttached()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
							"Esse \"" + aEsse->name() +
							"\" has not been detached from node.",
							"SScene::_removeEsse");
		}

		TypeEsseMap::iterator tit = m_EsseMap.find(aEsse->getType());
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "No such type esse in this scene.", "SSceneManager::removeEsse");
		}

		EsseMap *esses = tit->second;
		EsseMap::iterator it = esses->find(aEsse->getSObjID());
		if(it == esses->end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "The specified esse is not in this scene.", "SSceneManager::removeEsse");
		}

		esses->erase(it);
	}

	//  [1/2/2009 zhangxiang] -- in SSceneManager
	//  [1/6/2009 zhangxiang]
	SEsse *SScene::getTypedEsse(int aType, size_t aIndex) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "No such type esses in this scene.", "SSceneManager::getSpecifiedEsse");
		}

		EsseMap *esses = tit->second;
		if(aIndex >= esses->size()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Esse index out of bounds.", "SSceneManager::getTypedEsse");
		}

		EsseMap::const_iterator it = esses->begin();
		while(aIndex--) ++it;
		return it->second;

	}

	//  [1/2/2009 zhangxiang] -- in SSceneManager
	//  [1/6/2009 zhangxiang]
	size_t SScene::getTypedEsseNum(int aType) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "No such type esses in this scene.", "SSceneManager::getSpecifiedEsseNum");
		}

		return tit->second->size();
	}

	//  [1/2/2009 zhangxiang] -- in SSceneManager
	//  [1/6/2009 zhangxiang]
	size_t SScene::getEsseNum(void) const{
		size_t num = 0;

		EsseMap *esses;
		TypeEsseMap::const_iterator tit = m_EsseMap.begin();
		for(; tit!=m_EsseMap.end(); ++tit){
			esses = tit->second;
			num += tit->second->size();
		}

		return num;
	}

	//  [1/7/2009 zhangxiang]
	void SScene::merge(SSubScene *aScene){
		if(!aScene){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null SScene pointer.", "SScene::merge");
		}

		if(aScene->getSceneManager() &&
			aScene->getSceneManager() != this->getSceneManager()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Scene \"" + aScene->name() +
							"\" is in a different scene manger to mine.",
							"SScene::merge");
		}

		SSceneNode *node;
		SSceneJoint *sourceroot = aScene->root();
		// get the active nodes
		SNode::ConstChildIterator ncit = sourceroot->getConstChildIterator();
		for(; ncit.hasMoreElements(); ++ncit){
			node = static_cast<SSceneNode*>(ncit.value());
			// remove the node from the source scene
			sourceroot->removeChild(node);
			aScene->removeSceneNode(node);
			// add the node to me
			m_pRoot->addChild(node);
		}
		// get the inactive nodes
		SScene::ConstSceneNodeIterator snit = aScene->getConstSceneNodeIterator();
		for(; snit.hasMoreElements(); ++ncit){
			node = static_cast<SSceneNode*>(snit.value());
			// remove the node from the source scene
			aScene->removeSceneNode(node);
			// add the node to me
			addSceneNode(node);
		}

		SSceneManager *sm = aScene->getSceneManager();
		if(sm){
			sm->removeSubScene(aScene);
		}
		delete aScene;
	}

	//  [1/8/2009 zhangxiang]
	SScene::ConstTypedEsseIterator SScene::getConstEsseIterator(int aType) const{
		TypeEsseMap::const_iterator it = m_EsseMap.find(aType);
		if(it == m_EsseMap.end()){
			return ConstTypedEsseIterator(*(m_EsseMap.find(SEsse::SET_UNKNOWN)->second));
		}
		return ConstTypedEsseIterator(*(it->second));
	}

	//  [1/9/2009 zhangxiang]
	SScene::ConstSceneNodeIterator SScene::getConstSceneNodeIterator(void) const{
		return ConstSceneNodeIterator(m_SceneNodeMap);
	}

	//  [1/12/2009 zhangxiang]
	bool SScene::isLoopFrame(void) const{
		return m_bLoopFrame;
	}

	//  [1/12/2009 zhangxiang]
	void SScene::setLoopFrame(bool aLoop){
		m_bLoopFrame = aLoop;
	}

	//  [1/8/2009 zhangxiang]
	size_t SScene::frameNum(void) const{
		if(m_pAnimation){
			return m_pAnimation->maxFrameNum();
		}
		return 0;
	}

	//  [8/28/2008 zhangxiang] -- in SSceneManager
	//  [1/8/2009 zhangxiang]
	int SScene::currentFrame(void) const{
		return m_iCurrentFrame;
	}

	//  [8/28/2008 zhangxiang] -- in sSceneManager
	//  [1/8/2009 zhangxiang]
	void SScene::setCurrentFrame(size_t aFrame){
		if(m_pAnimation){
			size_t framenum = m_pAnimation->maxFrameNum();

			if(aFrame >= framenum){
				m_iCurrentFrame = static_cast<int>(framenum) - 1;
			}else if(aFrame < 0){
				m_iCurrentFrame = 0;
			}else{
				m_iCurrentFrame = static_cast<int>(aFrame);
			}
		}
	}

	//  [1/8/2009 zhangxiang]
	void SScene::stepFrame(int aSteps){
		if(m_pAnimation){
			m_iCurrentFrame += aSteps;

			int framenum = static_cast<int>(m_pAnimation->maxFrameNum());
			// greater than frame num
			int gdf = m_iCurrentFrame - framenum;
			
			if(gdf >= 0){
				if(m_bLoopFrame){
					m_iCurrentFrame = gdf;
				}else{
					m_iCurrentFrame = framenum - 1;
				}
			}else if(m_iCurrentFrame < 0){
				if(m_bLoopFrame){
					m_iCurrentFrame += framenum;
				}else{
					m_iCurrentFrame = 0;
				}
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	void SScene::_updateFromAnimation(void){
		if(m_pAnimation){
			SSceneNode *node;
			SAnimaJoint *joint;
			int jframenum = 0;
			size_t jcurrfr = 0;
			int deltafr = 0;

			SceneNodeMap::iterator it = m_SceneNodeMap.begin();
			SceneNodeMap::iterator eit = m_SceneNodeMap.end();
			for(; it!=eit; ++it){
				node = it->second;
				// find binding info
				int animaID = m_BindingMap.findAnimaJoint(node->getNodeID());
				if(animaID == -1)	// not binding info found
					continue;

				joint = m_pAnimation->findAnimaJoint(animaID);
				if(joint){
					// have a binded joint
					jframenum = static_cast<int>(joint->frameNum());
					deltafr = m_iCurrentFrame - jframenum;
					
					if(deltafr >= 0){
						if(m_bLoopFrame){
							jcurrfr = deltafr;
						}else{
							jcurrfr = jframenum - 1;
						}
					}else{
						jcurrfr = m_iCurrentFrame;
					}

					// set scene node transform
					if(joint->isPositionActive()){
					//	node->translate(joint->getPosition(jcurrfr), joint->getPositionTS());
						node->setRelativePosition(joint->getPosition(jcurrfr));
					}
					if(joint->isOrientationActive()){
					//	node->rotate(joint->getOrientation(jcurrfr), joint->getOrientationTS());
						node->setRelativeOrientation(joint->getOrientation(jcurrfr));
					}
					if(joint->isScaleActive()){
					//	node->scale(joint->getScale(jcurrfr));
						node->setRelativeScale(joint->getScale(jcurrfr));
					}
				}//#### end if AnimaJoint found
			}//#### end for SceneNode
		} //#### end if m_pAnimation
	}

	// SScene //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta