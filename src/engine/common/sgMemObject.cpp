#include "sgMemObject.h"
#include "sgMalloc.h"
#include <iostream>

namespace Sagitta
{
    void *sgMemObject::operator new(size_t sz)
    {
        void *ptr = sgMalloc(sz);
        if(!ptr) throw std::bad_alloc();
        return ptr;
    }

    void sgMemObject::operator delete(void *ptr)
    {
        sgFree(ptr);
    }

    void *sgMemObject::operator new[](size_t sz)
    {
        void *ptr = sgMalloc(sz);
        if(!ptr) throw std::bad_alloc();
        return ptr;
    }

    void sgMemObject::operator delete[](void *ptr)
    {
        sgFree(ptr);
    }

    void *sgMemObject::operator new (size_t sz, void *ptr)
    {
        (void)sz;
        return ptr;
    }

    void sgMemObject::operator delete(void *ptr, void*)
    {
        sgFree(ptr);
    }
}