//////////////////////////////////////////////////////
// file: SRenderContext.h @ 2008-12-13 by Zhang Xiang
// declares of the class SRenderContext
// SRenderContext is a class ...
//////////////////////////////////////////////////////
#ifndef __SRENDERCONTEXT_H__
#define __SRENDERCONTEXT_H__

// INCLUDES //////////////////////////////////////////
#include "SContext.h"
#include "GuiDef.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SWindow;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SRenderContext : public SContext{
	// member variables

	// constructors & destructor
	private:
		SRenderContext(const SRenderContext&);
		SRenderContext &operator=(const SRenderContext&);
	public:
		SRenderContext(SWindow *aWindow);
		virtual ~SRenderContext(void) = 0;

	// member functions
	public:
		/** Support for double buffers. */
		virtual void swapBuffers();

		/** Update the render context(for AGL, it's necessary). */
		virtual void update(void);

	}; //#### end class SRenderContext

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SRENDERCONTEXT_H__