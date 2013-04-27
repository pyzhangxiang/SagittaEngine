#include "sgClassMeta.h"
#include "sgException.h"
#include <cassert>

namespace Sagitta
{

    
    sgClassMeta::sgClassMeta(const sgStrHandle &name, 
                           const sgStrHandle &superName, 
                           Constructor creator)
    : mClassName(name), mSuperName(superName), mCreator(creator)
    {
        sgMetaCenter::instance().addMeta(this);
        
    }
    
    sgClassMeta::~sgClassMeta(void)
    {
        
    }
    
    sgClassMeta *sgClassMeta::getSuperMeta(void) const
    {
        return sgMetaCenter::instance().findMeta(mSuperName);
    }
    
    sgObject *sgClassMeta::createObject(void)
    {
        //sgAssert(mCreator);
        if(!mCreator)
        {
            THROW_SAGI_EXCEPT(sgException::ERR_NOT_IMPLEMENTED,
                              "cannot instance an abstract class("
                              + std::string(mClassName.getStr()) + ") ",
                              "sgClassMeta::createObject");
        }
        
        return mCreator();
    }
    
    bool sgClassMeta::isClass(const sgStrHandle &classtype) const
    {
        const sgClassMeta *meta = this;
        while(meta)
        {
            if(classtype == meta->getName())
                return true;
            meta = sgMetaCenter::instance().findMeta(meta->getSuperName());
        }
        return false;
    }
    
    //////////////////////////////
    
    void sgMetaCenter::addMeta(sgClassMeta *meta)
    {
        assert(meta);
        
        assert( findMeta(meta->getName()) == 0 );
        mMetaMap.insert(std::make_pair(meta->getName(), meta));
    }
    
    sgClassMeta *sgMetaCenter::findMeta(const sgStrHandle &name) const
    {
        MetaMap::const_iterator it = mMetaMap.find(name);
        if(it == mMetaMap.end())
            return NULL;
        return it->second;
    }
}

