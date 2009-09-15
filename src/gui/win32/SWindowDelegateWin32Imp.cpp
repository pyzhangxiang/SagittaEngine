//////////////////////////////////////////////////////
// file: SWindowDelegateWin32Imp.cpp @ 2008-12-8 by Zhang Xiang
// defines of the class SWindowDelegate
// SWindowDelegate is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "Gui/SWindowDelegate.h"
#include "Gui/SListener.h"
#include "Utils/SString.h"
#include "Utils/SException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	////////////////////////////////////////////////////////////////////////// SWindowDelegate //////////////////////////////////////////////////////////////////////////

	//  [12/12/2008 zhangxiang]
	SWindowDelegate::SWindowDelegate(uLong aBaseWndClassStyle, uLong aBaseWndAttributes, uLong aBaseWndExAttributes)
		: m_iWndClassStyle(aBaseWndClassStyle), m_iWndAttributes(aBaseWndAttributes), m_iWndExAttributes(aBaseWndExAttributes){
	
	}

	//  [12/12/2008 zhangxiang]
	SWindowDelegate::SWindowDelegate(void)
		: m_iWndClassStyle(CS_HREDRAW | CS_VREDRAW), m_iWndAttributes(WS_OVERLAPPEDWINDOW), m_iWndExAttributes(0){

	}

	/*																	--- outmoded ---
	//  [12/12/2008 zhangxiang]
	std::string SWindowDelegate::_setWindowStyle(void){
		return _setWindowStyle(CS_HREDRAW | CS_VREDRAW, WS_OVERLAPPEDWINDOW, 0);
	}

	//  [12/12/2008 zhangxiang]
	std::string SWindowDelegate::_setWindowStyle(uLong aWndClassStyle, uLong aWndAttributes, uLong aWndExAttributes){
		m_iWndClassStyle |= aWndClassStyle;
		m_iWndAttributes |= aWndAttributes;
		m_iWndExAttributes |= aWndExAttributes;

		std::string wcname = SString::to_string(m_iWndClassStyle);
						
		WNDCLASSEX wc;
		if(!GetClassInfoEx(0, wcname.c_str(), &wc)){
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = m_iWndClassStyle;
			wc.lpfnWndProc = SListener::dispatchEvent;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = 0;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = 0;
			wc.lpszClassName = wcname.c_str();
			wc.hIconSm = 0;
	
			if(!RegisterClassEx(&wc)){
				THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Register window class failed.", "SWindow::_registerWndClass");
			}
		}

		return wcname;
	}
	*/

	SWindowHandle SWindowDelegate::_createWindow(const std::string &aTitle, SRect aRect, SWindowHandle aHParent, SWindow *aBindedWindow){
		/*std::string wcname = _setWindowStyle();	--- outmoded --- */

		std::string wcname = SString::to_string(m_iWndClassStyle);

		WNDCLASSEX wc;
		if(!GetClassInfoEx(0, wcname.c_str(), &wc)){
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = m_iWndClassStyle;
			wc.lpfnWndProc = SListener::dispatchEvent;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = 0;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wc.lpszMenuName = 0;
			wc.lpszClassName = wcname.c_str();
			wc.hIconSm = 0;

			if(!RegisterClassEx(&wc)){
				THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Register window class failed.", "SWindow::_createWindow");
			}
		}

		int x = aRect.left;
		int y = aRect.top;
		int w = aRect.right - aRect.left;
		int h = aRect.bottom - aRect.top;
		SWindowHandle hWnd = CreateWindowEx(m_iWndExAttributes,
											wcname.c_str(),
											aTitle.c_str(),
											m_iWndAttributes,
											x, y, w, h,
											aHParent, 0, 0, aBindedWindow);
		if(!hWnd)
			THROW_SAGI_EXCEPT(SException::ERR_INTERNAL_ERROR, "Create window failed.", "SWindow::_createWindow");
		return hWnd;
	}

	//  [12/10/2008 zhangxiang]
	bool SWindowDelegate::showWindow(SWindowHandle aHandle){
		return static_cast<bool>(::ShowWindow(aHandle, SW_SHOWNORMAL));
	}

	//  [12/14/2008 zhangxiang]
	bool SWindowDelegate::updateWindow(SWindowHandle aHandle){
		return static_cast<bool>(::UpdateWindow(aHandle));
	}

	//  [12/11/2008 zhangxiang]
	bool SWindowDelegate::hideWindow(SWindowHandle aHandle){
		return static_cast<bool>(::ShowWindow(aHandle, SW_HIDE));
	}

	//  [12/11/2008 zhangxiang]
	bool SWindowDelegate::moveWindow(SWindowHandle aHandle, int aNewX, int aNewY, int aNewWidth, int aNewHeight){
		return static_cast<bool>(::MoveWindow(aHandle, aNewX, aNewY, aNewWidth, aNewHeight, true));
	}

	//  [12/14/2008 zhangxiang]
	bool SWindowDelegate::destroyWindow(SWindowHandle aHandle){
		return static_cast<bool>(::DestroyWindow(aHandle));
	}

	//  [12/14/2008 zhangxiang]
	bool SWindowDelegate::validateRect(SWindowHandle aHandle, const SRect *aRect){
		if(aRect){
			RECT rect;
			rect.left = aRect->left;
			rect.right = aRect->right;
			rect.top = aRect->top;
			rect.bottom = aRect->bottom;
			return static_cast<bool>(::ValidateRect(aHandle, &rect));
		}else{
			return static_cast<bool>(::ValidateRect(aHandle, 0));
		}
	}

	//  [12/14/2008 zhangxiang]
	bool SWindowDelegate::invalidateRect(SWindowHandle aHandle, const SRect *aRect, bool bErase){
		if(aRect){
			RECT rect;
			rect.left = aRect->left;
			rect.right = aRect->right;
			rect.top = aRect->top;
			rect.bottom = aRect->bottom;
			return static_cast<bool>(::InvalidateRect(aHandle, &rect, bErase));
		}else{
			return static_cast<bool>(::InvalidateRect(aHandle, 0, bErase));
		}
	}

	//  [12/14/2008 zhangxiang]
	bool SWindowDelegate::isWindowVisible(SWindowHandle aHandle){
		return static_cast<bool>(::IsWindowVisible(aHandle));
	}

	//  [12/16/2008 zhangxiang]
	bool SWindowDelegate::isWindow(SWindowHandle aHandle){
		return static_cast<bool>(::IsWindow(aHandle));
	}

	SRect SWindowDelegate::getWindowBounds(SWindowHandle aHandle){
		RECT wrect;
		::GetClientRect(aHandle, &wrect);
		return SRect(wrect.left, wrect.top, wrect.right, wrect.bottom);
	}

	////////////////////////////////////////////////////////////////////////// SWindowDelegate //////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////// SMainFrameDelegate //////////////////////////////////////////////////////////////////////////

	//  [12/12/2008 zhangxiang]
	SMainFrameDelegate::SMainFrameDelegate(void)
		: SWindowDelegate(CS_HREDRAW | CS_VREDRAW, WS_OVERLAPPEDWINDOW, 0){

	}

	//  [12/13/2008 zhangxiang]
	SMainFrameDelegate::SMainFrameDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
		: SWindowDelegate(CS_HREDRAW | CS_VREDRAW | aAddWndClassStyle,
		WS_OVERLAPPEDWINDOW | aAddWndAttributes,
		0 | aAddWndExAttributes){

	}

	/*											--- outmoded ---
	//  [12/12/2008 zhangxiang]
	std::string SMainFrameDelegate::_setWindowStyle(void){
		return SWindowDelegate::_setWindowStyle(0, 0, 0);
	}
	*/

	////////////////////////////////////////////////////////////////////////// SMainFrameDelegate //////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////// SWidgetFrameDelegate //////////////////////////////////////////////////////////////////////////

	//  [12/12/2008 zhangxiang]
	SWidgetDelegate::SWidgetDelegate(void)
		: SWindowDelegate(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, WS_CHILD/* | WS_VISIBLE*/, 0){

	}

	//  [12/13/2008 zhangxiang]
	SWidgetDelegate::SWidgetDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
		: SWindowDelegate(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | aAddWndClassStyle,
		WS_CHILD/* | WS_VISIBLE*/ | aAddWndAttributes,
		0 | aAddWndExAttributes){

	}

	/*											--- outmoded ---
	//  [12/12/2008 zhangxiang]
	std::string SWidgetDelegate::_setWindowStyle(void){
		return SWindowDelegate::_setWindowStyle(0, 0, 0);
	}
	*/

	////////////////////////////////////////////////////////////////////////// SWidgetFrameDelegate //////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////// SViewDelegate //////////////////////////////////////////////////////////////////////////

	//  [12/13/2008 zhangxiang]
	SViewDelegate::SViewDelegate(void)
		: SWidgetDelegate(0, 0, 0){

	}

	//  [12/13/2008 zhangxiang]
	SViewDelegate::SViewDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
		: SWidgetDelegate(aAddWndClassStyle, aAddWndAttributes, aAddWndExAttributes){

	}

	////////////////////////////////////////////////////////////////////////// SViewDelegate //////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////// SGraphicsViewDelegate //////////////////////////////////////////////////////////////////////////

	//  [12/13/2008 zhangxiang]
	SGraphicsViewDelegate::SGraphicsViewDelegate(void)
		: SViewDelegate(CS_OWNDC, WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0){

	}

	//  [12/13/2008 zhangxiang]
	SGraphicsViewDelegate::SGraphicsViewDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes)
		: SViewDelegate(CS_OWNDC | aAddWndClassStyle,
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | aAddWndAttributes,
		0 | aAddWndExAttributes){

	}

	////////////////////////////////////////////////////////////////////////// SGraphicsViewDelegate //////////////////////////////////////////////////////////////////////////

} // namespace Sagitta