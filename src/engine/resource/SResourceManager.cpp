//////////////////////////////////////////////////////
// file: SResourceManager.cpp 
// created by zhangxiang on 09-01-11
// define of the class SResourceManager
// SResourceManager is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SResourceManager.h"
#include "SResource.h"
#include "../../common/utils/SString.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	SResourceManager::~SResourceManager(void){
		// Notify Resources that i'm in destructor,
		// so don't try to remove yourself from me.
		_setDisposing();

		ResourceMap::iterator it = m_ResourceMap.begin();
		ResourceMap::iterator eit = m_ResourceMap.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ResourceMap.clear();
	}

	//  [1/11/2009 zhangxiang]
	void SResourceManager::addResource(SResource *aResource){
		if(!aResource){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null Resource pointer.", "SResourceManager::addResource");
		}

		m_ResourceMap.insert(std::make_pair(aResource->getRsID(), aResource));
	}

	//  [1/11/2009 zhangxiang]
	SResource *SResourceManager::removeResource(uLong aID){
		ResourceMap::iterator it = m_ResourceMap.find(aID);
		if(it == m_ResourceMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_ITEM_NOT_FOUND,
				"Resource with ID " + SString::to_string(aID),
				"SResourceManager::removeResource");
		}

		m_ResourceMap.erase(it);
	}

	//  [1/11/2009 zhangxiang]
	void SResourceManager::removeResource(SResource *aResource){
		if(!aResource){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null Resource pointer.", "SResourceManager::removeResource");
		}

		removeResource(aResource->getRsID());
	}

} // namespace Sagitta