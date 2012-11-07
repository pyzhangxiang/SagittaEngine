//  [28/10/2012 zhangxiang]

#ifndef __SGGLGUPPROGRAM_H__
#define __SGGLGUPPROGRAM_H__

#include "sgGpuProgram.h"

namespace Sagitta{

    class _SG_KernelExport sgGLShader : public sgShader
	{
        SG_META_DECLARE_ABSTRACT(sgGLShader)
    
	public:
        sgGLShader(void);
		virtual ~sgGLShader(void) = 0;
        
        
	protected:
        virtual bool prepareShader(void);
        
	}; //#### end class sgGLShader
    
    
    class _SG_KernelExport sgGLVertexShader : public sgGLShader
	{
        SG_META_DECLARE(sgGLVertexShader)
        
	public:
        sgGLVertexShader(void);
		virtual ~sgGLVertexShader(void);
        
	}; //#### end class sgGLVertexShader
    
    
    class _SG_KernelExport sgGLFragmentShader : public sgGLShader
	{
        SG_META_DECLARE(sgGLFragmentShader)
        
	public:
        sgGLFragmentShader(void);
		virtual ~sgGLFragmentShader(void);
        
	}; //#### end class sgGLFragmentShader
    
    
    class sgVertexBufferElement;
	class sgBuffer;
    
    class _SG_KernelExport sgGLGpuProgram : public sgGpuProgram
    {
        SG_META_DECLARE(sgGLGpuProgram)
        
    public:
        sgGLGpuProgram(void);
        virtual ~sgGLGpuProgram(void);
        
		virtual bool setParameter(const sgStrHandle &name, sgBuffer *data);
        virtual bool useProgram(void);

	protected:
        // create, attach, link
        virtual bool prepareProgram(void);
    private:
        bool getParameters(void);
        bool getAttributes(void);
        
    };
    
	
} // namespace Sagitta

#endif // __SGGLGUPPROGRAM_H__

