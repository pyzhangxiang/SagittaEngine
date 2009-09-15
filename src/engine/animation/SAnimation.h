//////////////////////////////////////////////////////
// file: SAnimation.h
// created by zhangxiang on 09-01-10
// declare of the class SAnimation
// SAnimation is a class ...
//////////////////////////////////////////////////////
#ifndef __SANIMATION_H__
#define __SANIMATION_H__

// INCLUDES //////////////////////////////////////////
#include "../resource/SResource.h"
#include "../resource/SAnimationDelegate.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SAnimaJoint;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SAnimation : public SResource{
	// type defines
		/// mapped by AnimaJoint id
		typedef std::map<uLong, SAnimaJoint*> AnimaJointMap;
		/// mapped by AnimaJoint name
		typedef std::multimap<StdString, SAnimaJoint*> NamedAnimaJointMap;

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
		SAnimation(SAnimationDelegate *aDelegate = new SAnimationDelegate);
		SAnimation(const StdString &aName, SAnimationDelegate *aDelegate = new SAnimationDelegate);
		~SAnimation(void);

	// member functions
	private:
		/** Internal hook does unloading. Overrides from SResource. */
		void unLoadImpl(void);

	public:
		/** Gets the maximum frame num. */
		size_t maxFrameNum(void) const;

		/** Gets frame time. */
		Real frameTime(void) const;
		/** Sets frame time. */
		void setFrameTime(Real aFrameTime);

		/** Creates a DataJoint. */
		SAnimaJoint *createAnimaJoint(size_t aFrameNum);
		/** Creates a DataJoint by name. */
		SAnimaJoint *createAnimaJoint(const StdString &aName, size_t aFrameNum);

		/** Finds a AnimaJoint by id.
			@return If not found, return 0.
		*/
		SAnimaJoint *findAnimaJoint(uLong aID) const;

		/** Finds AnimaJoint by name.
			@remarks Find in multimap, only return the first one if found.
			@return If not found, return 0;
		*/
		SAnimaJoint *findAnimaJoint(const StdString &aName) const;
		
		/** Adds a AnimaJoint to this Animation. */
		void addAnimaJoint(SAnimaJoint *aJoint);

		/** Remove a AnimaJoint by id. */
		SAnimaJoint *removeAnimaJoint(uLong aID);
		/** Remvoe a DataJoint. */
		void removeAnimaJoint(SAnimaJoint *aJoint);

	}; //#### end class SAnimation

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SANIMATION_H__