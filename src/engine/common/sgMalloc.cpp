#include "sgMalloc.h"
#include "sgMemObject.h"
#include "nedmalloc/nedmalloc.c"
#include <iostream>
#include <fstream>
//#include <direct.h>

namespace Sagitta
{

#define SG_USING_NEDMALLOC

#ifndef SG_USING_NEDMALLOC
	#define _SG_MALLOC ::malloc
	#define _SG_REALLOC ::realloc
	#define _SG_FREE ::free
#else
	#define _SG_MALLOC nedalloc::nedmalloc
	#define _SG_REALLOC nedalloc::nedrealloc
	#define _SG_FREE nedalloc::nedfree
#endif

#define SG_ALIGNMENT sizeof(size_t)

#ifndef SG_ALIGNMENT

    void *__sgMalloc(size_t size)
    {
        if(size == 0) size = 1;
        return _SG_MALLOC(size);
    }

    void *__sgRealloc(void *mem, size_t size)
    {
        return _SG_REALLOC(mem, size);
    }

    void __sgFree(void *mem)
    {
        _SG_FREE(mem);
    }

#else
    void *__sgMalloc(size_t size)
    {
        if(size == 0) size = SG_ALIGNMENT;
#ifndef SG_USING_NEDMALLOC
		return ::malloc(size);
#else
        return nedalloc::nedmemalign(SG_ALIGNMENT, size);
#endif
    }

    void *__sgRealloc(void *mem, size_t size)
    {
        return _SG_REALLOC(mem, size);
    }

    void __sgFree(void *mem)
    {
        _SG_FREE(mem);
    }

#endif

    //////////////////////////////////////////////////////////////////////////

    /*
inline void* operator new(size_t size) throw(std::bad_alloc)
{
    void *ptr = sgMalloc(size);
    if(!ptr) throw std::bad_alloc();
    return ptr;
}
inline void* operator new[](size_t size)
{
    void *ptr = sgMalloc(size);
    if(!ptr) throw std::bad_alloc();
    return ptr;
}
inline void operator delete(void *ptr)
{
    sgFree(ptr);
}
inline void operator delete[](void *ptr)
{
    sgFree(ptr);
}
*/
///// placement operator new
//inline void *operator new (size_t size, void *ptr){
//    (void)size;
//    return ptr;
//}
//
///// corresponding operator for placement delete (second param same as the first)
//inline void operator delete(void *ptr, void*){
//    sgFree(ptr);
//}

    //////////////////////////////////////////////////////////////////////////
    sgBaseAllocator *gAllocator = 0;

    void sgCreateAllocator(void)
    {
        if(!gAllocator)
        {
            gAllocator = new sgBaseAllocator();
        }
    }

    void sgDestroyAllocator(void)
    {
        if(gAllocator)
        {
            delete gAllocator;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    void *sgMalloc(size_t size)
    {
        if(!gAllocator)
        {
            sgCreateAllocator();
        }
        return gAllocator->malloc(size);
    }

    void *sgRealloc(void *mem, size_t size)
    {
        if(!gAllocator)
        {
            sgCreateAllocator();
        }
        return gAllocator->realloc(mem, size);
    }

    void sgFree(void *mem)
    {
        if(!gAllocator)
        {
            sgCreateAllocator();
        }
        gAllocator->free(mem);
    }

    //////////////////////////////////////////////////////////////////////////

    sgBaseAllocator::sgBaseAllocator()
    {
        if(!gAllocator)
            gAllocator = this;
    }

    sgBaseAllocator::~sgBaseAllocator( void )
    {
        if(gAllocator == this)
            gAllocator = 0;
    }

    void * sgBaseAllocator::malloc( size_t size )
    {
        return __sgMalloc(size);
    }

    void * sgBaseAllocator::realloc( void *mem, size_t size )
    {
        return __sgRealloc(mem, size);
    }

    void sgBaseAllocator::free( void *mem )
    {
        __sgFree(mem);
    }

    //////////////////////////////////////////////////////////////////////////

    

    sgTraceAllocator::sgTraceAllocator( void )
    {

    }

    sgTraceAllocator::~sgTraceAllocator( void )
    {
//		char current_dir[MAX_PATH];
//		getcwd(current_dir, MAX_PATH);
		std::ofstream out(/*std::string(current_dir) + "/*/"memory_leak.txt");
		if(out)
		{
			if(mTraceMap.empty())
			{
				out << "No memory leak\n";
			}
			else
			{
				out << "Memory leack -----------------------\n";
				size_t count = 1;
				TraceMap::iterator it = mTraceMap.begin();
				for(; it!=mTraceMap.end(); ++it, ++count)
				{
					out << "\nLeak(" << count << ")\n";

					out << "Address: " << it->second.mem_ptr << "\n";
					out << "Size:    " << it->second.mem_size << "\n";
					out << "Call Stack: \n" << it->second.callstack.to_string();

					out << "\n";
				}
				out << "Memory leack -----------------------\n";
			}
		}

		out.flush();
        
    }

    void * sgTraceAllocator::malloc( size_t size )
    {
        void *ptr = sgBaseAllocator::malloc(size);
        
        AllocInfo ai;
        ai.mem_size = size;
        ai.mem_ptr = ptr;
        mTraceMap[ptr] = ai;

        return ptr;
    }

    void * sgTraceAllocator::realloc( void *mem, size_t size )
    {
        void *oldptr = mem;
        void *ptr = sgBaseAllocator::realloc(mem, size);

        TraceMap::iterator it = mTraceMap.find(oldptr);
        if(it != mTraceMap.end())
        {
            mTraceMap.erase(it);
        }

        AllocInfo ai;
        ai.mem_size = size;
        ai.mem_ptr = ptr;
        mTraceMap[ptr] = ai;

        return ptr;
    }

    void sgTraceAllocator::free( void *mem )
    {
        sgBaseAllocator::free(mem);

        TraceMap::iterator it = mTraceMap.find(mem);
        if(it != mTraceMap.end())
        {
            mTraceMap.erase(it);
        }
    }
}