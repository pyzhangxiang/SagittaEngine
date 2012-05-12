/////////////////////////////////////////////////////////
// file: SApplicatioin.h @ 2007-11-17 by Zhang Xiang
// definition of the class SApplication
// SApplication is a class ...
/////////////////////////////////////////////////////////

// INCLUSIONS ///////////////////////////////////////////
#include "SApplication.h"
#include "SWindow.h"
#include "SMainFrame.h"
#include "SEvent.h"
#include "../common/utils/sgException.h"
#include "../common/utils/sgStringUtil.h"
#include <vector>

// DEFINITIONS //////////////////////////////////////////
namespace Sagitta{

	// static //////////////////////////////////////////////////////////////////////////
	SApplication *SApplication::single_app = 0;

	//  [12/9/2008 zhangxiang]
	SApplication *SApplication::instance_ptr(void){
		return SApplication::single_app;
	}

	//  [12/9/2008 zhangxiang]
	SApplication &SApplication::instance(void){
		return *SApplication::single_app;
	}

	//////////////////////////////////////////////////////////////////////////


	SApplication::SApplication() : SListener(), m_pMainWindow(0){
		if(SApplication::instance_ptr()){
			THROW_SAGI_EXCEPT(sgException::ERR_DUPLICATE_ITEM, "Cannot duplicate application, because it's singleton.", "SApplication::SApplication");
		}

		SApplication::single_app = this;

	} //#### end constructor

	SApplication::~SApplication(void){
		std::vector<SWindow*> tempv;
		tempv.reserve(m_WindowMap.size());
		WindowMap::const_iterator mit = m_WindowMap.begin();
		WindowMap::const_iterator meit = m_WindowMap.end(); 
		for(; mit!=meit; ++mit){
			tempv.push_back(mit->second);
		}

		std::vector<SWindow*>::iterator vit = tempv.begin();
		for(; vit!=tempv.end(); ++vit){
			delete *vit;
		}

	} //#### end destructor

	//  [12/9/2008 zhangxiang]
	SStatus SApplication::handleEvent(const SEvent &aEvent){
		SStatus err = NONPROCESSEDERR;

		switch(aEvent.type()){
			case SEvent::ET_CREATE:
			{
				const SCreateEvent &event = dynamic_cast<const SCreateEvent&>(aEvent);
				SWindow *window = dynamic_cast<SWindow*>(event.receiver());
				if(!window){
					THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
									"The receiver is not a window",
									"SApplication::handleEvent");
				}
				onWindowCreate(event.handle(), static_cast<SWindow*>(event.receiver()));
				err = NOERR;
				break;
			}

			case SEvent::ET_DESTROY:
			{
				SWindow *window = dynamic_cast<SWindow*>(aEvent.receiver());
				if(!window){
					THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
						"The receiver is not a window",
						"SApplication::handleEvent");
				}
				onWindowDispose(static_cast<SWindow*>(aEvent.receiver()));
				err = NOERR;
				break;
			}

			default:
			{
				err = NONPROCESSEDERR;
				break;
			}
		}

		return err;
	}

	//  [1/7/2009 zhangxiang]
	void SApplication::onWindowCreate(SWindowHandle aHandle, SWindow *aWindow){
		registerWindow(aHandle, aWindow);
	}

	//  [1/7/2009 zhangxiang]
	void SApplication::onWindowDispose(SWindow *aWindow){
		SWindow *toDispose = aWindow;
		unregisterWindow(aWindow);
		if(!aWindow->isDisposing()){
			delete aWindow;
		}
		if(m_pMainWindow == toDispose){
			quit(0);
		}
	}

	SMainFrame *SApplication::mainWindow(void) const{
		return m_pMainWindow;

	} //#### end mainWindow

	//  [12/9/2008 zhangxiang]
	void SApplication::setMainWindow(SMainFrame *aWindow){
		if(!aWindow){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL window pointer", "SApplication::setMainWindow");
		}

		if(!findWindow(aWindow)){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Window " + aWindow->title() + " " + sgStringUtil::to_string(aWindow->handle()) + " has not been registered.",
							"SApplication::setMainWindow");
		}else{
			m_pMainWindow = aWindow;
		}
	}

	//  [12/14/2008 zhangxiang]
	void SApplication::registerWindow(SWindowHandle aHandle, SWindow *aWindow){
		if(m_WindowMap.find(aHandle) != m_WindowMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Window is existed", "SApplication::registerWindow(SWindowHandle, SWindow*)");
		}else{
			m_WindowMap.insert(std::make_pair(aHandle, aWindow));
		}

	}

	//  [1/7/2009 zhangxiang]
	void SApplication::unregisterWindow(SWindow *aWindow){
		WindowMap::iterator it = m_WindowMap.find(aWindow->handle());
		if(it == m_WindowMap.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_ITEM_NOT_FOUND, "No such window: " + aWindow->title(), "SApplication::unregisterWindow");
		}else{
			m_WindowMap.erase(it);
		}
	}

	SWindow *SApplication::findWindow(SWindowHandle aHandle) const{
		WindowMap::const_iterator it = m_WindowMap.find(aHandle);
		if(it == m_WindowMap.end()){
			return 0;
		}else{
			return it->second;
		}

	} //#### end findWindow

	bool SApplication::findWindow(SWindow *aWindow) const{
		if(!aWindow){
			return false;
		}

		if(!findWindow(aWindow->handle())){
			return false;
		}else{
			return true;
		}
	}

	void SApplication::registerConnection(SController *_aSender, const std::string &_aSignal, SController *_aReceiver, const std::string &_aSlot){
		// for future...
	}

	void SApplication::response(SController *_aSender, const std::string &_aSignal){
		// for future...
	}

} // namespace Sagitta