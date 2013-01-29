
#ifndef __SGSTRHANDLE_H__
#define __SGSTRHANDLE_H__

#include "math/SagittaPlatform.h"
#include "Singleton.h"
#include "sgStlAllocator.h"
#include <vector>
#include <map>
#include <string>
#include <set>

namespace Sagitta
{
    class sgStrHandleCenter;
    
    class _SG_KernelExport sgStrHandle
    {
        friend class sgStrHandleCenter;
    public:
        static const sgStrHandle EmptyString;
		static const sgStrHandle UnknownString;
		static const sgStrHandle InvalidString;

    private:
        const char *mStr;
        
		sgStrHandle(void);
    public:
        sgStrHandle(const char *str);
        sgStrHandle operator = (const char *str);
        
        bool operator == (const sgStrHandle &rhs) const
        {
            return mStr == rhs.mStr;
        }
        
        bool operator != (const sgStrHandle &rhs) const
        {
            return mStr != rhs.mStr;
        }
        
        bool operator < (const sgStrHandle &rhs) const
        {
            return mStr < rhs.mStr;
        }

        bool isEmpty(void) const{ return mStr == EmptyString.mStr; }
        //bool isNull(void) const{ return mStr == 0; }

        const char *getStr() const{ return mStr; }

        const char *operator*() const;
        const char *operator()() const;
        
        static size_t getStringCount(void);
        static size_t getStringMemSize(void);

    };

	typedef sg_vector(sgStrHandle) StringHandleList;
	typedef sg_set(sgStrHandle) StringHandleSet;
    
    class sgStrHandleCenter : public Singleton<sgStrHandleCenter>
    {
    private:
        typedef std::map<std::string, sgStrHandle> StringMap;
        StringMap mStringMap;
        
        struct Pool
        {
            enum{ SIZE = 1024 * 1024 };
            size_t _occupied;
            char *_data;
            char *_available;
            Pool(void);
            void release(void);
            char *allocate(size_t sz);
        };
        
        std::vector<Pool> mPoolVec;
        Pool *mLatestPool;
        
        Pool *createNewPool(void);
        
        size_t mStringMemSize;
        
    public: 
        sgStrHandleCenter(void);
        
        ~sgStrHandleCenter(void);
        
        // should be called before application terminated
        void release(void);
        
        const char *getString(const char *str);
        
        size_t getStringCount() const;
        size_t getStringMemSize() const;
    };

}
#endif  // __SGSTRHANDLE_H__

