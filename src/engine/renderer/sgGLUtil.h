//  [28/10/2012 zhangxiang]

#ifndef __SGGLUTIL_H__
#define __SGGLUTIL_H__

#include "sgGLInclude.h"
#include "sgGpuProgram.h"
#include <string>

namespace Sagitta{

    _SG_KernelExport std::string sgCheckGLError(const std::string &file, int line);
    _SG_KernelExport int sgGetRendererDataType(GLenum glType);
    _SG_KernelExport GLenum sgGetRendererDataTypeInv(int sgType);
#define SG_CHECK_GL_ERROR() sgCheckGLError(__FILE__, __LINE__)
    
    class sgBuffer;
    
    void setUniform1f(int location, int extra, sgBuffer *data);
    void setUniform2fv(int location, int extra, sgBuffer *data);
    void setUniform3fv(int location, int extra, sgBuffer *data);
    void setUniform4fv(int location, int extra, sgBuffer *data);
    
    void setUniform1i(int location, int extra, sgBuffer *data);
    void setUniform2iv(int location, int extra, sgBuffer *data);
    void setUniform3iv(int location, int extra, sgBuffer *data);
    void setUniform4iv(int location, int extra, sgBuffer *data);
    /*
    void setUniform1ui(int location, int extra, sgBuffer *data);
    void setUniform2uiv(int location, int extra, sgBuffer *data);
    void setUniform3uiv(int location, int extra, sgBuffer *data);
    void setUniform4uiv(int location, int extra, sgBuffer *data);
    */
    /*
    void setUniform1b(int location, int extra, sgBuffer *data);
    void setUniform2bv(int location, int extra, sgBuffer *data);
    void setUniform3bv(int location, int extra, sgBuffer *data);
    void setUniform4bv(int location, int extra, sgBuffer *data);
    */
    
    void setUniformMatrixf22(int location, int extra, sgBuffer *data);
    void setUniformMatrixf33(int location, int extra, sgBuffer *data);
    void setUniformMatrixf44(int location, int extra, sgBuffer *data);

	
} // namespace Sagitta

#endif // __SGGLUTIL_H__

