//////////////////////////////////////////////////////
// file: SSceneManager.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class SSceneManager
// SSceneManager is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SSceneManager.h"
#include "SSubScene.h"
#include "SSceneNode.h"
#include "SEsse.h"
#include "SEntity.h"
#include "../buffer/SVertexData.h"
#include "../buffer/SVertexBufferElement.h"
#include "../buffer/SVertexIndexBuffer.h"
#include "../io/SFile.h"
#include "../../common/utils/SString.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/6/2009 zhangxiang]
	SSceneManager::SSceneManager() :
	SScene(),
	m_GLobalAmbientColor(Color::WHITE),
	m_iCurrentFrame(0){
		_setSceneManager(this);

		// unknown sub-scene
		SubSceneMap *unknown = new SubSceneMap;
		m_SubSceneMap.insert(std::make_pair(SSubScene::ST_UNKNOWN, unknown));
	}

	//  [1/6/2009 zhangxiang]
	SSceneManager::SSceneManager(const StdString &aName):
	SScene(aName),
	m_GLobalAmbientColor(Color::WHITE),
	m_iCurrentFrame(0){
		_setSceneManager(this);

		// unknown sub-scene
		SubSceneMap *unknown = new SubSceneMap;
		m_SubSceneMap.insert(std::make_pair(SSubScene::ST_UNKNOWN, unknown));
	}

	//  [1/6/2009 zhangxiang]
	SSceneManager::~SSceneManager(){
		// notify i'm in destructor
		// tell SubScene do not remove themself from me
		_setDisposing();

		TypedSubSceneMap::iterator tsmit = m_SubSceneMap.begin();
		TypedSubSceneMap::iterator tsmend = m_SubSceneMap.end();
		for(; tsmit!=tsmend; ++tsmit){
			SubSceneMap *subs = tsmit->second;
			SubSceneMap::iterator smit = subs->begin();
			SubSceneMap::iterator smeit = subs->end();
			for(; smit!=smeit; ++smit){
				delete smit->first;
				delete smit->second;
			}

			delete subs;
		}
	}

	//  [8/10/2008 zhangxiang]
	const Color &SSceneManager::globalAmbientColor(void) const{
		return m_GLobalAmbientColor;
	}

	//  [8/10/2008 zhangxiang]
	void SSceneManager::setGlobalAmbientColor(const Color &aColor){
		m_GLobalAmbientColor = aColor;
	}

	//  [1/3/2009 zhangxiang]
	SRenderOption &SSceneManager::globalRenderOption(void){
		return m_GlobalRenderOption;
	}

	//  [1/3/2009 zhangxiang]
	void SSceneManager::setGlobalRenderOption(const SRenderOption &aRO){
		m_GlobalRenderOption = aRO;
	}

	//  [1/7/2009 zhangxiang]
	void SSceneManager::addSubScene(SSubScene *aSubScene){
		if(!aSubScene){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null SSubScene pointer.",
							"SSceneManager::addSubScene");
		}

		// Continue when the SubScene does not in any SceneManager.
		if(aSubScene->getSceneManager()){
			if(aSubScene->getSceneManager() != this){
				THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
								"SubScene \"" + aSubScene->name() +
								"\" has been in another SceneManager.",
								"SSceneManager::addSubScene");
			}else{
				return ;
			}
		}

		SubSceneMap *subs;
		int stype = aSubScene->type();
		TypedSubSceneMap::iterator tit = m_SubSceneMap.find(stype);
		if(tit == m_SubSceneMap.end()){
			subs = new SubSceneMap;
			m_SubSceneMap.insert(std::make_pair(stype, subs));
		}else{
			subs = tit->second;
		}

		SSceneJoint *jnode = new SSceneJoint(aSubScene->name() + "_jnode", this);
		this->root()->addChild(jnode);
		jnode->addChild(aSubScene->root());

		subs->insert(std::make_pair(jnode, aSubScene));

		aSubScene->_setSceneManager(this);
	}

	//  [1/7/2009 zhangxiang]
	void SSceneManager::removeSubScene(SSubScene *aSubScene){
		if(!aSubScene){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null SSubScene pointer.", "SSceneManager::removeSubScene");
		}

		TypedSubSceneMap::iterator tit = m_SubSceneMap.find(aSubScene->type());
		if(tit == m_SubSceneMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"No such type SubScene in this SceneManager.",
							"SSceneManager::SSubScene");
		}

		SubSceneMap *subs = tit->second;
		SubSceneMap::iterator it = subs->find(static_cast<SSceneJoint*>(aSubScene->root()->parent()));
		if(it == subs->end()){
			THROW_SAGI_EXCEPT(SException::ERR_ITEM_NOT_FOUND,
							"A SubScene named \"" + aSubScene->name() +
							"\" is not in this SceneManager.",
							"SSceneManager::removeSubScene");
		}

		// first release the joint node
		SSceneJoint *jnode = it->first;
		jnode->removeChild(it->second->root());
		if(jnode->parent()){
			jnode->parent()->removeChild(jnode);
		}
		delete jnode;

		it->second->_setSceneManager(0);
		subs->erase(it);
	}

	//  [1/8/2009 zhangxiang]
	SSceneManager::ConstTypedSubSceneIterator SSceneManager::getConstSubSceneIterator(int aType) const{
		TypedSubSceneMap::const_iterator it = m_SubSceneMap.find(aType);
		if(it == m_SubSceneMap.end()){
			return ConstTypedSubSceneIterator(*(m_SubSceneMap.find(SSubScene::ST_UNKNOWN)->second));
		}
		
		return ConstTypedSubSceneIterator(*(it->second));
	}

	//  [1/8/2009 zhangxiang]
	SSceneManager::ConstTotalSubSceneIterator SSceneManager::getConstSubSceneIterator(void) const{
		return ConstTotalSubSceneIterator(m_SubSceneMap);
	}

	//  [1/12/2009 zhangxiang]
	void SSceneManager::setCurrentFrame(size_t aFrame){
		SScene::setCurrentFrame(aFrame);

		// set frame for sub-scenes
		SubSceneMap *subs;
		TypedSubSceneMap::iterator tsmit = m_SubSceneMap.begin();
		TypedSubSceneMap::iterator tsmend = m_SubSceneMap.end();
		for(; tsmit!=tsmend; ++tsmit){
			subs = tsmit->second;
			SubSceneMap::iterator smit = subs->begin();
			SubSceneMap::iterator smeit = subs->end();
			for(; smit!=smeit; ++smit){
				smit->second->setCurrentFrame(aFrame);
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	void SSceneManager::stepFrame(int aSteps){
		SScene::stepFrame(aSteps);

		// step frame for sub-scenes
		SubSceneMap *subs;
		TypedSubSceneMap::iterator tsmit = m_SubSceneMap.begin();
		TypedSubSceneMap::iterator tsmend = m_SubSceneMap.end();
		for(; tsmit!=tsmend; ++tsmit){
			subs = tsmit->second;
			SubSceneMap::iterator smit = subs->begin();
			SubSceneMap::iterator smeit = subs->end();
			for(; smit!=smeit; ++smit){
				smit->second->stepFrame(aSteps);
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	void SSceneManager::_updateFromAnimation(void){
		SScene::_updateFromAnimation();

		// update for visible sub-scenes
		SubSceneMap *subs;
		TypedSubSceneMap::iterator tsmit = m_SubSceneMap.begin();
		TypedSubSceneMap::iterator tsmend = m_SubSceneMap.end();
		for(; tsmit!=tsmend; ++tsmit){
			subs = tsmit->second;
			SubSceneMap::iterator smit = subs->begin();
			SubSceneMap::iterator smeit = subs->end();
			for(; smit!=smeit; ++smit){
				if(smit->second->isVisible())
					smit->second->_updateFromAnimation();
			}
		}
	}
	
} // namespace Sagitta