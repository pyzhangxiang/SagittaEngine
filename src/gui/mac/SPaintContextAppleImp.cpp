//////////////////////////////////////////////////////
// file: SPaintContextAppleImp.cpp 
// created by zhangxiang on 08-12-18
// defines of the class SPaintContext
// SPaintContext is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SPaintContext.h"
#include "../SWindow.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	SPaintContext::SPaintContext(SWindow *aWindow)
	: SContext(){
		OSStatus err = noErr;
		if(aWindow){
			m_ps = GetWindowPort(HIViewGetWindow(aWindow->handle()));
			err = QDBeginCGContext(m_ps, &m_ContextHandle);
			m_pWindow = aWindow;
		}
		
		if(err != noErr){
			THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Get paint context failed.", "SPaintContext::SPaintContext");
		}	
	}
	
	SPaintContext::~SPaintContext(void){
		// Calls for QDEndCGContext to release context.
		if(m_ContextHandle && m_pWindow){
			QDEndCGContext(m_ps, &m_ContextHandle);
			m_ContextHandle = 0;
		}
	}
	
} // namespace Sagitta