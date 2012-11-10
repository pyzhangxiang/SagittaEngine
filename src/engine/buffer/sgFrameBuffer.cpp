//////////////////////////////////////////////////////
// file: sgFrameBuffer.cpp 
// created by zhangxiang on 09-01-14
// define of the class sgFrameBuffer
// sgFrameBuffer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgFrameBuffer.h"
#include "math/sgColor.h"
#include "engine/common/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	// sgFrameBuffer //////////////////////////////////////////////////////////////////////////

    //  [10/11/2012 zhangxiang]
    sgFrameBuffer::sgFrameBuffer(void)
    : sgBuffer()
    , m_iLeft(0), m_iWidth(0), m_iHeight(0)
    , m_iDataSizeInBytes(0), m_iTopParentWidth(0)
    {
        
    }
    
	//  [1/14/2009 zhangxiang]
	sgFrameBuffer::sgFrameBuffer(size_t aWidth, size_t aHeight, size_t aDataSizeInBytes) :
	sgBuffer(aWidth * aHeight * aDataSizeInBytes), m_iLeft(0), m_iTop(0), m_iWidth(aWidth),
	m_iHeight(aHeight), m_iDataSizeInBytes(aDataSizeInBytes), m_iTopParentWidth(aWidth){

	}

	//  [1/14/2009 zhangxiang]
	sgFrameBuffer::sgFrameBuffer(const sgFrameBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	sgBuffer(aParentBuffer, (aParentBuffer.m_iWidth * aTop + aLeft) * aParentBuffer.m_iDataSizeInBytes),
	m_iLeft(aLeft), m_iTop(aTop), m_iWidth(aWidth), m_iHeight(aHeight),
	m_iDataSizeInBytes(aParentBuffer.m_iDataSizeInBytes),
	m_iTopParentWidth(aParentBuffer.m_iTopParentWidth){
		if(aParentBuffer.m_iWidth < aWidth ||
			aParentBuffer.m_iHeight < aHeight){
			release();
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Cannot ask for a bigger width or height than the parent one.",
				"sgFrameBuffer::sgFrameBuffer");
		}

	}

	//  [1/14/2009 zhangxiang]
	sgFrameBuffer::~sgFrameBuffer(void){

	}

	//  [1/15/2009 zhangxiang]
	size_t sgFrameBuffer::left(void) const{
		return m_iLeft;
	}

	//  [1/15/2009 zhangxiang]
	size_t sgFrameBuffer::top(void) const{
		return m_iTop;
	}

	//  [1/15/2009 zhangxiang]
	size_t sgFrameBuffer::width(void) const{
		return m_iWidth;
	}

	//  [1/15/2009 zhangxiang]
	size_t sgFrameBuffer::height(void) const{
		return m_iHeight;
	}

	//  [1/15/2009 zhangxiang]
	size_t sgFrameBuffer::getDataSizeInBytes(void) const{
		return m_iDataSizeInBytes;
	}

	//  [1/15/2009 zhangxiang]
	size_t sgFrameBuffer::getTopParentWidth(void) const{
		return m_iTopParentWidth;
	}

	//  [1/15/2009 zhangxiang]
	void sgFrameBuffer::resize(size_t aWidth, size_t aHeight, size_t aDataSizeInBytes){
		// if not the top parent frame buffer, will not succeed
        if(aDataSizeInBytes == 0)
            aDataSizeInBytes = m_iDataSizeInBytes;
        else
            m_iDataSizeInBytes = aDataSizeInBytes;
        
		size_t iSizeInBytes = aWidth * aHeight * aDataSizeInBytes;
		sgBuffer::resize(iSizeInBytes);

		m_iWidth = m_iTopParentWidth = aWidth;
		m_iHeight = aHeight;
	}

	// sgFrameBuffer //////////////////////////////////////////////////////////////////////////


	// sgColorBuffer //////////////////////////////////////////////////////////////////////////

	//  [1/15/2009 zhangxiang]
	sgColorBuffer::sgColorBuffer(size_t aWidth, size_t aHeight) :
	sgFrameBuffer(aWidth, aHeight, sizeof(Color::GLColor)){

	}

	//  [1/15/2009 zhangxiang]
	sgColorBuffer::sgColorBuffer(const sgColorBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	sgFrameBuffer(aParentBuffer, aLeft, aTop, aWidth, aHeight){

	}

	//  [1/15/2009 zhangxiang]
	sgColorBuffer::~sgColorBuffer(void){

	}

	// sgColorBuffer //////////////////////////////////////////////////////////////////////////


	// sgDepthBuffer //////////////////////////////////////////////////////////////////////////

	//  [1/15/2009 zhangxiang]
	sgDepthBuffer::sgDepthBuffer(size_t aWidth, size_t aHeight) :
	sgFrameBuffer(aWidth, aHeight, sizeof(Real)){

	}

	//  [1/15/2009 zhangxiang]
	sgDepthBuffer::sgDepthBuffer(const sgDepthBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	sgFrameBuffer(aParentBuffer, aLeft, aTop, aWidth, aHeight){

	}

	//  [1/15/2009 zhangxiang]
	sgDepthBuffer::~sgDepthBuffer(void){

	}

	// DepthBuffer //////////////////////////////////////////////////////////////////////////


	// sgStencilBuffer //////////////////////////////////////////////////////////////////////////

	//  [1/15/2009 zhangxiang]
	sgStencilBuffer::sgStencilBuffer(size_t aWidth, size_t aHeight) :
	sgFrameBuffer(aWidth, aHeight, sizeof(int)){

	}

	//  [1/15/2009 zhangxiang]
	sgStencilBuffer::sgStencilBuffer(const sgStencilBuffer &aParentBuffer,
							size_t aLeft, size_t aTop,
							size_t aWidth, size_t aHeight) :
	sgFrameBuffer(aParentBuffer, aLeft, aTop, aWidth, aHeight){

	}

	//  [1/15/2009 zhangxiang]
	sgStencilBuffer::~sgStencilBuffer(void){

	}

	// sgStencilBuffer //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta