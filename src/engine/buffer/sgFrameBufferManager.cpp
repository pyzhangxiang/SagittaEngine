//////////////////////////////////////////////////////
// file: sgFrameBufferManager.cpp 
// created by zhangxiang on 09-01-15
// define of the class sgFrameBufferManager
// sgFrameBufferManager is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgFrameBufferManager.h"
#include "sgFrameBuffer.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/15/2009 zhangxiang]
	sgFrameBufferManager::sgFrameBufferManager(void) :
//	sgObject(),
	m_pColorBuffer(0),
	m_pDepthBuffer(0),
	m_pStencilBuffer(0){

	}

	//  [1/15/2009 zhangxiang]
	sgFrameBufferManager::sgFrameBufferManager(int aTWidth, int aTHeight, bool aEnableSetencil) :
//	sgObject(),
	m_pColorBuffer(0),
	m_pDepthBuffer(0),
	m_pStencilBuffer(0){
		m_pColorBuffer = new SColorBuffer(aTWidth, aTHeight);
		m_pDepthBuffer = new SDepthBuffer(aTWidth, aTHeight);
		if(aEnableSetencil){
			m_pStencilBuffer = new SStencilBuffer(aTWidth, aTHeight);
		}
	}

	//  [1/15/2009 zhangxiang]
	sgFrameBufferManager::sgFrameBufferManager(const sgFrameBufferManager &aParentBuffers,
											size_t aLeft, size_t aTop,
											size_t aWidth, size_t aHeight) :
