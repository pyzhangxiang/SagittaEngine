//////////////////////////////////////////////////////
// file: SListenerAppleImpl.cpp 
// created by zhangxiang on 09-1-18
// defines of the class SListenerAppleImpl
// SListenerAppleImpl is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SListener.h"
#include "../SWindow.h"
#include "../SApplication.h"
#include "../SEvent.h"
#include "../SKeyMapping.h"
#include "../../common/utils/SBitOP.h"
#include "../../common/utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	SStatus SListener::dispatchEvent(EventHandlerCallRef aHandler, EventRef aEvent, void *aUserData){
		OSStatus err = eventNotHandledErr;
		SStatus sagiErr = NONPROCESSEDERR;
		
		SListener *receiver = 0;	
		
		uInt eclass = ::GetEventClass(aEvent);
		uInt ekind = ::GetEventKind(aEvent);
		switch(eclass){
			case kEventClassWindow:
			{
				receiver = (SListener*)aUserData;
				switch(ekind){
					case kEventWindowClosed:
					{
						// window is distroyed(may before destroy)
						sagiErr = static_cast<SListener*>(SApplication::instance_ptr())->handleEvent(SEvent(SEvent::ET_DESTROY, 0, 0, receiver));
						break;
					}
						
					case kEventWindowClose:
					{
						// standard window handler call for DisposeWindow
						sagiErr = receiver->handleEvent(SEvent(SEvent::ET_CLOSE, 0, 0, receiver));
						break;
					}
						
					case kEventWindowBoundsChanged:
					{
						Rect rect;
						err = ::GetEventParameter(aEvent, kEventParamCurrentBounds, typeQDRectangle,
												  0, sizeof(Rect), 0, &rect);
						if(err != noErr){
							goto PARAMETERMISSING;
						}
						
						UInt32 attri;
						err = ::GetEventParameter(aEvent, kEventParamAttributes, typeUInt32,
												  0, sizeof(UInt32), 0, &attri);
						if(err != noErr){
							goto PARAMETERMISSING;
						}
						if(attri & kWindowBoundsChangeOriginChanged){	// move event
							sagiErr = receiver->handleEvent(SMoveEvent(SEvent::ET_MOVE, rect.left, rect.top, receiver));
						}
						if(attri & kWindowBoundsChangeSizeChanged){	// resize event
							sagiErr = receiver->handleEvent(SResizeEvent(SEvent::ET_RESIZE, rect.right - rect.left, rect.bottom - rect.top, receiver));
						}
						break;
					}
				}
				break;
			}
				
			case kEventClassKeyboard:
			{
				break;
			}
				
			case kEventClassMouse:
			{
			//	HIViewGetViewForMouseEvent();
			//	HIViewClick();
				break;
			}
				
				// for HIView
			case kEventClassHIObject:
			{
				//SListener *instance = (SListener*)aUserData;
				SWindowHandle window = (SWindowHandle)aUserData;
				switch(ekind){
					case kEventHIObjectConstruct:
					{
						HIViewRef hiview;
						err = GetEventParameter(aEvent, kEventParamHIObjectInstance,
												typeHIObjectRef, 0, sizeof(HIObjectRef),
												0, (HIObjectRef*)(&hiview));
						if(err != noErr)
							goto PARAMETERMISSING;
						
						window = hiview;
						err = SetEventParameter(aEvent, kEventParamHIObjectInstance, typeVoidPtr, sizeof(window), &window);
						if(err != noErr)
							goto PARAMETERMISSING;
						
						sagiErr = NOERR;
						break;
					}
						
					case kEventHIObjectInitialize:
					{
						err = CallNextEventHandler(aHandler, aEvent);
						if(err == noErr){
							err = GetEventParameter(aEvent, 'w', typeVoidPtr,
													0, sizeof(SListener**), 0, &receiver);
							if(err != noErr)
								goto PARAMETERMISSING;
							
							sagiErr = static_cast<SListener*>(SApplication::instance_ptr())->handleEvent(SCreateEvent(SEvent::ET_CREATE, (SWindowHandle)window, receiver));
						}else{
							sagiErr = NONPROCESSEDERR;
						}
						
						break;
					}
						
					case kEventHIObjectDestruct:
					{
						sagiErr = NOERR;
						break;
					}
				}
				
				break;
			}
				
			case kEventClassControl:
			{
				SWindowHandle window = (SWindowHandle)aUserData;
				receiver = (SListener*)(SApplication::instance().findWindow(window));
				if(!receiver){
					goto PARAMETERMISSING;
				}
				
				switch(ekind){
					case kEventControlDraw:
					{
						sagiErr = receiver->handleEvent(SEvent(SEvent::ET_PAINT, 0, 0, receiver));
						break;
					}
						
					case kEventControlBoundsChanged:
					{
						Rect rect;
						err = ::GetEventParameter(aEvent, kEventParamCurrentBounds, typeQDRectangle, 0, sizeof(Rect), 0, &rect);
						if(err != noErr){
							goto PARAMETERMISSING;
						}
						
						UInt32 attri;
						err = ::GetEventParameter(aEvent, kEventParamAttributes, typeUInt32,
												  0, sizeof(UInt32), 0, &attri);
						if(err != noErr){
							goto PARAMETERMISSING;
						}
						
						if(attri & kWindowBoundsChangeOriginChanged){	// move event
							sagiErr = receiver->handleEvent(SMoveEvent(SEvent::ET_MOVE, rect.left, rect.top, receiver));
						}
						if(attri & kWindowBoundsChangeSizeChanged){	// resize event
							sagiErr = receiver->handleEvent(SResizeEvent(SEvent::ET_RESIZE, rect.right - rect.left, rect.bottom - rect.top, receiver));
						}
						break;
					}
				}
				
				break;
			}
		}
		
		if(sagiErr == NOERR)
			err = noErr;
		else err = eventNotHandledErr;
		
	PARAMETERMISSING:
		return err;
		
	}
	
	void SListener::sendEvent(const SEvent &aEvent, SWindow *aReceiver){
		if(!aReceiver){
			THROW_SAGI_EXCEPT(SException::ERR_INVALIDPARAMS,
							  "Null receiver pointer.", "SListener::sendEvent");
		}
		
		OSStatus err = noErr;
		
		EventRef evn;
		uInt evnclass, evnkind;
		EventAttributes attri = kEventAttributeNone;
		switch(aEvent.type()){
			case SEvent::ET_DESTROY:
			{
				evnclass = kEventClassWindow;
				evnkind = kEventWindowClosed;
				err = CreateEvent(0, evnclass, evnkind, GetCurrentEventTime(), attri, &evn);
				break;
			}
				
			case SEvent::ET_CLOSE:
			{
				evnclass = kEventClassWindow;
				evnkind = kEventWindowClose;
				err = CreateEvent(0, evnclass, evnkind, GetCurrentEventTime(), attri, &evn);
				break;
			}
				
			case SEvent::ET_MOVE:
			{
				evnclass = kEventClassHIObject;
				evnkind = kEventControlBoundsChanged;
				err = CreateEvent(0, evnclass, evnkind, GetCurrentEventTime(), attri, &evn);
				if(err != noErr)
					break;
				
				UInt32 imove = kWindowBoundsChangeOriginChanged;
				Rect currect;
				currect.left = aEvent.param1();
				currect.right = aEvent.param2();
				err = SetEventParameter(evn, kEventParamCurrentBounds, typeQDRectangle, sizeof(Rect), &currect);
				if(err != noErr){
					break;
				}
				
				err = SetEventParameter(evn, kEventParamAttributes, typeUInt32, sizeof(UInt32), &imove);
				if(err != noErr){
					break;
				}
				
				break;
			}
		}
		
		if(err != noErr){
			THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR,
							  "Create event or set event parameters failed.",
							  "SListener::sendEvent");
		}
		
		SendEventToEventTarget(evn, GetControlEventTarget(aReceiver->handle()));
	}
	
} // namespace Sagitta