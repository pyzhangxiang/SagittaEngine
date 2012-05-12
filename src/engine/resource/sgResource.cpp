//////////////////////////////////////////////////////
// file: sgResource.cpp 
// created by zhangxiang on 09-01-09
// define of the class sgResource
// sgResource is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgResource.h"
#include "sgResourceDelegate.h"
#include "sgResourceManager.h"
#include "../../common/utils/sgStringUtil.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong sgResource::ms_iResourceCount = 0;

	//  [1/9/2009 zhangxiang]
	sgResource::sgResource(sgResourceDelegate *aDelegate) :
	sgObject(), mc_iRsID(ms_iResourceCount),
	m_pDelegate(aDelegate), m_bUnloading(false){
		if(!m_pDelegate){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Null ResourceDelegate pointer.",
							"sgResource::sgResource");
		}

		m_sName = "Resource" + sgStringUtil::to_string(ms_iResourceCount++);

		// add to ResourceManager
		sgResourceManager::instance().addResource(this);
	}

	//  [1/9/2009 zhangxiang]
	sgResource::sgResource(const StdString &aName, sgResourceDelegate *aDelegate) :
	sgObject(), m_sName(aName), mc_iRsID(ms_iResourceCount++),
	m_pDelegate(aDelegate), m_bUnloading(false){
		if(!m_pDelegate){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null ResourceDelegate pointer.",
				"sgResource::sgResource");
		}

		// add to ResourceManager
		sgResourceManager::instance().addResource(this);
	}

	//  [1/9/2009 zhangxiang]
	sgResource::~sgResource(void){
		delete m_pDelegate;

		if(!sgResourceManager::instance().isDisposing()){
			sgResourceManager::instance().removeResource(this);
		}
	}

	//  [1/9/2009 zhangxiang]
	sgResourceDelegate *sgResource::del(void) const{
		return m_pDelegate;
	}

	//  [1/6/2009 zhangxiang] -- in sgScene
	//  [1/9/2009 zhangxiang]
	const StdString &sgResource::name(void) const{
		return m_sName;
	}

	//  [1/6/2009 zhangxiang] -- in sgScene
	//  [1/9/2009 zhangxiang]
	void sgResource::setName(const StdString &aName){
		m_sName = aName;
	}

	//  [1/11/2009 zhangxiang]
	uLong sgResource::getRsID(void) const{
		return mc_iRsID;
	}

	//  [1/9/2009 zhangxiang]
	void sgResource::load(const StdString &aFileName, int aLoadOption/* = LOP_MERGE */){
		m_pDelegate->load(aFileName, aLoadOption, this);
	}

	//  [1/9/2009 zhangxiang]
	void sgResource::unLoad(void){
		// notify unloading begin
		m_bUnloading = true;

		unLoadImpl();

		// notify unloading end
		m_bUnloading = false;
	}

	//  [1/9/2009 zhangxiang]
	bool sgResource::isUnloading(void) const{
		return m_bUnloading;
	}

} // namespace Sagitta