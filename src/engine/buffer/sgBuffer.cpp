//////////////////////////////////////////////////////
// file: sgBuffer.cpp 
// created by zhangxiang on 09-1-3
// declares of the class sgBuffer
// sgBuffer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgBuffer.h"
#include "engine/common/sgException.h"
#include <cassert>

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/4/2009 zhangxiang]
	sgBuffer::sgBuffer(void) :
	m_pData(0), m_iSizeInBytes(0),
	m_pRefCount(0), m_iOffsetInBytes(0){

	}

	//  [1/4/2009 zhangxiang]
	sgBuffer::sgBuffer(size_t aSizeInBytes) :
	m_pData(0), m_iSizeInBytes(aSizeInBytes),
	m_pRefCount(0), m_iOffsetInBytes(0){
		/*if(m_iSizeInBytes < 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Negative size for allocate.",
							"sgBuffer::sgBuffer");
		}*/

		if(m_iSizeInBytes > 0){
			m_pData = (uByte*)sgMalloc(m_iSizeInBytes); //new uByte[m_iSizeInBytes];
			m_pRefCount = (uInt*)sgMalloc(sizeof(uInt));//new uInt(1);
			*m_pRefCount = 1;
		}
	}

	//  [1/14/2009 zhangxiang]
/*	sgBuffer::sgBuffer(const sgBuffer &aBuffer) :
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
	sgBuffer::sgBuffer(const sgBuffer &aBuffer, size_t aOffsetInBytes) :
	m_pData(aBuffer.m_pData + aOffsetInBytes),
	m_iSizeInBytes(aBuffer.m_iSizeInBytes - aOffsetInBytes),
	m_pRefCount(aBuffer.m_pRefCount),
	m_iOffsetInBytes(aBuffer.m_iOffsetInBytes + aOffsetInBytes){
		if(aBuffer.m_iSizeInBytes <= aOffsetInBytes){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Invalid offset.", "sgBuffer::sgBuffer");
		}

		if(m_pRefCount){
			++(*m_pRefCount);
		}
	}

	//  [1/4/2009 zhangxiang]
	sgBuffer::~sgBuffer(void){
		release();
	}

	//  [1/14/2009 zhangxiang]
	void sgBuffer::release(void){
		if(m_pRefCount){
			if(--(*m_pRefCount) == 0){
				//delete [](m_pData - m_iOffsetInBytes);
				//delete m_pRefCount;
				sgFree(m_pData - m_iOffsetInBytes);
				sgFree(m_pRefCount);
				m_pData = 0;
				m_pRefCount = 0;
			}
		}
	}

	//  [1/14/2009 zhangxiang]
/*	sgBuffer &sgBuffer::operator=(const sgBuffer &aBuffer){
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
	size_t sgBuffer::getSizeInBytes(void) const{
		return m_iSizeInBytes;
	}

	//  [1/14/2009 zhangxiang]
	size_t sgBuffer::getOffsetInBytes(void) const{
		return m_iOffsetInBytes;
	}

	//  [1/5/2009 zhangxiang]
	void sgBuffer::resize(size_t aSizeInBytes){
		if(m_iOffsetInBytes > 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
							"Cannot resize this buffer with offset > 0.",
							"sgBuffer::resize");
		}

		if(m_pRefCount){
			if((*m_pRefCount) > 1){
				THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
								"Cannot resize this buffer with reference count > 0.",
								"sgBuffer::resize");
			}
		}

		if(aSizeInBytes < 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Negative size for allocate.",
				"sgBuffer::resize");
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
			m_pData = (uByte*)sgMalloc(m_iSizeInBytes); //new uByte[m_iSizeInBytes];
			m_pRefCount = (uInt*)sgMalloc(sizeof(uInt));//new uInt(1);
			*m_pRefCount = 1;
		}
	}

	//  [1/4/2009 zhangxiang]
	void *sgBuffer::data(void) const{
		return m_pData;
	}

	//  [1/5/2009 zhangxiang]
	void sgBuffer::readData(size_t aOffset, size_t aLength, void *outDest){
#ifdef _DEBUG
		assert((aOffset + aLength) <= m_iSizeInBytes);
#endif
		memcpy(outDest, m_pData + aOffset, aLength);
	}

} // namespace Sagitta