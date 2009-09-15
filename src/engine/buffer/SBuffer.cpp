//////////////////////////////////////////////////////
// file: SBuffer.cpp 
// created by zhangxiang on 09-1-3
// declares of the class SBuffer
// SBuffer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SBuffer.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/4/2009 zhangxiang]
	SBuffer::SBuffer(void) :
	m_pData(0), m_iSizeInBytes(0),
	m_pRefCount(0), m_iOffsetInBytes(0){

	}

	//  [1/4/2009 zhangxiang]
	SBuffer::SBuffer(size_t aSizeInBytes) :
	m_pData(0), m_iSizeInBytes(aSizeInBytes),
	m_pRefCount(0), m_iOffsetInBytes(0){
		/*if(m_iSizeInBytes < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							"Negative size for allocate.",
							"SBuffer::SBuffer");
		}*/

		if(m_iSizeInBytes > 0){
			m_pData = new uByte[m_iSizeInBytes];
			m_pRefCount = new uInt(1);
		}
	}

	//  [1/14/2009 zhangxiang]
/*	SBuffer::SBuffer(const SBuffer &aBuffer) :
	m_pData(aBuffer.m_pData),
	m_iSizeInBytes(aBuffer.m_iSizeInBytes),
	m_pRefCount(aBuffer.m_pRefCount),
	m_iOffsetInBytes(aBuffer.m_iOffsetInBytes){
		if(m_pRefCount){
			++(*m_pRefCount);
		}
	}
*/

	//  [1/14/2009 zhangxiang]
	SBuffer::SBuffer(const SBuffer &aBuffer, size_t aOffsetInBytes) :
	m_pData(aBuffer.m_pData + aOffsetInBytes),
	m_iSizeInBytes(aBuffer.m_iSizeInBytes - aOffsetInBytes),
	m_pRefCount(aBuffer.m_pRefCount),
	m_iOffsetInBytes(aBuffer.m_iOffsetInBytes + aOffsetInBytes){
		if(aBuffer.m_iSizeInBytes <= aOffsetInBytes){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Invalid offset.", "SBuffer::SBuffer");
		}

		if(m_pRefCount){
			++(*m_pRefCount);
		}
	}

	//  [1/4/2009 zhangxiang]
	SBuffer::~SBuffer(void){
		release();
	}

	//  [1/14/2009 zhangxiang]
	void SBuffer::release(void){
		if(m_pRefCount){
			if(--(*m_pRefCount) == 0){
				delete [](m_pData - m_iOffsetInBytes);
				delete m_pRefCount;
				m_pData = 0;
				m_pRefCount = 0;
			}
		}
	}

	//  [1/14/2009 zhangxiang]
/*	SBuffer &SBuffer::operator=(const SBuffer &aBuffer){
		if(m_pData == aBuffer.m_pData){
			return *this;
		}
		release();

		m_pData = aBuffer.m_pData;
		m_pRefCount = aBuffer.m_pRefCount;
		m_iOffsetInBytes = aBuffer.m_iOffsetInBytes;
		if(m_pRefCount){
			++(*m_pRefCount);
		}
		
		return *this;
	}
*/

	//  [1/5/2009 zhangxiang]
	size_t SBuffer::getSizeInBytes(void) const{
		return m_iSizeInBytes;
	}

	//  [1/14/2009 zhangxiang]
	size_t SBuffer::getOffsetInBytes(void) const{
		return m_iOffsetInBytes;
	}

	//  [1/5/2009 zhangxiang]
	void SBuffer::resize(size_t aSizeInBytes){
		if(m_iOffsetInBytes > 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
							"Cannot resize this buffer with offset > 0.",
							"SBuffer::resize");
		}

		if(m_pRefCount){
			if((*m_pRefCount) > 1){
				THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE,
								"Cannot resize this buffer with reference count > 0.",
								"SBuffer::resize");
			}
		}

		if(aSizeInBytes < 0){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Negative size for allocate.",
				"SBuffer::resize");
		}

		if(aSizeInBytes == m_iSizeInBytes){
			// same size with original one, do nothing
			return ;
		}

		// free original data
		release();

		// reallcate
		m_iSizeInBytes = aSizeInBytes;
		if(m_iSizeInBytes > 0){
			m_pData = new uByte[m_iSizeInBytes];
			m_pRefCount = new uInt(1);
		}
	}

	//  [1/4/2009 zhangxiang]
	void *SBuffer::data(void) const{
		return m_pData;
	}

	//  [1/5/2009 zhangxiang]
	void SBuffer::readData(size_t aOffset, size_t aLength, void *outDest){
#ifdef _DEBUG
		assert((aOffset + aLength) <= m_iSizeInBytes);
#endif
		memcpy(outDest, m_pData + aOffset, aLength);
	}

} // namespace Sagitta