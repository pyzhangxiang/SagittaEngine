//////////////////////////////////////////////////////
// file: sgVertexBufferElement.cpp @ 2008-10-8 by Zhang Xiang
// defines of the class sgVertexBufferElement
// sgVertexBufferElement is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgVertexBufferElement.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	sgVertexBufferElement::sgVertexBufferElement(int aType,
											uShort aCoordNum,
											size_t aVertexNum,
											uShort aSource,
											uShort aOffset)
	: sgBuffer(), m_Type(aType),
	m_iCoordNum(aCoordNum), m_iVertexNum(aVertexNum),
	m_iSource(aSource), m_iOffset(aOffset){
	/*	if(m_iCoordNum < 0 || m_iVertexNum < 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Negative parameters.",
							"sgVertexBufferElement::sgVertexBufferElement");
		}
	 */

		// Calculate vertex size
		m_iVertexSize = _calculateVertexSize();

		size_t iSizeInBytes = m_iVertexSize * m_iVertexNum;
		sgBuffer::resize(iSizeInBytes);
	}

	sgVertexBufferElement::~sgVertexBufferElement(void){
	}

	size_t sgVertexBufferElement::_calculateVertexSize(){
		switch(m_Type){
			case ET_VERTEX:
			case ET_NORMAL:
			case ET_TEXTURE_COORD:
			case ET_FOG_COORDINATE:
				{
					return m_iCoordNum * sizeof(Real);
				}

			case ET_COLOR:
				{
					return m_iCoordNum * sizeof(uByte);
				}

			default:
				THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Invalid VertexBufferElementType.", "sgVertexBufferElement::_calculateVertexSize");
		}
	}

	int sgVertexBufferElement::type(void) const{
		return m_Type;
	}

	uShort sgVertexBufferElement::coordNum(void) const{
		return m_iCoordNum;
	}

	size_t sgVertexBufferElement::vertexNum(void) const{
		return m_iVertexNum;
	}

	//  [1/5/2009 zhangxiang]
	void sgVertexBufferElement::resize(size_t aVertexNum){
		/*if(aVertexNum < 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Negative vertex num.", "sgVertexBufferElement::resize");
		}*/

		if(aVertexNum == m_iVertexNum){
			// same num with original one, do nothing
			return ;
		}
		m_iVertexNum = aVertexNum;

		size_t iSizeInBytes = m_iVertexSize * m_iVertexNum;
		sgBuffer::resize(iSizeInBytes);
	}

	//  [1/5/2009 zhangxiang]
	void sgVertexBufferElement::resize(uShort aCoordNum, size_t aVertexNum){
		/*if(aCoordNum < 0 || aVertexNum < 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Negative coordinate num or vertex num.", "sgVertexBufferElement::resize");
		}*/

		m_iCoordNum = aCoordNum;
		m_iVertexNum = aVertexNum;
		m_iVertexSize = _calculateVertexSize();

		size_t iSizeInBytes = m_iVertexSize * m_iVertexNum;
		sgBuffer::resize(iSizeInBytes);
	}

	uShort sgVertexBufferElement::source(void) const{
		return m_iSource;
	}

	uShort sgVertexBufferElement::offset(void) const{
		return m_iOffset;
	}

	//  [10/10/2008 zhangxiang]
	size_t sgVertexBufferElement::vertexSize(void) const{
		return m_iVertexSize;
	}

	size_t sgVertexBufferElement::dataNum(void) const{
		return m_iVertexNum * m_iCoordNum;
	}

	//  [1/15/2009 zhangxiang]
	void sgVertexBufferElement::_colorConversion(void){
		uInt *color = static_cast<uInt*>(data());
		for(size_t i=0; i<m_iVertexNum; ++i){
			// Conversion between ARGB and ABGR is always a case of flipping R/B
			*color = 
				((*color & 0x00ff0000) >> 16) |
				((*color & 0x000000ff) << 16) |
				((*color & 0xff00ff00));
			++color;
		}
	}

} // namespace Sagitta