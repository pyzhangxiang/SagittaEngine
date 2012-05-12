//////////////////////////////////////////////////////
// file: SApplicationAppleImpl.cpp 
// created by zhangxiang on 09-1-18
// defines of the class SApplicationAppleImpl
// SApplicationAppleImpl is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SApplication.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	
	//  [18/1/2009 zhangxiang]
	SStatus SApplication::run(void){
		::RunApplicationEventLoop();
		return 0;
	}
	
	//  [18/1/2009 zhangxiang]
	void SApplication::quit(int aQuitCode){
		::QuitApplicationEventLoop();
	}
	
} // namespace Sagitta