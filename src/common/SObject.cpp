//////////////////////////////////////////////////////
// file: SObject.h @ 2007-11-17 by Zhang Xiang
// declares of the class SObject
// SObject is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SObject.h"

// DECLARATION ///////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong SObject::ms_oid = 0;

	SObject::SObject(void) : m_iOId(ms_oid++), m_bDisposing(false){

	}

	SObject::~SObject(void){
		_setDisposing();
	}

	//  [1/8/2009 zhangxiang]
	void SObject::_setDisposing(void){
		m_bDisposing = true;
	}

	//  [1/8/2009 zhangxiang]
	bool SObject::isDisposing(void) const{
		return m_bDisposing;
	}

	//  [1/9/2009 zhangxiang]
	uLong SObject::id(void) const{
		return m_iOId;
	}

} // namespace Sagitta
