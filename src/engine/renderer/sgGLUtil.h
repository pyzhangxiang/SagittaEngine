//  [28/10/2012 zhangxiang]

#ifndef __SGGLUTIL_H__
#define __SGGLUTIL_H__

#include "sgGLInclude.h"
#include "sgGpuProgram.h"
#include <string>

namespace Sagitta{

    _SG_KernelExport std::string sgCheckGLError(const std::string &file, int line);
    _SG_KernelExport int sgGetRendererDataType(GLenum glType);
    
#define SG_CHECK_GL_ERROR() sgCheckGLError(__FILE__, __LINE__)
	
} // namespace Sagitta

#endif // __SGGLUTIL_H__

