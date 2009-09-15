//////////////////////////////////////////////////////
// file: SSimpleEntities.h @ 2008-8-10 by Zhang Xiang
// declares of some simple entities class
// ...
//////////////////////////////////////////////////////
#ifndef __SSIMPLEENTITIES_H__
#define __SSIMPLEENTITIES_H__

// INCLUDES //////////////////////////////////////////
#include "SEntity.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks
		4-----5
	   /|	 /|
	  / |	/ |
	 /  7--/--6
	3-----2  /
	| /   | /
	|/    |/
	0-----1
	@note

	@par

	*/
	class _SagittaExport SCube : public SEntity{
	// member variables
	private:
		Real m_fEdgeLength;

	// constructors & destructor
	public:
		SCube(Real aEdgeLength);
		SCube(const StdString &asName, Real aEdgeLength);
		virtual ~SCube(void);

	// member functions
	private:
		void init(void);
	public:
		Real edgeLength(void) const;

	}; //#### end class SCube


	/** class representation
	@remarks
		counterclockwise
			444
		   / | \
		  /  |  \
		 3 --1-- 2
		  \  |  /
		   \ | /
			000
		face: 0021, 0032, 0013, 1244, 2344, 3144
	@note

	@par

	*/
	class _SagittaExport SSphere : public SEntity{
	// member variables
	protected:
		Real m_fRadius;
		int m_iSlices;
		int m_iStacks;

	// constructors & destructor
	public:
		/** Constructor with auto-generated name.
			@param
				aRadius the radius of this sphere
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude).
			@param
				aStacks the number of subdivisions along the Z axis(similar to lines of latitude).
		*/
		SSphere(Real aRadius, int aSlices, int aStacks);
		/** Constructor with specified name.
			@param
				aRadius the radius of this sphere
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude).
			@param
				aStacks the number of subdivisions along the Z axis(similar to lines of latitude).
		*/
		SSphere(const StdString &asName, Real aRadius, int aSlices, int aStacks);
		virtual ~SSphere(void);

	// member functions
	private:
		void init(void);
	public:
		Real radius(void) const;
		int slices(void) const;
		int stacks(void) const;

	}; //#### end class SSphere


	/** class representation
	@remarks
		counterclockwise
		    4	
		  / | \
	     /  |  \
        /   |   \
	   1---0 3---2
		face: 021, 412, 032, 423, 013, 431

	@note

	@par

	*/
	class _SagittaExport SCone : public SEntity{
	// member variables
	private:
		Real m_fRadius;
		Real m_fHeight;
		int m_iSlices;

	// constructors & destructor
	public:
		/** Constructor with auto-generated name.
			@param
				aRadius the radius of the base of the cone
			@param
				aHeight the height of the cone
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude)
		*/
		SCone(Real aRadius, Real aHeight, int aSlices);
		/** Constructor with specified name.
			@param
				aRadius the radius of the base of the cone
			@param
				aHeight the height of the cone
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude)
		*/
		SCone(const StdString &asName, Real aRadius, Real aHeight, int aSlices);
		virtual ~SCone(void);

	// member functions
	private:
		void init(void);

	}; //#### end class SCone

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SSIMPLEENTITIES_H__