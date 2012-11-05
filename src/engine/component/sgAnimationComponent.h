/// @2012.9.27 by zhangxiang

#ifndef __SGANIMATIONCOMPONENT_H__
#define __SGANIMATIONCOMPONENT_H__


#include "sgComponent.h"
#include "engine/common/sgStrHandle.h"


namespace Sagitta{

	class sgAnimation;

    /// skeleton animation controller
	class _SG_KernelExport sgAnimationComponent : public sgComponent
	{
		SG_META_DECLARE(sgAnimationComponent)
        
    public:
        enum PLAY_MODE
        {
            PM_STOP_END,
            PM_STOP_START,
            PM_LOOP
        };
        
        enum PLAY_STATE
        {
            PS_PLAYING,
            PS_PAUSE
        };

	protected:
		sgStrHandle mAnimationFile;
        PLAY_MODE mPlayMode;
        PLAY_STATE mPlayState;
        Real mCurrentTime;

	public:
		sgAnimationComponent(void);
		virtual ~sgAnimationComponent(void);

		sgStrHandle getAnimationFile(void) const;
		void setAnimationFile(sgStrHandle aFile);
		sgAnimation *getAnimation(void) const;
        
        Real getCurrentTime(void) const;
        PLAY_MODE getPlayMode(void) const;
        PLAY_STATE getPlayState(void) const;

		void setCurrentTime(Real time);
		void setPlayMode(PLAY_MODE pm);
		void play(void);
		void play(Real time);
		void pause(void);
		void stop(void);
		/// the position at stop is set according to the stop mode(PM_STOP_START or PM_STOP_END)
		void stop2(void);
        
        virtual void update(Float32 deltaTime);
        void updateManually(Float32 deltaTime);

	}; 

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGANIMATIONCOMPONENT_H__
