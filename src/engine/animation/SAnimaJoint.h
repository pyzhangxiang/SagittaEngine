//////////////////////////////////////////////////////
// file: SAnimaJoint.h 
// created by zhangxiang on 09-01-10
// declare of the class SAnimaJoint
// SAnimaJoint is a class ...
//////////////////////////////////////////////////////
#ifndef __SANIMAJOINT_H__
#define __SANIMAJOINT_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"
#include "../../math/SMathHeader.h"
#include <vector>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SAnimaJoint : public SObject{
	// type defines
	protected:
		typedef std::vector<Vector3> Vector3List;
		typedef std::vector<Quaternion> QuaternionList;
	// static members
	private:
		static uLong ms_iDataJointCount;

	// member variables
	private:
		/// this DataJoint name
		StdString m_sName;

		/// this DataJoint id
		const uLong mc_iDjID;

		/// frame num
		size_t m_iFrameNum;

		/// is position data active, default - false
		bool m_bPosition;
		/// is orientation data active, default - false
		bool m_bOrientation;
		/// is scale data active, default - false
		bool m_bScale;

		/// transform space of position data denoted, default - TS_PARENT
		int m_iPositionTS;
		/// transform space of orientation data denoted, default - TS_LOCAL
		int m_iOrientationTS;

		/// relative position data
		Vector3List *m_pPositionList;
		/// relative quaternion data
		QuaternionList *m_pOrientationList;
		/// relative scale data
		Vector3List *m_pScaleList;

	// constructors & destructor
	public:
		SAnimaJoint(size_t aFrameNum);
		SAnimaJoint(const StdString &aName, size_t aFrameNum);
		~SAnimaJoint(void);

	// member functions
	public:
		/** Gets this resource name. */
		const StdString &name(void) const;
		/** Sets this resource name. */
		void setName(const StdString &aName);

		/** Gets this DataJoint id. */
		uLong getDjID(void) const;

		/** Gets frame num. */
		size_t frameNum(void) const;

		/** Checks is position data active. */
		bool isPositionActive(void) const;
		/** Sets the active property of position data. */
		void setPositionActive(bool aActive);

		/** Checks is orientation data active. */
		bool isOrientationActive(void) const;
		/** Sets the active property of orientation data. */
		void setOrientationActive(bool aActive);

		/** Checks is scale data active. */
		bool isScaleActive(void) const;
		/** Sets the active property of scale data. */
		void setScaleActive(bool aActive);

		/** Gets position transform space. */
		int getPositionTS(void) const;
		/** Sets position transform space. */
		void setPositionTS(int aTS);

		/** Gets orientation transform space. */
		int getOrientationTS(void) const;
		/** Sets orientation transform space. */
		void setOrientationTS(int aTS);

		const Vector3 &getPosition(size_t afr) const;
		const Quaternion &getOrientation(size_t afr) const;
		const Vector3 &getScale(size_t afr) const;
		void setPosition(const Vector3 &aRelativePosition, size_t afr);
		void setOrientation(const Quaternion &aRelativeOrientation, size_t afr);
		void setScale(const Vector3 &aRelativeScale, size_t afr);

		/** Inserts new frames into this DataJoint.
			@param
				aFrToInsert The position where the first element is inserted.
							Cannot greater than the original frame num.
				aNum The number of elements being inserted into the DataJoint
		*/
		void insertFrames(size_t aFrToInsert, size_t aNum);

		/** Clears this DataJoint.
			@remarks Makes frame num be zero.
		*/
		void clear(void);

		/** Reset frame num for this DataJoint.
			@remarks All data will be reset.
		*/
		void reset(size_t aFrameNum);

	}; //#### end class SDataJoint

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SANIMAJOINT_H__