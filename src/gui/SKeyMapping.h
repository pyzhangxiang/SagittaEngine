//////////////////////////////////////////////////////
// file: SKeyMapping.h @ 2008-12-15 by Zhang Xiang
// declares of the class SKeyMapping
// SKeyMapping is a class ...
//////////////////////////////////////////////////////
#ifndef __SKEYMAPPING_H__
#define __SKEYMAPPING_H__

// INCLUDES //////////////////////////////////////////
#include "GuiDef.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SKeyMapping{
	// static members
	public:
		static uLong SagiMouseButton(uLong aSysMB);
		static uLong CheckModifierKeys(void);
		static uLong SagiModifierKeys(uLong aSysMK);
		static uLong SagiKey(uLong aSysKeyCode);
		
	// member variables

	// constructors & destructor

	// member functions

	}; //#### end class SKeyMapping

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SKEYMAPPING_H__