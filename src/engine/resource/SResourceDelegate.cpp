//////////////////////////////////////////////////////
// file: SResourceDelegate.cpp 
// created by zhangxiang on 09-01-09
// define of the class SResourceDelegate
// SResourceDelegate is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SResourceDelegate.h"
#include "SResource.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/9/2009 zhangxiang]
	void SResourceDelegate::load(const StdString &aFileName, int aLoadOption, SResource *outResource){
		if(!outResource){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null resource pointer to output.", "SResourceDelegate::load");
		}
		if(aLoadOption == SResource::LOP_REFRESH){
			outResource->unLoad();
			loadImpl(aFileName, outResource);
		}else if(aLoadOption == SResource::LOP_MERGE){
			loadImpl(aFileName, outResource);
		}else{
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"No such load option.", "SResourceDelegate::load");
		}
	}

} // namespace Sagitta