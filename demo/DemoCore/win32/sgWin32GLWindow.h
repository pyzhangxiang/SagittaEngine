//
//  sgNSOpenGLView.h
//  cocoaogl
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef __SGWIN32GLWINDOW_H__
#define __SGWIN32GLWINDOW_H__

#include <windows.h>
#include <string>

class sgWin32GLWindow
{
private:
	HWND mhwnd;
	HDC mhdc;
	HGLRC mGLContext;
	int mX, mY, mW, mH;
	std::string mTitle;

public:
	sgWin32GLWindow(void);
	~sgWin32GLWindow(void);

	void setRect(int x, int y, int w, int h);
	void setTitle(const std::string &title);
	HWND create(void);

	HWND getHwnd(void) const{ return mhwnd; }
	HDC getHdc(void) const{ return mhdc; }

private:
	void buildRenderEnv(HDC mhdc);
	void releaseRenderEnv(void);
};

	

#endif	// __SGWIN32GLWINDOW_H__


