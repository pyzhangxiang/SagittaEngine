//////////////////////////////////////////////////////
// file: sgAnimation.cpp 
// created by zhangxiang on 09-01-10
// define of the class sgAnimation
// sgAnimation is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgAnimation.h"
#include "sgAnimaJoint.h"
#include "../../common/utils/sgStringUtil.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	sgAnimation::sgAnimation(sgAnimationDelegate *aDelegate /* = new sgAnimationDelegate */) :
	sgResource(aDelegate), m_iMaxFrameNum(0), m_fFrameTime(0.0333333){
		
	}

	//  [1/11/2009 zhangxiang]
	sgAnimation::sgAnimation(const StdString &aName, sgAnimationDelegate *aDelegate /* = new sgAnimationDelegate */) :
	sgResource(aName, aDelegate), m_iMaxFrameNum(0), m_fFrameTime(0.0333333){

	}

	//  [1/11/2009 zhangxiang]
	sgAnimation::~sgAnimation(void){
		unLoadImpl();
	}

	//  [1/11/2009 zhangxiang]
	void sgAnimation::unLoadImpl(void){
		AnimaJointMap::iterator it = m_AnimaJointMap.begin();
		AnimaJointMap::iterator eit = m_AnimaJointMap.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_AnimaJointMap.clear();
		m_NamedAnimaJointMap.clear();
	}

	//  [1/11/2009 zhangxiang]
	size_t sgAnimation::maxFrameNum(void) const{
		return m_iMaxFrameNum;
	}

	//  [1/11/2009 zhangxiang]
	Real sgAnimation::frameTime(void) const{
		return m_fFrameTime;
	}

	//  [1/11/2009 zhangxiang]
	void sgAnimation::setFrameTime(Real aFrameTime){
		m_fFrameTime = aFrameTime;
	}

	//  [1/11/2009 zhangxiang]
	sgAnimaJoint *sgAnimation::createAnimaJoint(size_t aFrameNum){
		sgAnimaJoint *ret = new sgAnimaJoint(aFrameNum);
	//	m_AnimaJointMap.insert(std::make_pair(ret->getDjID(), ret));
		addAnimaJoint(ret);
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	sgAnimaJoint *sgAnimation::createAnimaJoint(const StdString &aName, size_t aFrameNum){
		sgAnimaJoint *ret = new sgAnimaJoint(aName, aFrameNum);
	//	m_AnimaJointMap.insert(std::make_pair(ret->getDjID(), ret));
		addAnimaJoint(ret);
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	sgAnimaJoint *sgAnimation::findAnimaJoint(uLong aID) const{
		AnimaJointMap::const_iterator it = m_AnimaJointMap.find(aID);
		if(it == m_AnimaJointMap.end()){
			return 0;
		}else{
			return it->second;
		}
	}

	//  [1/11/2009 zhangxiang]
	sgAnimaJoint *sgAnimation::findAnimaJoint(const StdString &aName) const{
		NamedAnimaJointMap::const_iterator it = m_NamedAnimaJointMap.find(aName);
		if(it == m_NamedAnimaJointMap.end()){
			return 0;
		}else{
			return it->second;
		}
	}

	//  [1/11/2009 zhangxiang]
	void sgAnimation::addAnimaJoint(sgAnimaJoint *aJoint){
		if(!aJoint){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null AnimaJoint pointer.", "sgAnimation::addAnimaJoint");
		}

		if(m_AnimaJointMap.insert(std::make_pair(aJoint->getDjID(), aJoint)).second){
			m_NamedAnimaJointMap.insert(std::make_pair(aJoint->name(), aJoint));

			if(m_iMaxFrameNum < aJoint->frameNum()){
				m_iMaxFrameNum = aJoint->frameNum();
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	sgAnimaJoint *sgAnimation::removeAnimaJoint(uLong aID){
		AnimaJointMap::iterator it = m_AnimaJointMap.find(aID);
		if(it == m_AnimaJointMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_ITEM_NOT_FOUND,
				"Didn't find AnimaJoint by id " + sgStringUtil::to_string(aID),
				"sgAnimation::removeAnimaJoint");
		}

		sgAnimaJoint *ret = it->second;
		m_AnimaJointMap.erase(it);
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	void sgAnimation::removeAnimaJoint(sgAnimaJoint *aJoint){
		if(!aJoint){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null AnimaJoint pointer.", "sgAnimation::removeAnimaJoint");
		}

		removeAnimaJoint(aJoint->getDjID());
	}

} // namespace Sagitta