
#ifndef __SGIOUTIL_H__
#define __SGIOUTIL_H__

#include "math/SagittaPlatform.h"
#include "math/sgTypeDef.h"
#include <iostream>
#include <fstream>
#include <string>

namespace Sagitta{

    _SG_KernelExport UInt32 sgGetFileLength(std::ifstream &stream);

} // namespace Sagitta

#endif // __SGIOUTIL_H__

