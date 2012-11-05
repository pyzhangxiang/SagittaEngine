//
//  sgKeyDefine.h
//  Sagitta
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Sagitta_sgKeyDefine_h
#define Sagitta_sgKeyDefine_h

namespace Sagitta {
    // ENUMS
	enum MouseButton{
		MBT_NON = 0x0000,
		MBT_Left = 0x0010,
		MBT_Right = 0x0020,
		MBT_Middle = 0x0030
	};
    
	enum ModifierKeys{
		MKS_NON = 0x0000,
		MKS_Ctrl = 0x0001,
		MKS_Shift = 0x0002,
		MKS_Alt = 0x0004,
		MKS_Meta = 0x0008,
        
		MKS_CapsLock = 0x0100,
		MKS_NumLock = 0x0200,
		MKS_ScrollLock = 0x0300,
        
		MKS_LButton = 0x10,
		MKS_RButton = 0x20,
		MKS_MButton = 0x30,
        
		MKS_CtrlAndShift = MKS_Ctrl | MKS_Shift,
		MKS_CtrlAndAlt = MKS_Ctrl | MKS_Alt,
		MKS_ShiftAndAlt = MKS_Shift | MKS_Alt,
		MKS_CtrlAndShiftAndAlt = MKS_Ctrl | MKS_Shift | MKS_Alt
	};
    
	enum KeyCode{
		Key_Unknown = 0x0000,
		// misc keys
		Key_Esc = 0x0100,
		Key_Tab = 0x0101,
		Key_Backspace = 0x0102,
		Key_Return = 0x0103,
		Key_Enter = 0x0104,
		Key_Insert = 0x0105,
		Key_Delete = 0x0106,
		Key_Pause = 0x0107,
		Key_Print = 0x0108,
		Key_SysReq = 0x0109,
		Key_Clear = 0x010a,
        
		// cursor movement
		Key_Home = 0x0111,
		Key_End = 0x0112,
		Key_PageUp = 0x0113,
		Key_PageDown = 0x0114,
		Key_Up = 0x0115,
		Key_Left = 0x0116,
		Key_Down = 0x0117,
		Key_Right = 0x0118,
        
		// modifiers
		Key_Ctrl = 0x0201,
		Key_Shift = 0x0202,
		Key_Alt = 0x0204,
		Key_Meta = 0x0208,
		Key_CapsLock = 0x0210,
		Key_NumLock = 0x0220,
		Key_ScrollLock = 0x0230,
        
		// function keys
		Key_F1 = 0x0350,
		Key_F2 = 0x0351,
		Key_F3 = 0x0352,
		Key_F4 = 0x0353,
		Key_F5 = 0x0354,
		Key_F6 = 0x0355,
		Key_F7 = 0x0356,
		Key_F8 = 0x0357,
		Key_F9 = 0x0358,
		Key_F10 = 0x0359,
		Key_F11 = 0x035a,
		Key_F12 = 0x035b,
        
		// ASCII 32(space) - 126(~)
	/*	Key_0 = 0x40,
		Key_1 = 0x41,
		Key_2 = 0x42,
		Key_3 = 0x43,
		Key_4 = 0x44,
		Key_5 = 0x45,
		Key_6 = 0x46,
		Key_7 = 0x47,
		Key_8 = 0x48,
		Key_9 = 0x49,
        
		Key_Space = 0x60,
		Key_Any = Key_Space,
		Key_A = 0x61,
		Key_B = 0x62,
		Key_C = 0x63,
		Key_D = 0x64,
		Key_E = 0x65,
		Key_F = 0x66,
		Key_G = 0x67,
		Key_H = 0x68,
		Key_I = 0x69,
		Key_J = 0x6a,
		Key_K = 0x6b,
		Key_L = 0x6c,
		Key_M = 0x6d,
		Key_N = 0x6e,
		Key_O = 0x6f,
		Key_P = 0x70,
		Key_Q = 0x71,
		Key_R = 0x72,
		Key_S = 0x73,
		Key_T = 0x74,
		Key_U = 0x75,
		Key_V = 0x76,
		Key_W = 0x77,
		Key_X = 0x78,
		Key_Y = 0x79,
		Key_Z = 0x7a,
        
		Key_BackQuote = 0x7b,		// `
		Key_Tilde = 0x7c,			// ~
		Key_Exclam = 0x7d,			// !
		Key_At = 0x7e,				// @
		Key_Sharp = 0x7f,			// #
		Key_Dollar = 0x80,			// $
		Key_Percent = 0x81,			// %
		Key_Caret = 0x82,			// ^
		Key_Ampersand = 0x83,		// &
		Key_Asterisk = 0x84,		// *
		Key_ParentLeft = 0x85,		// (
		Key_ParentRight = 0x86,		// )
		Key_Minus = 0x87,			// -
		Key_Underscore = 0x88,		// _
		Key_Plus = 0x89,			// +
		Key_Equal = 0x8a,			// =
		Key_BracketLeft = 0x8b,		// [
		Key_BracketRight = 0x8c,	// ]
		Key_BraceLeft = 0x8d,		// {
		Key_BraceRight = 0x8e,		// }
		Key_Backslash = 0x8f,		// \ ------------------
		Key_Bar = 0x90,				// |
		Key_Semicolon = 0x91,		// ;
		Key_Colon = 0x92,			// :
		Key_Quote = 0x93,			// '
		Key_DoubleQuote = 0x94,		// "
		Key_Comma = 0x95,			// ,
		Key_Less = 0x96,			// <
		Key_Period = 0x97,			// .
		Key_Greater = 0x98,			// >
		Key_Slash = 0x99,			// /
		Key_Question = 0x9a,		// ?
       */ 
	};
}


#endif
