//
//  sgCocoaToSagitta.m
//  Sagitta
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "sgWin32ToSagitta.h"
#include "../sgKeyDefine.h"
#include <windows.h>

unsigned int sgWin32ToSagitta::getMofifiers(void){
	unsigned int mks = Sagitta::MKS_NON;

	if(::GetKeyState(VK_CONTROL) < 0){
		mks |= Sagitta::MKS_Ctrl;
	}

	if(::GetKeyState(VK_SHIFT) < 0){
		mks |= Sagitta::MKS_Shift;
	}

	if(::GetKeyState(VK_MENU) < 0){
		mks |= Sagitta::MKS_Alt;
	}

	if(::GetKeyState(VK_CAPITAL) > 0){
		mks |= Sagitta::MKS_CapsLock;
	}

	if(::GetKeyState(VK_NUMLOCK) > 0){
		mks |= Sagitta::MKS_NumLock;
	}

	if(::GetKeyState(VK_SCROLL) > 0){
		mks |= Sagitta::MKS_ScrollLock;
	}

	return mks;
}

unsigned int sgWin32ToSagitta::getMofifiers( unsigned int aSysMK )
{
	unsigned int mks = Sagitta::MKS_NON;

	if(aSysMK & MK_CONTROL){
		mks |= Sagitta::MKS_Ctrl;
	}

	if(aSysMK & MK_SHIFT){
		mks |= Sagitta::MKS_Shift;
	}

	if(aSysMK & MK_ALT){
		mks |= Sagitta::MKS_Alt;
	}

	return mks;
}

int sgWin32ToSagitta::getKey( int keyChar )
{
	int res = Sagitta::Key_Unknown;

	if(keyChar >= 32    // space
		|| keyChar <= 126)   // ~
	{
		res = keyChar;
	}
	else if(keyChar >= VK_F1
		|| keyChar <= VK_F12)
	{
		res = Sagitta::Key_F1 + keyChar - VK_F1;
	}
	else
	{
		switch(keyChar)
		{
		case VK_UP:
			res = Sagitta::Key_Up;
			break;
		case VK_DOWN:
			res = Sagitta::Key_Down;
			break;
		case VK_LEFT:
			res = Sagitta::Key_Left;
			break;
		case VK_RIGHT:
			res = Sagitta::Key_Right;
			break;
		case VK_ESCAPE:    // esc
			res = Sagitta::Key_Esc;
			break;
		case VK_RETURN:    // return 
			res = Sagitta::Key_Enter;
			break;
		}
	}

	return res;
}

unsigned int sgWin32ToSagitta::getMouseButtons( unsigned int aSysMK )
{
	unsigned int mks = Sagitta::MKS_NON;

	if(aSysMK & MK_LBUTTON){
		mks |= Sagitta::MKS_LButton;
	}

	if(aSysMK & MK_RBUTTON){
		mks |= Sagitta::MKS_RButton;
	}

	if(aSysMK & MK_MBUTTON){
		mks |= Sagitta::MKS_MButton;
	}

	return mks;
}
