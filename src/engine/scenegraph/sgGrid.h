//////////////////////////////////////////////////////
// file: sgGrid.h @ 2008-8-2 by Zhang Xiang
// declares of the class sgGrid
// sgGrid is a class ...
//////////////////////////////////////////////////////
#ifndef __SGGRID_H__
#define __SGGRID_H__

// INCLUDES //////////////////////////////////////////
#include "sgEntity.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgGrid : public sgEntity{
	// member variables

	// constructors & destructor
	public:
		sgGrid(uInt aiLengthPerUnit, uInt aiHUnitNum, uInt aiVUnitNum, const Color &arColor = Color::WHITE);
		virtual ~sgGrid(void);

		// member functions

	}; //#### end class sgGrid

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGGRID_H__

