//////////////////////////////////////////////////////
// file: SSubScene.cpp 
// created by zhangxiang on 09-01-06
// declares of the class SSubScene
// SSubScene is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SSubScene.h"
#include "SSceneManager.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/6/2009 zhangxiang]
	SSubScene::SSubScene(int aType /* = ST_UNKNOWN */) :
	SScene(), mc_iType(aType){

	}

	//  [1/7/2009 zhangxiang]
	SSubScene::SSubScene(const StdString &aName, int aType /* = ST_UNKNOWN */) :
	SScene(aName), mc_iType(aType){

	}

	//  [1/11/2009 zhangxiang]
	SSubScene::SSubScene(SSceneDelegate *aDelegate, int aType /* = ST_UNKNOWN */) :
	SScene(aDelegate), mc_iType(aType){

	}

	//  [1/11/2009 zhangxiang]
	SSubScene::SSubScene(const StdString &aName, SSceneDelegate *aDelegate, int aType /* = ST_UNKNOWN */) :
	SScene(aName, aDelegate), mc_iType(aType){

	}

	//  [1/7/2009 zhangxiang]
	SSubScene::~SSubScene(void){
		SSceneManager *manager = getSceneManager();
		if(manager && !(manager->isDisposing())){
			manager->removeSubScene(this);
		}
	}

	//  [1/7/2009 zhangxiang]
	int SSubScene::type(void) const{
		return mc_iType;
	}

} // namespace Sagitta