//////////////////////////////////////////////////////
// file: SGraphicsFrame.h @ 2007-12-3 by Zhang Xiang
// definitions of the class SGraphicsFrame
// SGraphicsFrame is a class ...
//////////////////////////////////////////////////////

// INCLUSIONS ////////////////////////////////////////
#include "SGraphicsFrame.h"
#include "SGraphicsView.h"
#include "SWindowDelegate.h"
#include "SEvent.h"
#include "../engine/scenegraph/SSceneManager.h"
#include "../common/utils/SException.h"

// DECLARATIONS //////////////////////////////////////

// DEFINITIONS ///////////////////////////////////////
namespace Sagitta{

	SGraphicsFrame::SGraphicsFrame(SMainFrameDelegate *aDelegate, const std::string &aTitle /* = "New Window" */, SRect aRect /* = SRect(0, 0, 170, 170) */, SMainFrame *aParent /* = 0 */)
		: SMainFrame(aDelegate, aTitle, aRect, aParent), m_pSceneManager(0){
		init();
	}

	SGraphicsFrame::SGraphicsFrame(const std::string &aTitle /* = "New Window" */, SRect aRect /* = SRect(0, 0, 170, 170) */, SMainFrame *aParent /* = 0 */)
		: SMainFrame(aTitle, aRect, aParent), m_pSceneManager(0){
		init();
	}

	SGraphicsFrame::~SGraphicsFrame(void){
		if(m_pSceneManager)
			delete m_pSceneManager;
	}

	//  [12/14/2008 zhangxiang]
	void SGraphicsFrame::init(void){
		m_pSceneManager = new SSceneManager;
		try{
			SGraphicsView *gview = new SGraphicsView(this, SGraphicsView::RT_OPENGL);
			gview->setSizePolicy(SWidget::SP_FILLED);
			gview->setSceneManager(m_pSceneManager);
		}catch(SException&){
			delete m_pSceneManager;
			throw;
		}
	}

	//  [12/14/2008 zhangxiang]
	bool SGraphicsFrame::onEraseBkgnd(const SContext &aContext){
		return true;
	}

} // namespace Sagitta
