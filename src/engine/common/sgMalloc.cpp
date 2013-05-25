#include "sgMalloc.h"
#include "sgMemObject.h"
#include "sgMemChoc.h"
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

namespace __internal
{

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

}

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

#define ENV_8 0.125f
#define BLOCK_SIZE_SIZE 31
static const unsigned short BLOCK_SIZE_ARR[] = {8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 
				112, 120, 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 
				208, 216, 224, 232, 240, 248, 256};


    sgBaseAllocator::sgBaseAllocator()
    {
        if(!gAllocator)
            gAllocator = this;
		
		mSmallMemManager._init();
    }

    sgBaseAllocator::~sgBaseAllocator( void )
    {
		mSmallMemManager._release();

        if(gAllocator == this)
            gAllocator = 0;
    }

    void * sgBaseAllocator::malloc( size_t size )
    {
		void *ret = 0;

		size_t exp_size = size + sizeof(sgMemChocBox*);
		size_t index = exp_size * ENV_8;
		if(index > 31)
		{
			ret = __internal::__sgMalloc(exp_size);
			(*(sgMemChocBox**)ret)= 0;
		}
		else
		{
			sgMemChocBox *box = mSmallMemManager.getMemChocBox(BLOCK_SIZE_ARR[index]);
			ret = box->allocate();
			(*(sgMemChocBox**)ret)= box;
		}

		return (sgMemChocBox**)ret + 1;

    }

    void * sgBaseAllocator::realloc( void *mem, size_t size )
    {
        return __internal::__sgRealloc(mem, size);
    }

    void sgBaseAllocator::free( void *mem )
    {
		sgMemChocBox **flag = (sgMemChocBox**)mem - 1;
		sgMemChocBox *box = *flag;
		
		if(box == 0)
		{
			__internal::__sgFree((void*)flag);
		}
		else
		{
			box->deallocate((void*)flag);
		}
        
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

	///////////////////////////////////////////////

	void SmallMemManager::_init( void )
	{

	}

	void SmallMemManager::_release( void )
	{
		SmallObjectPool::iterator it = mSmallObjectPool.end();
		for(; it!=mSmallObjectPool.end(); ++it)
		{
			delete it->second;
		}
	}

	sgMemChocBox * SmallMemManager::getMemChocBox( size_t size )
	{
		sgMemChocBox *box = 0;
		SmallObjectPool::iterator it = mSmallObjectPool.find(size);
		if(it != mSmallObjectPool.end())
		{
			box = it->second;
		}
		else
		{
			box = new sgMemChocBox(size);
			mSmallObjectPool.insert(std::make_pair(size, box));
		}

		return box;
	}
}