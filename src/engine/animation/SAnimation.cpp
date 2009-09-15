//////////////////////////////////////////////////////
// file: SAnimation.cpp 
// created by zhangxiang on 09-01-10
// define of the class SAnimation
// SAnimation is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SAnimation.h"
#include "SAnimaJoint.h"
#include "../../common/utils/SString.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/11/2009 zhangxiang]
	SAnimation::SAnimation(SAnimationDelegate *aDelegate /* = new SAnimationDelegate */) :
	SResource(aDelegate), m_iMaxFrameNum(0), m_fFrameTime(0.0333333){
		
	}

	//  [1/11/2009 zhangxiang]
	SAnimation::SAnimation(const StdString &aName, SAnimationDelegate *aDelegate /* = new SAnimationDelegate */) :
	SResource(aName, aDelegate), m_iMaxFrameNum(0), m_fFrameTime(0.0333333){

	}

	//  [1/11/2009 zhangxiang]
	SAnimation::~SAnimation(void){
		unLoadImpl();
	}

	//  [1/11/2009 zhangxiang]
	void SAnimation::unLoadImpl(void){
		AnimaJointMap::iterator it = m_AnimaJointMap.begin();
		AnimaJointMap::iterator eit = m_AnimaJointMap.end();
		for(; it!=eit; ++it){
			delete it->second;
		}
		m_AnimaJointMap.clear();
		m_NamedAnimaJointMap.clear();
	}

	//  [1/11/2009 zhangxiang]
	size_t SAnimation::maxFrameNum(void) const{
		return m_iMaxFrameNum;
	}

	//  [1/11/2009 zhangxiang]
	Real SAnimation::frameTime(void) const{
		return m_fFrameTime;
	}

	//  [1/11/2009 zhangxiang]
	void SAnimation::setFrameTime(Real aFrameTime){
		m_fFrameTime = aFrameTime;
	}

	//  [1/11/2009 zhangxiang]
	SAnimaJoint *SAnimation::createAnimaJoint(size_t aFrameNum){
		SAnimaJoint *ret = new SAnimaJoint(aFrameNum);
	//	m_AnimaJointMap.insert(std::make_pair(ret->getDjID(), ret));
		addAnimaJoint(ret);
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	SAnimaJoint *SAnimation::createAnimaJoint(const StdString &aName, size_t aFrameNum){
		SAnimaJoint *ret = new SAnimaJoint(aName, aFrameNum);
	//	m_AnimaJointMap.insert(std::make_pair(ret->getDjID(), ret));
		addAnimaJoint(ret);
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	SAnimaJoint *SAnimation::findAnimaJoint(uLong aID) const{
		AnimaJointMap::const_iterator it = m_AnimaJointMap.find(aID);
		if(it == m_AnimaJointMap.end()){
			return 0;
		}else{
			return it->second;
		}
	}

	//  [1/11/2009 zhangxiang]
	SAnimaJoint *SAnimation::findAnimaJoint(const StdString &aName) const{
		NamedAnimaJointMap::const_iterator it = m_NamedAnimaJointMap.find(aName);
		if(it == m_NamedAnimaJointMap.end()){
			return 0;
		}else{
			return it->second;
		}
	}

	//  [1/11/2009 zhangxiang]
	void SAnimation::addAnimaJoint(SAnimaJoint *aJoint){
		if(!aJoint){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null AnimaJoint pointer.", "SAnimation::addAnimaJoint");
		}

		if(m_AnimaJointMap.insert(std::make_pair(aJoint->getDjID(), aJoint)).second){
			m_NamedAnimaJointMap.insert(std::make_pair(aJoint->name(), aJoint));

			if(m_iMaxFrameNum < aJoint->frameNum()){
				m_iMaxFrameNum = aJoint->frameNum();
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	SAnimaJoint *SAnimation::removeAnimaJoint(uLong aID){
		AnimaJointMap::iterator it = m_AnimaJointMap.find(aID);
		if(it == m_AnimaJointMap.end()){
			THROW_SAGI_EXCEPT(SException::ERR_ITEM_NOT_FOUND,
				"Didn't find AnimaJoint by id " + SString::to_string(aID),
				"SAnimation::removeAnimaJoint");
		}

		SAnimaJoint *ret = it->second;
		m_AnimaJointMap.erase(it);
		return ret;
	}

	//  [1/11/2009 zhangxiang]
	void SAnimation::removeAnimaJoint(SAnimaJoint *aJoint){
		if(!aJoint){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Null AnimaJoint pointer.", "SAnimation::removeAnimaJoint");
		}

		removeAnimaJoint(aJoint->getDjID());
	}

} // namespace Sagitta