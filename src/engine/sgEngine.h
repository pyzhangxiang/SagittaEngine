#ifndef __SGENGINE_H__
#define __SGENGINE_H__

#include "math/SagittaPlatform.h"
#include "engine/common/sgStrHandle.h"
#include <string>

namespace Sagitta
{
	//struct _SG_KernelExport sgEngine
	//{
		_SG_KernelExport void sgEngineInit(const sgStrHandle &rendererType,
                                           const std::string &assetRootDir);
		_SG_KernelExport void sgEngineShutdown(void);
	//};

}


#endif  // __SGENGINE_H__
