//////////////////////////////////////////////////////
// file: SSimpleEntities.h @ 2008-8-10 by Zhang Xiang
// declares of some simple entities class
// ...
//////////////////////////////////////////////////////
#ifndef __SGSIMPLEENTITIES_H__
#define __SGSIMPLEENTITIES_H__

// INCLUDES //////////////////////////////////////////
#include "sgEntity.h"

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
	class _SG_KernelExport sgCube : public sgEntity{
	// member variables
	private:
		Real m_fEdgeLength;

	// constructors & destructor
	public:
		sgCube(Real aEdgeLength);
		sgCube(const StdString &asName, Real aEdgeLength);
		virtual ~sgCube(void);

	// member functions
	private:
		void init(void);
	public:
		Real edgeLength(void) const;

	}; //#### end class sgCube


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
	class _SG_KernelExport sgSphere : public sgEntity{
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
		sgSphere(Real aRadius, int aSlices, int aStacks);
		/** Constructor with specified name.
			@param
				aRadius the radius of this sphere
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude).
			@param
				aStacks the number of subdivisions along the Z axis(similar to lines of latitude).
		*/
		sgSphere(const StdString &asName, Real aRadius, int aSlices, int aStacks);
		virtual ~sgSphere(void);

	// member functions
	private:
		void init(void);
	public:
		Real radius(void) const;
		int slices(void) const;
		int stacks(void) const;

	}; //#### end class sgSphere


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
	class _SG_KernelExport sgCone : public sgEntity{
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
		sgCone(Real aRadius, Real aHeight, int aSlices);
		/** Constructor with specified name.
			@param
				aRadius the radius of the base of the cone
			@param
				aHeight the height of the cone
			@param
				aSlices the number of subdivisions around the Z axis(similar to lines of longitude)
		*/
		sgCone(const StdString &asName, Real aRadius, Real aHeight, int aSlices);
		virtual ~sgCone(void);

	// member functions
	private:
		void init(void);

	}; //#### end class sgCone

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGSIMPLEENTITIES_H__