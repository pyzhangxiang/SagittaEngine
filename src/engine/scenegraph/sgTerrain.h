//////////////////////////////////////////////////////
// file: sgTerrain.h @ 2008-1-31 by Zhang Xiang
// declares of the class sgTerrain
// sgTerrain is a class ...
//////////////////////////////////////////////////////
#ifndef __SGTERRAIN_H__
#define __SGTERRAIN_H__

// INCLUDES //////////////////////////////////////////
#include "sgRenderable.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgTerrain : public sgRenderable{
	// member variables

	// constructors & destructor
	public:
		sgTerrain(void);
		~sgTerrain(void);

	// member functions
	public:
		void computeCenter(void);
		void computeRadius(void);

	public:

	}; //#### end class sgTerrain

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGTERRAIN_H__

