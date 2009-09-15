//////////////////////////////////////////////////////
// file: SVertexBufferElement.cpp @ 2008-10-8 by Zhang Xiang
// defines of the class SVertexBufferElement
// SVertexBufferElement is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SVertexBufferElement.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SVertexBufferElement::SVertexBufferElement(int aType,
											uShort aCoordNum,
											size_t aVertexNum,
											uShort aSource,
											uShort aOffset)
	: SBuffer(), m_Type(aType),
	m_iCoordNum(aCoordNum), m_iVertexNum(aVertexNum),
	m_iSource(aSource), m_iOffset(aOffset){
	/*	if(m_iCoordNum < 0 || m_iVertexNum < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Negative parameters.",
							"SVertexBufferElement::SVertexBufferElement");
		}
	 */

		// Calculate vertex size
		m_iVertexSize = _calculateVertexSize();

		size_t iSizeInBytes = m_iVertexSize * m_iVertexNum;
		SBuffer::resize(iSizeInBytes);
	}

	SVertexBufferElement::~SVertexBufferElement(void){
	}

	size_t SVertexBufferElement::_calculateVertexSize(){
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
				THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS, "Invalid VertexBufferElementType.", "SVertexBufferElement::_calculateVertexSize");
		}
	}

	int SVertexBufferElement::type(void) const{
		return m_Type;
	}

	uShort SVertexBufferElement::coordNum(void) const{
		return m_iCoordNum;
	}

	size_t SVertexBufferElement::vertexNum(void) const{
		return m_iVertexNum;
	}

	//  [1/5/2009 zhangxiang]
	void SVertexBufferElement::resize(size_t aVertexNum){
		/*if(aVertexNum < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Negative vertex num.", "SVertexBufferElement::resize");
		}*/

		if(aVertexNum == m_iVertexNum){
			// same num with original one, do nothing
			return ;
		}
		m_iVertexNum = aVertexNum;

		size_t iSizeInBytes = m_iVertexSize * m_iVertexNum;
		SBuffer::resize(iSizeInBytes);
	}

	//  [1/5/2009 zhangxiang]
	void SVertexBufferElement::resize(uShort aCoordNum, size_t aVertexNum){
		/*if(aCoordNum < 0 || aVertexNum < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Negative coordinate num or vertex num.", "SVertexBufferElement::resize");
		}*/

		m_iCoordNum = aCoordNum;
		m_iVertexNum = aVertexNum;
		m_iVertexSize = _calculateVertexSize();

		size_t iSizeInBytes = m_iVertexSize * m_iVertexNum;
		SBuffer::resize(iSizeInBytes);
	}

	uShort SVertexBufferElement::source(void) const{
		return m_iSource;
	}

	uShort SVertexBufferElement::offset(void) const{
		return m_iOffset;
	}

	//  [10/10/2008 zhangxiang]
	size_t SVertexBufferElement::vertexSize(void) const{
		return m_iVertexSize;
	}

	size_t SVertexBufferElement::dataNum(void) const{
		return m_iVertexNum * m_iCoordNum;
	}

	//  [1/15/2009 zhangxiang]
	void SVertexBufferElement::_colorConversion(void){
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