//////////////////////////////////////////////////////
// file: SKeyMappingWin32Imp.cpp @ 2008-12-15 by Zhang Xiang
// defines of the class SKeyMapping
// SKeyMapping is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SKeyMapping.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/15/2008 zhangxiang]
	uLong SKeyMapping::SagiMouseButton(uLong aSysMB){
		switch(aSysMB){
			case VK_LBUTTON:
			{
				return Sagitta::MBT_Left;
			}

			case VK_RBUTTON:
			{
				return Sagitta::MBT_Right;
			}

			case VK_MBUTTON:
			{
				return Sagitta::MBT_Middle;
			}

			default:
			{
				return Sagitta::MBT_NON;
			}
		}
	}

	//  [12/15/2008 zhangxiang]
	uLong SKeyMapping::CheckModifierKeys(void){
		uLong mks = Sagitta::MKS_NON;

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

	//  [12/15/2008 zhangxiang]
	uLong SKeyMapping::SagiModifierKeys(uLong aSysMK){
		uLong mks = Sagitta::MKS_NON;

		if(aSysMK & MK_CONTROL){
			mks |= Sagitta::MKS_Ctrl;
		}

		if(aSysMK & MK_SHIFT){
			mks |= Sagitta::MKS_Shift;
		}

		/*if(aSysMK & MK_ALT){
			mks |= Sagitta::MKS_Alt;
		}*/

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

	//  [12/15/2008 zhangxiang]
	uLong SKeyMapping::SagiKey(uLong aSysKeyCode){
		switch(aSysKeyCode){
			case VK_ESCAPE:
			{
				return Sagitta::Key_Esc;
			}

			case VK_TAB:
			{
				return Sagitta::Key_Tab;
			}

			case VK_BACK:
			{
				return Sagitta::Key_Backspace;
			}

			case VK_RETURN:
			{
				return Sagitta::Key_Enter;
			}

			case VK_INSERT:
			{
				return Sagitta::Key_Insert;
			}

			case VK_DELETE:
			{
				return Sagitta::Key_Delete;
			}

			case VK_PAUSE:
			{
				return Sagitta::Key_Pause;
			}

			case VK_PRINT:
			{
				return Sagitta::Key_Print;
			}

			case VK_SNAPSHOT:
			{
				return Sagitta::Key_SysReq;
			}

			case VK_CLEAR:
			{
				return Sagitta::Key_Clear;
			}

			case VK_HOME:
			{
				return Sagitta::Key_Home;
			}

			case VK_END:
			{
				return Sagitta::Key_End;
			}

			case VK_PRIOR:
			{
				return Sagitta::Key_PageUp;
			}

			case VK_NEXT:
			{
				return Sagitta::Key_PageDown;
			}

			case VK_UP:
			{
				return Sagitta::Key_Up;
			}

			case VK_LEFT:
			{
				return Sagitta::Key_Left;
			}

			case VK_DOWN:
			{
				return Sagitta::Key_Down;
			}

			case VK_RIGHT:
			{
				return Sagitta::Key_Right;
			}

			case VK_CONTROL:
			{
				return Sagitta::Key_Ctrl;
			}

			case VK_SHIFT:
			{
				return Sagitta::Key_Shift;
			}

			case VK_MENU:
			{
				return Sagitta::Key_Alt;
			}

			case VK_CAPITAL:
			{
				return Sagitta::Key_CapsLock;
			}

			case VK_NUMLOCK:
			{
				return Sagitta::Key_NumLock;
			}

			case VK_SCROLL:
			{
				return Sagitta::Key_ScrollLock;
			}

			case VK_F1:
			{
				return Sagitta::Key_F1;
			}

			case VK_F2:
			{
				return Sagitta::Key_F2;
			}

			case VK_F3:
			{
				return Sagitta::Key_F3;
			}

			case VK_F4:
			{
				return Sagitta::Key_F4;
			}

			case VK_F5:
			{
				return Sagitta::Key_F5;
			}

			case VK_F6:
			{
				return Sagitta::Key_F6;
			}

			case VK_F7:
			{
				return Sagitta::Key_F7;
			}

			case VK_F8:
			{
				return Sagitta::Key_F8;
			}

			case VK_F9:
			{
				return Sagitta::Key_F9;
			}

			case VK_F10:
			{
				return Sagitta::Key_F10;
			}

			case VK_F11:
			{
				return Sagitta::Key_F11;
			}

			case VK_F12:
			{
				return Sagitta::Key_F12;
			}

			case '0':
			{
				return Sagitta::Key_0;
			}

			case '1':
			{
				return Sagitta::Key_1;
			}

			case '2':
			{
				return Sagitta::Key_2;
			}

			case '3':
			{
				return Sagitta::Key_3;
			}

			case '4':
			{
				return Sagitta::Key_4;
			}

			case '5':
			{
				return Sagitta::Key_5;
			}

			case '6':
			{
				return Sagitta::Key_6;
			}

			case '7':
			{
				return Sagitta::Key_7;
			}

			case '8':
			{
				return Sagitta::Key_8;
			}

			case '9':
			{
				return Sagitta::Key_9;
			}

			case 'A':
			{
				return Sagitta::Key_A;
			}

			case 'B':
			{
				return Sagitta::Key_B;
			}

			case 'C':
			{
				return Sagitta::Key_C;
			}

			case 'D':
			{
				return Sagitta::Key_D;
			}

			case 'E':
			{
				return Sagitta::Key_E;
			}

			case 'F':
			{
				return Sagitta::Key_F;
			}

			case 'G':
			{
				return Sagitta::Key_G;
			}

			case 'H':
			{
				return Sagitta::Key_H;
			}

			case 'I':
			{
				return Sagitta::Key_I;
			}

			case 'J':
			{
				return Sagitta::Key_J;
			}

			case 'K':
			{
				return Sagitta::Key_K;
			}

			case 'L':
			{
				return Sagitta::Key_L;
			}

			case 'M':
			{
				return Sagitta::Key_M;
			}

			case 'N':
			{
				return Sagitta::Key_N;
			}

			case 'O':
			{
				return Sagitta::Key_O;
			}

			case 'P':
			{
				return Sagitta::Key_P;
			}

			case 'Q':
			{
				return Sagitta::Key_Q;
			}

			case 'R':
			{
				return Sagitta::Key_R;
			}

			case 'S':
			{
				return Sagitta::Key_S;
			}

			case 'T':
			{
				return Sagitta::Key_T;
			}

			case 'U':
			{
				return Sagitta::Key_U;
			}

			case 'V':
			{
				return Sagitta::Key_V;
			}

			case 'W':
			{
				return Sagitta::Key_W;
			}

			case 'X':
			{
				return Sagitta::Key_X;
			}

			case 'Y':
			{
				return Sagitta::Key_Y;
			}

			case 'Z':
			{
				return Sagitta::Key_Z;
			}

			case VK_SPACE:
			{
				return Sagitta::Key_Space;
			}

			case VK_OEM_3:
			{
				return Sagitta::Key_Tilde;
			}

			case VK_OEM_MINUS:
			{
				return Sagitta::Key_Minus;
			}

			case VK_OEM_PLUS:
			{
				return Sagitta::Key_Plus;
			}

			case VK_OEM_4:
			{
				return Sagitta::Key_BraceLeft;
			}

			case VK_OEM_6:
			{
				return Sagitta::Key_BraceRight;
			}

			case VK_OEM_5:
			{
				return Sagitta::Key_Bar;
			}

			case VK_OEM_1:
			{
				return Sagitta::Key_Colon;
			}

			case VK_OEM_7:
			{
				return Sagitta::Key_DoubleQuote;
			}

			case VK_OEM_COMMA:
			{
				return Sagitta::Key_Comma;
			}

			case VK_OEM_PERIOD:
			{
				return Sagitta::Key_Period;
			}

			case VK_OEM_2:
			{
				return Sagitta::Key_Slash;
			}

			default:
			{
				return Sagitta::Key_Unknown;
			}

		}
	}

} // namespace Sagitta