#ifndef __SGMEMOBJECT_H__
#define __SGMEMOBJECT_H__

#include "math/SagittaPlatform.h"
#include "sgMalloc.h"

namespace Sagitta
{
    class _SG_KernelExport sgMemObject
    {
    public:
        sgMemObject(void){}
        virtual ~sgMemObject(void){}

        static void *operator new(size_t sz);

        static void operator delete(void *ptr);

        static void *operator new[](size_t sz);

        static void operator delete[](void *ptr);

        /// placement operator new
        static void *operator new (size_t sz, void *ptr);

        /// corresponding operator for placement delete (second param same as the first)
        static void operator delete(void *ptr, void*);

        /// operator new, with debug line info
       /* static void *operator new(size_t sz, const char *file, int line, const char *func){
            void *ptr = sgMalloc(sz);
            if(!ptr) throw std::bad_alloc();
            return ptr;
        }*/

        /// only called if there is an exception in corresponding "new"
        /*void operator delete(void *ptr, const char*, int, const char*){
            sgFree(ptr);
        }*/

        /// array operator new, with debug line info
        /*static void *operator new[](size_t sz, const char *file, int line, const char *func){
            return sgFree(sz);
        }*/

        /*void operator delete[](void *ptr, const char*, int, const char*){
            sgFree(ptr);
        }*/
        
    };
}



#endif  // __SGMEMOBJECT_H__