//	sgObject(),
	m_pColorBuffer(0),
	m_pDepthBuffer(0),
	m_pStencilBuffer(0){
		m_pColorBuffer = new SColorBuffer(*(aParentBuffers.m_pColorBuffer),
										aLeft, aTop, aWidth, aHeight);
		m_pDepthBuffer = new SDepthBuffer(*(aParentBuffers.m_pDepthBuffer),
										aLeft, aTop, aWidth, aHeight);
		if(aParentBuffers.m_pStencilBuffer){
			m_pStencilBuffer = new SStencilBuffer(*(aParentBuffers.m_pStencilBuffer),
												aLeft, aTop, aWidth, aHeight);
		}
	}

	//  [1/15/2009 zhangxiang]
	sgFrameBufferManager::~sgFrameBufferManager(void){
		_release();
	}
	
	//  [1/15/2009 zhangxiang]
	void sgFrameBufferManager::_release(void){
		if(m_pColorBuffer){
			delete m_pColorBuffer;
			m_pColorBuffer = 0;
		}
		if(m_pDepthBuffer){
			delete m_pDepthBuffer;
			m_pDepthBuffer = 0;
		}
		if(m_pStencilBuffer){
			delete m_pStencilBuffer;
			m_pStencilBuffer = 0;
		}
	}

	//  [1/15/2009 zhangxiang]
	void sgFrameBufferManager::getSliceFromAnother(const sgFrameBufferManager &aParentBuffers,
												size_t aLeft, size_t aTop,
												size_t aWidth, size_t aHeight){
		_release();
		if(aParentBuffers.getColorBuffer()){
			m_pColorBuffer = new SColorBuffer(*(aParentBuffers.getColorBuffer()),
											aLeft, aTop, aWidth, aHeight);
		}
		if(aParentBuffers.getDepthBuffer()){
			m_pDepthBuffer = new SDepthBuffer(*(aParentBuffers.getDepthBuffer()),
											aLeft, aTop, aWidth, aHeight);
		}
		if(aParentBuffers.getStencilBuffer()){
			m_pStencilBuffer = new SStencilBuffer(*(aParentBuffers.getStencilBuffer()),
												aLeft, aTop, aWidth, aHeight);
		}
	}


	//  [1/15/2009 zhangxiang]
	SColorBuffer *sgFrameBufferManager::getColorBuffer(void) const{
		return m_pColorBuffer;
	}

	//  [1/15/2009 zhangxiang]
	SDepthBuffer *sgFrameBufferManager::getDepthBuffer(void) const{
		return m_pDepthBuffer;
	}

	//  [1/15/2009 zhangxiang]
	SStencilBuffer *sgFrameBufferManager::getStencilBuffer(void) const{
		return m_pStencilBuffer;
	}

	//  [1/15/2009 zhangxiang]
	void sgFrameBufferManager::clearColor(const Color::GLColor &aValue){
		if(!m_pColorBuffer)
			return ;

		size_t width = m_pColorBuffer->width();
		size_t height = m_pColorBuffer->height();
		size_t topwidth = m_pColorBuffer->getTopParentWidth();

		Color::GLColor *colorbuffer = static_cast<Color::GLColor*>(m_pColorBuffer->data());
		
		size_t size_per_data = sizeof(Color::GLColor);
		// first clear one row
		*colorbuffer = aValue;
		Color::GLColor *dest = colorbuffer + 1;
		size_t cleared;
		size_t rest;
		size_t cleared_in_bytes;
		for(;;){
			cleared = dest - colorbuffer;
			rest = width - cleared;
			cleared_in_bytes = size_per_data * cleared;
			if(cleared >= rest){
				memcpy(dest, colorbuffer, size_per_data * rest);
				break;
			}else{
				memcpy(dest, colorbuffer, cleared_in_bytes);
				dest += cleared;
			}
		}
		
		// then the rest rows
		// every time, copy 8 times
		size_t size_per_row = size_per_data * width;
		Color::GLColor *end_row = colorbuffer + topwidth * height;
		Color::GLColor *firs_trow = colorbuffer;
		colorbuffer += topwidth;
		
		size_t rest_row = height - 1;
		switch(rest_row & 7){
			case 0:
				while(colorbuffer != end_row){
					memcpy(colorbuffer, firs_trow, size_per_row);
					colorbuffer += topwidth;

			case 7:
				memcpy(colorbuffer, firs_trow, size_per_row);
				colorbuffer += topwidth;

			case 6:
				memcpy(colorbuffer, firs_trow, size_per_row);
				colorbuffer += topwidth;

			case 5:
				memcpy(colorbuffer, firs_trow, size_per_row);
				colorbuffer += topwidth;

			case 4:
				memcpy(colorbuffer, firs_trow, size_per_row);
				colorbuffer += topwidth;

			case 3:
				memcpy(colorbuffer, firs_trow, size_per_row);
				colorbuffer += topwidth;

			case 2:
				memcpy(colorbuffer, firs_trow, size_per_row);
				colorbuffer += topwidth;

			case 1:
				memcpy(colorbuffer, firs_trow, size_per_row);
				colorbuffer += topwidth;
				}
		}
	}

	//  [1/15/2009 zhangxiang]
	void sgFrameBufferManager::clearDepth(Real aValue){
		if(!m_pDepthBuffer)
			return ;

		size_t width = m_pDepthBuffer->width();
		size_t height = m_pDepthBuffer->height();
		size_t topwidth = m_pDepthBuffer->getTopParentWidth();

		Real *depthbuffer = static_cast<Real*>(m_pDepthBuffer->data());

		size_t size_per_data = sizeof(Real);
		// first clear one row
		*depthbuffer = aValue;
		Real *dest = depthbuffer + 1;
		size_t cleared;
		size_t rest;
		size_t cleared_in_bytes;
		for(;;){
			cleared = dest - depthbuffer;
			rest = width - cleared;
			cleared_in_bytes = size_per_data * cleared;
			if(cleared >= rest){
				memcpy(dest, depthbuffer, size_per_data * rest);
				break;
			}else{
				memcpy(dest, depthbuffer, cleared_in_bytes);
				dest += cleared;
			}
		}

		// then the rest rows
		// every time, copy 8 times
		size_t size_per_row = size_per_data * width;
		Real *end_row = depthbuffer + topwidth * height;
		Real *firs_trow = depthbuffer;
		depthbuffer += topwidth;

		size_t rest_row = height - 1;
		switch(rest_row & 7){
			case 0:
				while(depthbuffer != end_row){
					memcpy(depthbuffer, firs_trow, size_per_row);
					depthbuffer += topwidth;

			case 7:
				memcpy(depthbuffer, firs_trow, size_per_row);
				depthbuffer += topwidth;

			case 6:
				memcpy(depthbuffer, firs_trow, size_per_row);
				depthbuffer += topwidth;

			case 5:
				memcpy(depthbuffer, firs_trow, size_per_row);
				depthbuffer += topwidth;

			case 4:
				memcpy(depthbuffer, firs_trow, size_per_row);
				depthbuffer += topwidth;

			case 3:
				memcpy(depthbuffer, firs_trow, size_per_row);
				depthbuffer += topwidth;

			case 2:
				memcpy(depthbuffer, firs_trow, size_per_row);
				depthbuffer += topwidth;

			case 1:
				memcpy(depthbuffer, firs_trow, size_per_row);
				depthbuffer += topwidth;
				}
		}
	}

	//  [1/15/2009 zhangxiang]
	void sgFrameBufferManager::clearStencil(int aValue){
		if(!m_pStencilBuffer)
			return ;

		size_t width = m_pStencilBuffer->width();
		size_t height = m_pStencilBuffer->height();
		size_t topwidth = m_pStencilBuffer->getTopParentWidth();

		int *stencilbuffer = static_cast<int*>(m_pStencilBuffer->data());
		
		size_t size_per_data = sizeof(Real);
		// first clear one row
		*stencilbuffer = aValue;
		int *dest = stencilbuffer + 1;
		size_t cleared;
		size_t rest;
		size_t cleared_in_bytes;
		for(;;){
			cleared = dest - stencilbuffer;
			rest = width - cleared;
			cleared_in_bytes = size_per_data * cleared;
			if(cleared >= rest){
				memcpy(dest, stencilbuffer, size_per_data * rest);
				break;
			}else{
				memcpy(dest, stencilbuffer, cleared_in_bytes);
				dest += cleared;
			}
		}

		// then the rest rows
		// every time, copy 8 times
		size_t size_per_row = size_per_data * width;
		int *end_row = stencilbuffer + topwidth * height;
		int *firs_trow = stencilbuffer;
		stencilbuffer += topwidth;

		size_t rest_row = height - 1;
		switch(rest_row & 7){
			case 0:
				while(stencilbuffer != end_row){
					memcpy(stencilbuffer, firs_trow, size_per_row);
					stencilbuffer += topwidth;

			case 7:
				memcpy(stencilbuffer, firs_trow, size_per_row);
				stencilbuffer += topwidth;

			case 6:
				memcpy(stencilbuffer, firs_trow, size_per_row);
				stencilbuffer += topwidth;

			case 5:
				memcpy(stencilbuffer, firs_trow, size_per_row);
				stencilbuffer += topwidth;

			case 4:
				memcpy(stencilbuffer, firs_trow, size_per_row);
				stencilbuffer += topwidth;

			case 3:
				memcpy(stencilbuffer, firs_trow, size_per_row);
				stencilbuffer += topwidth;

			case 2:
				memcpy(stencilbuffer, firs_trow, size_per_row);
				stencilbuffer += topwidth;

			case 1:
				memcpy(stencilbuffer, firs_trow, size_per_row);
				stencilbuffer += topwidth;
				}
		}
	}

	//  [1/15/2009 zhangxiang]
	void sgFrameBufferManager::_resize(int aTWidth, int aTHeight){
		m_pColorBuffer->resize(aTWidth, aTHeight);
		m_pDepthBuffer->resize(aTWidth, aTHeight);
		if(m_pStencilBuffer){
			m_pStencilBuffer->resize(aTWidth, aTHeight);
		}
	}

	//  [1/16/2009 zhangxiang]
	size_t sgFrameBufferManager::left(void) const{
		if(m_pColorBuffer){
			return m_pColorBuffer->left();
		}else{
			return 0;
		}
	}

	//  [1/16/2009 zhangxiang]
	size_t sgFrameBufferManager::top(void) const{
		if(m_pColorBuffer){
			return m_pColorBuffer->top();
		}else{
			return 0;
		}
	}

	//  [1/16/2009 zhangxiang]
	size_t sgFrameBufferManager::width(void) const{
		if(m_pColorBuffer){
			return m_pColorBuffer->width();
		}else{
			return 0;
		}
	}

	//  [1/16/2009 zhangxiang]
	size_t sgFrameBufferManager::height(void) const{
		if(m_pColorBuffer){
			return m_pColorBuffer->height();
		}else{
			return 0;
		}
	}

	//  [1/16/2009 zhangxiang]
	size_t sgFrameBufferManager::getTopParentWidth(void) const{
		if(m_pColorBuffer){
			return m_pColorBuffer->getTopParentWidth();
		}else{
			return 0;
		}
	}

} // namespace Sagitta