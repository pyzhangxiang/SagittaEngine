//////////////////////////////////////////////////////
// file: sgResourceDelegate.cpp 
// created by zhangxiang on 09-01-09
// define of the class sgResourceDelegate
// sgResourceDelegate is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgResourceDelegate.h"
#include "sgResource.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/9/2009 zhangxiang]
	void sgResourceDelegate::load(const StdString &aFileName, int aLoadOption, sgResource *outResource){
		if(!outResource){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null resource pointer to output.", "sgResourceDelegate::load");
		}
		if(aLoadOption == sgResource::LOP_REFRESH){
			outResource->unLoad();
			loadImpl(aFileName, outResource);
		}else if(aLoadOption == sgResource::LOP_MERGE){
			loadImpl(aFileName, outResource);
		}else{
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"No such load option.", "sgResourceDelegate::load");
		}
	}

} // namespace Sagitta