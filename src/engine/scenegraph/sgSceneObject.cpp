
#include "sgSceneObject.h"
#include "engine/component/sgComponent.h"
#include "sgScene.h"
#include "sgSkeleton.h"
#include "engine/common/sgException.h"

namespace Sagitta{

	
	SG_META_DEFINE(sgSceneObject, sgNode)

	//  [1/1/2009 zhangxiang]
	sgSceneObject::sgSceneObject(void) 
	: sgNode(), mpScene(0)
    , mbSceneChanged(true), mpSkeleton(0)
    , mCastShadow(true)
    , mIsDebugObj(false), mDebugObjectToShow(0)
    {

	}


	//  [1/1/2009 zhangxiang]
	sgSceneObject::~sgSceneObject(void)
    {
        // and I will be dettached from the scene I am belong to
        // ~sgNode() will let me detached from my parent
    //this->setScene(0);
        // every child will be destroy in ~sgNode()
        // so their scene will be set to zero in their own destruction
        // so let's just assign mpScen to be 0
        mpScene = 0;
        
		if(mpSkeleton)
        {
            // destroy skeleton attached on me
			sgObject::destroyObject(mpSkeleton);
            mpSkeleton = 0;
        }
        
        // destroy all component
        ComponentMap::iterator it = mComponentMap.begin();
        for(; it!=mComponentMap.end(); ++it)
        {
            sgObject::destroyObject(it->second);
        }
        mComponentMap.clear();
	}

	sgComponent * sgSceneObject::createComponent( const sgStrHandle &type )
	{
		sgComponent *comp = getComponent(type);
		if(comp)
			return comp;

		comp = dynamic_cast<sgComponent*>(sgObject::createObject(type));
		if(comp)
		{
			mComponentMap.insert(std::make_pair(type, comp));
			comp->setParent(this);
		}
		return comp;
	}

	void sgSceneObject::removeComponent( const sgStrHandle &type )
	{
		ComponentMap::iterator it = mComponentMap.find(type);
		if(it != mComponentMap.end())
        {
            sgObject::destroyObject(it->second);
			mComponentMap.erase(it);
        }
	}

	sgComponent * sgSceneObject::getComponent( const sgStrHandle &type ) const
	{
		ComponentMap::const_iterator it = mComponentMap.find(type);
		if(it != mComponentMap.end())
			return it->second;

		return 0;
	}

	void sgSceneObject::update( Float32 deltaTime )
	{
		// update by components
		// like animation
        ComponentMap::iterator it = mComponentMap.begin();
        for(; it!=mComponentMap.end(); ++it)
        {
            it->second->update(deltaTime);
        }
        
        if(mpSkeleton)
            mpSkeleton->update(deltaTime);

		sgNode::update(deltaTime);
	}
    
    void sgSceneObject::setCastShadow(bool bCastShadow)
    {
        if(mCastShadow == bCastShadow)
            return ;
        
        mCastShadow = bCastShadow;
 /*
        ChildNodeMap::iterator it = m_Children.begin();
        for(; it!=m_Children.end(); ++it)
        {
            sgSceneObject *child = dynamic_cast<sgSceneObject*>(it->second);
            if(child)
            {
                child->setCastShadow(bCastShadow);
            }
        }
  */
    }

    void sgSceneObject::onSetParent(sgNode *aParent)
    {
        sgNode::onSetParent(aParent);
        if(!aParent)
        {
            // removed from the scene
            this->setScene(0);
            return ;
        }
        
        sgSceneObject *obj = dynamic_cast<sgSceneObject*>(aParent);
        
        // a normal node should not be used in a scene
        //sgAssert(obj);
        if(!obj)
        {
            THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
                          "a normal node should not be used in a scene",
                          "sgSceneObject::onSetParent");
        }
        
        this->setScene(obj->getScene());
        
    }

	void sgSceneObject::setScene( sgScene *pScene )
	{
		if(mpScene == pScene)
			return ;
		mpScene = pScene;
		mbSceneChanged = true;

        // let all children know which scene they belong to
        sgSceneObject *obj = 0;
		ChildNodeMap::iterator it = m_Children.begin();
		ChildNodeMap::iterator eit = m_Children.end();
		for(; it!=eit; ++it)
		{
            obj = dynamic_cast<sgSceneObject*>(it->second);
            if(!obj)
                continue;
			obj->setScene(pScene);
		}
	}

	sgScene * sgSceneObject::getScene( void ) const
	{
		return mpScene;
	}
    
    sgSkeleton *sgSceneObject::getSkeleton(void) const
    {
        return mpSkeleton;
    }
    
    sgSkeleton *sgSceneObject::setSkeleton(sgSkeleton *pSkeleton)
    {
        sgSkeleton *original = mpSkeleton;
        if(original == pSkeleton)
            return original;
        if(original)
            original->setBelongTo(0);
        
        mpSkeleton = pSkeleton;
        mpSkeleton->setBelongTo(this);
        return original;
    }

	void sgSceneObject::getAllObjects( SceneObjectVec &objVec )
	{
		objVec.push_back(this);

        sgSceneObject *obj = 0;
		ChildNodeMap::iterator it = m_Children.begin();
		ChildNodeMap::iterator eit = m_Children.end();
		for(; it!=eit; ++it)
		{
            obj = dynamic_cast<sgSceneObject*>(it->second);
            if(!obj)
                continue;
			obj->getAllObjects(objVec);
		}
	}
    
    void sgSceneObject::setIsDebugObj(bool debug)
    {
        if(mIsDebugObj == debug)
            return ;
        
        mIsDebugObj = debug;
        if(mIsDebugObj)
        {
            setCastShadow(false);
        }
        
        ChildNodeMap::iterator it = m_Children.begin();
        for(; it!=m_Children.end(); ++it)
        {
            sgSceneObject *child = dynamic_cast<sgSceneObject*>(it->second);
            if(child)
            {
                child->setIsDebugObj(debug);
            }
        }
    }
    
    sgSceneObject *sgSceneObject::getDebugObjectToShow(void) const
    {
        return mDebugObjectToShow;
    }
    
    sgSceneObject *sgSceneObject::setDebugObjectToShow(sgSceneObject *obj)
    {
        if(mDebugObjectToShow == obj)
            return mDebugObjectToShow;
        
        sgSceneObject *original = mDebugObjectToShow;
        if(original)
            original->setParent(0);
        
        mDebugObjectToShow = obj;
        mDebugObjectToShow->setParent(this);
        mDebugObjectToShow->setIsDebugObj(true);
        return original;
    }
    
    void sgSceneObject::showDebug(bool show)
    {
        if(!mDebugObjectToShow)
            return ;
        
        if(show)
        {
            mDebugObjectToShow->setActive(true);
        }
        else
        {
            mDebugObjectToShow->setActive(false);
        }
    }

} // namespace Sagitta
