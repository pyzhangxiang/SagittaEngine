//////////////////////////////////////////////////////
// file: SController.h @ 2008-12-7 by Zhang Xiang
// declares of the class SController
// SController is a class ...
//////////////////////////////////////////////////////
#ifndef __SCONTROLLER_H__
#define __SCONTROLLER_H__

// INCLUDES //////////////////////////////////////////
#include "../common/SDelegate.h"
#include "GuiDef.h"
#include <string>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** The role of controller of the pattern MVC
	@remarks

	@note

	@par

	*/
	class _GuiExport SController : public SDelegate{
	// member variables

	// constructors & destructor
	public:
		virtual ~SController(void) = 0;

	// member functions
	public:
		virtual void _action(const std::string &_aSlot, const SController *aSender){};

	}; //#### end class SController

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SCONTROLLER_H__