//////////////////////////////////////////////////////
// file: SGrid.h @ 2008-8-2 by Zhang Xiang
// declares of the class SGrid
// SGrid is a class ...
//////////////////////////////////////////////////////
#ifndef __SGRID_H__
#define __SGRID_H__

// INCLUDES //////////////////////////////////////////
#include "SEntity.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SGrid : public SEntity{
	// member variables

	// constructors & destructor
	public:
		SGrid(uInt aiLengthPerUnit, uInt aiHUnitNum, uInt aiVUnitNum, const Color &arColor = Color::WHITE);
		virtual ~SGrid(void);

		// member functions

	}; //#### end class SGrid

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SMENU_H__