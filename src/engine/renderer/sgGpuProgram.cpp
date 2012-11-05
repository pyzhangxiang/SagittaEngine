//  [28/10/2012 zhangxiang]
#include "sgGpuProgram.h"
#include "engine/common/sgIOUtil.h"
#include "engine/resource/sgResourceCenter.h"
namespace Sagitta{

    SG_META_DEFINE_ABSTRACT(sgShader, sgResource)
    
    sgShader::sgShader(void)
    : sgResource(), mShaderId(0), mActive(false)
    {
        
    }
    
    sgShader::~sgShader(void)
    {
        
    }
    
    void sgShader::onSetFilename(void)
    {
        std::ifstream file((sgResourceCenter::instance()->getRootDir() + getFilename().getStr()).c_str());
        if(!file.good())
            return ;
        
        UInt32 len = sgGetFileLength(file);
        if(len == 0)
            return ;
        
        mShaderSource = "";
        char buffer[256];
		memset(buffer, 0, sizeof(buffer));
        while(!file.eof())
        {
            file.read(buffer, 255);
            mShaderSource += buffer;
        }
        
        mActive = prepareShader();
    }
    
    // sgGpuProgram ///////////////////////////////
    SG_META_DEFINE_ABSTRACT(sgGpuProgram, sgObject)
    
    sgGpuProgram::sgGpuProgram(void)
    : sgObject()
    , mProgramId(0)
    , mVertexShaderFileName(sgStrHandle::EmptyString)
    , mFragmentShaderFileName(sgStrHandle::EmptyString)
    , mActive(false)
    {
        
    }
    
    sgGpuProgram::~sgGpuProgram(void)
    {
    }
    
    sgShader *sgGpuProgram::getVertexShader(void) const
    {
        sgShader *shader = dynamic_cast<sgShader*>(sgResourceCenter::instance()->findResource(mVertexShaderFileName));
        return shader;
    }
    
    sgShader *sgGpuProgram::getFragmentShader(void) const
    {
        sgShader *shader = dynamic_cast<sgShader*>(sgResourceCenter::instance()->findResource(mFragmentShaderFileName));
        return shader;
    }
    
    bool sgGpuProgram::setShader(const sgStrHandle &vertexShaderFilename,
                   const sgStrHandle &fragmentShaderFilename)
    {
        if(mVertexShaderFileName == vertexShaderFilename
           && mFragmentShaderFileName == fragmentShaderFilename)
        {
            return mActive;
        }
        
        mVertexShaderFileName = vertexShaderFilename;
        mFragmentShaderFileName = fragmentShaderFilename;
        mActive = prepareProgram();
        return mActive;
    }

} // namespace Sagitta