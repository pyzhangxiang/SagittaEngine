//////////////////////////////////////////////////////
// file: sgResourceCenter.cpp 
// created by zhangxiang on 09-01-11
// define of the class sgResourceCenter
// sgResourceCenter is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgResourceCenter.h"
#include "sgResource.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//SG_META_DEFINE(sgResourceCenter, sgObject)
    
    sgResourceCenter::sgResourceCenter(void)
    : mRootDir("./")
    {
        
    }

	//  [1/11/2009 zhangxiang]
	sgResourceCenter::~sgResourceCenter(void){
        ResourceMap todel = m_ResourceMap;
		ResourceMap::iterator it = todel.begin();
		ResourceMap::iterator eit = todel.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_ResourceMap.clear();
	}

	//  [1/11/2009 zhangxiang]
	void sgResourceCenter::addResource(sgResource *aResource){
		if(!aResource)
        {
			return ;
		}

		m_ResourceMap.insert(std::make_pair(aResource->getFilename(), aResource));
	}

	//  [1/11/2009 zhangxiang]
	void sgResourceCenter::removeResource(const sgStrHandle &filename)
    {
		ResourceMap::iterator it = m_ResourceMap.find(filename);
		if(it == m_ResourceMap.end())
        {
            return ;
		}
		m_ResourceMap.erase(it);
	}

	//  [1/11/2009 zhangxiang]
	void sgResourceCenter::removeResource(sgResource *aResource)
    {
		if(!aResource)
        {
			return ;
		}

		removeResource(aResource->getFilename());
	}
    
    sgResource *sgResourceCenter::findResource(const sgStrHandle &filename)
    {
        ResourceMap::iterator it = m_ResourceMap.find(filename);
		if(it == m_ResourceMap.end())
        {
            return 0;
		}
        
        return it->second;
    }
    
    sgResource *sgResourceCenter::createResource(const sgStrHandle &type, const sgStrHandle &filename)
    {
        
        sgResource *res = findResource(filename);
        if(res)
            return res;
        
        sgClassMeta *meta = sgMetaCenter::instance().findMeta(type);
        if(!meta || !meta->isClass(sgResource::GetClassName()))
            return 0;
        
        res = (sgResource*)(sgObject::createObject(type));
        if(!res)
            return 0;
        // load resource
        // in setFilename, a virtual function onSetFilename will be called
        // if need, you can do loading in it
        res->setFilename(filename);
        
        // add to resource list
        addResource(res);
        
        return res;
    }
    
    bool sgResourceCenter::destroyResource(sgResource *resource)
    {
        if(!resource)
            return false;
        
        return destroyResource(resource->getFilename());
    }
    
    bool sgResourceCenter::destroyResource(const sgStrHandle &filename)
    {
        sgResource *res = findResource(filename);
        if(res)
        {
            removeResource(filename);
            sgObject::destroyObject(res);
            return true;
        }
        
        return false;
    }
    
    void sgResourceCenter::setRootDir(const std::string &dir)
    {
        mRootDir = dir;
        if(mRootDir.empty())
        {
            mRootDir = "./";
        }
        else if(mRootDir[mRootDir.size() - 1] != '/' ||
           mRootDir[mRootDir.size() - 1] != '/')
        {
            mRootDir += "/";
        }
    }
    
    std::string sgResourceCenter::getResourcePath(const std::string &subname)
    {
        if(subname.find(mRootDir) == 0)
            return subname;
        return mRootDir + subname;
    }

} // namespace Sagitta