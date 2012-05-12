//////////////////////////////////////////////////////
// file: SRenderContextWin32Imp.cpp @ 2008-12-13 by Zhang Xiang
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

	//  [12/13/2008 zhangxiang]
	SRenderContext::SRenderContext(SWindow *aWindow)
		: SContext(){
		if(aWindow){
			m_ContextHandle = ::GetDC(aWindow->handle());
			m_pWindow = aWindow;
		}

		if(!m_ContextHandle){
			THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Get client dc failed.", "SRenderContext::SRenderContext(SWindow*)")
		}
	}

	//  [12/13/2008 zhangxiang]
	SRenderContext::~SRenderContext(void){
		// ReleaseDC will be called in the super class's destructor
	}

	//  [12/14/2008 zhangxiang]
	void SRenderContext::swapBuffers(void){
		if(!::SwapBuffers(m_ContextHandle)){
			THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Swap buffer failed.", "SRenderContext::swapBuffers(void)")
		}
	}

	void SRenderContext::update(void){

	}

} // namespace Sagitta