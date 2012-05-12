//////////////////////////////////////////////////////
// file: sgVertexIndexBuffer.h @ 2008-8-1 by Zhang Xiang
// declares of the class sgVertexIndexBuffer
// sgVertexIndexBuffer is a class ...
//////////////////////////////////////////////////////
#ifndef __SGVERTEXINDEXBUFFER_H__
#define __SGVERTEXINDEXBUFFER_H__

// INCLUDES //////////////////////////////////////////
#include "sgBuffer.h"
#include <vector>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgVertexIndexBuffer : public sgBuffer{
	// member variables
	private:
		/// index type, same as sgVertexBufferElement type
		int m_iType;

		/// polygon type
		uInt m_iPolyType;

		/// polygon num
		size_t m_iPolyNum;

	// constructors & destructor
	private:
		// Disables copy constructor and assignment operator function
		sgVertexIndexBuffer(const sgVertexIndexBuffer &);
		sgVertexIndexBuffer &operator = (const sgVertexIndexBuffer &);

	public:
		/** Constructor.
			@param
				aType Index type, such as vertex index, normal index ...
			@param
				aPolyType Polygon type, 3 - triangle ...
			@param
				aPolyNum Polygon num.
		*/
		sgVertexIndexBuffer(int aType, uInt aPolyType = 0, size_t aPolyNum = 0);
		virtual ~sgVertexIndexBuffer(void);

	// member functions
	public:
		/** Gets my type, the value is the same as sgVertexBufferElement type. */
		int type(void) const;

		/** Gets my polygon type. */
		uInt polyType(void) const;

		/** Gets polygon num. */
		size_t polyNum(void) const;

		/** Overrides from sgBuffer.
			@remarks
				Resets polygon num.
			@param aPolyNum
				Unlike sgBuffer::resize param, but a new num
				of polygon. The new size in bytes will be calculated and
				call for sgBuffer::resize to do the true resization.
		*/
		void resize(size_t aPolyNum);

		/** Resets polygon type and num.
		*/
		void resize(uInt aPolyType, size_t aPolyNum);

		/** Gets data num, for triangle return 3. */
		size_t dataNum(void) const;

	}; //#### end class sgVertexIndexBuffer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGVERTEXINDEXBUFFER_H__

