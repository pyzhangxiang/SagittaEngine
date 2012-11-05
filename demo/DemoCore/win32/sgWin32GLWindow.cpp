//
//  sgNSOpenGLView.m
//  cocoaogl
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "sgWin32GLWindow.h"
#include "sgWin32ToSagitta.h"
#include "../sgDemo.h"
#include <engine/common/sgBitOP.h>
#include <engine/common/sgException.h>
#include <iostream>
 
LRESULT CALLBACK SagittaWinProc(HWND aHWnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam);

sgWin32GLWindow::sgWin32GLWindow( void )
: mhwnd(0)
{

}

sgWin32GLWindow::~sgWin32GLWindow( void )
{
	releaseRenderEnv();
}

void sgWin32GLWindow::setRect( int x, int y, int w, int h )
{
	mX = x;
	mY = y;
	mW = w;
	mH = h;
}

void sgWin32GLWindow::setTitle( const std::string &title )
{
	mTitle = title;
}

HWND sgWin32GLWindow::create( void )
{
	if(mhwnd)
		return mhwnd;

	static std::string wcname = "SagittaGLWindow";

	WNDCLASSEX wc;
	if(!GetClassInfoEx(0, wcname.c_str(), &wc)){
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = SagittaWinProc;
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
			THROW_SAGI_EXCEPT(Sagitta::sgException::ERR_INTERNAL_ERROR, "Register window class failed.", "sgWin32GLWindow::create");
		}
	}

	mhwnd = CreateWindowEx(0,
		wcname.c_str(),
		mTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		mX, mY, mW, mH,
		0, 0, 0, 0);
	if(!mhwnd)
		THROW_SAGI_EXCEPT(Sagitta::sgException::ERR_INTERNAL_ERROR, "Create window failed.", "sgWin32GLWindow::create");

	// opengl context
	/*HDC */mhdc = ::GetDC(mhwnd);
	if(!mhdc)
	{
		THROW_SAGI_EXCEPT(Sagitta::sgException::ERR_INTERNAL_ERROR, "Create window failed, no hdc.", "sgWin32GLWindow::create");
	}
	//try{
		buildRenderEnv(mhdc);
	//}catch(Sagitta::sgException &e){
		;
	//}
	//::ReleaseDC(mhwnd, mhdc);

	::ShowWindow(mhwnd, SW_SHOWNORMAL);
	::UpdateWindow(mhwnd);

	return mhwnd;
}

void sgWin32GLWindow::buildRenderEnv( HDC mhdc )
{
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ::ChoosePixelFormat(mhdc, &pfd);
	if(!iFormat){
		THROW_SAGI_EXCEPT(Sagitta::sgException::ERR_INTERNAL_ERROR, "Choose pixel format failed.", "sgWin32GLWindow::buildRenderEnv(void)");
	}

	::SetPixelFormat(mhdc, iFormat, &pfd);
	mGLContext = ::wglCreateContext(mhdc);
	if(!mGLContext){
		THROW_SAGI_EXCEPT(Sagitta::sgException::ERR_INTERNAL_ERROR, "Create GL render context failed.", "sgWin32GLWindow::buildRenderEnv(void)");
	}

	if(!::wglMakeCurrent(mhdc, mGLContext)){
		::wglDeleteContext(mGLContext);
		THROW_SAGI_EXCEPT(Sagitta::sgException::ERR_INTERNAL_ERROR, "Make current GL render context failed.", "sgWin32GLWindow::buildRenderEnv(void)");
	}

	GetApp()->initialize();
}

void sgWin32GLWindow::releaseRenderEnv( void )
{
//	HDC mhdc = ::GetDC(mhwnd);
	if(mhdc){
		::wglMakeCurrent(0, 0);
		::wglDeleteContext(mGLContext);
		::ReleaseDC(mhwnd, mhdc);
	}
}

LRESULT CALLBACK SagittaWinProc(HWND aHWnd, UINT aMessage, WPARAM aWParam, LPARAM aLParam){
	LRESULT err = 0;

	switch(aMessage){
	case WM_CREATE:
		{
			GetApp()->OnCreate();
		}
		return err;

	case WM_DESTROY:
		{
			GetApp()->OnDestroy();
			PostQuitMessage(0);
		}
		break;

	//case WM_CLOSE:
	//	{
	//		//
	//	}
	//	break;

	case WM_SIZE:
		{
			GetApp()->resize(Sagitta::sgBitOP::lo16(aLParam)/* & 0xffff*/, Sagitta::sgBitOP::hi16(aLParam)/* >> 16*/);
		}
		return err;

	//case WM_MOVE:
	//	{
	//		//
	//	}
	//	break;

	//case WM_PAINT:
	//	{
	//		//
	//	}
	//	break;

	//case WM_ERASEBKGND:
	//	{
	//		//
	//	}
	//	break;

		// key event
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			sgKeyEvent e;
			e.key = sgWin32ToSagitta::getKey(aWParam);
			e.modifiers = sgWin32ToSagitta::getMofifiers();
			if(Sagitta::sgBitOP::lo16(aLParam) > 1)
				e.repeat = true;
			GetApp()->keyPressEvent(e);
		}
		return err;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			sgKeyEvent e;
			e.key = sgWin32ToSagitta::getKey(aWParam);
			e.modifiers = sgWin32ToSagitta::getMofifiers();;
			if(Sagitta::sgBitOP::lo16(aLParam) > 1)
				e.repeat = true;
			GetApp()->keyPressEvent(e);
		}
		return err;
	/*case WM_CHAR:
	case WM_SYSCHAR:
		{
			keycode = aWParam;
		}
		break;
*/
		// mouse event
	case WM_MOUSEWHEEL:
		{
			sgWheelEvent e;
			e.delta = (float)(Sagitta::sgBitOP::hi16(aWParam));
			e.modifiers = sgWin32ToSagitta::getMofifiers(Sagitta::sgBitOP::lo16(aWParam));
			e.x = Sagitta::sgBitOP::lo16(aLParam);
			e.y = Sagitta::sgBitOP::hi16(aLParam);
			GetApp()->wheelEvent(e);
		}
		return err;

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
			sgMouseEvent e;
			e.buttons = sgWin32ToSagitta::getMouseButtons(aWParam);
			e.modifiers = sgWin32ToSagitta::getMofifiers(aWParam);
			e.x = Sagitta::sgBitOP::lo16(aLParam);
			e.y = Sagitta::sgBitOP::hi16(aLParam);
			static int lastX = e.x;
			static int lastY = e.y;
			e.deltaX = e.x - lastX;
			e.deltaY = e.y - lastY;
			lastX = e.x;
			lastY = e.y;
			
			switch(aMessage){
			case WM_MOUSEMOVE:
				{
					GetApp()->mouseMoveEvent(e);
				}
				break;

			case WM_LBUTTONDOWN:
				{
					e.buttons = Sagitta::MBT_Left;
					GetApp()->mousePressEvent(e);
				}
				break;

			case WM_LBUTTONUP:
				{
					e.buttons = Sagitta::MBT_Left;
					
				}
				break;

			case WM_LBUTTONDBLCLK:
				{
					e.buttons = Sagitta::MBT_Left;
					GetApp()->mouseDoubleClickEvent(e);
				}
				break;

			case WM_RBUTTONDOWN:
				{
					e.buttons = Sagitta::MBT_Right;
					GetApp()->mousePressEvent(e);
				}
				break;

			case WM_RBUTTONUP:
				{
					e.buttons = Sagitta::MBT_Right;
					GetApp()->mouseReleaseEvent(e);
				}
				break;

			case WM_RBUTTONDBLCLK:
				{
					e.buttons = Sagitta::MBT_Right;
					GetApp()->mouseDoubleClickEvent(e);
				}
				break;

			case WM_MBUTTONDOWN:
				{
					e.buttons = Sagitta::MBT_Middle;
					GetApp()->mousePressEvent(e);
				}
				break;

			case WM_MBUTTONUP:
				{
					e.buttons = Sagitta::MBT_Middle;
					GetApp()->mouseReleaseEvent(e);
				}
				break;

			case WM_MBUTTONDBLCLK:
				{
					e.buttons = Sagitta::MBT_Middle;
					GetApp()->mouseDoubleClickEvent(e);
				}
				break;
			}
		}
		return err;

	//default:
		
	}
	err = DefWindowProc(aHWnd, aMessage, aWParam, aLParam);
	return err;
}