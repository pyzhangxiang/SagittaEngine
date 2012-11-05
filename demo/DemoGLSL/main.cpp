
#include "DemoGLSL.h"
#include <engine/common/sgException.h>
#include <iostream>
#include <string>
using namespace Sagitta;

int main(int argc, char *argv[]){
	int result = 0;
	try{
        DemoGLSL demo;
        demo.setFrameRate(1.0f / 30.0f);
        demo.setWindowPos(-1, -1);
        demo.setWindowSize(800, 600);
        demo.setWindowTitle("Demo_GLSL");
		result = demo.run(argc, argv);
	}catch(std::exception &e){
		sgMessageBox("Error", e.what());
	}
	return result;

} 
