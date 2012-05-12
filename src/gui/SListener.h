//////////////////////////////////////////////////////
// file: SListener.h @ 2007-11-17 by Zhang Xiang
// declaration of the class SListener
// SListener is a class ...
//////////////////////////////////////////////////////
#ifndef __SLISTENER_H__
#define __SLISTENER_H__

// INCLUSION /////////////////////////////////////////
#include "SController.h"

// DECLARATION ///////////////////////////////////////
namespace Sagitta{

	class SEvent;
	class SWindow;

	class _SG_GuiExport SListener : public SController{
	// member variables

	// contructors & destructor
	public:
		SListener(void){}
		virtual ~SListener(void){}

	// static functions
	public:
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
		static SStatus CALLBACK dispatchEvent(HWND aHWnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam);
#elif SAGITTA_PLATFORM == SAGITTA_PLATFORM_APPLE
		static SStatus dispatchEvent(EventHandlerCallRef aHandler, EventRef aEvent, void *aUserData);
#else
#endif
		

	// member functions
	protected:
		virtual SStatus handleEvent(const SEvent &aEvent) = 0;

	public:
		void sendEvent(const SEvent &aEvent, SWindow *aReceiver);
		
	};

} // namespace Sagitta

#endif // __SLISTENER_H__