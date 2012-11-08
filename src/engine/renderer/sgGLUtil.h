//  [28/10/2012 zhangxiang]

#ifndef __SGGLUTIL_H__
#define __SGGLUTIL_H__

#include "sgGLInclude.h"
#include "sgGpuProgram.h"
#include <string>

namespace Sagitta{

    _SG_KernelExport std::string sgCheckGLError(const std::string &file, int line);
    _SG_KernelExport int sgGetRendererDataType(GLenum glType);
    _SG_KernelExport GLenum sgGetGLDataType(int sgType);
#define SG_CHECK_GL_ERROR() sgCheckGLError(__FILE__, __LINE__)
    
    void sgSetUniform1f(int location, int extra, const void *data);
    void sgSetUniform2fv(int location, int extra, const void *data);
    void sgSetUniform3fv(int location, int extra, const void *data);
    void sgSetUniform4fv(int location, int extra, const void *data);
    
    void sgSetUniform1i(int location, int extra, const void *data);
    void sgSetUniform2iv(int location, int extra, const void *data);
    void sgSetUniform3iv(int location, int extra, const void *data);
    void sgSetUniform4iv(int location, int extra, const void *data);
    /*
    void sgSetUniform1ui(int location, int extra, const void *data);
    void sgSetUniform2uiv(int location, int extra, const void *data);
    void sgSetUniform3uiv(int location, int extra, const void *data);
    void sgSetUniform4uiv(int location, int extra, const void *data);
    */
    /*
    void sgSetUniform1b(int location, int extra, const void *data);
    void sgSetUniform2bv(int location, int extra, const void *data);
    void sgSetUniform3bv(int location, int extra, const void *data);
    void sgSetUniform4bv(int location, int extra, const void *data);
    */
    
    void sgSetUniformMatrixf22(int location, int extra, const void *data);
    void sgSetUniformMatrixf33(int location, int extra, const void *data);
    void sgSetUniformMatrixf44(int location, int extra, const void *data);

	
} // namespace Sagitta

#endif // __SGGLUTIL_H__

