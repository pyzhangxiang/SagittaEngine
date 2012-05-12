//////////////////////////////////////////////////////
// file: SRenderContextAppleImp.cpp 
// created by zhangxiang on 08-12-17
// defines of the class SRenderContext
// SRenderContext is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SRenderContext.h"
#include "../SWindow.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	SRenderContext::SRenderContext(SWindow *aWindow)
	: SContext(){
		OSStatus err = noErr;
		if(aWindow){
		//	err = CreateCGContextForPort((GrafPtr) GetWindowPort (aWindow->handle()), &m_ContextHandle);
			m_ContextHandle = 0;
			m_pWindow = aWindow;
		}
		
		if(err != noErr){
			THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Create context for port failed.", "SRenderContext::SRenderContext");
		}
	}
	
	SRenderContext::~SRenderContext(void){
		// context will be called in the super class's destructor
	}
	
	void SRenderContext::swapBuffers(void){
		// quick draw does not support for double buffers ... may be...
	}
	
	void SRenderContext::update(void){
		// do nothing, let sub-classes to implement
	}
	
} // namespace Sagitta