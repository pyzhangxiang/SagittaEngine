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
    
    class sgBuffer;
    
    void sgSetUniform1f(int location, size_t extra, sgBuffer *data);
    void sgSetUniform2fv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform3fv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform4fv(int location, size_t extra, sgBuffer *data);
    
    void sgSetUniform1i(int location, size_t extra, sgBuffer *data);
    void sgSetUniform2iv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform3iv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform4iv(int location, size_t extra, sgBuffer *data);
    /*
    void sgSetUniform1ui(int location, size_t extra, sgBuffer *data);
    void sgSetUniform2uiv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform3uiv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform4uiv(int location, size_t extra, sgBuffer *data);
    */
    /*
    void sgSetUniform1b(int location, size_t extra, sgBuffer *data);
    void sgSetUniform2bv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform3bv(int location, size_t extra, sgBuffer *data);
    void sgSetUniform4bv(int location, size_t extra, sgBuffer *data);
    */
    
    void sgSetUniformMatrixf22(int location, size_t extra, sgBuffer *data);
    void sgSetUniformMatrixf33(int location, size_t extra, sgBuffer *data);
    void sgSetUniformMatrixf44(int location, size_t extra, sgBuffer *data);

	
} // namespace Sagitta

#endif // __SGGLUTIL_H__

