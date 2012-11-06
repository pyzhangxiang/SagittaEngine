//////////////////////////////////////////////////////
// file: sgBuffer.h 
// created by zhangxiang on 09-1-3
// declares of the class sgBuffer
// sgBuffer is a class ...
//////////////////////////////////////////////////////
#ifndef __SGBUFFER_H__
#define __SGBUFFER_H__

// INCLUDES //////////////////////////////////////////
#include "engine/common/sgObject.h"
#include "math/SagittaPlatform.h"
#include "math/sgTypeDef.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgBuffer : public sgObject{
	// member variables
	private:
		/// the true data
		uByte *m_pData;

		/// size in bytes
		size_t m_iSizeInBytes;

		/// reference count
		uInt *m_pRefCount;

		/// offset in bytes to the original address
		size_t m_iOffsetInBytes;

	// constructors & destructor
	public:
		sgBuffer(void);
		sgBuffer(size_t aSizeInBytes);
	//	sgBuffer(const sgBuffer &aBuffer);
		sgBuffer(const sgBuffer &aBuffer, size_t aOffset);
		virtual ~sgBuffer(void);

	// member functions
	protected:
		/** Releases the raw pointer. */
		void release(void);

	public:
		/** Assignment. */
	//	sgBuffer &operator=(const sgBuffer &aBuffer);

		/** Gets the size of this buffer in bytes. */
		size_t getSizeInBytes(void) const;

		/** Gets offset in bytes. */
		size_t getOffsetInBytes(void) const;

		/** Resizes this buffer.
			@param
				aSizeInBytes New size value in bytes.
		*/
		virtual void resize(size_t aSizeInBytes);

		/** Gets the true data. */
		void *data(void) const;

		/** Reads data from the buffer and places it in the memory pointed to by pDest.
			@param aOffset The byte offset from the start of the buffer to read
			@param aLength The size of the area to read, in bytes
			@param outDest The area of memory in which to place the data, must be large enough to 
				accommodate the data!
		*/
		void readData(size_t aOffset, size_t aLength, void *outDest);

	}; //#### end class sgBuffer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGBUFFER_H__

