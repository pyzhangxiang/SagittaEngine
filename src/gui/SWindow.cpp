//////////////////////////////////////////////////////
// file: SWindow.cpp @ 2007-11-16 by Zhang Xiang
// definition of the class SWindow
// SWindow is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SWindow.h"
#include "SWidget.h"
#include "SWindowDelegate.h"
#include "SContext.h"
#include "SPaintContext.h"
#include "SApplication.h"
#include "SEvent.h"
#include "../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SWindow::SWindow(SWindowDelegate *aDelegate, const StdString &aTitle, SRect aRect, SWindow *aParent) :
	SListener(), m_Delegate(aDelegate),
	m_sTitle(aTitle), m_pParent(aParent),
	m_Handle(0), m_bVisible(false){
		if(!m_Delegate){
			m_Delegate = new SWindowDelegate;
		}
		SWindowHandle hParent = 0;
		if(aParent){
			hParent = aParent->handle();
	//		aParent->addChild(this);
		}

		// for future... should catch exception
		// Calls for system call to create a window and initializes m_Handle.
		m_Handle = m_Delegate->_createWindow(aTitle, aRect, hParent, this);
			
		// initializes size info
		SRect content_rect = m_Delegate->getWindowBounds(m_Handle);
		m_iX = content_rect.left;
		m_iY = content_rect.top;
		m_iWidth = content_rect.right - content_rect.left;
		m_iHeight = content_rect.bottom - content_rect.top;

		// Before call the following function, m_Handle should have been init.
		// Let SApplication to take charge this.
	//	SApplication::instance().registerWindow(this);		
	}

	//  [12/9/2008 zhangxiang]
	SWindow::~SWindow(void){
		_setDisposing();
		dispose();
		delete m_Delegate;
	}

	//  [12/12/2008 zhangxiang]
	SWindowDelegate *SWindow::del(void) const{
		return m_Delegate;
	}

	SStatus SWindow::handleEvent(const SEvent &aEvent){
		SStatus err;

		switch(aEvent.type()){
			case SEvent::ET_CLOSE:
			{
				if(onClose())
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_MOVE:
			{
				const SMoveEvent &event = dynamic_cast<const SMoveEvent&>(aEvent);
				m_iX = event.x();
				m_iY = event.y();

				onMove(m_iX, m_iY);
				err = NOERR;
			}
			break;

			case SEvent::ET_RESIZE:
			{
				const SResizeEvent &event = dynamic_cast<const SResizeEvent&>(aEvent);
				m_iWidth = event.width();
				m_iHeight = event.height();

				onResize(m_iWidth, m_iHeight);
				err	= NOERR;
			}
			break;

			case SEvent::ET_ERASEBKGND:
			{
				const SEraseBkgndEvent &event = dynamic_cast<const SEraseBkgndEvent&>(aEvent);
				SContext pc(event.contextHandle(), this, false);
				if(onEraseBkgnd(pc))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_PAINT:
			{
				if(onPaint())
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_KEY_DOWN:
			{
				const SKeyEvent &event = dynamic_cast<const SKeyEvent&>(aEvent);
				if(onKeyDown(event.key(), event.repeatCount(), event.modifiers()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_KEY_UP:
			{
				const SKeyEvent &event = dynamic_cast<const SKeyEvent&>(aEvent);
				if(onKeyUp(event.key(), event.repeatCount(), event.modifiers()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_CHAR:
			{
				const SKeyEvent &event = dynamic_cast<const SKeyEvent&>(aEvent);
				if(onChar(event.key(), event.repeatCount(), event.modifiers()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_MOUSE_MOVE:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				uLong k = event.button();
				bool res = false;
				switch(k){
					case Sagitta::MBT_NON:
					{
						res = onMouseMove(event.modifiers(), event.point());
					}
					break;

					case Sagitta::MBT_Left:
					{
						res = onLMouseDragged(event.modifiers(), event.point());
					}
					break;

					case Sagitta::MBT_Right:
					{
						res = onRMouseDragged(event.modifiers(), event.point());
					}
					break;

					case Sagitta::MBT_Middle:
					{
						res = onMMouseDragged(event.modifiers(), event.point());
					}
					break;
				
				}
				if(res)
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_LBUTTON_DOWN:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onLMouseDown(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_LBUTTON_UP:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onLMouseUp(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_LBUTTON_DBLCLK:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onLMouseDblClk(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_RBUTTON_DOWN:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onRMouseDown(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_RBUTTON_UP:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onRMouseUp(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_RBUTTON_DBLCLK:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onRMouseDblClk(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_MBUTTON_DOWN:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onMMouseDown(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_MBUTTON_UP:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onMMouseUp(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			case SEvent::ET_MBUTTON_DBLCLK:
			{
				const SMouseEvent &event = dynamic_cast<const SMouseEvent&>(aEvent);
				if(onMMouseDblClk(event.modifiers(), event.point()))
					err = NOERR;
				else err = NONPROCESSEDERR;
			}
			break;

			default:
			{
				err = NONPROCESSEDERR;
			}
			break;
		}

		return err;
	}

	//  [12/16/2008 zhangxiang]
	bool SWindow::dispose(void){
		if(m_Handle && m_Delegate->isWindow(m_Handle)){
			if(m_Delegate->destroyWindow(m_Handle)){
				m_Handle = 0;
				return true;
			}
		}
		return false;
	}

	//  [12/14/2008 zhangxiang]
	bool SWindow::onClose(void){
		return false;
	}

	//  [12/14/2008 zhangxiang]
	bool SWindow::onEraseBkgnd(const SContext &aContext){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onMouseMove(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onLMouseDown(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onLMouseDragged(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onLMouseUp(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/15/2008 zhangxiang]
	bool SWindow::onLMouseDblClk(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onRMouseDown(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onRMouseDragged(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onRMouseUp(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/15/2008 zhangxiang]
	bool SWindow::onRMouseDblClk(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onMMouseDown(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onMMouseDragged(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onMMouseUp(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/15/2008 zhangxiang]
	bool SWindow::onMMouseDblClk(uLong aModifiers, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onMouseWheel(uLong aModifiers, int aZDelta, const SPoint &aPoint){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onKeyDown(uLong aKey, uLong aRepeatCount, uLong aModifiers){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onKeyUp(uLong aKey, uLong aRepeatCount, uLong aModifiers){
		return false;
	}

	//  [12/15/2008 zhangxiang]
	bool SWindow::onChar(uLong aChar, uLong aRepeatCount, uLong aModifiers){
		return false;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onPaint(void){
		SPaintContext context(this);

		onDraw(context);

		return true;
	}

	//  [12/14/2008 zhangxiang]
	void SWindow::onDraw(const SContext &aContext){

	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::onResize(uLong aWidth, uLong aHeight){
		// Notifies children that they should update.
		notifyChildren();

		return true;
	}

	//  [12/12/2008 zhangxiang]
	bool SWindow::onMove(int aX, int aY){
		return false;
	}

	const std::string &SWindow::title(void) const{
		return m_sTitle;
	}

	void SWindow::setTitle(const std::string &aTitle){
		m_sTitle = aTitle;
	}

	SWindow *SWindow::parent(void) const{
		return m_pParent;
	}

	SWindowHandle SWindow::handle(void) const{
		return m_Handle;
	}

	//  [12/10/2008 zhangxiang]
	SPoint SWindow::position(void) const{
		return SPoint(m_iX, m_iY);
	}

	//  [12/12/2008 zhangxiang]
	int SWindow::leftX(void) const{
		return m_iX;
	}

	//  [12/12/2008 zhangxiang]
	int SWindow::topY(void) const{
		return m_iY;
	}

	//  [12/10/2008 zhangxiang]
	int SWindow::width(void) const{
		return m_iWidth;
	}

	//  [12/10/2008 zhangxiang]
	int SWindow::height(void) const{
		return m_iHeight;
	}

	//  [12/10/2008 zhangxiang]
	bool SWindow::isVisible(void) const{
		return m_bVisible;
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::show(void){
		m_bVisible = true;
		bool result = m_Delegate->showWindow(m_Handle);
		m_Delegate->updateWindow(m_Handle);
		return result;
	}

	//  [12/14/2008 zhangxiang]
	bool SWindow::update(void){
		return m_Delegate->updateWindow(m_Handle);
	}

	//  [12/9/2008 zhangxiang]
	bool SWindow::hide(void){
		m_bVisible = false;
		return m_Delegate->hideWindow(m_Handle);
	}

	//  [12/10/2008 zhangxiang]
	bool SWindow::move(int aNewX, int aNewY, int aNewWidth, int aNewHeight){
		return m_Delegate->moveWindow(m_Handle, aNewX, aNewY, aNewWidth, aNewHeight);
	}

	//  [12/16/2008 zhangxiang]
	bool SWindow::validateRect(const SRect *aRect){
		return m_Delegate->validateRect(m_Handle, aRect);
	}

	//  [12/16/2008 zhangxiang]
	bool SWindow::invalidateRect(const SRect *aRect, bool bErase /* = true */){
		return m_Delegate->invalidateRect(m_Handle, aRect, bErase);
	}

	//  [12/12/2008 zhangxiang] -- in SMainFrame
	//  [1/7/2009 zhangxiang]
	void SWindow::addChild(SWidget *aWidget){
		if(!aWidget){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
				"Null widget ponter.", "SWindow::addChild");
		}
		m_Children.insert(aWidget);
	}

	//  [12/12/2008 zhangxiang] -- in SMainFrame
	//  [1/7/2009 zhangxiang]
	void SWindow::_removeChild(SWidget *aWidget){
		ChildList::iterator it = m_Children.find(aWidget);
		if(it == m_Children.end()){
			THROW_SAGI_EXCEPT(sgException::ERR_ITEM_NOT_FOUND,
							"Widget \"" + aWidget->title() +
							"\" is not a child of Window \"" +
							this->title() + "\".", "SWindow::removeChild");
		}else{
			m_Children.erase(it);
		}
	}

	//  [1/3/2009 zhangxiang] -- in SMainFrame
	//  [1/7/2009 zhangxiang]
	SWidget *SWindow::getChild(uInt aIndex) const{
		if(aIndex >= m_Children.size()){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS,
							"Index out of range.", "SWindow::getChild");
		}
		ChildList::const_iterator it = m_Children.begin();
		while(aIndex--) ++it;
		return *it;
	}

	//  [12/12/2008 zhangxiang] -- in SMainFrame
	//  [1/7/2009 zhangxiang]
	void SWindow::notifyChildren(void){
		ChildList::iterator it = m_Children.begin();
		for(; it != m_Children.end(); ++it){
			(*it)->_needUpdate();
		}
	}

} // namespace Sagitta