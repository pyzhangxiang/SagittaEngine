//////////////////////////////////////////////////////
// file: sgScene.cpp @ 2008-7-30 by Zhang Xiang
// defines of the class sgScene
// sgScene is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgScene.h"
#include "sgSubScene.h"
#include "sgSceneNode.h"
#include "sgEsse.h"
#include "sgEntity.h"
#include "sgSceneManager.h"
#include "../animation/sgAnimation.h"
#include "../animation/sgAnimaJoint.h"
#include "../buffer/sgVertexData.h"
#include "../buffer/sgVertexBufferElement.h"
#include "../buffer/sgVertexIndexBuffer.h"
#include "../io/sgFile.h"
#include "../../common/utils/sgException.h"
#include "../../common/utils/sgStringUtil.h"

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
			THROW_SAGI_EXCEPT(sgException::ERR_ITEM_NOT_FOUND,
				"No binding info for SceneNode with id " + sgStringUtil::to_string(aSceneNodeID),
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
	void sgScene::SSceneJoint::doSthIfNotInTheSameScene(SAbstractSceneNode *aNode){
		if(!(this->getScene()) || !(this->getScene()->getSceneManager())){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Node \"" + name() + "\" is not in any SceneManager, "
				"so cannot add any Node to be child that in other scenes.",
				"SSceneJoint::doSthIfNotInTheSameScene");
		}
		if(aNode->getScene()->getSceneManager()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
				"Node \"" + aNode->name() + "\" has been in a SceneManager",
				"SSceneJoint::doSthIfNotInTheSameScene");
		}
		// ok, go on...
	}

	// SSceneJoint //////////////////////////////////////////////////////////////////////////

	// sgScene //////////////////////////////////////////////////////////////////////////

	uLong sgScene::ms_iSceneCount = 1;

	//  [8/18/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	sgScene::sgScene(sgSceneDelegate *aDelegate/* = new sgSceneDelegate */) :
	sgResource(aDelegate), m_pRoot(0), m_pAnimation(0), m_pSceneManager(0),
	m_bVisible(true), m_iCurrentFrame(0), m_bLoopFrame(false){
		StdString name = "Scene" + sgStringUtil::to_string(sgScene::ms_iSceneCount++);
		setName(name);

//		m_pRoot = new sgSceneNode("SceneRoot");
//		addSceneNode(m_pRoot);
		m_pRoot = new SSceneJoint("SceneRoot", this);
		m_pRoot->notifyRoot();

		// esse map
		EsseMap *unknown = new EsseMap;
		m_EsseMap.insert(std::make_pair(sgEsse::SET_UNKNOWN, unknown));
	}

	//  [8/18/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	sgScene::sgScene(const StdString &aName, sgSceneDelegate *aDelegate/* = new sgSceneDelegate */) :
	sgResource(aName, aDelegate), m_pRoot(0), m_pAnimation(0), m_pSceneManager(0),
	m_bVisible(true), m_iCurrentFrame(0), m_bLoopFrame(false){
		sgScene::ms_iSceneCount++;

	//	m_pRoot = new sgSceneNode("SceneRoot");
	//	addSceneNode(m_pRoot);
		m_pRoot = new SSceneJoint("SceneRoot", this);
		m_pRoot->notifyRoot();

		// esse map
		EsseMap *unknown = new EsseMap;
		m_EsseMap.insert(std::make_pair(sgEsse::SET_UNKNOWN, unknown));
	}

	//  [8/18/2008 zhangxiang]
	//  [1/6/2009 zhangxiang]
	sgScene::~sgScene(void){
		// notify scene nodes don't
		// remove from me because i'm in
		// destructor
		_setDisposing();

		unLoad();

		// at last, delete root
		delete m_pRoot;
	}

	void sgScene::unLoadImpl(void){
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
	void sgScene::_setSceneManager(sgSceneManager *aSM){
		m_pSceneManager = aSM;
	}

	//  [1/6/2009 zhangxiang]
	sgScene::SSceneJoint *sgScene::root(void) const{
		return m_pRoot;
	}

	//  [1/6/2009 zhangxiang]
	void sgScene::addChildToRoot(sgSceneNode *aChild){
		m_pRoot->addChild(aChild);
	}

	//  [1/8/2009 zhangxiang]
	bool sgScene::isVisible(void) const{
		return m_bVisible;
	}

	//  [1/8/2009 zhangxiang]
	void sgScene::setVisible(bool aVisible){
		m_bVisible = aVisible;
	}

	//  [1/11/2009 zhangxiang]
	sgAnimation *sgScene::getAnimation(void) const{
		return m_pAnimation;
	}

	//  [1/11/2009 zhangxiang]
	sgAnimation *sgScene::setAnimation(sgAnimation *aAnimation){
		sgAnimation *ret = m_pAnimation;
		m_pAnimation = aAnimation;
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	const BindingMap &sgScene::bindingMap(void) const{
		return m_BindingMap;
	}

	//  [1/11/2009 zhangxiang]
	BindingMap &sgScene::bindingMap(void){
		return m_BindingMap;
	}

	//  [8/18/2008 zhangxiang]
	sgSceneManager *sgScene::getSceneManager(void) const{
		return m_pSceneManager;
	}

	//  [1/6/2009 zhangxiang]
	sgSceneNode *sgScene::createSceneNode(void){
		sgSceneNode *ret = new sgSceneNode();
		m_pRoot->addChild(ret);

		return ret;
	}

	//  [1/6/2009 zhangxiang]
	sgSceneNode *sgScene::createSceneNode(const StdString &asName){
		sgSceneNode *ret = new sgSceneNode(asName);
		m_pRoot->addChild(ret);

		return ret;
	}

	//  [1/6/2009 zhangxiang]
	void sgScene::addSceneNode(sgSceneNode *aNode){
		if(!aNode){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL node pointer.", "sgScene::addSceneNode");
		}

		if(aNode->getScene()){
			if(aNode->getScene() != this){
				THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "This node is not in the scene.", "sgScene::addSceneNode");
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
			addSceneNode(static_cast<sgSceneNode*>(aNode->getChild(i)));
		}
	}

	//  [1/2/2009 zhangxiang] -- in sgSceneManager
	//  [1/6/2009 zhangxiang]
	void sgScene::removeSceneNode(sgSceneNode *aNode){
		if(!aNode){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL node pointer.", "sgScene::removeSceneNode");
		}

		if(aNode->parent()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "This node has parent, cannot be removed from a scene.", "sgScene::removeSceneNode");
		}

		SceneNodeMap::iterator it = m_SceneNodeMap.find(aNode->getNodeID());
		if(it == m_SceneNodeMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "The specified node is not in this scene.", "sgScene::removeSceneNode");
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
			removeSceneNode(static_cast<sgSceneNode*>(aNode->getChild(i)));
		}
	}

	//  [1/2/2009 zhangxiang] -- in sgSceneManager
	//  [1/6/2009 zhangxiang]
	void sgScene::_addEsse(sgEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL esse pointer.", "sgSceneManager::addEsse");
		}

		// esse should have been attached to a node
		if(!aEsse->isAttached()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Esse \"" + aEsse->name() +
							"\" is not attached on a node.",
							"sgScene::_addEsse");
		}

		if(aEsse->getScene()){
			if(aEsse->getScene() != this){
				THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Esse \"" + aEsse->name() +"\" is in another scene.", "sgSceneManager::addEsse");
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

	//  [1/2/2009 zhangxiang] -- in sgSceneManager
	//  [1/6/2009 zhangxiang]
	void sgScene::_removeEsse(sgEsse *aEsse){
		if(!aEsse){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL esse pointer.", "sgSceneManager::removeEsse");
		}

		// esse should have been detached from node
		if(aEsse->isAttached()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Esse \"" + aEsse->name() +
							"\" has not been detached from node.",
							"sgScene::_removeEsse");
		}

		TypeEsseMap::iterator tit = m_EsseMap.find(aEsse->getType());
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "No such type esse in this scene.", "sgSceneManager::removeEsse");
		}

		EsseMap *esses = tit->second;
		EsseMap::iterator it = esses->find(aEsse->getSObjID());
		if(it == esses->end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "The specified esse is not in this scene.", "sgSceneManager::removeEsse");
		}

		esses->erase(it);
	}

	//  [1/2/2009 zhangxiang] -- in sgSceneManager
	//  [1/6/2009 zhangxiang]
	sgEsse *sgScene::getTypedEsse(int aType, size_t aIndex) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "No such type esses in this scene.", "sgSceneManager::getSpecifiedEsse");
		}

		EsseMap *esses = tit->second;
		if(aIndex >= esses->size()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Esse index out of bounds.", "sgSceneManager::getTypedEsse");
		}

		EsseMap::const_iterator it = esses->begin();
		while(aIndex--) ++it;
		return it->second;

	}

	//  [1/2/2009 zhangxiang] -- in sgSceneManager
	//  [1/6/2009 zhangxiang]
	size_t sgScene::getTypedEsseNum(int aType) const{
		TypeEsseMap::const_iterator tit = m_EsseMap.find(aType);
		if(tit == m_EsseMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "No such type esses in this scene.", "sgSceneManager::getSpecifiedEsseNum");
		}

		return tit->second->size();
	}

	//  [1/2/2009 zhangxiang] -- in sgSceneManager
	//  [1/6/2009 zhangxiang]
	size_t sgScene::getEsseNum(void) const{
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
	void sgScene::merge(sgSubScene *aScene){
		if(!aScene){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null sgScene pointer.", "sgScene::merge");
		}

		if(aScene->getSceneManager() &&
			aScene->getSceneManager() != this->getSceneManager()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Scene \"" + aScene->name() +
							"\" is in a different scene manger to mine.",
							"sgScene::merge");
		}

		sgSceneNode *node;
		SSceneJoint *sourceroot = aScene->root();
		// get the active nodes
		sgNode::ConstChildIterator ncit = sourceroot->getConstChildIterator();
		for(; ncit.hasMoreElements(); ++ncit){
			node = static_cast<sgSceneNode*>(ncit.value());
			// remove the node from the source scene
			sourceroot->removeChild(node);
			aScene->removeSceneNode(node);
			// add the node to me
			m_pRoot->addChild(node);
		}
		// get the inactive nodes
		sgScene::ConstSceneNodeIterator snit = aScene->getConstSceneNodeIterator();
		for(; snit.hasMoreElements(); ++ncit){
			node = static_cast<sgSceneNode*>(snit.value());
			// remove the node from the source scene
			aScene->removeSceneNode(node);
			// add the node to me
			addSceneNode(node);
		}

		sgSceneManager *sm = aScene->getSceneManager();
		if(sm){
			sm->removeSubScene(aScene);
		}
		delete aScene;
	}

	//  [1/8/2009 zhangxiang]
	sgScene::ConstTypedEsseIterator sgScene::getConstEsseIterator(int aType) const{
		TypeEsseMap::const_iterator it = m_EsseMap.find(aType);
		if(it == m_EsseMap.end()){
			return ConstTypedEsseIterator(*(m_EsseMap.find(sgEsse::SET_UNKNOWN)->second));
		}
		return ConstTypedEsseIterator(*(it->second));
	}

	//  [1/9/2009 zhangxiang]
	sgScene::ConstSceneNodeIterator sgScene::getConstSceneNodeIterator(void) const{
		return ConstSceneNodeIterator(m_SceneNodeMap);
	}

	//  [1/12/2009 zhangxiang]
	bool sgScene::isLoopFrame(void) const{
		return m_bLoopFrame;
	}

	//  [1/12/2009 zhangxiang]
	void sgScene::setLoopFrame(bool aLoop){
		m_bLoopFrame = aLoop;
	}

	//  [1/8/2009 zhangxiang]
	size_t sgScene::frameNum(void) const{
		if(m_pAnimation){
			return m_pAnimation->maxFrameNum();
		}
		return 0;
	}

	//  [8/28/2008 zhangxiang] -- in sgSceneManager
	//  [1/8/2009 zhangxiang]
	int sgScene::currentFrame(void) const{
		return m_iCurrentFrame;
	}

	//  [8/28/2008 zhangxiang] -- in sSceneManager
	//  [1/8/2009 zhangxiang]
	void sgScene::setCurrentFrame(size_t aFrame){
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
	void sgScene::stepFrame(int aSteps){
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
	void sgScene::_updateFromAnimation(void){
		if(m_pAnimation){
			sgSceneNode *node;
			sgAnimaJoint *joint;
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

	// sgScene //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta