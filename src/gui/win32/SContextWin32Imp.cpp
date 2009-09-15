//////////////////////////////////////////////////////
// file: SContextWin32Imp.cpp @ 2008-12-13 by Zhang Xiang
// defines of the class SContext
// SContext is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "Gui/SContext.h"
#include "Gui/SWindow.h"
#include "Utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/13/2008 zhangxiang]
	SContext::SContext(void) : m_ContextHandle(0), m_pWindow(0), m_bReleaseSelf(true){

	}

	SContext::SContext(SContextHandle aContextHandle, SWindow *aWindow, bool bReleaseSelf /* = true */)
		: m_ContextHandle(0), m_pWindow(0), m_bReleaseSelf(bReleaseSelf){
		if(aContextHandle && aWindow){
			m_ContextHandle = aContextHandle;
			m_pWindow = aWindow;
		}
	}

	//  [12/18/2008 zhangxiang]
	//SContext::SContext(const SContext& aContext){
	//	THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "Cannot use copy constructor.", "SContext::SContext");
	//}

	//  [12/13/2008 zhangxiang]
	SContext::~SContext(void){
		if(m_bReleaseSelf){
			release();
		}
	}

	//  [12/18/2008 zhangxiang]
	/*SContext &SContext::operator =(const SContext &aContext){
		THROW_SAGI_EXCEPT(SException::ERR_INVALID_STATE, "Cannot use assignment constructor.", "SContext::operator=");
	}*/

	//  [12/14/2008 zhangxiang]
	void SContext::release(void){
		if(m_ContextHandle && m_pWindow){
			if(!::IsWindow(m_pWindow->handle())){
				THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Release dc failed : The binding window has been destroyed.", "SContext::release(void)");
			}

			if(!::ReleaseDC(m_pWindow->handle(), m_ContextHandle)){
				THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Release dc failed.", "SContext::release(void)");
			}
		}
	}

} // namespace Sagitta