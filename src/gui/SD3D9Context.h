//////////////////////////////////////////////////////
// file: SD3D9Context.h @ 2008-12-13 by Zhang Xiang
// declares of the class SD3D9Context
// SD3D9Context is a class ...
//////////////////////////////////////////////////////
#ifndef __SD3D9CONTEXT_H__
#define __SD3D9CONTEXT_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderContext.h"
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32

#include <d3dx9.h>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SWindow;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _GuiExport SD3D9Context : public SRenderContext{
	// member variables
	private:
		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pDevice;

	// constructors & destructor
	private:
		SD3D9Context(const SD3D9Context&);
		SD3D9Context &operator=(const SD3D9Context&);
	public:
		SD3D9Context(SWindow *aWindow);
		~SD3D9Context(void);

	// member functions
	private:
		/** Overrides from SRenderContext. */
		void buildRenderEnv(void);

		/** Overrides from SRenderContext. */
		void disposeRenderEnv(void);

	}; //#### end class SD3D9Context

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////
#endif

#endif // __SD3D9CONTEXT_H__
