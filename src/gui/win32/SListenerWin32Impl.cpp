//////////////////////////////////////////////////////
// file: SListenerWin32Impl.cpp 
// created by zhangxiang on 09-01-07
// define of the class SListener
// SListener is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SListener.h"
#include "../SWindow.h"
#include "../SApplication.h"
#include "../SEvent.h"
#include "../SKeyMapping.h"
#include "../../common/utils/sgBitOP.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SStatus SListener::dispatchEvent(HWND aHWnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam){
		SStatus err = NONPROCESSEDERR;

		// window create event.
		// Let application handle it to register it to the single application. May be for future...
		// But now, do it in SWindow's constructor.
		if(aMessage == WM_CREATE){
			if(static_cast<SListener*>(SApplication::instance_ptr())->handleEvent(SCreateEvent(SEvent::ET_CREATE, aHWnd, (SListener*)((LPCREATESTRUCT)aLParam)->lpCreateParams)) != NOERR){
				goto DEFHANDLER;
			}

			return NOERR;
		}

		SListener *receiver = (SListener*)(SApplication::instance().findWindow(aHWnd));	
		if(!receiver){
			goto DEFHANDLER;
		}

		switch(aMessage){
			case WM_DESTROY:
				{
					err = static_cast<SListener*>(SApplication::instance_ptr())->handleEvent(SEvent(SEvent::ET_DESTROY, aWParam, aLParam, receiver));
				}
				break;

			case WM_CLOSE:
				{
					err = receiver->handleEvent(SEvent(SEvent::ET_CLOSE, aWParam, aLParam, receiver));
				}
				break;

			case WM_SIZE:
				{
					err = receiver->handleEvent(SResizeEvent(SEvent::ET_RESIZE, sgBitOP::lo16(aLParam)/* & 0xffff*/, sgBitOP::hi16(aLParam)/* >> 16*/, receiver));
				}
				break;

			case WM_MOVE:
				{
					err = receiver->handleEvent(SMoveEvent(SEvent::ET_MOVE, (int)sgBitOP::lo16(aLParam), (int)sgBitOP::hi16(aLParam), receiver));
				}
				break;

			case WM_PAINT:
				{
					err = receiver->handleEvent(SEvent(SEvent::ET_PAINT, aWParam, aLParam, receiver));
				}
				break;

			case WM_ERASEBKGND:
				{
					err = receiver->handleEvent(SEraseBkgndEvent(SEvent::ET_ERASEBKGND, (SContextHandle)aWParam, receiver));
					if(err == NOERR)
						return 1;
				}
				break;

				// key event
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYUP:
			case WM_CHAR:
			case WM_SYSCHAR:
				{
					uLong keycode = Sagitta::Key_Unknown;
					uLong modifiers = SKeyMapping::CheckModifierKeys();
					uLong count = sgBitOP::lo16(aLParam);
					SEvent::EventType type = SEvent::ET_INVALID;

					switch(aMessage){
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				{
					type = SEvent::ET_KEY_DOWN;
					keycode = SKeyMapping::SagiKey(aWParam);
				}
				break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				{
					type = SEvent::ET_KEY_UP;
					keycode = SKeyMapping::SagiKey(aWParam);
				}
				break;

			case WM_CHAR:
			case WM_SYSCHAR:
				{
					type = SEvent::ET_CHAR;
					keycode = aWParam;
				}
				break;
					}

					err = receiver->handleEvent(SKeyEvent(type, keycode, modifiers, count, receiver));
				}
				break;

				// mouse event
			case WM_MOUSEWHEEL:
				{
					int zdelta = (short)(sgBitOP::hi16(aWParam));
					uLong modifiers = SKeyMapping::SagiModifierKeys(sgBitOP::lo16(aWParam));
					SPoint point(sgBitOP::lo16(aLParam), sgBitOP::hi16(aLParam));

					err = receiver->handleEvent(SWheelEvent(SEvent::ET_MOUSE_WHEEL, zdelta, modifiers, point, receiver));
				}
				break;

			case WM_MOUSEMOVE:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDBLCLK:
				{
					uLong button = Sagitta::MBT_NON;
					uLong modifiers = SKeyMapping::SagiModifierKeys(aWParam);
					SPoint point(sgBitOP::lo16(aLParam), sgBitOP::hi16(aLParam));
					SEvent::EventType type = SEvent::ET_INVALID;

					switch(aMessage){
			case WM_MOUSEMOVE:
				{
					if(modifiers & Sagitta::MKS_LButton)
						button = Sagitta::MBT_Left;
					else if(modifiers & Sagitta::MKS_RButton)
						button = Sagitta::MBT_Right;
					else if(modifiers & Sagitta::MKS_MButton)
						button = Sagitta::MBT_Middle;
					else
						button = Sagitta::MBT_NON;
					type = SEvent::ET_MOUSE_MOVE;
				}
				break;

			case WM_LBUTTONDOWN:
				{
					button = Sagitta::MBT_Left;
					type = SEvent::ET_LBUTTON_DOWN;
				}
				break;

			case WM_LBUTTONUP:
				{
					button = Sagitta::MBT_Left;
					type = SEvent::ET_LBUTTON_UP;
				}
				break;

			case WM_LBUTTONDBLCLK:
				{
					button = Sagitta::MBT_Left;
					type = SEvent::ET_LBUTTON_DBLCLK;
				}
				break;

			case WM_RBUTTONDOWN:
				{
					button = Sagitta::MBT_Right;
					type = SEvent::ET_RBUTTON_DOWN;
				}
				break;

			case WM_RBUTTONUP:
				{
					button = Sagitta::MBT_Right;
					type = SEvent::ET_RBUTTON_UP;
				}
				break;

			case WM_RBUTTONDBLCLK:
				{
					button = Sagitta::MBT_Right;
					type = SEvent::ET_RBUTTON_DBLCLK;
				}
				break;

			case WM_MBUTTONDOWN:
				{
					button = Sagitta::MBT_Middle;
					type = SEvent::ET_MBUTTON_DOWN;
				}
				break;

			case WM_MBUTTONUP:
				{
					button = Sagitta::MBT_Middle;
					type = SEvent::ET_MBUTTON_UP;
				}
				break;

			case WM_MBUTTONDBLCLK:
				{
					button = Sagitta::MBT_Middle;
					type = SEvent::ET_MBUTTON_DBLCLK;
				}
				break;
					}

					err = receiver->handleEvent(SMouseEvent(type, button, modifiers, point, receiver));
				}
				break;

			default:
				goto DEFHANDLER;
		}

		if(err == NOERR)
			return NOERR;

