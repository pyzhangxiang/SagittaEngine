// 2007-11-6

#include "DemoSkeletonAnimation.h"
#include <engine/common/sgException.h>
#include <iostream>
#include <string>
using namespace Sagitta;

int main(int argc, char *argv[]){
	int result = 0;
	try{
        DemoSkeletonAnimation demo;
        demo.setFrameRate(1.0f / 30.0f);
        demo.setWindowPos(-1, -1);
        demo.setWindowSize(800, 600);
        demo.setWindowTitle("Demo03_SkeletonAnimation");
		result = demo.run(argc, argv);
	}catch(std::exception &e){
		sgMessageBox("Error", e.what());
	}

	return result;

}

