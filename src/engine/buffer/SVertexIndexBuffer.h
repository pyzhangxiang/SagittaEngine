//////////////////////////////////////////////////////
// file: SVertexIndexBuffer.h @ 2008-8-1 by Zhang Xiang
// declares of the class SVertexIndexBuffer
// SVertexIndexBuffer is a class ...
//////////////////////////////////////////////////////
#ifndef __SVERTEXINDEXBUFFER_H__
#define __SVERTEXINDEXBUFFER_H__

// INCLUDES //////////////////////////////////////////
#include "SBuffer.h"
#include <vector>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SVertexIndexBuffer : public SBuffer{
	// member variables
	private:
		/// index type, same as SVertexBufferElement type
		int m_iType;

		/// polygon type
		uInt m_iPolyType;

		/// polygon num
		size_t m_iPolyNum;

	// constructors & destructor
	private:
		// Disables copy constructor and assignment operator function
		SVertexIndexBuffer(const SVertexIndexBuffer &);
		SVertexIndexBuffer &operator = (const SVertexIndexBuffer &);

	public:
		/** Constructor.
			@param
				aType Index type, such as vertex index, normal index ...
			@param
				aPolyType Polygon type, 3 - triangle ...
			@param
				aPolyNum Polygon num.
		*/
		SVertexIndexBuffer(int aType, uInt aPolyType = 0, size_t aPolyNum = 0);
		virtual ~SVertexIndexBuffer(void);

	// member functions
	public:
		/** Gets my type, the value is the same as SVertexBufferElement type. */
		int type(void) const;

		/** Gets my polygon type. */
		uInt polyType(void) const;

		/** Gets polygon num. */
		size_t polyNum(void) const;

		/** Overrides from SBuffer.
			@remarks
				Resets polygon num.
			@param aPolyNum
				Unlike SBuffer::resize param, but a new num
				of polygon. The new size in bytes will be calculated and
				call for SBuffer::resize to do the true resization.
		*/
		void resize(size_t aPolyNum);

		/** Resets polygon type and num.
		*/
		void resize(uInt aPolyType, size_t aPolyNum);

		/** Gets data num, for triangle return 3. */
		size_t dataNum(void) const;

	}; //#### end class SVertexIndexBuffer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SVERTEXINDEXBUFFER_H__