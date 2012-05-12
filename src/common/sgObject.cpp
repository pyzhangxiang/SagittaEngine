//////////////////////////////////////////////////////
// file: sgObject.h @ 2007-11-17 by Zhang Xiang
// declares of the class sgObject
// sgObject is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgObject.h"

// DECLARATION ///////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong sgObject::ms_oid = 0;

	sgObject::sgObject(void) : m_iOId(ms_oid++), m_bDisposing(false){

	}

	sgObject::~sgObject(void){
		_setDisposing();
	}

	//  [1/8/2009 zhangxiang]
	void sgObject::_setDisposing(void){
		m_bDisposing = true;
	}

	//  [1/8/2009 zhangxiang]
	bool sgObject::isDisposing(void) const{
		return m_bDisposing;
	}

	//  [1/9/2009 zhangxiang]
	uLong sgObject::id(void) const{
		return m_iOId;
	}



} // namespace Sagitta
