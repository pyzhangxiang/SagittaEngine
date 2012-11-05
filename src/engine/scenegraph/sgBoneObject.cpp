
#include "sgBoneObject.h"
#include "sgSkeleton.h"

namespace Sagitta{

	
	SG_META_DEFINE(sgBoneObject, sgSceneObject)

	//  [1/1/2009 zhangxiang]
	sgBoneObject::sgBoneObject(void) 
	: sgSceneObject(), mpSkeletonBelongTo(0)
    {

	}


	//  [1/1/2009 zhangxiang]
	sgBoneObject::~sgBoneObject(void)
    {
		if(mpSkeletonBelongTo)	
        {
            mpSkeletonBelongTo->removeBoneObject(this->getName());
            mpSkeletonBelongTo = 0;
        }
	}
    
    sgSkeleton *sgBoneObject::getSkeletonBelongTo(void) const
    {
        return mpSkeletonBelongTo;
    }

    void sgBoneObject::onSetParent(sgNode *aParent)
    {
        sgSceneObject::onSetParent(aParent);
        
        if(!aParent)
        {
            if(mpSkeletonBelongTo)
                mpSkeletonBelongTo->removeBoneObject(this->getName());
            return ;
        }
        
        sgBoneObject *obj = dynamic_cast<sgBoneObject*>(aParent);
        if(!obj)
        {
            // not a bone object
            // if it is a normal node
            // sgSceneObject::onSetParent will throw an exception
            // if it is a sgSceneObject
            // do nothing
            return ;
        }
        
        if(mpSkeletonBelongTo == obj->getSkeletonBelongTo())
        {
            // have in the same skeleton
            return ;
        }
        mpSkeletonBelongTo = obj->getSkeletonBelongTo();
        if(mpSkeletonBelongTo)
        {
            // add me to the skeleton
            mpSkeletonBelongTo->addBoneObject(this);
        }
        
    }

} // namespace Sagitta
