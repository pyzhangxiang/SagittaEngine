//////////////////////////////////////////////////////
// file: SFrameBuffer.cpp 
// created by zhangxiang on 09-01-14
// define of the class SFrameBuffer
// SFrameBuffer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SFrameBuffer.h"
#include "../../math/SColor.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// SFrameBuffer //////////////////////////////////////////////////////////////////////////

	//  [1/14/2009 zhangxiang]
	SFrameBuffer::SFrameBuffer(size_t aWidth, size_t aHeight, size_t aDataSizeInBytes) :
	SBuffer(aWidth * aHeight * aDataSizeInBytes), m_iLeft(0), m_iTop(0), m_iWidth(aWidth),
	m_iHeight(aHeight), m_iDataSizeInBytes(aDataSizeInBytes), m_iTopParentWidth(aWidth){

	}

	//  [1/14/2009 zhangxiang]
	SFrameBuffer::SFrameBuffer(const SFrameBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	SBuffer(aParentBuffer, (aParentBuffer.m_iWidth * aTop + aLeft) * aParentBuffer.m_iDataSizeInBytes),
	m_iLeft(aLeft), m_iTop(aTop), m_iWidth(aWidth), m_iHeight(aHeight),
	m_iDataSizeInBytes(aParentBuffer.m_iDataSizeInBytes),
	m_iTopParentWidth(aParentBuffer.m_iTopParentWidth){
		if(aParentBuffer.m_iWidth < aWidth ||
			aParentBuffer.m_iHeight < aHeight){
			release();
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
				"Cannot ask for a bigger width or height than the parent one.",
				"SFrameBuffer::SFrameBuffer");
		}

	}

	//  [1/14/2009 zhangxiang]
	SFrameBuffer::~SFrameBuffer(void){

	}

	//  [1/15/2009 zhangxiang]
	size_t SFrameBuffer::left(void) const{
		return m_iLeft;
	}

	//  [1/15/2009 zhangxiang]
	size_t SFrameBuffer::top(void) const{
		return m_iTop;
	}

	//  [1/15/2009 zhangxiang]
	size_t SFrameBuffer::width(void) const{
		return m_iWidth;
	}

	//  [1/15/2009 zhangxiang]
	size_t SFrameBuffer::height(void) const{
		return m_iHeight;
	}

	//  [1/15/2009 zhangxiang]
	size_t SFrameBuffer::getDataSizeInBytes(void) const{
		return m_iDataSizeInBytes;
	}

	//  [1/15/2009 zhangxiang]
	size_t SFrameBuffer::getTopParentWidth(void) const{
		return m_iTopParentWidth;
	}

	//  [1/15/2009 zhangxiang]
	void SFrameBuffer::resize(size_t aWidth, size_t aHeight){
		// if not the top parent frame buffer, will not succeed
		size_t iSizeInBytes = aWidth * aHeight * m_iDataSizeInBytes;
		SBuffer::resize(iSizeInBytes);

		m_iWidth = m_iTopParentWidth = aWidth;
		m_iHeight = aHeight;
	}

	// SFrameBuffer //////////////////////////////////////////////////////////////////////////


	// SColorBuffer //////////////////////////////////////////////////////////////////////////

	//  [1/15/2009 zhangxiang]
	SColorBuffer::SColorBuffer(size_t aWidth, size_t aHeight) :
	SFrameBuffer(aWidth, aHeight, sizeof(Color::GLColor)){

	}

	//  [1/15/2009 zhangxiang]
	SColorBuffer::SColorBuffer(const SColorBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	SFrameBuffer(aParentBuffer, aLeft, aTop, aWidth, aHeight){

	}

	//  [1/15/2009 zhangxiang]
	SColorBuffer::~SColorBuffer(void){

	}

	// SColorBuffer //////////////////////////////////////////////////////////////////////////


	// SDepthBuffer //////////////////////////////////////////////////////////////////////////

	//  [1/15/2009 zhangxiang]
	SDepthBuffer::SDepthBuffer(size_t aWidth, size_t aHeight) :
	SFrameBuffer(aWidth, aHeight, sizeof(Real)){

	}

	//  [1/15/2009 zhangxiang]
	SDepthBuffer::SDepthBuffer(const SDepthBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	SFrameBuffer(aParentBuffer, aLeft, aTop, aWidth, aHeight){

	}

	//  [1/15/2009 zhangxiang]
	SDepthBuffer::~SDepthBuffer(void){

	}

	// DepthBuffer //////////////////////////////////////////////////////////////////////////


	// SStencilBuffer //////////////////////////////////////////////////////////////////////////

	//  [1/15/2009 zhangxiang]
	SStencilBuffer::SStencilBuffer(size_t aWidth, size_t aHeight) :
	SFrameBuffer(aWidth, aHeight, sizeof(int)){

	}

	//  [1/15/2009 zhangxiang]
	SStencilBuffer::SStencilBuffer(const SStencilBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	SFrameBuffer(aParentBuffer, aLeft, aTop, aWidth, aHeight){

	}

	//  [1/15/2009 zhangxiang]
	SStencilBuffer::~SStencilBuffer(void){

	}

	// SStencilBuffer //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta