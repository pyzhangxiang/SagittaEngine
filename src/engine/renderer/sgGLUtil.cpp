//  [28/10/2012 zhangxiang]

#include "sgGLUtil.h"
#include "sgRenderer.h"
#include "engine/buffer/sgBuffer.h"
#include "engine/common/sgException.h"
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
                return sgRenderer::DT_F;
            case GL_FLOAT_VEC2:
                return sgRenderer::DT_FV2;
            case GL_FLOAT_VEC3:
                return sgRenderer::DT_FV3;
            case GL_FLOAT_VEC4:
                return sgRenderer::DT_FV4;
            case GL_INT:
                return sgRenderer::DT_I;
            case GL_INT_VEC2:
                return sgRenderer::DT_IV2;
            case GL_INT_VEC3:
                return sgRenderer::DT_IV3;
            case GL_INT_VEC4:
                return sgRenderer::DT_IV4;
            case GL_BOOL:
                return sgRenderer::DT_B;
            case GL_BOOL_VEC2:
                return sgRenderer::DT_BV2;
            case GL_BOOL_VEC3:
                return sgRenderer::DT_BV3;
            case GL_BOOL_VEC4:
                return sgRenderer::DT_BV4;
            case GL_FLOAT_MAT2:
                return sgRenderer::DT_FM22;
            case GL_FLOAT_MAT3:
                return sgRenderer::DT_FM33;
            case GL_FLOAT_MAT4:
                return sgRenderer::DT_FM44;
            case GL_SAMPLER_2D:
            case GL_SAMPLER_CUBE:
                return sgRenderer::DT_TEXTURE;
            default:
                return sgRenderer::DT_NIL;
        }
    
    }
    
    GLenum sgGetRendererDataTypeInv(int sgType)
    {
        
    }
    
    void setUniform1f(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / sizeof(Float32);
        glUniform1fv(location, count, (Float32*)(data->data()));
    }
    void setUniform2fv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(Float32) * 2);
        glUniform2fv(location, count, (Float32*)(data->data()));
    }
    void setUniform3fv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(Float32) * 3);
        glUniform3fv(location, count, (Float32*)(data->data()));
    }
    void setUniform4fv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(Float32) * 4);
        glUniform4fv(location, count, (Float32*)(data->data()));
    }
    
    void setUniform1i(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / sizeof(int);
        glUniform1iv(location, count, (int*)(data->data()));
    }
    void setUniform2iv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(int) * 2);
        glUniform2iv(location, count, (int*)(data->data()));
    }
    void setUniform3iv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(int) * 3);
        glUniform3iv(location, count, (int*)(data->data()));
    }
    void setUniform4iv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(int) * 4);
        glUniform4iv(location, count, (int*)(data->data()));
    }
    /*
    void setUniform1ui(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / sizeof(UInt32);
        glUniform1uiv(location, count, (UInt32*)(data->data()));
    }
    void setUniform2uiv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(UInt32) * 2);
        glUniform2uiv(location, count, (UInt32*)(data->data()));
    }
    void setUniform3uiv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(UInt32) * 3);
        glUniform3uiv(location, count, (UInt32*)(data->data()));
    }
    void setUniform4uiv(int location, int extra, sgBuffer *data)
    {
        int count = data->getSizeInBytes() / (sizeof(UInt32) * 4);
        glUniform4uiv(location, count, (UInt32*)(data->data()));
    }
    */
    /*
    void setUniform1b(int location, int extra, sgBuffer *data);
    void setUniform2bv(int location, int extra, sgBuffer *data);
    void setUniform3bv(int location, int extra, sgBuffer *data);
    void setUniform4bv(int location, int extra, sgBuffer *data);
    */
    
    void setUniformMatrixf22(int location, int extra, sgBuffer *data)
    {
        int transpose = extra;
        int count = data->getSizeInBytes() / (sizeof(Float32) * 4);
        glUniformMatrix2fv(location, count, transpose, (Float32*)(data->data()));
    }
    void setUniformMatrixf33(int location, int extra, sgBuffer *data)
    {
        int transpose = extra;
        int count = data->getSizeInBytes() / (sizeof(Float32) * 9);
        glUniformMatrix3fv(location, count, transpose, (Float32*)(data->data()));
    }
    void setUniformMatrixf44(int location, int extra, sgBuffer *data)
    {
        int transpose = extra;
        int count = data->getSizeInBytes() / (sizeof(Float32) * 16);
        glUniformMatrix4fv(location, count, transpose, (Float32*)(data->data()));
    }

} // namespace Sagitta