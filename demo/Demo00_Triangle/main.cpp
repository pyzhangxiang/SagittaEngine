
#include "DemoTriangle.h"
#include <engine/common/sgException.h>
#include <iostream>
#include <string>
using namespace Sagitta;

int main(int argc, char *argv[]){
	int result = 0;
	try{
        DemoTriangle demo;
        demo.setFrameRate(1.0f / 30.0f);
        demo.setWindowPos(-1, -1);
        demo.setWindowSize(800, 600);
        demo.setWindowTitle("Demo_Triangle");
		result = demo.run(argc, argv);
	}
	/*catch(sgException &e){
	//	delete mainwindow;
	//	MessageBox(0, e.what(), 0, MB_ABORTRETRYIGNORE);
		std::cout<< e.what() << std::endl;
	}*/catch(std::exception &e){
		//std::cout << e.what() << std::endl;
		sgMessageBox("Error", e.what());
	}
	return result;

} 
