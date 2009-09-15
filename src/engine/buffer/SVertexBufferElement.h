//////////////////////////////////////////////////////
// file: SVertexBufferElement.h @ 2008-10-8 by Zhang Xiang
// declares of the class SVertexBufferElement
// SVertexBufferElement is a class ...
//////////////////////////////////////////////////////
#ifndef __SVERTEXBUFFERELEMENT_H__
#define __SVERTEXBUFFERELEMENT_H__

// INCLUDES //////////////////////////////////////////
#include "SBuffer.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SVertexBufferElement : public SBuffer{
	public:
		enum ElementType{
			ET_UNKNOWN = -1,
			ET_VERTEX,
			ET_COLOR,
			ET_NORMAL,
			ET_TEXTURE_COORD,
			ET_FOG_COORDINATE
		};

	// member variables
	private:
		/// vetex element type
		int m_Type;
		/// coordinate num of a vertex element
		uShort m_iCoordNum;
		/// vertex count
		size_t m_iVertexNum;
		/// stream source index for d3d
		uShort m_iSource;
		/// the offset in the buffer that this element starts at
		uShort m_iOffset;
		/// vertex size of this element, 3 * sizeof(Real) for positioin
		size_t m_iVertexSize;

	// constructors & destructor
	private:
		// Disables copy constructor and assignment operator function
		SVertexBufferElement(const SVertexBufferElement &);
		SVertexBufferElement &operator = (const SVertexBufferElement &);

	public:
		SVertexBufferElement(int aType, 
							uShort aCoordNum,
							size_t aVertexNum,
							uShort aSource,
							uShort aOffset);
		~SVertexBufferElement(void);

	// member functions
	private:
		/** Calculates vertex size in bytes. */
		size_t _calculateVertexSize();
	public:
		/** Gets this element's type. */
		int type(void) const;

		/** Gets this element's coordinate's num. For position Vector3, the value is 3.*/
		uShort coordNum(void) const;

		/** Gets vertex num. */
		size_t vertexNum(void) const;

		/** Overrides from SBuffer.
			@remarks
				Resets vertex num.
			@param aVertexNum
				Unlike SBuffer::resize param, but a new num
				of vertex. The new size in bytes will be calculated and
				call for SBuffer::resize to do the true resization.
		*/
		void resize(size_t aVertexNum);

		/** Resets coordinates num and vertex num.
		*/
		void resize(uShort aCoordNum, size_t aVertexNum);

		/** Gets source. for D3D. */
		uShort source(void) const;

		/** Gets the offset of this element in the buffer. */
		uShort offset(void) const;

		/** Gets a vertex size in bytes. */
		size_t vertexSize(void) const;

		/** Gets true data num in the buffer. For a Vector3, the value 3. */
		size_t dataNum(void) const;

		/** Converts ARGB to ABGR for OpenGL.
			@remarks Only available for ET_COLOR, and called by GLRenderer::render
		*/ 
		void _colorConversion(void);
	

	}; //#### end class SVertexBufferElement

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SVERTEXBUFFERELEMENT_H__