DEFHANDLER:
		return static_cast<SStatus>(DefWindowProc(aHWnd, aMessage, aWParam, aLParam));

	}

	void SListener::sendEvent(const SEvent &aEvent, SWindow *aReceiver){
		if(!aReceiver){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null receiver pointer.", "SListener::sendEvent");
		}

		UINT msg;
		WPARAM wparam = 0;
		LPARAM lparam = 0;
		switch(aEvent.type()){
			case SEvent::ET_DESTROY:
			{
				msg = WM_DESTROY;
				break;
			}

			case SEvent::ET_CLOSE:
			{
				msg = WM_CLOSE;
				break;
			}

			/*case SEvent::ET_RESIZE:
			{
				msg = WM_SIZE;
				break;
			}*/

			case SEvent::ET_MOVE:
			{
				msg = WM_MOVE;
				wparam = 0;
				lparam = ((uInt)(aEvent.param1()) & 0xffff) + ((uInt)(aEvent.param2()) << 16);
				break;
			}

			case SEvent::ET_PAINT:
			{
				msg = WM_PAINT;
				break;
			}

			default:
			{
				goto DONOTPROCESS;
			}
		}

		::PostMessage(aReceiver->handle(), msg, wparam, lparam);

DONOTPROCESS:
		;
	}

} // namespace Sagitta