
#include "sgStrHandle.h"
#include "Singleton.h"
#include "sgMalloc.h"
#include <map>
#include <vector>
#include <string>
#include <cassert>

namespace Sagitta
{
    sgStrHandle sgStrHandle::EmptyString("");

	sgStrHandle::sgStrHandle( void ) : mStr(0)
	{

	}

    sgStrHandle::sgStrHandle(const char *str)
    {
        if(!str)
            mStr = sgStrHandle::EmptyString.mStr;
        else
            mStr = sgStrHandleCenter::instance().getString(str);
    }

    sgStrHandle sgStrHandle::operator=(const char *str)
    {
        if(!str)
            mStr = sgStrHandle::EmptyString.mStr;
        else
            mStr = sgStrHandleCenter::instance().getString(str);

        return *this;
    }

    const char *sgStrHandle::operator*() const
    {
        return mStr;
    }
    const char *sgStrHandle::operator()() const
    {
        return mStr;
    }

    size_t sgStrHandle::getStringCount(void)
    {
        return sgStrHandleCenter::instance().getStringCount();
    }

    size_t sgStrHandle::getStringMemSize(void)
    {
        return sgStrHandleCenter::instance().getStringMemSize();
    }

    ////////////////
    
    sgStrHandleCenter::Pool::Pool(void)
    {
        _occupied = 0;
        _data = new char[SIZE]; // (char*)sgMalloc(SIZE);
        _available = _data;
    }
    void sgStrHandleCenter::Pool::release(void)
    {
        //sgFree(_data);
        delete []_data;
    }
    char *sgStrHandleCenter::Pool::allocate(size_t sz)
    {
        if(SIZE - _occupied < sz)
            return 0;
        _occupied += sz;
        
        char *ret = _available;
        _available += sz;
        return ret;
    }


        
    sgStrHandleCenter::Pool *sgStrHandleCenter::createNewPool(void)
    {
        mPoolVec.push_back(Pool());
        return &(mPoolVec[mPoolVec.size() - 1]);
    }
        
        
    sgStrHandleCenter::sgStrHandleCenter(void)
    {
        mStringMemSize = 0;
        mLatestPool = createNewPool();
    }
    
    sgStrHandleCenter::~sgStrHandleCenter(void)
    {
        release();
    }
    
    void sgStrHandleCenter::release(void)
    {
        for(size_t i=0; i<mPoolVec.size(); ++i)
        {
            mPoolVec[i].release();
        }
		mStringMap.clear();
        mPoolVec.clear();
    }
    
    const char *sgStrHandleCenter::getString(const char *str)
    {
        if(!str)
            return sgStrHandle::EmptyString.getStr();
        
        std::string stdstr = str;
        StringMap::iterator it = mStringMap.find(stdstr);
        if(it != mStringMap.end())
            return it->second.getStr();
        
        size_t len = stdstr.size() + 1; // '\0'
        
        assert(mLatestPool);
        char *ptr = mLatestPool->allocate(len);
        
        if(!ptr)
        {
            for(size_t i=0; i<mPoolVec.size(); ++i)
            {
                ptr = mPoolVec[i].allocate(len);
                if(ptr)
                {
                    mLatestPool = &(mPoolVec[i]);
                    break;
                }
            }
            
            if(!ptr)
            {
                mLatestPool = createNewPool();
                ptr = mLatestPool->allocate(len);
            }
        }
        
        if(ptr)
        {
            strncpy(ptr, str, len-1);
            ptr[len-1] = '\0';
            sgStrHandle strHandle;
            strHandle.mStr = ptr;
            mStringMap.insert(std::make_pair(stdstr, strHandle));
            
            mStringMemSize += len;
            
            return ptr;
        }
        
        return sgStrHandle::EmptyString.getStr();
    }
    
    size_t sgStrHandleCenter::getStringCount() const{ return mStringMap.size(); }
    size_t sgStrHandleCenter::getStringMemSize() const{ return Pool::SIZE * mPoolVec.size(); }
}
