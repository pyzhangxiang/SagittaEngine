//////////////////////////////////////////////////////
// file: sgResourceManager.cpp 
// created by zhangxiang on 09-01-11
// define of the class sgResourceManager
// sgResourceManager is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgResourceManager.h"
#include "sgResource.h"
#include "../../common/utils/sgStringUtil.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	sgResourceManager::~sgResourceManager(void){
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
	void sgResourceManager::addResource(sgResource *aResource){
		if(!aResource){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null Resource pointer.", "sgResourceManager::addResource");
		}

		m_ResourceMap.insert(std::make_pair(aResource->getRsID(), aResource));
	}

	//  [1/11/2009 zhangxiang]
	sgResource *sgResourceManager::removeResource(uLong aID){
		ResourceMap::iterator it = m_ResourceMap.find(aID);
		if(it == m_ResourceMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_ITEM_NOT_FOUND,
				"Resource with ID " + sgStringUtil::to_string(aID),
				"sgResourceManager::removeResource");
		}

		m_ResourceMap.erase(it);
	}

	//  [1/11/2009 zhangxiang]
	void sgResourceManager::removeResource(sgResource *aResource){
		if(!aResource){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null Resource pointer.", "sgResourceManager::removeResource");
		}

		removeResource(aResource->getRsID());
	}

} // namespace Sagitta