//////////////////////////////////////////////////////
// file: SView.cpp @ 2008-12-12 by Zhang Xiang
// defines of the class SView
// SView is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SView.h"
#include "SWindowDelegate.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/13/2008 zhangxiang]
	SView::SView(SViewDelegate *aDelegate, SMainFrame *aParent, const std::string &aTitle, SRect aRect)
		: SWidget(aDelegate, aParent, aTitle, aRect){

	}

	//  [12/13/2008 zhangxiang]
	SView::SView(SMainFrame *aParent, const std::string &aTitle, SRect aRect)
		: SWidget(new SViewDelegate(), aParent, aTitle, aRect){

	}

	//  [12/13/2008 zhangxiang]
	SView::~SView(void){

	}

} // namespace Sagitta