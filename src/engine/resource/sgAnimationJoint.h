
//  [18/10/2012 zhangxiang]

#ifndef __SGANIMATIONJOINT_H__
#define __SGANIMATIONJOINT_H__

#include "math/SagittaPlatform.h"
#include "math/sgMathHeader.h"
#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"
#include <vector>


namespace Sagitta{

	class _SG_KernelExport sgAnimationJoint : public sgObject
    {
        SG_META_DECLARE(sgAnimationJoint)
        
	protected:
        typedef sg_vector(Real) RealList;
		typedef sg_vector(Vector3) Vector3List;
		typedef sg_vector(Quaternion) QuaternionList;

	private:


		/// frame num
		size_t mKeyNum;

        // must ascending
        RealList mTList;
		/// relative position data
		Vector3List mPositionList;
		/// relative quaternion data
		QuaternionList mOrientationList;
		/// relative scale data
		Vector3List mScaleList;

		bool mPositionActive;
		bool mOrientationActive;
		bool mScaleActive;

	// constructors & destructor
	public:
        sgAnimationJoint(void);
		~sgAnimationJoint(void);

	// member functions
	public:

		size_t getKeyNum(void) const;

        Real getKeyT(size_t afr) const;
		const Vector3 &getKeyPosition(size_t afr) const;
		const Quaternion &getKeyOrientation(size_t afr) const;
		const Vector3 &getKeyScale(size_t afr) const;
        void setKeyT(Real t, size_t afr);
		void setKeyPosition(const Vector3 &aRelativePosition, size_t afr);
		void setKeyOrientation(const Quaternion &aRelativeOrientation, size_t afr);
		void setKeyScale(const Vector3 &aRelativeScale, size_t afr);
        
        Vector3 getPosition(Real t) const;
		Quaternion getOrientation(Real t) const;
		Vector3 getScale(Real t) const;

		bool isPositionActive(void) const;
		bool isOrientationActive(void) const;
		bool isScaleActive(void) const;

		void setPositionActive(bool active);
		void setOrientationActive(bool active);
		void setScaleActive(bool active);

		/** Inserts new frames into this DataJoint.
			@param
				aFrToInsert The position where the first element is inserted.
							Cannot greater than the original frame num.
				aNum The number of elements being inserted into the DataJoint
		*/
		void insertKeys(size_t aFrToInsert, size_t aNum);

		/** Clears this DataJoint.
			@remarks Makes frame num be zero.
		*/
		void clear(void);

		/** Reset frame num for this DataJoint.
			@remarks All data will be reset.
		*/
		void reset(size_t aKeyNum);
        
        Real getStartTime(void) const;
        Real getEndTime(void) const;
        Real getDuaration(void) const;

	}; //#### end class SDataJoint

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGANIMATIONJOINT_H__
