
#include "sgAnimation.h"
#include "sgAnimationJoint.h"

namespace Sagitta{

	SG_META_DEFINE(sgAnimation, sgResource)
	
	//  [1/11/2009 zhangxiang]
	sgAnimation::sgAnimation(void)
    : sgResource(), mkeyNum(0)//, m_fFrameTime(0.0333333)
    {
		
	}
    
	//  [1/11/2009 zhangxiang]
	sgAnimation::~sgAnimation(void)
    {
        AnimationJointMap::iterator it = m_AnimaJointMap.begin();
		AnimationJointMap::iterator eit = m_AnimaJointMap.end();
		for(; it!=eit; ++it){
            sgObject::destroyObject(it->second);
		}
		m_AnimaJointMap.clear();
	}

    
	//  [1/11/2009 zhangxiang]
	size_t sgAnimation::getKeyNum(void) const{
		return mkeyNum;
	}
    
    void sgAnimation::setKeyNum(size_t num)
    {
        if(mkeyNum == num)
            return ;
        
        mkeyNum = num;
        AnimationJointMap::iterator it = m_AnimaJointMap.begin();
		AnimationJointMap::iterator eit = m_AnimaJointMap.end();
		for(; it!=eit; ++it){
            it->second->reset(mkeyNum);
		}
    }
    
/*	//  [1/11/2009 zhangxiang]
	Real sgAnimation::frameTime(void) const{
		return m_fFrameTime;
	}
    
	//  [1/11/2009 zhangxiang]
	void sgAnimation::setFrameTime(Real aFrameTime){
		m_fFrameTime = aFrameTime;
	}
  */  
	//  [1/11/2009 zhangxiang]
	sgAnimationJoint *sgAnimation::createAnimaJoint(const std::string &aName)
    {
        
		sgAnimationJoint *ret = getAnimaJoint(aName);
        if(ret)
            return ret;
        
        ret = (sgAnimationJoint*)sgObject::createObject(sgAnimationJoint::GetClassName());
        ret->setName(aName);
		m_AnimaJointMap.insert(std::make_pair(aName, ret));
		return ret;
	}
    
    
	//  [1/11/2009 zhangxiang]
	sgAnimationJoint *sgAnimation::getAnimaJoint(const std::string &aName) const{
		AnimationJointMap::const_iterator it = m_AnimaJointMap.find(aName);
		if(it == m_AnimaJointMap.end()){
			return 0;
		}else{
			return it->second;
		}
	}
    
	//  [1/11/2009 zhangxiang]
    //  [18/10/2012 zhangxiang]
	void sgAnimation::removeAnimaJoint(const std::string &aName){
		AnimationJointMap::iterator it = m_AnimaJointMap.find(aName);
		if(it != m_AnimaJointMap.end())
        {
            sgObject::destroyObject(it->second);
            m_AnimaJointMap.erase(it);
		}
	}
    
    Real sgAnimation::getStartTime(void) const
    {
        if(m_AnimaJointMap.empty())
            return 0.0f;
        return m_AnimaJointMap.begin()->second->getStartTime();
    }
    
    Real sgAnimation::getEndTime(void) const
    {
        if(m_AnimaJointMap.empty())
            return 0.0f;
        return m_AnimaJointMap.begin()->second->getEndTime();
    }
    
    Real sgAnimation::getDuaration(void) const
    {
        return getEndTime() - getStartTime();
    }
    
} // namespace Sagitta
