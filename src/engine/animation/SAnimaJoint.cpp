//////////////////////////////////////////////////////
// file: SAnimaJoint.cpp 
// created by zhangxiang on 09-01-10
// define of the class SAnimaJoint
// SAnimaJoint is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SAnimaJoint.h"
#include "../scenegraph/SNode.h"
#include "../../common/utils/SString.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	uLong SAnimaJoint::ms_iDataJointCount = 0;

	//  [1/10/2009 zhangxiang]
	SAnimaJoint::SAnimaJoint(size_t aFrameNum) :
	mc_iDjID(ms_iDataJointCount), m_iFrameNum(aFrameNum),
	m_bPosition(false), m_bOrientation(false), m_bScale(false),
	m_iPositionTS(SNode::TS_PARENT), m_iOrientationTS(SNode::TS_LOCAL),
	m_pPositionList(0), m_pOrientationList(0), m_pScaleList(0){
	//	m_RelativePositionList.resize(m_iFrameNum, Vector3::ZERO);
	//	m_RelativeOrientationList.resize(m_iFrameNum, Quaternion::IDENTITY);
	//	m_RelativeScaleList.resize(m_iFrameNum, Vector3::UNIT_SCALE);

		m_sName = "DataJoint" + SString::to_string(ms_iDataJointCount++);
	}

	SAnimaJoint::SAnimaJoint(const StdString &aName, size_t aFrameNum) :
	m_sName(aName), mc_iDjID(ms_iDataJointCount++), m_iFrameNum(aFrameNum),
	m_bPosition(false), m_bOrientation(false), m_bScale(false),
	m_iPositionTS(SNode::TS_PARENT), m_iOrientationTS(SNode::TS_LOCAL),
	m_pPositionList(0), m_pOrientationList(0), m_pScaleList(0){
	//	m_RelativePositionList.resize(m_iFrameNum, Vector3::ZERO);
	//	m_RelativeOrientationList.resize(m_iFrameNum, Quaternion::IDENTITY);
	//	m_RelativeScaleList.resize(m_iFrameNum, Vector3::UNIT_SCALE);
	}

	//  [1/10/2009 zhangxiang]
	uLong SAnimaJoint::getDjID(void) const{
		return mc_iDjID;
	}

	//  [1/10/2009 zhangxiang]
	SAnimaJoint::~SAnimaJoint(){
		if(m_pPositionList)
			delete m_pPositionList;
		if(m_pOrientationList)
			delete m_pOrientationList;
		if(m_pScaleList)
			delete m_pScaleList;
	}

	//  [1/10/2009 zhangxiang]
	const StdString &SAnimaJoint::name(void) const{
		return m_sName;
	}

	//  [1/10/2009 zhangxiang]
	void SAnimaJoint::setName(const StdString &aName){
		m_sName = aName;
	}

	//  [1/11/2009 zhangxiang]
	size_t SAnimaJoint::frameNum(void) const{
		return m_iFrameNum;
	}

	//  [1/11/2009 zhangxiang]
	bool SAnimaJoint::isPositionActive(void) const{
		return m_bPosition;
	}

	//  [1/11/2009 zhangxiang]
	void SAnimaJoint::setPositionActive(bool aActive){
		if(m_bPosition != aActive){
			m_bPosition = aActive;
			if(m_bPosition){
				if(!m_pPositionList){
					m_pPositionList = new Vector3List(m_iFrameNum, Vector3::ZERO);
				}
			}else{
				if(m_pPositionList){
					delete m_pPositionList;
					m_pPositionList = 0;
				}
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	bool SAnimaJoint::isOrientationActive(void) const{
		return m_bOrientation;
	}

	//  [1/11/2009 zhangxiang]
	void SAnimaJoint::setOrientationActive(bool aActive){
		if(m_bOrientation != aActive){
			m_bOrientation = aActive;
			if(m_bOrientation){
				if(!m_pOrientationList){
					m_pOrientationList = new QuaternionList(m_iFrameNum, Quaternion::IDENTITY);
				}
			}else{
				if(m_pOrientationList){
					delete m_pOrientationList;
					m_pOrientationList = 0;
				}
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	bool SAnimaJoint::isScaleActive(void) const{
		return m_bScale;
	}

	//  [1/11/2009 zhangxiang]
	void SAnimaJoint::setScaleActive(bool aActive){
		if(m_bScale != aActive){
			m_bScale = aActive;
			if(m_bScale){
				if(!m_pScaleList){
					m_pScaleList = new Vector3List(m_iFrameNum, Vector3::UNIT_SCALE);
				}
			}else{
				if(m_pScaleList){
					delete m_pScaleList;
					m_pScaleList = 0;
				}
			}
		}
	}

	//  [1/11/2009 zhangxiang]
	int SAnimaJoint::getPositionTS(void) const{
		return m_iPositionTS;
	}

	//  [1/11/2009 zhangxiang]
	void SAnimaJoint::setPositionTS(int aTS){
		m_iPositionTS = aTS;
	}

	//  [1/11/2009 zhangxiang]
	int SAnimaJoint::getOrientationTS(void) const{
		return m_iOrientationTS;
	}

	//  [1/11/2009 zhangxiang]
	void SAnimaJoint::setOrientationTS(int aTS){
		m_iOrientationTS = aTS;
	}

	//  [1/10/2009 zhangxiang]
	const Vector3 &SAnimaJoint::getPosition(size_t afr) const{
		if(!m_pPositionList)
			return Vector3::ZERO;

		if(afr >= m_iFrameNum){
			THROW_SAGI_EXCEPT(SException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "SDataJoint::getPosition");
		}

		return (*m_pPositionList)[afr];
	}

	//  [1/10/2009 zhangxiang]
	const Quaternion &SAnimaJoint::getOrientation(size_t afr) const{
		if(!m_pOrientationList)
			return Quaternion::IDENTITY;

		if(afr >= m_iFrameNum){
			THROW_SAGI_EXCEPT(SException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "SDataJoint::getOrientation");
		}

		return (*m_pOrientationList)[afr];
	}

	//  [1/10/2009 zhangxiang]
	const Vector3 &SAnimaJoint::getScale(size_t afr) const{
		if(!m_pScaleList)
			return Vector3::UNIT_SCALE;

		if(afr >= m_iFrameNum){
			THROW_SAGI_EXCEPT(SException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "SDataJoint::getScale");
		}

		return (*m_pScaleList)[afr];
	}

	//  [1/10/2009 zhangxiang]
	void SAnimaJoint::setPosition(const Vector3 &aRelativePosition, size_t afr){
		if(!m_pPositionList)
			return ;

		if(afr >= m_iFrameNum){
			THROW_SAGI_EXCEPT(SException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "SDataJoint::setPosition");
		}

		(*m_pPositionList)[afr] = aRelativePosition;
	}

	//  [1/10/2009 zhangxiang]
	void SAnimaJoint::setOrientation(const Quaternion &aRelativeOrientation, size_t afr){
		if(!m_pOrientationList)
			return ;

		if(afr >= m_iFrameNum){
			THROW_SAGI_EXCEPT(SException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "SDataJoint::setOrientation");
		}

		(*m_pOrientationList)[afr] = aRelativeOrientation;
	}

	//  [1/10/2009 zhangxiang]
	void SAnimaJoint::setScale(const Vector3 &aRelativeScale, size_t afr){
		if(!m_pScaleList)
			return ;

		if(afr >= m_iFrameNum){
			THROW_SAGI_EXCEPT(SException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "SDataJoint::setScale");
		}

		(*m_pScaleList)[afr] = aRelativeScale;
	}

	//  [1/10/2009 zhangxiang]
	void SAnimaJoint::insertFrames(size_t aFrToInsert, size_t aNum){
		if(aFrToInsert > m_iFrameNum){
			THROW_SAGI_EXCEPT(SException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "SDataJoint::insertFrames");
		}

		if(m_pPositionList){
			std::vector<Vector3>::iterator pit = m_pPositionList->begin();
			m_pPositionList->insert(pit + aFrToInsert, aNum, Vector3::ZERO);
		}
		if(m_pOrientationList){
			std::vector<Quaternion>::iterator qit = m_pOrientationList->begin();
			m_pOrientationList->insert(qit + aFrToInsert, aNum, Quaternion::IDENTITY);
		}
		if(m_pScaleList){
			std::vector<Vector3>::iterator sit = m_pScaleList->begin();
			m_pScaleList->insert(sit + aFrToInsert, aNum, Vector3::UNIT_SCALE);
		}
	}

	//  [1/10/2009 zhangxiang]
	void SAnimaJoint::clear(void){
		if(m_pPositionList)
			m_pPositionList->clear();
		
		if(m_pOrientationList)
			m_pOrientationList->clear();

		if(m_pScaleList)
			m_pScaleList->clear();
	}

	//  [1/10/2009 zhangxiang]
	void SAnimaJoint::reset(size_t aFrameNum){
		m_iFrameNum = aFrameNum;

		if(m_pPositionList)
			m_pPositionList->resize(m_iFrameNum, Vector3::ZERO);

		if(m_pOrientationList)
			m_pOrientationList->resize(m_iFrameNum, Quaternion::IDENTITY);
		
		if(m_pScaleList)
			m_pScaleList->resize(m_iFrameNum, Vector3::UNIT_SCALE);
	}

} // namespace Sagitta