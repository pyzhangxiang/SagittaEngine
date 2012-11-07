//////////////////////////////////////////////////////
// file: sgVertexBufferElement.h @ 2008-10-8 by Zhang Xiang
// declares of the class sgVertexBufferElement
// sgVertexBufferElement is a class ...
//////////////////////////////////////////////////////
#ifndef __SGVERTEXBUFFERELEMENT_H__
#define __SGVERTEXBUFFERELEMENT_H__

// INCLUDES //////////////////////////////////////////
#include "sgBuffer.h"
#include "engine/common/sgStrHandle.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgVertexBufferElement : public sgBuffer{
	public:
		enum ElementType{
			ET_UNKNOWN = -1,
			ET_VERTEX,
			ET_COLOR,
			ET_NORMAL,
			ET_TEXTURE_COORD,
			ET_FOG_COORDINATE
		};

		static const sgStrHandle VertexAttributeName;
		static const sgStrHandle ColorAttributeName;
		static const sgStrHandle NormalAttributeName;
		static const sgStrHandle UV0AttributeName;
		static const sgStrHandle FogAttributeName;

	// member variables
	private:
		// element name, for shader attributes
		sgStrHandle mName;
		// data type, e.g. RDT_F, RDT_I
		UInt32 mDataType;

		// should be normalized in gpu, default false
		bool mShouldNormalize;

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
		sgVertexBufferElement(const sgVertexBufferElement &);
		sgVertexBufferElement &operator = (const sgVertexBufferElement &);

	public:
		sgVertexBufferElement(const sgStrHandle &name, 
							UInt32 aDataType,
							uShort aCoordNum,
							size_t aVertexNum,
							uShort aSource,
							uShort aOffset);
		~sgVertexBufferElement(void);

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

		/** Overrides from sgBuffer.
			@remarks
				Resets vertex num.
			@param aVertexNum
				Unlike sgBuffer::resize param, but a new num
				of vertex. The new size in bytes will be calculated and
				call for sgBuffer::resize to do the true resization.
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
	
		UInt32 getDataType(void) const{ return mDataType; }
		sgStrHandle getName(void) const{ return mName; }
		//void setName(const sgStrHandle &name);
		
		bool shouldNormalize(void) const{ return mShouldNormalize; }
		void setShouldNormalize(bool norm);

	}; //#### end class sgVertexBufferElement

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGVERTEXBUFFERELEMENT_H__

