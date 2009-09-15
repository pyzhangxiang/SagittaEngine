//////////////////////////////////////////////////////
// file: SVertexIndexBuffer.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class SVertexIndexBuffer
// SVertexIndexBuffer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SVertexIndexBuffer.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/4/2009 zhangxiang]
	SVertexIndexBuffer::SVertexIndexBuffer(int aType, uInt aPolyType/* = 0 */, size_t aPolyNum/* = 0 */)
	: m_iType(aType), m_iPolyType(aPolyType), m_iPolyNum(aPolyNum){
		if(m_iPolyType < 0 || m_iPolyNum < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Negative parameters.",
							"SVertexIndexBuffer::SVertexIndexBuffer");
		}

		size_t iSizeInBytes = sizeof(size_t) * m_iPolyType * m_iPolyNum;
		SBuffer::resize(iSizeInBytes);
	}

	//  [8/1/2008 zhangxiang]
	SVertexIndexBuffer::~SVertexIndexBuffer(void){
		
	}

	//  [1/4/2009 zhangxiang]
	int SVertexIndexBuffer::type(void) const{
		return m_iType;
	}

	//  [8/1/2008 zhangxiang]
	uInt SVertexIndexBuffer::polyType(void) const{
		return m_iPolyType;
	}

	//  [8/1/2008 zhangxiang]
	size_t SVertexIndexBuffer::polyNum(void) const{
		return m_iPolyNum;
	}

	//  [1/5/2009 zhangxiang]
	void SVertexIndexBuffer::resize(size_t aPolyNum){
		if(aPolyNum < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Negative polygon num.", "SVertexBufferElement::resize");
		}

		if(aPolyNum == m_iPolyNum){
			// same num with original one, do nothing
			return ;
		}
		m_iPolyNum = aPolyNum;

		size_t iSizeInBytes = sizeof(size_t) * m_iPolyType * m_iPolyNum;
		SBuffer::resize(iSizeInBytes);
	}

	//  [1/5/2009 zhangxiang]
	void SVertexIndexBuffer::resize(uInt aPolyType, size_t aPolyNum){
		if(aPolyType < 0 || aPolyNum < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Negative polygon type or num.", "SVertexBufferElement::resize");
		}

		m_iPolyType = aPolyType;
		m_iPolyNum = aPolyNum;

		size_t iSizeInBytes = sizeof(size_t) * m_iPolyType * m_iPolyNum;
		SBuffer::resize(iSizeInBytes);
	}

	//  [8/1/2008 zhangxiang]
	size_t SVertexIndexBuffer::dataNum(void) const{
		return m_iPolyNum * m_iPolyType;
	}

} // namespace Sagitta