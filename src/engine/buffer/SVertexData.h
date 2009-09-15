//////////////////////////////////////////////////////
// file: SVertexData.h @ 2008-8-1 by Zhang Xiang
// declares of the class SVertexData
// SVertexData is a class ...
//////////////////////////////////////////////////////
#ifndef __SVERTEXDATA_H__
#define __SVERTEXDATA_H__

// INCLUDES //////////////////////////////////////////
#include "../../common/SObject.h"
#include "../../common/utils/SIterator.h"
#include <map>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SVertexBufferElement;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SVertexData : public SObject{
	private:
		// mapped by element type
		typedef std::map<int, SVertexBufferElement*> ElementList;

	public:
		typedef SMapIterator<ElementList> Iterator;
		typedef SConstMapIterator<ElementList> ConstIterator;

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
		SVertexData(const SVertexData &);
		SVertexData &operator = (const SVertexData &);

	public:
		SVertexData(size_t aiVertexNum = 0);
		virtual ~SVertexData(void);

	// member functions
	private:
		/** Adds an element to this VertexBuffer. */
		void addElement(SVertexBufferElement *aElement);

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
		SVertexBufferElement *createElement(int aType, uShort aCoordNum, size_t aVertexNum);

		/** Gets an element by type. */
		SVertexBufferElement *getElement(int aType) const;

		/** Gets the iterator of the element list. */
		Iterator getIterator(void);
		/** Gets the const iterator of the element list. */
		ConstIterator getConstIterator(void) const;

	}; //#### end class SVertexData


	class SVertexIndexBuffer;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SagittaExport SIndexData : public SObject{
	private:
	// mapped by index type
		typedef std::map<int, SVertexIndexBuffer*> ElementList;

	public:
		typedef SMapIterator<ElementList> Iterator;
		typedef SConstMapIterator<ElementList> ConstIterator;

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
		SIndexData(const SIndexData &);
		SIndexData &operator = (const SIndexData &);

	public:
		SIndexData(uInt aPolyType, size_t aPolyNum);
		virtual ~SIndexData(void);

		// member functions
	private:
		/** Adds an element to this VertexBuffer. */
		void addElement(SVertexIndexBuffer *aElement);

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
		SVertexIndexBuffer *createElement(int aType);

		/** Gets an element by type. */
		SVertexIndexBuffer *getElement(int aType) const;

		/** Gets the iterator of the element list. */
		Iterator getIterator(void);
		/** Gets the const iterator of the element list. */
		ConstIterator getConstIterator(void) const;

	}; //#### end class SVertexData

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SVERTEXDATA_H__