//  [28/10/2012 zhangxiang]

#include "sgGLUtil.h"
#include "sgRenderer.h"
#include "engine/common/sgException.h"
#include "engine/common/sgUtil.h"
#include <sstream>

namespace Sagitta{
    
    std::string sgCheckGLError(const std::string &file, int line)
    {
        GLenum glErr;
        
        std::stringstream ss;
        
        glErr = glGetError();
        while (glErr != GL_NO_ERROR)
        {
            const GLubyte* sError = 0;//gluErrorString(glErr);
            
            if (sError)
                ss << "GL Error #" << glErr << "(" << sError << ") " << " in File " << file << " at line: " << line << ";     ";
            else
                ss << "GL Error #" << glErr << " (no message available)" << " in File " << file << " at line: " << line << ";    ";
			
            glErr = glGetError();
        }
        return ss.str();
    }
    
    int sgGetRendererDataType(GLenum glType)
    {
        switch (glType) {
            case GL_FLOAT:
                return RDT_F;
            case GL_FLOAT_VEC2:
                return RDT_FV2;
            case GL_FLOAT_VEC3:
                return RDT_FV3;
            case GL_FLOAT_VEC4:
                return RDT_FV4;
            case GL_INT:
                return RDT_I;
            case GL_INT_VEC2:
                return RDT_IV2;
            case GL_INT_VEC3:
                return RDT_IV3;
            case GL_INT_VEC4:
                return RDT_IV4;
            case GL_BOOL:
                return RDT_B;
            case GL_BOOL_VEC2:
                return RDT_BV2;
            case GL_BOOL_VEC3:
                return RDT_BV3;
            case GL_BOOL_VEC4:
                return RDT_BV4;
            case GL_FLOAT_MAT2:
                return RDT_FM22;
            case GL_FLOAT_MAT3:
                return RDT_FM33;
            case GL_FLOAT_MAT4:
                return RDT_FM44;
            case GL_SAMPLER_2D:
            case GL_SAMPLER_CUBE:
                return RDT_TEXTURE;
            default:
                return RDT_NIL;
        }
    
    }
    
    GLenum sgGetGLDataType(int sgType)
    {
		switch (sgType) {
		case RDT_F:
			return GL_FLOAT;
		case RDT_FV2:
			return GL_FLOAT_VEC2;
		case RDT_FV3:
			return GL_FLOAT_VEC3;
		case RDT_FV4:
			return GL_FLOAT_VEC4;
		case RDT_I:
			return GL_INT;
		case RDT_IV2:
			return GL_INT_VEC2;
		case RDT_IV3:
			return GL_INT_VEC3;
		case RDT_IV4:
			return GL_INT_VEC4;
		case RDT_UBYTE:
			return GL_UNSIGNED_BYTE;
		/*case RDT_UBYTE2:
			return GL_INT_VEC2;
		case RDT_UBYTE3:
			return GL_INT_VEC3;
		case RDT_UBYTE4:
			return GL_INT_VEC4;*/
		case RDT_B:
			return GL_BOOL;
		case RDT_BV2:
			return GL_BOOL_VEC2;
		case RDT_BV3:
			return GL_BOOL_VEC3;
		case RDT_BV4:
			return GL_BOOL_VEC4;
		case RDT_FM22:
			return GL_FLOAT_MAT2;
		case RDT_FM33:
			return GL_FLOAT_MAT3;
		case RDT_FM44:
			return GL_FLOAT_MAT4;
		/*case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			return RDT_TEXTURE;*/
		default:
			return 0;
		}
    }
    
    void sgSetUniform1f(int location, int extra, const void *data)
    {
        glUniform1fv(location, extra, (GLfloat*)data);
    }
    void sgSetUniform2fv(int location, int extra, const void *data)
    {
        glUniform2fv(location, extra, (GLfloat*)data);
    }
    void sgSetUniform3fv(int location, int extra, const void *data)
    {
        glUniform3fv(location, extra, (GLfloat*)data);
    }
    void sgSetUniform4fv(int location, int extra, const void *data)
    {
        glUniform4fv(location, extra, (GLfloat*)data);
    }
    
    void sgSetUniform1i(int location, int extra, const void *data)
    {
        glUniform1iv(location, extra, (GLint*)data);
    }
    void sgSetUniform2iv(int location, int extra, const void *data)
    {
        glUniform2iv(location, extra, (GLint*)data);
    }
    void sgSetUniform3iv(int location, int extra, const void *data)
    {
        glUniform3iv(location, extra, (GLint*)data);
    }
    void sgSetUniform4iv(int location, int extra, const void *data)
    {
        glUniform4iv(location, extra, (GLint*)data);
    }
    /*
    void sgSetUniform1ui(int location, int extra, const void *data)
    {
        int count = data->getSizeInBytes() / sizeof(UInt32);
        glUniform1uiv(location, count, (UInt32*)(data->data()));
    }
    void sgSetUniform2uiv(int location, int extra, const void *data)
    {
        int count = data->getSizeInBytes() / (sizeof(UInt32) * 2);
        glUniform2uiv(location, count, (UInt32*)(data->data()));
    }
    void sgSetUniform3uiv(int location, int extra, const void *data)
    {
        int count = data->getSizeInBytes() / (sizeof(UInt32) * 3);
        glUniform3uiv(location, count, (UInt32*)(data->data()));
    }
    void sgSetUniform4uiv(int location, int extra, const void *data)
    {
        int count = data->getSizeInBytes() / (sizeof(UInt32) * 4);
        glUniform4uiv(location, count, (UInt32*)(data->data()));
    }
    */
    /*
    void sgSetUniform1b(int location, int extra, const void *data);
    void sgSetUniform2bv(int location, int extra, const void *data);
    void sgSetUniform3bv(int location, int extra, const void *data);
    void sgSetUniform4bv(int location, int extra, const void *data);
    */
    
    void sgSetUniformMatrixf22(int location, int extra, const void *data)
    {
        int transpose = extra & 0x1;
        int count = extra >> 1;
        glUniformMatrix2fv(location, count, transpose, (GLfloat*)data);
    }
    void sgSetUniformMatrixf33(int location, int extra, const void *data)
    {
		int transpose = extra & 0x1;
		int count = extra >> 1;
        glUniformMatrix3fv(location, count, transpose, (GLfloat*)data);
    }
    void sgSetUniformMatrixf44(int location, int extra, const void *data)
    {
		int transpose = extra & 0x1;
		int count = extra >> 1;
        glUniformMatrix4fv(location, count, transpose, (GLfloat*)data);
    }

} // namespace Sagitta