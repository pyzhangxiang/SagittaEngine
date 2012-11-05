//////////////////////////////////////////////////////
// file: sgResource.cpp 
// created by zhangxiang on 09-01-09
// define of the class sgResource
// sgResource is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgResource.h"
#include "sgResourceCenter.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SG_META_DEFINE_ABSTRACT(sgResource, sgObject)

	//  [1/9/2009 zhangxiang]
	sgResource::sgResource() : sgObject(), mFilename(sgStrHandle::EmptyString)
    {
	}

	//  [1/9/2009 zhangxiang]
	sgResource::~sgResource(void)
    {
	}

	const Sagitta::sgStrHandle &sgResource::getFilename( void ) const
	{
		return mFilename;
	}
    
    void sgResource::setFilename(const sgStrHandle &filename)
    {
        mFilename = filename;
        onSetFilename();
    }

} // namespace Sagitta