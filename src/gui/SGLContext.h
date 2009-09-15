//////////////////////////////////////////////////////
// file: SGLContext.h @ 2008-12-13 by Zhang Xiang
// declares of the class SGLContext
// SGLContext is a class ...
//////////////////////////////////////////////////////
#ifndef __SGLCONTEXT_H__
#define __SGLCONTEXT_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderContext.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SWindow;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SGLContext : public SRenderContext{
	// member variables
	private:
		SGLContextHandle m_GLContextHandle;

	// constructors & destructor
	private:
		SGLContext(const SGLContext&);
		SGLContext &operator=(const SGLContext&);
	public:
		SGLContext(SWindow *aWindow);
		~SGLContext(void);

	// member functions
	private:
		/** Overrides from SRenderContext. */
		void buildRenderEnv(void);

		/** Overrides from SRenderContext. */
		void disposeRenderEnv(void);
		
		/** Overrides from SRenderContext. */
		void swapBuffers(void);
		
		/** Updates the context. */
		void update(void);

	}; //#### end class SGLContext

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGLCONTEXT_H__