//////////////////////////////////////////////////////
// file: SWindow.h @ 2007-11-16 by Zhang Xiang
// declaration of the class SWindow
// SWindow is a class ...
//////////////////////////////////////////////////////
#ifndef __SWINDOW_H__
#define __SWINDOW_H__

// INCLUDES //////////////////////////////////////////
#include "SListener.h"
#include <set>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SWindowDelegate;
	class SContext;
	class SWidget;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _GuiExport SWindow : public SListener{
	// type defines
	protected:
		typedef std::set<SWidget*> ChildList;

	// enum defines
		
	// member variables
	protected:
		/** Widget children. 
		*	@remarks
		*		Window hold the ownership, but
		*		don't delete them in destructor
		*		directly, because the operating system
		*		will send destroy event to all children to
		*		dispose them.
		*/
		ChildList m_Children;

	private:
		/** Takes charge window operations, such as create, show, hide, move and so on.
		*	This window holds the ownership.
		*/
		SWindowDelegate *m_Delegate;

		/** window title */
		std::string m_sTitle;

		/** parent */
		SWindow *m_pParent;

		/** Stores the platform depended handle */
		SWindowHandle m_Handle;

		// window properties
		/** Stores the screen coordinates of the upper-left corner of this window's client area.
		*	@remarks
		*		The values are set in move event handler(include initialization).
		*/
		int m_iX, m_iY;
		/** Stores the width and height of this window's client area.
		*	@remarks
		*		The values are set in resize event handler(include initialization). 
		*/
		int m_iWidth, m_iHeight;

		/** if visible */
		bool m_bVisible;

	// constructors & destructor
	public:
		SWindow(SWindowDelegate *aDelegate, const std::string &aTitle, SRect aRect, SWindow *aParent);
		virtual ~SWindow(void) = 0;

	// member functions
	private:
		/** dispose the window handle.
		*	@remarks
		*		Only called in SWindow::~SWindow()
		*/
		bool dispose(void);

	protected:
		/** Gets this window's delegate. */
		SWindowDelegate *del(void) const;

		/** Overrides from SListener. */
		SStatus handleEvent(const SEvent &aEvent);

		// handle specified event ------------------------

		/** Handles window close event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onClose(void);

		/** Handles background erase event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onEraseBkgnd(const SContext &aContext);
		
		// mouse event
		/** Handles mouse move event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onMouseMove(uLong aModifiers, const SPoint &aPoint);
		/** Handles left mouse button down event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onLMouseDown(uLong aModifiers, const SPoint &aPoint);
		/** Handles left mouse button down and dragged event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onLMouseDragged(uLong aModifiers, const SPoint &aPoint);
		/** Handles left mouse button up event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onLMouseUp(uLong aModifiers, const SPoint &aPoint);
		/** Handles left mouse button double click event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onLMouseDblClk(uLong aModifiers, const SPoint &aPoint);
		/** Handles right mouse button down event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onRMouseDown(uLong aModifiers, const SPoint &aPoint);
		/** Handles right mouse button down and dragged event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onRMouseDragged(uLong aModifiers, const SPoint &aPoint);
		/** Handles right mouse button up event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onRMouseUp(uLong aModifiers, const SPoint &aPoint);
		/** Handles right mouse button double click event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onRMouseDblClk(uLong aModifiers, const SPoint &aPoint);
		/** Handles middle mouse button down event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onMMouseDown(uLong aModifiers, const SPoint &aPoint);
		/** Handles middle mouse button down and dragged event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onMMouseDragged(uLong aModifiers, const SPoint &aPoint);
		/** Handles middle mouse button up event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onMMouseUp(uLong aModifiers, const SPoint &aPoint);
		/** Handles middle mouse button double click event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onMMouseDblClk(uLong aModifiers, const SPoint &aPoint);
		
		/** Handles mouse wheel event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onMouseWheel(uLong aModifiers, int aZDelta, const SPoint &aPoint);

		// key event
		/** Handles key down event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onKeyDown(uLong aKey, uLong aRepeatCount, uLong aModifiers);
		/** Handles key up event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onKeyUp(uLong aKey, uLong aRepeatCount, uLong aModifiers);
		/** Handles char event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onChar(uLong aChar, uLong aRepeatCount, uLong aModifiers);

		/** Handles paint event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onPaint(void);
		/** Does painting. Called by onPaint.
		*/
		virtual void onDraw(const SContext &aContext);

		/** Handles resize event. 
		*	@remarks
		*		While the client area size are changed,
		*		notify children.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onResize(uLong aWidth, uLong aHeight);

		/** Handles window move event.
		*	@return If the event was processed, true - has been processed; false - didn't processed.
		*/
		virtual bool onMove(int aX, int aY);

	public:
		/** Gets this window's title. */
		const StdString &title(void) const;
		/** Sets this window's title. */
		void setTitle(const std::string &aTitle);

		/** Gets this window's parent. */
		SWindow *parent(void) const;

		/** Gets this window's handle. */
		SWindowHandle handle(void) const;

		/** Gets the left-top position of the client area. */
		SPoint position(void) const;
		/** Gets the x-coordinate of the left-top pos. */
		int leftX(void) const;
		/** Gets the y-coordinate of the left-top pos. */
		int topY(void) const;
		/** Gets the width value of the client area. */
		int width(void) const;
		/** Gets the height value of the client area. */
		int height(void) const;

		/** Check if this window is visible. */
		bool isVisible(void) const;

		// window operation
		/** Sets the specified window visible.
		*	@return true - the window was previously visible;
		*			false - the window was not previously visible.
		*/
		bool show(void);
		/** Redraw this window. 
		*	@return true - successful;
		*			false - failed.
		*/
		bool update(void);
		/** Sets the specified window invisible.
		*	@return true - the window was previously visible;
		*			false - the window was not previously visible.
		*/
		bool hide(void);

		/** Moves to a new position and scales this window's size,
		*	@remarks
		*		Calls for SWindowDelegate::move and it's implement depends on platform in SWindowDelegate.
		*		The new position is relative to the upper-left corner of the screen for SMainFrame;
		*		and is relative to the upper-left of the parent window's client area for SWidget.
		*
		*		The Operation System will send resize event. The size propety will be updated in the event handler.
		*/
		bool move(int aNewX, int aNewY, int aNewWidth, int aNewHeight);

		/** Validates a rect of the client area.
			@remarks
				Flushs the rect.
		*/
		bool validateRect(const SRect *aRect);

		/** Invalidates a rect of the client area.
			@remarks
				Asks to repaint.
		*/
		bool invalidateRect(const SRect *aRect, bool bErase = true);

		/** Adds a widget to this window. */
		void addChild(SWidget *aWidget);

		/** Removes a widget from this window.
			@remarks Internal method, only called by SWidget::~SWidget()
		*/
		void _removeChild(SWidget *aWidget);

		/** Gets a child by index. */
		SWidget *getChild(uInt aIndex) const;

		/** If changed, notifies children to update. */
		void notifyChildren(void);

	}; //#### end class SWindow

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SWINDOW_H__