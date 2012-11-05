//  [28/10/2012 zhangxiang]

#include "sgGLGpuProgram.h"
#include "sgGLUtil.h"
#include "engine/common/sgException.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgLogSystem.h"

namespace Sagitta{
    
    SG_META_DEFINE_ABSTRACT(sgGLShader, sgShader)

    sgGLShader::sgGLShader(void) : sgShader()
    {
    }
    
    sgGLShader::~sgGLShader(void)
    {
        if(mShaderId > 0)
		{
			glDeleteShader(mShaderId);
		}
    }

    bool sgGLShader::prepareShader(void)
    {
		std::string errInfo = std::string("    ") + std::string(this->getFilename().getStr()) + std::string("    sgGLShader::prepareShader");

        // create
        sgAssert(glCreateShader , "no function glCreateShader");
		mShaderId = glCreateShader(mShaderType);
        if(mShaderId == 0)
        {
            sgLogSystem::instance()->error(SG_CHECK_GL_ERROR() + errInfo);
            return false;
        }

        // compile
        const GLchar *s[1];
        s[0] = mShaderSource.c_str();
        glShaderSource(mShaderId, 1, s , 0);
        
        std::string err;
        err = SG_CHECK_GL_ERROR();
        if(!err.empty())
        {
            sgLogSystem::instance()->error(err + errInfo);
            return false;
        }
        
        glCompileShader(mShaderId);
        err = SG_CHECK_GL_ERROR();
        if(!err.empty())
        {
            sgLogSystem::instance()->error(err + errInfo);
            return false;
        }
        
        //check error
        GLsizei temp;
        glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &temp);
        if(temp == GL_FALSE)
        {
            glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &temp);
            char *log = (char*)sgMalloc(temp);
            glGetShaderInfoLog(mShaderId, temp, &temp, log);
            sgLogSystem::instance()->error(std::string(log) + errInfo);
            
            sgFree(log);
            
