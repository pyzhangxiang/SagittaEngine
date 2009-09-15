//////////////////////////////////////////////////////
// file: SApplicationWin32Impl.cpp 
// created by zhangxiang on 09-01-07
// define of the class SApplication
// SApplication is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SApplication.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/7/2009 zhangxiang]
	SStatus SApplication::run(void){
		MSG msg;
		while(::GetMessage(&msg, NULL, 0, 0)){
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		/*while(msg->message != WM_QUIT){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		}
		}*/
		return static_cast<SStatus>(msg.wParam);
	}

	//  [1/7/2009 zhangxiang]
	void SApplication::quit(int aQuitCode){
		::PostQuitMessage(aQuitCode);
	}

} // namespace Sagitta