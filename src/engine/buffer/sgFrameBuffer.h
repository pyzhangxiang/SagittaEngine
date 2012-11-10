//////////////////////////////////////////////////////
// file: sgFrameBuffer.h 
// created by zhangxiang on 09-01-14
// declare of the class sgFrameBuffer
// sgFrameBuffer is a class ...
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
	class _SG_KernelExport sgFrameBuffer : public sgBuffer{
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
        sgFrameBuffer(void);
		sgFrameBuffer(size_t aWidth, size_t aHeight, size_t aDataSizeInBytes);
		sgFrameBuffer(const sgFrameBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~sgFrameBuffer(void);

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

		/** Resizes with new width and height. 
            if aDataSizeInBytes == 0 then
                use the original data size in bytes
         */
		void resize(size_t aWidth, size_t aHeight, size_t aDataSizeInBytes = 0);

	}; //#### end class sgFrameBuffer


	/** ColorBuffer
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	class _SG_KernelExport sgColorBuffer : public sgFrameBuffer{
	// member variables
		
	// constructors & destructor
	public:
		sgColorBuffer(size_t aWidth, size_t aHeight);
		sgColorBuffer(const sgColorBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~sgColorBuffer(void);
		
	// member functions
		
	}; //#### end class sgColorBuffer


	/** DepthBuffer
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	class _SG_KernelExport sgDepthBuffer : public sgFrameBuffer{
	// member variables
		
	// constructors & destructor
	public:
		sgDepthBuffer(size_t aWidth, size_t aHeight);
		sgDepthBuffer(const sgDepthBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~sgDepthBuffer(void);
		
	// member functions
		
	}; //#### end class sgDepthBuffer


	/** StencilBuffer
	 @remarks
	 
	 @note
	 
	 @par
	 
	 */
	class _SG_KernelExport sgStencilBuffer : public sgFrameBuffer{
	// member variables
		
	// constructors & destructor
	public:
		sgStencilBuffer(size_t aWidth, size_t aHeight);
		sgStencilBuffer(const sgStencilBuffer &aParentBuffer,
					size_t aLeft, size_t aTop,
					size_t aWidth, size_t aHeight);
		~sgStencilBuffer(void);
		
	// member functions
		
	}; //#### end class sgStencilBuffer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGFRAMEBUFFER_H__

