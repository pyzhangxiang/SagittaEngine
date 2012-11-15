//////////////////////////////////////////////////////
// file: sgResourceCenter.cpp 
// created by zhangxiang on 09-01-11
//////////////////////////////////////////////////////

#include "sgResourceCenter.h"
#include "sgResource.h"
#include "sgImageLoader.h"
#include "engine/common/sgClassMeta.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgException.h"

namespace Sagitta{

	//SG_META_DEFINE(sgResourceCenter, sgObject)
    
    sgResourceCenter::sgResourceCenter(void)
    : mRootDir("./")
    {
        registerImageLoader(".png", sgImageLoader_stb::GetClassName());
		registerImageLoader(".tga", sgImageLoader_stb::GetClassName());
		registerImageLoader(".bmp", sgImageLoader_stb::GetClassName());
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

	bool sgResourceCenter::registerImageLoader( const std::string &imageFileExt, const sgStrHandle &imageLoaderType )
	{
		sgClassMeta *meta = sgMetaCenter::instance().findMeta(imageLoaderType);
		if(meta == NULL)
			return false;

		if(! (meta->isClass(sgImageLoader::GetClassName())) )
			return false;

		mImageLoaderTypeMap[imageFileExt] = imageLoaderType.getStr();
		return true;
	}

	sgImageLoader * sgResourceCenter::getImageLoader( const std::string &imageFileExt ) const
	{
		ImageLoaderTypeMap::const_iterator it = mImageLoaderTypeMap.find(imageFileExt);
		if(it == mImageLoaderTypeMap.end())
			return NULL;

		sgImageLoader *imgLoader = (sgImageLoader*)sgObject::createObject(sgStrHandle(it->second.c_str()));
		return imgLoader;
	}

} // namespace Sagitta
