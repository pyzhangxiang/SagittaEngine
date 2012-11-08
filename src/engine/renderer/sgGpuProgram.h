//  [28/10/2012 zhangxiang]
#ifndef __SGGUPPROGRAM_H__
#define __SGGUPPROGRAM_H__

#include "engine/resource/sgResource.h"
#include <map>

namespace Sagitta{

	class _SG_KernelExport sgShader : public sgResource
	{
        SG_META_DECLARE_ABSTRACT(sgShader)
        
    protected:
        UInt32 mShaderId;
        std::string mShaderSource;
        /// set in the inherited class
        UInt32 mShaderType;

        //std::string mErrorInfo;
    private:
        /// if this shader is compiled
        bool mActive;
        
	public:
        sgShader(void);
		virtual ~sgShader(void);
        
        UInt32 getShaderId(void) const{ return mShaderId; }
        bool isActive(void) const{ return mActive; }

        UInt32 getShaderType(void) const{ return mShaderType; }
        //const std::string &getErrorInfo(void) const{ return mErrorInfo; }
        
	protected:
        // load the shader file and compile it to create a shader
		virtual void onSetFilename(void);
        
        virtual bool prepareShader(void) = 0;

	}; //#### end class sgShader
    
    
    struct sgGpuParameter
    {
        std::string name;
        int type;
        int location;
    };
    
    struct sgGpuAttribute
    {
        std::string name;
        int type;
        int location;
    };

	class sgVertexBufferElement;
	class sgBuffer;
    
    class _SG_KernelExport sgGpuProgram : public sgObject
    {
        SG_META_DECLARE_ABSTRACT(sgGpuProgram)
        
	public:
        typedef sg_map(sgStrHandle, sgGpuParameter) ParameterList;
        typedef sg_map(sgStrHandle, sgGpuAttribute) AttributeList;

	protected:    
        UInt32 mProgramId;
        sgStrHandle mVertexShaderFileName;
        sgStrHandle mFragmentShaderFileName;
        ParameterList mParameterList;
        AttributeList mAttributeList;
        
        //std::string mErrorInfo;
    private:
        bool mActive;   // if linked
        
    public:
        sgGpuProgram(void);
        virtual ~sgGpuProgram(void);
        
        UInt32 getProgramId(void) const{ return mProgramId; }
        bool isActive(void) const{ return mActive; }
        //const std::string &getErrorInfo(void) const{ return mErrorInfo; }
        
        sgShader *getVertexShader(void) const;
        sgShader *getFragmentShader(void) const;
        
        bool setShader(const sgStrHandle &vertexShaderFilename,
                       const sgStrHandle &fragmentShaderFilename);

		const AttributeList &getAttributeList(void) const{ return mAttributeList; }
        
		virtual bool setParameter(const sgStrHandle &name, size_t extra, sgBuffer *data) = 0;
        virtual bool useProgram(void) = 0;

    protected:
        // create, attach, link
        virtual bool prepareProgram(void) = 0;
        
    };

} // namespace Sagitta


#endif // __SGGUPPROGRAM_H__

