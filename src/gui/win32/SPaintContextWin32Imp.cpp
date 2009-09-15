//////////////////////////////////////////////////////
// file: SPaintContextWin32Imp.cpp @ 2008-12-14 by Zhang Xiang
// defines of the class SPaintContext
// SPaintContext is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "Gui/SPaintContext.h"
#include "Gui/SWindow.h"
#include "Utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/14/2008 zhangxiang]
	SPaintContext::SPaintContext(SWindow *aWindow)
		: SContext(){
		if(aWindow){
			m_ps = new PAINTSTRUCT;
			m_ContextHandle = ::BeginPaint(aWindow->handle(), m_ps);
			m_pWindow = aWindow;
		}

		if(!m_ContextHandle){
			THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Get paint dc failed.", "SPaintContext::SPaintContext(SWindow*)")
		}
	}

	//  [12/14/2008 zhangxiang]
	SPaintContext::~SPaintContext(void){
		// Calls for ::EndPaint to release dc
		if(m_ContextHandle && m_pWindow){
			::EndPaint(m_pWindow->handle(), m_ps);
			m_ContextHandle = 0;
			delete m_ps;
			m_ps = 0;
		}
	}

} // namespace Sagitta