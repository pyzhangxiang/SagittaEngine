
#include "sgAnimationComponent.h"
#include "engine/resource/sgResourceCenter.h"
#include "engine/resource/sgAnimation.h"


namespace Sagitta
{
	SG_META_DEFINE(sgAnimationComponent, sgComponent)

	sgAnimationComponent::sgAnimationComponent( void )
    : mAnimationFile(sgStrHandle::EmptyString)
    , mPlayMode(PM_STOP_END)
    , mPlayState(PS_PAUSE)
    , mCurrentTime(0.0f)
	{

	}

	sgAnimationComponent::~sgAnimationComponent( void )
	{

	}

	sgStrHandle sgAnimationComponent::getAnimationFile( void ) const
	{
		return mAnimationFile;
	}

	void sgAnimationComponent::setAnimationFile( sgStrHandle aFile )
	{
		mAnimationFile = aFile;
	}

	sgAnimation * sgAnimationComponent::getAnimation( void ) const
	{
		return dynamic_cast<sgAnimation*>(sgResourceCenter::instance()->findResource(mAnimationFile));
	}
    
    Real sgAnimationComponent::getCurrentTime(void) const
    {
        return mCurrentTime;
    }
    
    sgAnimationComponent::PLAY_MODE sgAnimationComponent::getPlayMode(void) const
    {
        return mPlayMode;
    }
    
    sgAnimationComponent::PLAY_STATE sgAnimationComponent::getPlayState(void) const
    {
        return mPlayState;
    }
    
    void sgAnimationComponent::updateManually(Float32 deltaTime)
    {
        sgAnimation *pAnimation = dynamic_cast<sgAnimation*>(sgResourceCenter::instance()->findResource(mAnimationFile));
        if(!pAnimation)
            return ;
        
        Real startTime = pAnimation->getStartTime();
        Real endTime = pAnimation->getEndTime();
        mCurrentTime += deltaTime;
        if(mCurrentTime > endTime)
        {
            if(mPlayMode == PM_STOP_END)
            {
                mCurrentTime = endTime;
                mPlayState = PS_PAUSE;
            }
            else if(mPlayMode == PM_STOP_START)
            {
                mCurrentTime = startTime;
                mPlayState = PS_PAUSE;
            }
            else
            {
                // loop
                mCurrentTime = startTime;
            }
        }
        else if(mCurrentTime < startTime)
        {
            mCurrentTime = startTime;
        }
    }

    void sgAnimationComponent::update(Float32 deltaTime)
    {
        if(mPlayState == PS_PAUSE)
            return ;
        
        sgAnimation *pAnimation = dynamic_cast<sgAnimation*>(sgResourceCenter::instance()->findResource(mAnimationFile));
        if(!pAnimation)
            return ;
        
        updateManually(deltaTime);
        
    }

	void sgAnimationComponent::setCurrentTime( Real time )
	{
		mCurrentTime = time;
	}

	void sgAnimationComponent::setPlayMode( PLAY_MODE pm )
	{
		mPlayMode = pm;
	}

	void sgAnimationComponent::play( void )
	{
		mPlayState = PS_PLAYING;
	}

	void sgAnimationComponent::play( Real time )
	{
		mCurrentTime = time;
		mPlayState = PS_PLAYING;
	}

	void sgAnimationComponent::pause( void )
	{
		mPlayState = PS_PAUSE;
	}

	void sgAnimationComponent::stop( void )
	{
		sgAnimation *pAnimation = getAnimation();
		if(pAnimation)
		{
			mCurrentTime = pAnimation->getStartTime();
		}
		mPlayState = PS_PAUSE;
	}

	void sgAnimationComponent::stop2( void )
	{
		sgAnimation *pAnimation = dynamic_cast<sgAnimation*>(sgResourceCenter::instance()->findResource(mAnimationFile));
		if(pAnimation)
		{
			if(mPlayMode == PM_STOP_END)
			{
				mCurrentTime = pAnimation->getEndTime();;
			}
			else
			{
				mCurrentTime = pAnimation->getStartTime();;
				mPlayState = PS_PAUSE;
			}
		}

		mPlayState = PS_PAUSE;		
	}
}
