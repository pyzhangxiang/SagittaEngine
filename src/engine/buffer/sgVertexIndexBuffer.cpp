//////////////////////////////////////////////////////
// file: sgVertexIndexBuffer.cpp @ 2008-8-1 by Zhang Xiang
// defines of the class sgVertexIndexBuffer
// sgVertexIndexBuffer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgVertexIndexBuffer.h"
#include "engine/common/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/4/2009 zhangxiang]
	sgVertexIndexBuffer::sgVertexIndexBuffer(int aType, uInt aPolyType/* = 0 */, size_t aPolyNum/* = 0 */)
	: m_iType(aType), m_iPolyType(aPolyType), m_iPolyNum(aPolyNum){
		if(m_iPolyType < 0 || m_iPolyNum < 0){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Negative parameters.",
							"sgVertexIndexBuffer::sgVertexIndexBuffer");
		}

		size_t iSizeInBytes = sizeof(size_t) * m_iPolyType * m_iPolyNum;
		sgBuffer::resize(iSizeInBytes);
	}

	//  [8/1/2008 zhangxiang]
	sgVertexIndexBuffer::~sgVertexIndexBuffer(void){
		
	}

	//  [1/4/2009 zhangxiang]
	int sgVertexIndexBuffer::type(void) const{
		return m_iType;
	}

	//  [8/1/2008 zhangxiang]
	uInt sgVertexIndexBuffer::polyType(void) const{
		return m_iPolyType;
	}

	//  [8/1/2008 zhangxiang]
	size_t sgVertexIndexBuffer::polyNum(void) const{
		return m_iPolyNum;
	}

	//  [1/5/2009 zhangxiang]
	void sgVertexIndexBuffer::resize(size_t aPolyNum){

		if(aPolyNum == m_iPolyNum){
			// same num with original one, do nothing
			return ;
		}
		m_iPolyNum = aPolyNum;

		size_t iSizeInBytes = sizeof(size_t) * m_iPolyType * m_iPolyNum;
		sgBuffer::resize(iSizeInBytes);
	}

	//  [1/5/2009 zhangxiang]
	void sgVertexIndexBuffer::resize(uInt aPolyType, size_t aPolyNum){

		m_iPolyType = aPolyType;
		m_iPolyNum = aPolyNum;

		size_t iSizeInBytes = sizeof(size_t) * m_iPolyType * m_iPolyNum;
		sgBuffer::resize(iSizeInBytes);
	}

	//  [8/1/2008 zhangxiang]
	size_t sgVertexIndexBuffer::dataNum(void) const{
		return m_iPolyNum * m_iPolyType;
	}

} // namespace Sagitta