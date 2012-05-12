//////////////////////////////////////////////////////
// file: SContext.h @ 2008-12-13 by Zhang Xiang
// declares of the class SContext
// SContext is a class ...
//////////////////////////////////////////////////////
#ifndef __SCONTEXT_H__
#define __SCONTEXT_H__

// INCLUDES //////////////////////////////////////////
#include "../common/sgObject.h"
#include "GuiDef.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SWindow;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SContext{
	// member variables
	protected:
		SContextHandle m_ContextHandle;
		SWindow *m_pWindow;

		bool m_bReleaseSelf;

	// constructors & destructor
	private:
		SContext(const SContext&);
		SContext &operator=(const SContext&);
	public:
		SContext(void);
		/** This is a dangerous way to construct a context. */
		SContext(SContextHandle aContextHandle, SWindow *aWindow, bool bReleaseSelf = true);
		
		virtual ~SContext(void);

	// member functions
	public:
		void release(void);


	}; //#### end class SContext

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SCONTEXT_H__