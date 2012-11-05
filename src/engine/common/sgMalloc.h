#ifndef __SGMALLOC_H__
#define __SGMALLOC_H__

#include "math/SagittaPlatform.h"
#include "stacktrace/call_stack.hpp"
#include <map>

namespace Sagitta
{
    _SG_KernelExport void sgCreateAllocator(void);
    _SG_KernelExport void sgDestroyAllocator(void);


    _SG_KernelExport void *sgMalloc(size_t size);
    _SG_KernelExport void *sgRealloc(void *mem, size_t size);
    _SG_KernelExport void sgFree(void *mem);


    class sgBaseAllocator
    {
    public:
        sgBaseAllocator(void);
        virtual ~sgBaseAllocator(void);

        virtual void *malloc(size_t size);
        virtual void *realloc(void *mem, size_t size);
        virtual void free(void *mem);
    };

    class sgTraceAllocator : public sgBaseAllocator
    {
    private:
        struct AllocInfo
        {
            stacktrace::call_stack callstack;
            size_t mem_size;
            void *mem_ptr;
            AllocInfo(void) : callstack(3), mem_size(0), mem_ptr(0){}
        };
        typedef std::map<void*, AllocInfo> TraceMap;
        TraceMap mTraceMap;

    public:
        sgTraceAllocator(void);
        virtual ~sgTraceAllocator(void);

        virtual void *malloc(size_t size);
        virtual void *realloc(void *mem, size_t size);
        virtual void free(void *mem);
    };
}



#endif  // __SGMALLOC_H__
