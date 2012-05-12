//////////////////////////////////////////////////////
// file: SGLContextAppleImp.cpp 
// created by zhangxiang on 08-12-17
// defines of the class SGLContext
// SGLContext is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SGLContext.h"
#include "../SWindow.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	SGLContext::SGLContext(SWindow *aWindow) : SRenderContext(aWindow), m_GLContextHandle(0){
		try{
			buildRenderEnv();
		}catch(sgException &e){
			CGContextRelease(m_ContextHandle);
			// log for future ...
			// SLog(e.what());
		}
	}
	
	SGLContext::~SGLContext(void){
		disposeRenderEnv();
		// context will be called in the super class's destructor
	}
	
	void SGLContext::buildRenderEnv(void){
		OSStatus err = noErr;
		Rect rectPort;
		int attributes[] = {
			AGL_RGBA, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 24, AGL_NONE
		};
		
		AGLPixelFormat aglformat;
		
		aglformat = aglChoosePixelFormat(0, 0, attributes);
		err = aglGetError();
		if(err != AGL_NO_ERROR){
			THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Choose AGL pixel format faild: " + std::string((const char*)(aglErrorString(err))), "SGLContext::buildRenderEnv");
		}
							  
		if(aglformat){
			m_GLContextHandle = aglCreateContext(aglformat, 0);
			err = aglGetError();
			if(err != AGL_NO_ERROR){
				THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Choose AGL pixel format faild: " + std::string((const char*)(aglErrorString(err))), "SGLContext::buildRenderEnv");
			}
		}
		
		if(m_GLContextHandle){
		//	if(!aglSetDrawable(m_GLContextHandle, GetWindowPort(m_pWindow->handle()))){
			if(!aglSetHIViewRef(m_GLContextHandle, m_pWindow->handle())){
				err = aglGetError();
				if(err != AGL_NO_ERROR){
					aglDestroyContext (m_GLContextHandle);
					m_GLContextHandle = 0;
					aglDestroyPixelFormat (aglformat);
					aglformat = 0;					
					THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Set drawable faild: " + std::string((const char*)(aglErrorString(err))), "SGLContext::buildRenderEnv");
				}				
			}
			
			if(!aglSetCurrentContext(m_GLContextHandle)){
				err = aglGetError();
				if(err != AGL_NO_ERROR){
				//	aglSetDrawable (m_GLContextHandle, 0);
					aglSetHIViewRef(m_GLContextHandle, 0);
					aglDestroyContext (m_GLContextHandle);
					m_GLContextHandle = 0;
					aglDestroyPixelFormat (aglformat);
					aglformat = 0;
					THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Set current context faild: " + std::string((const char*)(aglErrorString(err))), "SGLContext::buildRenderEnv");
				}				
			}
		}
		
		aglDestroyPixelFormat(aglformat);
	}
	
	void SGLContext::disposeRenderEnv(void){
		if(m_GLContextHandle){
		//	aglSetDrawable(m_GLContextHandle, 0);
			aglSetHIViewRef(m_GLContextHandle, 0);
			aglDestroyContext(m_GLContextHandle);
			m_GLContextHandle = 0;
		}
	}
	
	void SGLContext::swapBuffers(void){
		aglSwapBuffers(m_GLContextHandle);
	}
	
	void SGLContext::update(void){
		aglUpdateContext(m_GLContextHandle);
	}
	
} // namespace Sagitta