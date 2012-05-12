//////////////////////////////////////////////////////
// file: sgFrameBufferManager.h 
// created by zhangxiang on 09-01-15
// declare of the class sgFrameBufferManager
// sgFrameBufferManager is a class ...
//////////////////////////////////////////////////////
#ifndef __SGFRAMEBUFFERMANAGER_H__
#define __SGFRAMEBUFFERMANAGER_H__

// INCLUDES //////////////////////////////////////////
//#include "../../common/sgObject.h"
#include "../../common/configs/config.h"
#include "../../math/sgColor.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SColorBuffer;
	class SDepthBuffer;
	class SStencilBuffer;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgFrameBufferManager{// : public sgObject{
	// member variables
	private:
		/// frame buffers
		SColorBuffer *m_pColorBuffer;
		SDepthBuffer *m_pDepthBuffer;
		SStencilBuffer *m_pStencilBuffer;

	// constructors & destructor
	public:
		sgFrameBufferManager(void);
		sgFrameBufferManager(int aTWidth, int aTHeight, bool aEnableSetencil);
		sgFrameBufferManager(const sgFrameBufferManager &aParentBuffers,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~sgFrameBufferManager(void);

	// member functions
	public:
		/** Releases frame buffers. Internal method. */
		void _release(void);

		/** Gets slice frame buffers from another frame buffer manager. */
		void getSliceFromAnother(const sgFrameBufferManager &aParentBuffers,
								size_t aLeft, size_t aTop,
								size_t aWidth, size_t aHeight);

		/** Gets color buffer. */
		SColorBuffer *getColorBuffer(void) const;

		/** Gets depth buffer. */
		SDepthBuffer *getDepthBuffer(void) const;

		/** Gets stencil buffer. */
		SStencilBuffer *getStencilBuffer(void) const;

		/** Clears color buffer. */
		void clearColor(const Color::GLColor &aValue);

		/** Clears depth buffer. */
		void clearDepth(Real aValue);

		/** Clears stencil buffer. */
		void clearStencil(int aValue);

		/** Resize frame buffers.
			@remarks
				Only called by SagiRenderer::doSthWhenResize
		*/
		void _resize(int aTWidth, int aTHeight);

		/** Gets left. */
		size_t left(void) const;

		/** Gets top. */
		size_t top(void) const;

		/** Gets width. */
		size_t width(void) const;

		/** Gets height. */
		size_t height(void) const;

		/** Gets offset in width. */
		size_t getTopParentWidth(void) const;


	}; //#### end class sgFrameBufferManager

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGFRAMEBUFFERMANAGER_H__

