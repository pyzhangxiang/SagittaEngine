//////////////////////////////////////////////////////
// file: WinMain.cpp @ 2007-11-6 by Zaif Chang	
// the WinMain function for the project				
//////////////////////////////////////////////////////


// INCLUSION ///////////////////////////////////////////
#include "MainWindow.h"
#include <gui/SApplication.h>
#include <gui/SGraphicsFrame.h>
#include <engine/scenegraph/SSceneManager.h>
#include <engine/scenegraph/SEntity.h>
#include <common/utils/SException.h>
#include <iostream>
#include <string>
using namespace Sagitta;

// FUNCTIONS /////////////////////////////////////////

//====================================================
// WinMain for win32
// or main for mac or linux
//
//====================================================
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
#else 
int main(int argc, char *argv[]){
#endif
	int result = 0;
	try{
		SApplication app;
		MainWindow *mainwindow = new MainWindow;
		SApplication::instance().setMainWindow(mainwindow);
		mainwindow->show();
	
		result = SApplication::instance().run();
	}catch(SException &e){
	//	delete mainwindow;
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
		MessageBox(0, e.what(), 0, MB_ABORTRETRYIGNORE);
#else
		std::cout<< e.what() << std::endl;
#endif
	}catch(std::exception &e){
		std::cout << e.what() << std::endl;
	}
	return result;

} //#### end WinMain