            return false;
        }
        
        return true;
    }
    
    // sgGLVertexShader ///////////////////////////////////
    
    SG_META_DEFINE(sgGLVertexShader, sgGLShader)

    sgGLVertexShader::sgGLVertexShader(void) : sgGLShader()
    {
        mShaderType = GL_VERTEX_SHADER;
    }
    
    sgGLVertexShader::~sgGLVertexShader(void)
    {
        
    }
    
    
    // sgGLFragmentShader ///////////////////////////////////
    
    SG_META_DEFINE(sgGLFragmentShader, sgGLShader)
    
    sgGLFragmentShader::sgGLFragmentShader(void) : sgGLShader()
    {
        mShaderType = GL_FRAGMENT_SHADER;
    }
    
    sgGLFragmentShader::~sgGLFragmentShader(void)
    {
        
    }
    
    // sgGLGpuProgram ///////////////////////////////////////
    
    SG_META_DEFINE(sgGLGpuProgram, sgGpuProgram)
    
    sgGLGpuProgram::sgGLGpuProgram(void) : sgGpuProgram()
    {
        mProgramId = glCreateProgram();
        if(mProgramId == 0)
        {
            sgLogSystem::instance()->error(SG_CHECK_GL_ERROR());
        }
    }

	sgGLGpuProgram::~sgGLGpuProgram(void)
	{
		if(mProgramId > 0)
		{
			glDeleteProgram(mProgramId);
			mProgramId = 0;
		}
	}
    
    bool sgGLGpuProgram::useProgram(void)
    {
        if(!isActive())
            return false;
        
        glUseProgram(mProgramId);
        std::string err = SG_CHECK_GL_ERROR();
        if(!err.empty())
        {
        //    mErrorInfo += err;
			sgLogSystem::instance()->error(err + "    sgGLGpuProgram::useProgram");
            return false;
        }
        
        return true;
    }
    
    bool sgGLGpuProgram::prepareProgram(void)
    {
        if(mProgramId == 0)
            return false;

		std::string errInfo = "    sgGLGpuProgram::prepareProgram";

        sgShader *vs = this->getVertexShader();
        if(!vs->isActive() || vs->getShaderType() != GL_VERTEX_SHADER)
        {
            sgLogSystem::instance()->error(std::string("No vertex shader") + errInfo);
            return false;
        }
        sgShader *fs = this->getFragmentShader();
        if(!fs->isActive() || fs->getShaderType() != GL_FRAGMENT_SHADER)
        {
            sgLogSystem::instance()->error(std::string("No fragment shader") + errInfo);
            return false;
        }
        
        glAttachShader(mProgramId, vs->getShaderId());
        std::string err = SG_CHECK_GL_ERROR();
        if(!err.empty())
        {
            sgLogSystem::instance()->error(err + errInfo);
            return false;
        }
        glAttachShader(mProgramId, fs->getShaderId());
        err = SG_CHECK_GL_ERROR();
        if(!err.empty())
        {
            sgLogSystem::instance()->error(err + errInfo);
            return false;
        }
        
        glLinkProgram(mProgramId);
        err = SG_CHECK_GL_ERROR();
        if(!err.empty())
        {
            sgLogSystem::instance()->error(err);
            return false;
        }
        
        //check error
        GLsizei temp;
        glGetProgramiv(mProgramId, GL_LINK_STATUS, &temp);
        if(temp == GL_FALSE)
        {
            glGetProgramiv(mProgramId, GL_INFO_LOG_LENGTH, &temp);
            char *log = (char*)sgMalloc(temp);
            glGetProgramInfoLog(mProgramId, temp, &temp, log);
            sgFree(log);
            
            sgLogSystem::instance()->error(std::string(log) + errInfo);
            
            return false;
        }
        
        if(!getParameters() || !getAttributes())
            return false;
        
        return true;
    }
    
    bool sgGLGpuProgram::getParameters(void)
    {
		int count;
		int maxChar;
		GLint size;
		GLenum type;
		glGetProgramiv(mProgramId, GL_ACTIVE_UNIFORMS, &count);
		glGetProgramiv(mProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxChar);
        mParameterList.clear();
        mParameterList.reserve(count);
		char *name = (char*)sgMalloc(maxChar);
		for(int i=0; i<count; ++i)
		{
			glGetActiveUniform(mProgramId, i, maxChar, NULL, &size, &type, name);
			// check is predefined uniform variant
            std::string strName = name;
			if(sgStringUtil::start_with(strName, "gl_"))
			{
				continue;
			}
			if(sgStringUtil::start_with(strName, "[0]"))
			{
				size_t index = strName.find_first_of('[');
				strName = strName.substr(0, index);
			}
            
            sgGpuParameter param;
            param.name = strName;
			param.type = sgGetRendererDataType(type);
            param.location = glGetUniformLocation(mProgramId, name);
            mParameterList.push_back(param);
            
		}
		sgFree(name);
        
        std::string err = SG_CHECK_GL_ERROR();
        if(!err.empty())
            return false;
        else
            return true;

    }
    
    bool sgGLGpuProgram::getAttributes(void)
    {
        int count;
		int maxChar;
		GLint size;
		GLenum type;
		glGetProgramiv(mProgramId, GL_ACTIVE_ATTRIBUTES, &count);
		glGetProgramiv(mProgramId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxChar);
        
        mAttributeList.clear();
        mAttributeList.reserve(count);
        
		char *name = (char*)sgMalloc(maxChar);
		for(int i=0; i<count; ++i)
		{
			glGetActiveAttrib(mProgramId, i, maxChar, NULL, &size, &type, name);
            // check is predefined attribute variant
			std::string strName = name;
			if(sgStringUtil::start_with(strName, "gl_"))
			{
				continue;
			}
            
            sgGpuAttribute attr;
            attr.name = strName;
			attr.type = sgGetRendererDataType(type);
            attr.location = glGetAttribLocation(mProgramId, name);
            mAttributeList.push_back(attr);
		}
        sgFree(name);
        
		std::string err = SG_CHECK_GL_ERROR();
        if(!err.empty())
            return false;
        else
            return true;
		
    }
    
} // namespace Sagitta