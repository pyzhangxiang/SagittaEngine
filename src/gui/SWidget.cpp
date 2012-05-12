//////////////////////////////////////////////////////
// file: SWidget.cpp @ 2008-12-11 by Zhang Xiang
// defines of the class SWidget
// SWidget is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SWidget.h"
#include "SWindowDelegate.h"
#include "SMainFrame.h"
#include "SEvent.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/12/2008 zhangxiang]
	SWidget::SWidget(SWidgetDelegate *aDelegate, SMainFrame *aParent, const std::string &aTitle, SRect aRect)
		: SWindow(aDelegate, aTitle, aRect, aParent), m_SizePolicy(SWidget::SP_RELEVANT), /*m_bParentChanged(true), --- Outmoded --- */
		m_iScreenX(0), m_iScreenY(0), m_fWidthScale(-1), m_fHeightScale(-1){

		aParent->addChild(this);
		// resize according to mine size policy
	//	_update();
		show();
	}

	//  [12/12/2008 zhangxiang]
	SWidget::SWidget(SMainFrame *aParent, const std::string &aTitle, SRect aRect)
		: SWindow(new SWidgetDelegate(), aTitle, aRect, aParent), m_SizePolicy(SWidget::SP_RELEVANT), /*m_bParentChanged(true), --- Outmoded --- */
		m_iScreenX(0), m_iScreenY(0), m_fWidthScale(-1), m_fHeightScale(-1){

		aParent->addChild(this);
		// resize according to mine size policy
	//	_update();
		show();
	}

	//  [12/12/2008 zhangxiang]
	SWidget::~SWidget(void){
		if(parent()){
			parent()->_removeChild(this);
		}
	}

	/*	--- Outmoded ---
	//  [12/12/2008 zhangxiang]
	SStatus SWidget::handleEvent(const SEvent &aEvent){
		switch(aEvent.type()){
			case SEvent::ET_PAINT:
			{
		//		_update();
				if(onPaint())
					return NOERR;
				else return NONPROCESSEDERR;
			}
			break;

			default:
				break;
		}

		return SWindow::handleEvent(aEvent);
	}
	*/

	//  [12/12/2008 zhangxiang]
	bool SWidget::policyMove(int aParentNewX, int aParentNewY, int aParentNewWidth, int aParentNewHeight){
		int parentoldx = m_iScreenX - leftX();
		int parentoldy = m_iScreenY - topY();

		int newx, newy;
		int newwidth, newheight;

		switch(m_SizePolicy){
			case SP_FIXED:
			{
				newx = leftX() + parentoldx - aParentNewX;
				newy = topY() + parentoldy - aParentNewY;
				newwidth = width();
				newheight = height();
			}
			break;

			case SP_RELEVANT:
			{
				newx = leftX();
				newy = topY();
				newwidth = m_fWidthScale * aParentNewWidth;
				newheight = m_fHeightScale * aParentNewHeight;
			}
			break;

			case SP_FILLED:
			{
				newx = 0;
				newy = 0;
				newwidth = aParentNewWidth;
				newheight = aParentNewHeight;
			}
			break;

			default:
				return 0;
		}

		m_iScreenX = aParentNewX + newx;
		m_iScreenY = aParentNewY + newy;
		m_fWidthScale = (float)newwidth / (float)aParentNewWidth;
		m_fHeightScale = (float)newheight / (float)aParentNewHeight;

		return move(newx, newy, newwidth, newheight);

		
	}

	//  [12/12/2008 zhangxiang]
	void SWidget::_update(){
		/* --- Outmoded ---
		if(!m_bParentChanged)
			return ;
		*/

		if(m_fWidthScale < 0 || m_fHeightScale < 0){
			// initialize relevant properties
			m_iScreenX = parent()->leftX() + leftX();
			m_iScreenY = parent()->topY() + topY();

			m_fWidthScale = (float)(width()) / (float)(parent()->width());
			m_fHeightScale = (float)(height()) / (float)(parent()->height());

			if(m_fWidthScale > 1)
				m_fWidthScale = 1;
			if(m_fHeightScale > 1)
				m_fHeightScale = 1;

		//	return ;
		}

		// size update, the parameters are new values
		policyMove(parent()->leftX(), parent()->topY(), parent()->width(), parent()->height());

		// for future ...

		/*m_bParentChanged = false; --- Outmoded --- */
	}

	//  [12/12/2008 zhangxiang]
	SWidget::SizePolicy SWidget::sizepolicy(void) const{
		return m_SizePolicy;
	}

	//  [12/12/2008 zhangxiang]
	void SWidget::setSizePolicy(SWidget::SizePolicy aSizePolicy){
		if(m_SizePolicy != aSizePolicy){
			// size policy changed, update
			m_SizePolicy = aSizePolicy;
	//		_update();
		}
		// else, do nothing
	}

	//  [12/12/2008 zhangxiang]
	void SWidget::_needUpdate(void){
		/*m_bParentChanged = true; --- Outmoded --- */
		_update();
	}

} // namespace Sagitta