//////////////////////////////////////////////////////
// file: SPaintContext.h @ 2008-12-14 by Zhang Xiang
// declares of the class SPaintContext
// SPaintContext is a class ...
//////////////////////////////////////////////////////
#ifndef __SPAINTCONTEXT_H__
#define __SPAINTCONTEXT_H__

// INCLUDES //////////////////////////////////////////
#include "SContext.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SPaintContext : public SContext{
	// member variables
	protected:
		SPaintPortHandle m_ps;

	// constructors & destructor
	public:
		SPaintContext(SWindow *aWindow);
		~SPaintContext(void);

	// member functions

	}; //#### end class SPaintContext

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SPAINTCONTEXT_H__