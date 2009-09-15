//////////////////////////////////////////////////////////////
// file: GuiDef @ 2007-11-17 by Zhang Xiang
// declaration of the global variables and functions for Gui
//////////////////////////////////////////////////////////////
#ifndef __GUIDEF_H__
#define __GUIDEF_H__

// INCLUDES ////////////////////////////////////////////
#include "../common/configs/SagittaPrerequisites.h"

#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#	ifndef WINVER				// Allow use of features specific to Windows XP or later.
#	define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#	endif

#	ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#	define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#	endif						

#	ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#	define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#	endif

#	ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#	define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#	endif

#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#	include <windows.h>

#elif SAGITTA_PLATFORM == SAGITTA_PLATFORM_APPLE
#	include <Carbon/Carbon.h>
#	include <AGL/agl.h>
#else
#endif

// DECLARES //////////////////////////////////////////
namespace Sagitta{

	// TYPE DEFINE
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
	typedef HWND SWindowHandle;
	typedef HDC SContextHandle;
	typedef HGLRC SGLContextHandle;
	typedef LPPAINTSTRUCT SPaintPortHandle;
#elif SAGITTA_PLATFORM == SAGITTA_PLATFORM_APPLE
	typedef HIViewRef SWindowHandle;
	typedef CGContextRef SContextHandle;
	typedef AGLContext SGLContextHandle;
	typedef CGrafPtr SPaintPortHandle;
#else
#endif

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
		Key_Ctrl = 0x0001,
		Key_Shift = 0x0002,
		Key_Alt = 0x0004,
		Key_Meta = 0x0008,
		Key_CapsLock = 0x0010,
		Key_NumLock = 0x0020,
		Key_ScrollLock = 0x0030,

		// function keys
		Key_F1 = 0x0050,
		Key_F2 = 0x0051,
		Key_F3 = 0x0052,
		Key_F4 = 0x0053,
		Key_F5 = 0x0054,
		Key_F6 = 0x0055,
		Key_F7 = 0x0056,
		Key_F8 = 0x0057,
		Key_F9 = 0x0058,
		Key_F10 = 0x0059,
		Key_F11 = 0x005a,
		Key_F12 = 0x005b,

		// ASCII
		Key_0 = 0x40,
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

	};

	// STRUCTS
	
	// VARIABLES

	// FUNCTIONS

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __GUIDEF_H__