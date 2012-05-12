//////////////////////////////////////////////////////
// file: SContextAppleImp.cpp 
// created by zhangxiang on 08-12-17
// defines of the class SContext
// SContext is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SContext.h"
#include "../SWindow.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	SContext::SContext(void) : m_ContextHandle(0), m_pWindow(0), m_bReleaseSelf(true){
		
	}
	
	SContext::SContext(SContextHandle aContextHandle, SWindow *aWindow, bool bReleaseSelf/* = true */)
	: m_ContextHandle(aContextHandle), m_pWindow(aWindow), m_bReleaseSelf(bReleaseSelf){
		
	}
/*			no need
	SContext::SContext(const SContext &aContext){
		THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE, "Cannot use copy constructor.", "SContext::SContext");
	}
*/	
	SContext::~SContext(void){
		if(m_bReleaseSelf){
			release();
		}
	}
/*		no need
	SContext &SContext::operator=(const SContext &aContext){
		THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE, "Cannot use assignment operator.", "SContext::operator=");
	}
*/	
	void SContext::release(void){
		if(!HIViewIsValid(m_pWindow->handle())){
			THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Release context failed : The binded window has been destroyed.", "SContext::release");
		}
		
		CGContextRelease(m_ContextHandle);
	}
	
} // namespace Sagitta