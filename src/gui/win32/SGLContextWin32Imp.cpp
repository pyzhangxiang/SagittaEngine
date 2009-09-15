//////////////////////////////////////////////////////
// file: SGLContextWin32Imp.cpp @ 2008-12-13 by Zhang Xiang
// defines of the class SGLContext
// SGLContext is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "Gui/SGLContext.h"
#include "Gui/SWindow.h"
#include "Utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/14/2008 zhangxiang]
	SGLContext::SGLContext(SWindow *aWindow) : SRenderContext(aWindow){
		try{
			buildRenderEnv();
		}catch(SException &e){
			::ReleaseDC(m_pWindow->handle(), m_ContextHandle);
			// log for future ...
			// SLog(e.what());
		}
	}

	//  [12/14/2008 zhangxiang]
	SGLContext::~SGLContext(void){
		disposeRenderEnv();
		// ReleaseDC will be called in the super class's destructor
	}

	//  [12/14/2008 zhangxiang]
	void SGLContext::buildRenderEnv(void){
		PIXELFORMATDESCRIPTOR pfd;
		int iFormat;

		ZeroMemory( &pfd, sizeof( pfd ) );
		pfd.nSize = sizeof( pfd );
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		iFormat = ::ChoosePixelFormat(m_ContextHandle, &pfd);
		if(!iFormat){
			THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Choose pixel format failed.", "SGLContext::buildRenderEnv(void)");
		}

		::SetPixelFormat(m_ContextHandle, iFormat, &pfd);
		m_GLContextHandle = ::wglCreateContext(m_ContextHandle);
		if(!m_GLContextHandle){
			THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Create GL render context failed.", "SGLContext::buildRenderEnv(void)");
		}
		
		if(!::wglMakeCurrent(m_ContextHandle, m_GLContextHandle)){
			::wglDeleteContext(m_GLContextHandle);
			THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Make current GL render context failed.", "SGLContext::buildRenderEnv(void)");
		}
	}

	//  [12/14/2008 zhangxiang]
	void SGLContext::disposeRenderEnv(void){
		if(m_GLContextHandle){
			::wglMakeCurrent(0, 0);
			::wglDeleteContext(m_GLContextHandle);
		}
	}

	//  [12/20/2008 zhangxiang]
	void SGLContext::swapBuffers(void){
		SRenderContext::swapBuffers();
	}

	void SGLContext::update(void){

	}

} // namespace Sagitta