
#include "sgSkeleton.h"
#include "sgSceneObject.h"
#include "sgBoneObject.h"
#include "engine/component/sgAnimationComponent.h"
#include "engine/resource/sgAnimation.h"
#include "engine/resource/sgAnimationJoint.h"

namespace Sagitta{
    
	
	SG_META_DEFINE(sgSkeleton, sgObject)
    
	//  [1/1/2009 zhangxiang]
	sgSkeleton::sgSkeleton(void)
	: sgObject(), mpRoot(0)
    {
		mpRoot = (sgSceneObject*)sgObject::createObject(sgSceneObject::GetClassTypeName());
        mpBoneRoot = (sgBoneObject*)sgObject::createObject(sgBoneObject::GetClassTypeName());
        mpBoneRoot->setParent(mpRoot);
        // let bone root belong to me
        mpBoneRoot->mpSkeletonBelongTo = this;
        addBoneObject(mpBoneRoot);
	}
    
    
	//  [1/1/2009 zhangxiang]
	sgSkeleton::~sgSkeleton(void)
    {
        // first clear map
        // so that while in the destruction of bone objects
        // removing from me would be faster
        mBoneNodeMap.clear();
        
        // detach from parent object
        // and I will be dettached from the scene I am belong to
    //mpRoot->setParent(0);
        //parent and scene will be set to zero in the destruction of sgNode and sgSceneObject
        // so we don't need to set root's separent as zero manually
        
        // destruct all objects belong to me
		sgObject::destroyObject(mpRoot);
	}
    
	sgSceneObject * sgSkeleton::getRoot( void ) const
	{
		return mpRoot;
	}
    
    sgBoneObject *sgSkeleton::getBoneRoot(void) const
    {
        return mpBoneRoot;
    }
    
	void sgSkeleton::update( Float32 deltaTime )
	{
		// set nodes' transform by their animation joints
        sgSceneObject *parent = this->parent();
        if(!parent)
            return ;
        
        sgAnimationComponent *comp = (sgAnimationComponent*)parent->getComponent(sgAnimationComponent::GetClassTypeName());
        if(!comp)
            return ;
        
        sgAnimation *pAnimation = comp->getAnimation();
        if(!pAnimation)
            return ;
        
        Real animTime = comp->getCurrentTime();
        BoneNodeMap::iterator it = mBoneNodeMap.begin();
        for( ; it!=mBoneNodeMap.end(); ++it)
        {
            sgSceneObject *node = it->second;
            sgAnimationJoint *joint = pAnimation->getAnimaJoint(it->first);
			if(joint)
			{
				if(joint->isPositionActive())
					node->setRelativePosition(joint->getPosition(animTime));
				if(joint->isOrientationActive())
					node->setRelativeOrientation(joint->getOrientation(animTime));
				if(joint->isScaleActive())
					node->setRelativeScale(joint->getScale(animTime));
			}
        }
	}
    
    void sgSkeleton::setBelongTo(sgSceneObject *parent)
    {
        mpRoot->setParent(parent);
    }
    
    sgSceneObject *sgSkeleton::parent(void) const
    {
        return dynamic_cast<sgSceneObject*>(mpRoot->parent());
    }
    
    
    void sgSkeleton::showDebug(bool show)
    {
        BoneNodeMap::iterator it = mBoneNodeMap.begin();
        for(; it!=mBoneNodeMap.end(); ++it)
        {
            it->second->showDebug(show);
        }
    }

	bool sgSkeleton::addBoneObject( sgBoneObject *node )
	{
		BoneNodeMap::iterator it = mBoneNodeMap.find(node->getName());
		if(it != mBoneNodeMap.end())
        {
			return false;
        }
		mBoneNodeMap.insert(std::make_pair(node->getName(), node));
        return true;
	}
    
    void sgSkeleton::removeBoneObject(sgBoneObject *node)
    {
        if(!node)
            return ;
        removeBoneObject(node->getName());
    }
    
    void sgSkeleton::removeBoneObject(const std::string &name)
    {
        BoneNodeMap::iterator it = mBoneNodeMap.find(name);
        if(it != mBoneNodeMap.end())
        {
            mBoneNodeMap.erase(it);
        }
    }

	sgBoneObject * sgSkeleton::getBoneNode( const std::string &bonename ) const
	{
		BoneNodeMap::const_iterator it = mBoneNodeMap.find(bonename);
		if(it == mBoneNodeMap.end())
		{
			return 0;
		}

		return it->second;
	}

} // namespace Sagitta
