//////////////////////////////////////////////////////
// file: SWindowDelegateAppleImp.cpp 
// created by zhangxiang on 08-12-17
// defines of the class SWindowDelegate
// SWindowDelegate is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SWindowDelegate.h"
#include "../SListener.h"
#include "../../common/utils/sgStringUtil.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	////////////////////////////////////////// SWindowDelegate //////////////////////////////////////////
	
	SWindowDelegate::SWindowDelegate(uLong aBaseWndClassStyle, uLong aBaseWndAttributes, uLong aBaseWndExAttributes)
	: m_iWndClassStyle(aBaseWndClassStyle),
	m_iWndAttributes(aBaseWndAttributes),
	m_iWndExAttributes(aBaseWndExAttributes){
		
	}
	
	SWindowDelegate::SWindowDelegate(void)
	: m_iWndClassStyle(kDocumentWindowClass),
	m_iWndAttributes(kWindowStandardDocumentAttributes | kWindowStandardHandlerAttribute),
	m_iWndExAttributes(kHIViewFeatureAllowsSubviews){
		
	}
	
	SWindowHandle SWindowDelegate::_createWindow(const std::string &aTitle, SRect aRect, SWindowHandle aHParent, SWindow *aBindedWindow){
		OSStatus err = noErr;

		// HIView initialization event
		EventRef initEvent;
		err = CreateEvent(0, kEventClassHIObject, kEventHIObjectInitialize,
						  GetCurrentEventTime(), 0, &initEvent);
		
		// error code
		// ...
		
		err = SetEventParameter(initEvent, 'w', typeVoidPtr, sizeof(SWindow**), &aBindedWindow);
		// error code ...
		
		HIViewRef parent_hiview;
		HIViewRef new_hiview;
		
		if(!aHParent){
			// main frame, create a new window
			WindowRef window;
			Rect wrect = { aRect.top, aRect.left, aRect.bottom, aRect.right };
			err = CreateNewWindow(m_iWndClassStyle, m_iWndAttributes, &wrect, &window);
			if(err != noErr){
				THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Create window failed.", "SWindowDelegate::_createWindow");
			}
		
			static const EventTypeSpec kWindowEventList[] ={
		//		{ kEventClassApplication, kEventAppQuit},
				{ kEventClassCommand, kEventCommandProcess },
			//	{ kEventClassWindow, kEventWindowInit },
				{ kEventClassWindow, kEventWindowShown },
				{ kEventClassWindow, kEventWindowActivated },
				{ kEventClassWindow, kEventWindowClose },
				{ kEventClassWindow, kEventWindowClosed },	// true destroy
			//	{ kEventClassWindow, kEventWindowDrawContent },
				{ kEventClassWindow, kEventWindowBoundsChanged },
				{ kEventClassKeyboard, kEventRawKeyDown },
				{ kEventClassKeyboard, kEventRawKeyUp },
				{ kEventClassKeyboard, kEventRawKeyRepeat },
				{ kEventClassMouse, kEventMouseDown },
				{ kEventClassMouse, kEventMouseUp },
				{ kEventClassMouse, kEventMouseMoved },
				{ kEventClassMouse, kEventMouseDragged },
				{ kEventClassMouse, kEventMouseWheelMoved },
			};		
		
			// set the binded mainframe window to be the userdata
			InstallWindowEventHandler(window, NewEventHandlerUPP(SListener::dispatchEvent),
									  GetEventTypeCount(kWindowEventList), kWindowEventList,
									  aBindedWindow, 0);
			
			// then create a HIView to be the root widget in the window
			static const EventTypeSpec sRootHIViewEventList[] = {
				{ kEventClassHIObject, kEventHIObjectConstruct },
				{ kEventClassHIObject, kEventHIObjectDestruct },
				{ kEventClassHIObject, kEventHIObjectInitialize },
				
				{ kEventClassControl, kEventControlDraw },
				//	{ kEventClassControl, kEventControlHitTest },
			//	{ kEventClassControl, kEventControlGetClickActivation },
			//	{ kEventClassControl, kEventControlSetFocusPart },
			//	{ kEventClassControl, kEventControlBoundsChanged }
			};
			
			// register HIView class
			static HIObjectClassRef sRootHIViewClass = 0;
			static CFStringRef sRootHIViewClassID = CFSTR("SagittaRootHIView");
			if(!sRootHIViewClass){
				err = HIObjectRegisterSubclass(sRootHIViewClassID, kHIViewClassID,
											   0, SListener::dispatchEvent,
											   GetEventTypeCount(sRootHIViewEventList),
											   sRootHIViewEventList, 0, &sRootHIViewClass);
				//	verify_noerr(err);
			}
			
			// create root HIView
			err = HIObjectCreate(sRootHIViewClassID, initEvent, (HIObjectRef*)&new_hiview);
			if(err != noErr){
				THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Create HIView failed.", "SWindowDelegate::_createWindow");
			}
			
			// set hiview's features
			HIViewFeatures oriFea;
			HIViewGetFeatures(new_hiview, &oriFea);
			HIViewChangeFeatures(new_hiview, m_iWndExAttributes, oriFea);
			
			// set parent
			::GetRootControl(window, &parent_hiview);
			HIViewAddSubview(parent_hiview, new_hiview);
			
			// set frame
			HIRect contentRect;
			HIViewGetBounds(parent_hiview, &contentRect);
			HIViewSetFrame(new_hiview, &contentRect);
			
			// set hiview to be the root widget
			HILayoutInfo li;
			li.version = kHILayoutInfoVersionZero;
			
			err = HIViewGetLayoutInfo(new_hiview, &li);
			if(err != noErr){
				THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Get root view's layout info failed.", "SWindowDelegate::_createWindow");
			}
			
			li.scale.x.toView = parent_hiview;
			li.scale.x.kind = kHILayoutScaleAbsolute;
			li.scale.x.ratio = 1.0;
			li.scale.y.toView = parent_hiview;
			li.scale.y.kind = kHILayoutScaleAbsolute;
			li.scale.y.ratio = 1.0;
			
			err = HIViewSetLayoutInfo(new_hiview, &li);
			if(err != noErr){
				THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Set root view's layout info failed.", "SWindowDelegate::_createWindow");
			}
		
		}else{
			// widget, create custrom HIView for widget
			static const EventTypeSpec sWidgetHIViewEventList[] = {
				{ kEventClassHIObject, kEventHIObjectConstruct },
				{ kEventClassHIObject, kEventHIObjectDestruct },
				{ kEventClassHIObject, kEventHIObjectInitialize },
				
				{ kEventClassControl, kEventControlDraw },
				{ kEventClassControl, kEventControlHitTest },
				{ kEventClassControl, kEventControlGetClickActivation },
				{ kEventClassControl, kEventControlSetFocusPart },
				{ kEventClassControl, kEventControlBoundsChanged }
			};
			
			// register widget HIView class
			static HIObjectClassRef sWidgetHIViewClass = 0;
			static CFStringRef sWidgetHIViewClassID = CFSTR("SagittaHIView");
			if(!sWidgetHIViewClass){
				err = HIObjectRegisterSubclass(sWidgetHIViewClassID, kHIViewClassID,
											   0, SListener::dispatchEvent,
											   GetEventTypeCount(sWidgetHIViewEventList),
											   sWidgetHIViewEventList, 0, &sWidgetHIViewClass);
				//	verify_noerr(err);
			}
			
			// create widget HIView
			err = HIObjectCreate(sWidgetHIViewClassID, initEvent, (HIObjectRef*)&new_hiview);
			if(err != noErr){
				THROW_SAGI_EXCEPT(sgException::ERR_INTERNAL_ERROR, "Create HIView failed.", "SWindowDelegate::_createWindow");
			}
			
			// set hiview's features
			HIViewFeatures oriFea;
			HIViewGetFeatures(new_hiview, &oriFea);
			HIViewChangeFeatures(new_hiview, m_iWndExAttributes, oriFea);
			
			// set parent
			parent_hiview = aHParent;
			HIViewAddSubview(parent_hiview, new_hiview);
		
			// set frame
			HIRect vrect = {
				{ aRect.left, aRect.top }, { aRect.right - aRect.left, aRect.bottom - aRect.top }
			}; 
			HIViewSetFrame(new_hiview, &vrect);
		}
		
		return (SWindowHandle)new_hiview;
	}
	
	bool SWindowDelegate::showWindow(SWindowHandle aHandle){
		//ShowWindow(aHandle);
		bool ret = static_cast<bool>(HIViewIsVisible((HIViewRef)aHandle));
		HIViewSetVisible((HIViewRef)aHandle, true);
		return ret;
	}
	
	bool SWindowDelegate::updateWindow(SWindowHandle aHandle){
		// There's no update window function in carbon.
		// So, do nothing.
		return true;
	}
	
	bool SWindowDelegate::hideWindow(SWindowHandle aHandle){
		bool ret = static_cast<bool>(HIViewIsVisible((HIViewRef)aHandle));
		HIViewSetVisible((HIViewRef)aHandle, true);
		return ret;
	}
	
	bool SWindowDelegate::moveWindow(SWindowHandle aHandle, int aNewX, int aNewY, int aNewWidth, int aNewHeight){
		HIRect rect = { { aNewX, aNewY }, { aNewWidth, aNewHeight } };
		HIViewSetFrame((HIViewRef)aHandle, &rect);
		return true;
	}
	
	bool SWindowDelegate::destroyWindow(SWindowHandle aHandle){
		CFRelease(aHandle);
		return true;
	}
	
	bool SWindowDelegate::validateRect(SWindowHandle aHandle, const SRect *aRect){
		OSStatus err = noErr;
		if(aRect){
			HIRect rect = { { aRect->left, aRect->top }, { aRect->right - aRect->left, aRect->bottom - aRect->top } };
			err = HIViewSetNeedsDisplayInRect((HIViewRef)aHandle, &rect, false);
		}else{
			err = HIViewSetNeedsDisplay((HIViewRef)aHandle, false);
		}
		
		if(err == noErr){
			return true;
		}else{
			return false;
		}
	}
	
	bool SWindowDelegate::invalidateRect(SWindowHandle aHandle, const SRect *aRect, bool bErase){
		OSStatus err = noErr;
		if(aRect){
			HIRect rect = { { aRect->left, aRect->top }, { aRect->right - aRect->left, aRect->bottom - aRect->top } };
			err = HIViewSetNeedsDisplayInRect((HIViewRef)aHandle, &rect, true);
		}else{
			err = HIViewSetNeedsDisplay((HIViewRef)aHandle, true);
		}
		
		if(err == noErr){
			return true;
		}else{
			return false;
		}
	}
	
	bool SWindowDelegate::isWindowVisible(SWindowHandle aHandle){
		return static_cast<bool>(HIViewIsVisible((HIViewRef)aHandle));
	}
	
	bool SWindowDelegate::isWindow(SWindowHandle aHandle){
		return static_cast<bool>(HIViewIsValid((HIViewRef)aHandle));
	}
	
	SRect SWindowDelegate::getWindowBounds(SWindowHandle aHandle){
		HIRect vrect;
		::HIViewGetBounds((HIViewRef)aHandle, &vrect);
		return SRect((int)vrect.origin.x, (int)vrect.origin.y,
					 (int)(vrect.origin.x + vrect.size.width),
					 (int)(vrect.origin.y + vrect.size.height));
	}
	
	////////////////////////////////////////// SWindowDelegate //////////////////////////////////////////
	
	
	////////////////////////////////////////// SMainFrameDelegate //////////////////////////////////////////
	
	SMainFrameDelegate::SMainFrameDelegate(void)
	: SWindowDelegate(kDocumentWindowClass, 
					  kWindowCompositingAttribute | kWindowStandardDocumentAttributes |
					  kWindowStandardHandlerAttribute, kHIViewFeatureAllowsSubviews){
		
	}
	
	SMainFrameDelegate::SMainFrameDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
	: SWindowDelegate(kDocumentWindowClass | aAddWndClassStyle,
					  kWindowCompositingAttribute | kWindowStandardDocumentAttributes |
					  kWindowStandardHandlerAttribute | aAddWndAttributes,
					  kHIViewFeatureAllowsSubviews | aAddWndExAttributes){
		
	}
	
	bool SMainFrameDelegate::showWindow(SWindowHandle aHandle){
		WindowRef window = ::HIViewGetWindow((HIViewRef)aHandle);
		bool ret = static_cast<bool>(::IsWindowVisible(window));
	/*	if(!ret){	//---- no need any more ----
			// if is not visible before, send move and resize event
			Rect rect;
			::GetWindowBounds(window, kWindowContentRgn, &rect);
			::MoveWindow(window, rect.left + 10, rect.top + 10, true);
			::SizeWindow(window, rect.right - rect.left, rect.bottom - rect.top, true);
		}
	*/ 
		// show the true window
		::ShowWindow(window);
		// show the root view
		SWindowDelegate::showWindow(aHandle);
		return ret;
	}
	
	bool SMainFrameDelegate::hideWindow(SWindowHandle aHandle){
		WindowRef window = ::HIViewGetWindow((HIViewRef)aHandle);
		bool ret = static_cast<bool>(::IsWindowVisible(window));
		// hide the root view
		SWindowDelegate::hideWindow(aHandle);
		// hide the true window
		::HideWindow(window);
		return ret;
	}
	
	bool SMainFrameDelegate::moveWindow(SWindowHandle aHandle, int aNewX, int aNewY, int aNewWidth, int aNewHeight){
		WindowRef window = ::HIViewGetWindow((HIViewRef)aHandle);
		::MoveWindow(window, aNewX, aNewY, true);
		::SizeWindow(window, aNewWidth, aNewHeight, true);
		return true;
	}
	
	bool SMainFrameDelegate::destroyWindow(SWindowHandle aHandle){
		WindowRef window = ::HIViewGetWindow((HIViewRef)aHandle);
		// first dispose the root view -- no need, all views in the window will be dispose automatically
	//	SWindowDelegate::destroyWindow(aHandle);
		// then I can dispose myself - the true window
		::DisposeWindow(window);
		return true;
	}
	
	bool SMainFrameDelegate::isWindowVisible(SWindowHandle aHandle){
		return static_cast<bool>(::IsWindowVisible(::HIViewGetWindow((HIViewRef)aHandle)));
	}
	
	bool SMainFrameDelegate::isWindow(SWindowHandle aHandle){
		return static_cast<bool>(::IsValidWindowPtr(::HIViewGetWindow((HIViewRef)aHandle)));
	}
	
	SRect SMainFrameDelegate::getWindowBounds(SWindowHandle aHandle){
		Rect wrect;
		::GetWindowBounds(::HIViewGetWindow((HIViewRef)aHandle), kWindowContentRgn, &wrect);
		return SRect(wrect.left, wrect.top, wrect.right, wrect.bottom);
	}
	
	////////////////////////////////////////// SMainFrameDelegate //////////////////////////////////////////
	
	
	////////////////////////////////////////// SWidgetDelegate //////////////////////////////////////////
	
	SWidgetDelegate::SWidgetDelegate(void)
	: SWindowDelegate(kDrawerWindowClass, kWindowCompositingAttribute | kWindowStandardHandlerAttribute, 0){
		
	}
	
	SWidgetDelegate::SWidgetDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
	: SWindowDelegate(kDrawerWindowClass | aAddWndClassStyle,
					  kWindowCompositingAttribute | kWindowStandardHandlerAttribute | aAddWndAttributes,
					  0 | aAddWndExAttributes){
		
	}
	
	////////////////////////////////////////// SWidgetDelegate //////////////////////////////////////////
	
	
	////////////////////////////////////////// SViewDelegate //////////////////////////////////////////
	
	SViewDelegate::SViewDelegate(void)
	: SWidgetDelegate(0, 0, 0){
		
	}
	
	SViewDelegate::SViewDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
	: SWidgetDelegate(aAddWndClassStyle, aAddWndAttributes, aAddWndExAttributes){
		
	}
	
	////////////////////////////////////////// SViewDelegate //////////////////////////////////////////
	
	
	////////////////////////////////////////// SGraphicsViewDelegate //////////////////////////////////////////
	
	SGraphicsViewDelegate::SGraphicsViewDelegate(void)
	: SViewDelegate(0, 0, 0){
		
	}
	
	SGraphicsViewDelegate::SGraphicsViewDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
	: SViewDelegate(aAddWndClassStyle, aAddWndAttributes, aAddWndExAttributes){
		
	}
	
	////////////////////////////////////////// SGraphicsViewDelegate //////////////////////////////////////////
	
} // namespace Sagitta