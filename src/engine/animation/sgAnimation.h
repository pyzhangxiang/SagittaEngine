//////////////////////////////////////////////////////
// file: sgAnimation.h
// created by zhangxiang on 09-01-10
// declare of the class sgAnimation
// sgAnimation is a class ...
//////////////////////////////////////////////////////
#ifndef __SGANIMATION_H__
#define __SGANIMATION_H__

// INCLUDES //////////////////////////////////////////
#include "../resource/sgResource.h"
#include "../resource/sgAnimationDelegate.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgAnimaJoint;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgAnimation : public sgResource{
	// type defines
		/// mapped by AnimaJoint id
		typedef std::map<uLong, sgAnimaJoint*> AnimaJointMap;
		/// mapped by AnimaJoint name
		typedef std::multimap<StdString, sgAnimaJoint*> NamedAnimaJointMap;

	// member variables
	private:
		/// maximum frame num
		size_t m_iMaxFrameNum;

		/// frame time - default 0.0333333
		Real m_fFrameTime;

		/// AnimaJoint mapped by id
		AnimaJointMap m_AnimaJointMap;

		/// AnimaJoint mapped by name
		NamedAnimaJointMap m_NamedAnimaJointMap;

	// constructors & destructor
	public:
		sgAnimation(sgAnimationDelegate *aDelegate = new sgAnimationDelegate);
		sgAnimation(const StdString &aName, sgAnimationDelegate *aDelegate = new sgAnimationDelegate);
		~sgAnimation(void);

	// member functions
	private:
		/** Internal hook does unloading. Overrides from sgResource. */
		void unLoadImpl(void);

	public:
		/** Gets the maximum frame num. */
		size_t maxFrameNum(void) const;

		/** Gets frame time. */
		Real frameTime(void) const;
		/** Sets frame time. */
		void setFrameTime(Real aFrameTime);

		/** Creates a DataJoint. */
		sgAnimaJoint *createAnimaJoint(size_t aFrameNum);
		/** Creates a DataJoint by name. */
		sgAnimaJoint *createAnimaJoint(const StdString &aName, size_t aFrameNum);

		/** Finds a AnimaJoint by id.
			@return If not found, return 0.
		*/
		sgAnimaJoint *findAnimaJoint(uLong aID) const;

		/** Finds AnimaJoint by name.
			@remarks Find in multimap, only return the first one if found.
			@return If not found, return 0;
		*/
		sgAnimaJoint *findAnimaJoint(const StdString &aName) const;
		
		/** Adds a AnimaJoint to this Animation. */
		void addAnimaJoint(sgAnimaJoint *aJoint);

		/** Remove a AnimaJoint by id. */
		sgAnimaJoint *removeAnimaJoint(uLong aID);
		/** Remvoe a DataJoint. */
		void removeAnimaJoint(sgAnimaJoint *aJoint);

	}; //#### end class sgAnimation

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGANIMATION_H__
