//////////////////////////////////////////////////////
// file: SResource.cpp 
// created by zhangxiang on 09-01-09
// define of the class SResource
// SResource is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SResource.h"
#include "SResourceDelegate.h"
#include "SResourceManager.h"
#include "../../common/utils/SString.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong SResource::ms_iResourceCount = 0;

	//  [1/9/2009 zhangxiang]
	SResource::SResource(SResourceDelegate *aDelegate) :
	SObject(), mc_iRsID(ms_iResourceCount),
	m_pDelegate(aDelegate), m_bUnloading(false){
		if(!m_pDelegate){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Null ResourceDelegate pointer.",
							"SResource::SResource");
		}

		m_sName = "Resource" + SString::to_string(ms_iResourceCount++);

		// add to ResourceManager
		SResourceManager::instance().addResource(this);
	}

	//  [1/9/2009 zhangxiang]
	SResource::SResource(const StdString &aName, SResourceDelegate *aDelegate) :
	SObject(), m_sName(aName), mc_iRsID(ms_iResourceCount++),
	m_pDelegate(aDelegate), m_bUnloading(false){
		if(!m_pDelegate){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null ResourceDelegate pointer.",
				"SResource::SResource");
		}

		// add to ResourceManager
		SResourceManager::instance().addResource(this);
	}

	//  [1/9/2009 zhangxiang]
	SResource::~SResource(void){
		delete m_pDelegate;

		if(!SResourceManager::instance().isDisposing()){
			SResourceManager::instance().removeResource(this);
		}
	}

	//  [1/9/2009 zhangxiang]
	SResourceDelegate *SResource::del(void) const{
		return m_pDelegate;
	}

	//  [1/6/2009 zhangxiang] -- in SScene
	//  [1/9/2009 zhangxiang]
	const StdString &SResource::name(void) const{
		return m_sName;
	}

	//  [1/6/2009 zhangxiang] -- in SScene
	//  [1/9/2009 zhangxiang]
	void SResource::setName(const StdString &aName){
		m_sName = aName;
	}

	//  [1/11/2009 zhangxiang]
	uLong SResource::getRsID(void) const{
		return mc_iRsID;
	}

	//  [1/9/2009 zhangxiang]
	void SResource::load(const StdString &aFileName, int aLoadOption/* = LOP_MERGE */){
		m_pDelegate->load(aFileName, aLoadOption, this);
	}

	//  [1/9/2009 zhangxiang]
	void SResource::unLoad(void){
		// notify unloading begin
		m_bUnloading = true;

		unLoadImpl();

		// notify unloading end
		m_bUnloading = false;
	}

	//  [1/9/2009 zhangxiang]
	bool SResource::isUnloading(void) const{
		return m_bUnloading;
	}

} // namespace Sagitta