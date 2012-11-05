//  [18/10/2012 zhangxiang]

#ifndef __SGANIMATION_H__
#define __SGANIMATION_H__

#include "sgResource.h"
#include "engine/common/sgStlAllocator.h"


namespace Sagitta
{
    class sgAnimationJoint;

	class _SG_KernelExport sgAnimation : public sgResource
	{
		SG_META_DECLARE(sgAnimation)
		
        // type defines
		/// mapped by AnimaJoint id
		typedef sg_map(std::string, sgAnimationJoint*) AnimationJointMap;
        
        // member variables
	private:
		/// maximum frame num
		size_t mkeyNum;
        
		/// frame time - default 0.0333333
//		Real m_fFrameTime;
        
        
		AnimationJointMap m_AnimaJointMap;
        
        
        // constructors & destructor
	public:
		sgAnimation(void);
		~sgAnimation(void);
    
        
	public:
		/** Gets the maximum frame num. */
		size_t getKeyNum(void) const;
        void setKeyNum(size_t num);
        
		/** Gets frame time. */
//		Real frameTime(void) const;
		/** Sets frame time. */
//		void setFrameTime(Real aFrameTime);
        
		sgAnimationJoint *createAnimaJoint(const std::string &aName);
		sgAnimationJoint *getAnimaJoint(const std::string &aName) const;
		void removeAnimaJoint(const std::string &aName);
        
        Real getStartTime(void) const;
        Real getEndTime(void) const;
        Real getDuaration(void) const;
		
	}; //#### end class sgAnimation

} // namespace Sagitta



#endif // __SGANIMATION_H__

