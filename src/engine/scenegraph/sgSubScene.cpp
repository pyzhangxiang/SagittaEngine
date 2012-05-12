//////////////////////////////////////////////////////
// file: sgSubScene.cpp 
// created by zhangxiang on 09-01-06
// declares of the class sgSubScene
// sgSubScene is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgSubScene.h"
#include "sgSceneManager.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [1/6/2009 zhangxiang]
	sgSubScene::sgSubScene(int aType /* = ST_UNKNOWN */) :
	sgScene(), mc_iType(aType){

	}

	//  [1/7/2009 zhangxiang]
	sgSubScene::sgSubScene(const StdString &aName, int aType /* = ST_UNKNOWN */) :
	sgScene(aName), mc_iType(aType){

	}

	//  [1/11/2009 zhangxiang]
	sgSubScene::sgSubScene(sgSceneDelegate *aDelegate, int aType /* = ST_UNKNOWN */) :
	sgScene(aDelegate), mc_iType(aType){

	}

	//  [1/11/2009 zhangxiang]
	sgSubScene::sgSubScene(const StdString &aName, sgSceneDelegate *aDelegate, int aType /* = ST_UNKNOWN */) :
	sgScene(aName, aDelegate), mc_iType(aType){

	}

	//  [1/7/2009 zhangxiang]
	sgSubScene::~sgSubScene(void){
		sgSceneManager *manager = getSceneManager();
		if(manager && !(manager->isDisposing())){
			manager->removeSubScene(this);
		}
	}

	//  [1/7/2009 zhangxiang]
	int sgSubScene::type(void) const{
		return mc_iType;
	}

} // namespace Sagitta