//////////////////////////////////////////////////////
// file: SMainFrame.cpp @ 2008-12-11 by Zhang Xiang
// defines of the class SMainFrame
// SMainFrame is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SMainFrame.h"
#include "SWindowDelegate.h"
#include "SWidget.h"
#include "SEvent.h"
#include "../common/utils/sgException.h"
#include "../common/utils/sgStringUtil.h"
#include <algorithm>

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/12/2008 zhangxiang]
	SMainFrame::SMainFrame(SMainFrameDelegate *aDelegate, const std::string &aTitle, SRect aRect, SMainFrame *aParent)
		: SWindow(aDelegate, aTitle, aRect, aParent){

	}

	//  [12/12/2008 zhangxiang]
	SMainFrame::SMainFrame(const std::string &aTitle, Sagitta::SRect aRect, Sagitta::SMainFrame *aParent)
		: SWindow(new SMainFrameDelegate(), aTitle, aRect, aParent){

	}

	//  [12/12/2008 zhangxiang]
	SMainFrame::~SMainFrame(void){
	//	m_Children.clear(); ---- no need
	}
	
	//  [12/14/2008 zhangxiang]
	bool SMainFrame::onClose(void){
		return SWindow::onClose();
		//SMainFrame::ChildList::iterator it = m_Children.begin();
		//for(; it != m_Children.end(); ++it){
		//	sendEvent(SEvent(SEvent::ET_CLOSE, 0, 0, *it), *it);
		//}
		//
		////	del()->destroyWindow(handle());
		//dispose();
		//
		//// Tell system that this event has been processed.
		//return true;
	}

} // namespace Sagitta