//////////////////////////////////////////////////////
// file: SFrameBuffer.h 
// created by zhangxiang on 09-01-14
// declare of the class SFrameBuffer
// SFrameBuffer is a class ...
//////////////////////////////////////////////////////
#ifndef __SGFRAMEBUFFER_H__
#define __SGFRAMEBUFFER_H__

// INCLUDES //////////////////////////////////////////
#include "sgBuffer.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** FrameBuffer
	@remarks
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		1 w i d t h 1 0 o f f s e t 0
		1			h				0
		1			e				0
		1			i				0
		1			g				0
		1			h				0
		1			t				0
		1			1				0
		0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	@note

	@par

	*/
	class _SG_KernelExport SFrameBuffer : public sgBuffer{
	// member variables
	private:
		/// left-top corner
		size_t m_iLeft, m_iTop;

		/// size of this frame buffer
		size_t m_iWidth, m_iHeight;

		/// one element data size in bytes, e.t. Vector3: sizeof(Vector3)
		size_t m_iDataSizeInBytes;

		/// offset in width
		size_t m_iTopParentWidth;

	// constructors & destructor
	public:
		SFrameBuffer(size_t aWidth, size_t aHeight, size_t aDataSizeInBytes);
		SFrameBuffer(const SFrameBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~SFrameBuffer(void);

	// member functions
	public:
		/** Gets left. */
		size_t left(void) const;

		/** Gets top. */
		size_t top(void) const;

		/** Gets width. */
		size_t width(void) const;

		/** Gets height. */
		size_t height(void) const;

		/** Gets the size in bytes of one data element. */
		size_t getDataSizeInBytes(void) const;

		/** Gets offset in width. */
		size_t getTopParentWidth(void) const;

		/** Resizes with new width and height. */
		void resize(size_t aWidth, size_t aHeight);

	}; //#### end class SFrameBuffer


	/** ColorBuffer
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	class _SG_KernelExport SColorBuffer : public SFrameBuffer{
	// member variables
		
	// constructors & destructor
	public:
		SColorBuffer(size_t aWidth, size_t aHeight);
		SColorBuffer(const SColorBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~SColorBuffer(void);
		
	// member functions
		
	}; //#### end class SColorBuffer


	/** DepthBuffer
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	class _SG_KernelExport SDepthBuffer : public SFrameBuffer{
	// member variables
		
	// constructors & destructor
	public:
		SDepthBuffer(size_t aWidth, size_t aHeight);
		SDepthBuffer(const SDepthBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~SDepthBuffer(void);
		
	// member functions
		
	}; //#### end class SDepthBuffer


	/** StencilBuffer
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	class _SG_KernelExport SStencilBuffer : public SFrameBuffer{
	// member variables
		
	// constructors & destructor
	public:
		SStencilBuffer(size_t aWidth, size_t aHeight);
		SStencilBuffer(const SStencilBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~SStencilBuffer(void);
		
	// member functions
		
	}; //#### end class SStencilBuffer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGFRAMEBUFFER_H__

