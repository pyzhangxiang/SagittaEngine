//////////////////////////////////////////////////////
// file: sgVertexData.h @ 2008-8-1 by Zhang Xiang
// declares of the class sgVertexData
// sgVertexData is a class ...
//////////////////////////////////////////////////////
#ifndef __SGVERTEXDATA_H__
#define __SGVERTEXDATA_H__

// INCLUDES //////////////////////////////////////////
#include "engine/common/sgMemObject.h"
#include "engine/common/sgStrHandle.h"
#include "engine/common/sgStlAllocator.h"
#include "math/SagittaPlatform.h"
#include "math/sgTypeDef.h"
#include "engine/common/sgIterator.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class sgVertexBufferElement;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgVertexData : public sgMemObject{
	private:
		// mapped by element type
		typedef sg_map(int, sgVertexBufferElement*) ElementList;

	public:
		typedef sgMapIterator<ElementList> Iterator;
		typedef sgConstMapIterator<ElementList> ConstIterator;

		// member variables
	private:
		/** Data buffers. Vertices, normal, color, texture, fog. */
		ElementList m_ElementList;

		/** vertex num. */
		size_t m_iVertexNum;

		/** source count, for D3D */
		uShort m_iSourceCount;

		// constructors & destructor
	private:
		sgVertexData(const sgVertexData &);
		sgVertexData &operator = (const sgVertexData &);

	public:
		sgVertexData(size_t aiVertexNum = 0);
		virtual ~sgVertexData(void);

	// member functions
	private:
		/** Adds an element to this VertexBuffer. */
		void addElement(sgVertexBufferElement *aElement);

	public:
		/** Gets the number of vertex. */
		size_t vertexNum(void) const;
		/** Sets the number of vertex.
			@remarks Only succeed when the element list is empty.
		*/
		void setVertexNum(size_t aVertexNum);

		/** Gets the size of element list. */
		size_t elementNum(void) const;

		/** Clears the element list. */
		void clear(void);

		/** Creates an element. */
		sgVertexBufferElement *createElement(const sgStrHandle &name, UInt32 aDataType, uShort aCoordNum, size_t aVertexNum);

		/** Gets an element by type. */
		sgVertexBufferElement *getElement(int aType) const;

		/** Gets the iterator of the element list. */
		Iterator getIterator(void);
		/** Gets the const iterator of the element list. */
		ConstIterator getConstIterator(void) const;

		size_t getElementNum(void) const{ return m_ElementList.size(); }

	}; //#### end class sgVertexData


	class sgVertexIndexBuffer;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgIndexData : public sgMemObject{
	private:
	// mapped by index type
		typedef sg_map(int, sgVertexIndexBuffer*) ElementList;

	public:
		typedef sgMapIterator<ElementList> Iterator;
		typedef sgConstMapIterator<ElementList> ConstIterator;

	// member variables
	private:
		/** Index buffers. Vertices, normal, color, texture, fog. */
		ElementList m_ElementList;

		/** Polygon type. */
		uInt m_iPolyType;

		/** Polygon num. */
		size_t m_iPolyNum;

	// constructors & destructor
	private:
		sgIndexData(const sgIndexData &);
		sgIndexData &operator = (const sgIndexData &);

	public:
		sgIndexData(uInt aPolyType, size_t aPolyNum);
		virtual ~sgIndexData(void);

		// member functions
	private:
		/** Adds an element to this VertexBuffer. */
		void addElement(sgVertexIndexBuffer *aElement);

	public:
		/** Gets the number of polygon. */
		size_t polyNum(void) const;
		/** Sets the number of polygon.
			@remarks Only succeed when the element list is empty.
		*/
		void setPolyNum(size_t aPolyNum);

		/** Gets the polygon type. */
		uInt polyType(void) const;
		/** Sets the polygon type.
			@remarks Only succeed when the element list is empty.
		*/
		void setPolyType(uInt aPolyType);

		/** Gets the size of element list. */
		size_t elementNum(void) const;

		/** Clears the element list. */
		void clear(void);

		/** Creates an element. */
		sgVertexIndexBuffer *createElement(int aType);

		/** Gets an element by type. */
		sgVertexIndexBuffer *getElement(int aType) const;

		/** Gets the iterator of the element list. */
		Iterator getIterator(void);
		/** Gets the const iterator of the element list. */
		ConstIterator getConstIterator(void) const;

		size_t getElementNum(void) const{ return m_ElementList.size(); }

	}; //#### end class sgVertexData

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGVERTEXDATA_H__

