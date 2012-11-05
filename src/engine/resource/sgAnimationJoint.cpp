
#include "sgAnimationJoint.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgException.h"


namespace Sagitta{
    
    SG_META_DEFINE(sgAnimationJoint, sgObject)

	//  [1/10/2009 zhangxiang]
    //  [18/10/2012 zhangxiang]
	sgAnimationJoint::sgAnimationJoint(void) 
	: mKeyNum(0), mPositionActive(false)
	, mOrientationActive(false)
	, mScaleList(false)
    {
	
	}

	//  [1/10/2009 zhangxiang]
	sgAnimationJoint::~sgAnimationJoint(void)
    {
	}

	//  [1/11/2009 zhangxiang]
	size_t sgAnimationJoint::getKeyNum(void) const{
		return mKeyNum;
	}
    
    //  [19/10/2012 zhangxiang]
    Real sgAnimationJoint::getKeyT(size_t afr) const
    {
        if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
                              "Out of range.", "sgAnimationJoint::getKeyT");
		}
        
		return mTList[afr];
    }


	//  [1/10/2009 zhangxiang]
	const Vector3 &sgAnimationJoint::getKeyPosition(size_t afr) const{
		if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "sgAnimationJoint::getKeyPosition");
		}

		return mPositionList[afr];
	}

	//  [1/10/2009 zhangxiang]
	const Quaternion &sgAnimationJoint::getKeyOrientation(size_t afr) const{
		if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "sgAnimationJoint::getKeyOrientation");
		}

		return mOrientationList[afr];
	}

	//  [1/10/2009 zhangxiang]
	const Vector3 &sgAnimationJoint::getKeyScale(size_t afr) const{
		if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "sgAnimationJoint::getKeyScale");
		}

		return mScaleList[afr];
	}
    
    //  [19/10/2012 zhangxiang]
    void sgAnimationJoint::setKeyT(Real t, size_t afr)
    {
        if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
                              "Out of range.", "sgAnimationJoint::setKeyT");
		}
        
        if(mKeyNum == 1)
        {
            mTList[0] = t;
            return ;
        }
        
        if(afr == mKeyNum - 1)
        {
            if(t <= mTList[afr-1])
            {
                THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
                                  "t list must be ascending.", "sgAnimationJoint::setKeyT");
            }
        }
        else if(afr == 0)
        {
            if(t >= mTList[1])
            {
                THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
                                  "t list must be ascending.", "sgAnimationJoint::setKeyT");
            }
        }
        else
        {
            if(t >= mTList[afr+1]
               || t <= mTList[afr-1])
            {
                THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
                                  "t list must be ascending.", "sgAnimationJoint::setKeyT");
            }
        }
        
		mTList[afr] = t;
    }

	//  [1/10/2009 zhangxiang]
	void sgAnimationJoint::setKeyPosition(const Vector3 &aRelativePosition, size_t afr){
		if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "sgAnimationJoint::setKeyPosition");
		}

		mPositionList[afr] = aRelativePosition;
	}

	//  [1/10/2009 zhangxiang]
	void sgAnimationJoint::setKeyOrientation(const Quaternion &aRelativeOrientation, size_t afr){
		if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "sgAnimationJoint::setKeyOrientation");
		}

		mOrientationList[afr] = aRelativeOrientation;
	}

	//  [1/10/2009 zhangxiang]
	void sgAnimationJoint::setKeyScale(const Vector3 &aRelativeScale, size_t afr){
		if(afr >= mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
				"Out of range.", "sgAnimationJoint::setKeyScale");
		}

		mScaleList[afr] = aRelativeScale;
	}
    
    //  [19/10/2012 zhangxiang]
    Vector3 sgAnimationJoint::getPosition(Real t) const
    {
        size_t i = 0;
        size_t left = 0;
        size_t right = 1;
        for ( i = 2; i < mKeyNum; ++i )
        {
            if ( t < mTList[i-1] )
            {
                left = i - 1;
                right = i;
                break;
            }
            
        }
        if(i == mKeyNum)
        {
            left = mKeyNum - 1;
            right = mKeyNum;
        }
        
        Vector3 ypval = ( mPositionList[right-1] - mPositionList[left-1] )
		/ ( mTList[right-1] - mTList[left-1] );
        
        return mPositionList[left-1] + ypval * ( t - mTList[left-1] );
        
    }
    
    Quaternion sgAnimationJoint::getOrientation(Real t) const
    {
        size_t i = 0;
        size_t left = 0;
        size_t right = 1;
        for ( i = 2; i < mKeyNum; ++i )
        {
            if ( t < mTList[i-1] )
            {
                left = i - 1;
                right = i;
                break;
            }
            
        }
        if(i == mKeyNum)
        {
            left = mKeyNum - 1;
            right = mKeyNum;
        }
        
        return Quaternion::Slerp(t - mTList[left-1], mOrientationList[left-1], mOrientationList[right-1]);
    }
    
    Vector3 sgAnimationJoint::getScale(Real t) const
    {
        size_t i = 0;
        size_t left = 0;
        size_t right = 1;
        for ( i = 2; i < mKeyNum; ++i )
        {
            if ( t < mTList[i-1] )
            {
                left = i - 1;
                right = i;
                break;
            }
            
        }
        if(i == mKeyNum)
        {
            left = mKeyNum - 1;
            right = mKeyNum;
        }
        
        Vector3 ypval = ( mScaleList[right-1] - mScaleList[left-1] )
		/ ( mTList[right-1] - mTList[left-1] );
        
        return mScaleList[left-1] + ypval * ( t - mTList[left-1] );
    }

	//  [1/10/2009 zhangxiang]
	void sgAnimationJoint::insertKeys(size_t aFrToInsert, size_t aNum){
        if(aNum == 0)
            return ;
        
        if(aFrToInsert > mKeyNum){
			THROW_SAGI_EXCEPT(sgException::ERR_RT_ASSERTION_FAILED,
                              "Out of range.", "sgAnimationJoint::insertKeys");
		}
        
        Vector3List::iterator pit = mPositionList.begin();
		mPositionList.insert(pit + aFrToInsert, aNum, Vector3::ZERO);
		
        QuaternionList::iterator qit = mOrientationList.begin();
		mOrientationList.insert(qit + aFrToInsert, aNum, Quaternion::IDENTITY);
		
        Vector3List::iterator sit = mScaleList.begin();
		mScaleList.insert(sit + aFrToInsert, aNum, Vector3::UNIT_SCALE);
        
        RealList::iterator tit = mTList.begin();
		mTList.insert(tit + aFrToInsert, aNum, 0.0f);
        if(aFrToInsert == 0)
        {
            for(size_t i=0; i<aNum; ++i)
            {
                mTList[i] = i;
            }
            for(size_t i=aNum; i<mTList.size(); ++i)
            {
                mTList[i] = aNum + mTList[i];
            }
        }
        else if(aFrToInsert == mKeyNum)
        {
            for(size_t i=mKeyNum; i<mTList.size(); ++i)
            {
                mTList[i] = mTList[i-1] + 1.0f;
            }
        }
        else
        {
            Real delta = (mTList[aFrToInsert+aNum] - mTList[aFrToInsert-1]) / aNum;
            for(size_t i=aFrToInsert; i<aFrToInsert+aNum; ++i)
            {
                mTList[i] = mTList[i-1] + delta;
            }
        }
        
        mKeyNum += aNum;
		
	}

	//  [1/10/2009 zhangxiang]
	void sgAnimationJoint::clear(void){
        mTList.clear();
        mPositionList.clear();
        mOrientationList.clear();
        mScaleList.clear();
        mKeyNum = 0;
	}

	//  [1/10/2009 zhangxiang]
	void sgAnimationJoint::reset(size_t aKeyNum){
		mKeyNum = aKeyNum;
		mPositionList.resize(mKeyNum, Vector3::ZERO);
		mOrientationList.resize(mKeyNum, Quaternion::IDENTITY);
		mScaleList.resize(mKeyNum, Vector3::UNIT_SCALE);
        mTList.resize(mKeyNum, 0.0f);
        for(size_t i=0; i<mTList.size(); ++i)
        {
            mTList[i] = i;
        }
	}
    
    Real sgAnimationJoint::getStartTime(void) const
    {
        if(mTList.empty())
            return 0.0f;
        return mTList[0];
    }
    
    Real sgAnimationJoint::getEndTime(void) const
    {
        if(mTList.empty())
            return 0.0f;
        return mTList[mTList.size()-1];
    }
    
    Real sgAnimationJoint::getDuaration(void) const
    {
        return getEndTime() - getStartTime();
    }

	bool sgAnimationJoint::isPositionActive( void ) const
	{
		return mPositionActive;
	}

	bool sgAnimationJoint::isOrientationActive( void ) const
	{
		return mOrientationActive;
	}

	bool sgAnimationJoint::isScaleActive( void ) const
	{
		return mScaleActive;
	}

	void sgAnimationJoint::setPositionActive( bool active )
	{
		mPositionActive = active;
	}

	void sgAnimationJoint::setOrientationActive( bool active )
	{
		mOrientationActive = active;
	}

	void sgAnimationJoint::setScaleActive( bool active )
	{
		mScaleActive = active;
	}

} // namespace Sagitta
