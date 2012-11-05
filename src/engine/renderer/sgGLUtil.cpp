//  [28/10/2012 zhangxiang]

#include "sgGLUtil.h"
#include "sgRenderer.h"
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

} // namespace Sagitta