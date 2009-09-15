//////////////////////////////////////////////////////
// file: SWindowDelegate.h @ 2008-12-10 by Zhang Xiang
// declares of the class SWindowDelegate
// SWindowDelegate is a class ...
//////////////////////////////////////////////////////
#ifndef __SWINDOWDELEGATE_H__
#define __SWINDOWDELEGATE_H__

// INCLUDES //////////////////////////////////////////
#include "../common/SDelegate.h"
#include "GuiDef.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SWindow;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _GuiExport SWindowDelegate : public SDelegate{
	// member variables
	private:
		/** For Win32, it's the window class style like CS_HREDRAW;
		*	For Mac OS X's Carbon, it's the WindowClass;
		*	For Linux ...
		*/
		uLong m_iWndClassStyle;

		/** For Win32, it's the style of the window being created;
		*	For Mac OS X's Carbon, it's the WindowAttributes;
		*	For Linux ...
		*/
		uLong m_iWndAttributes;

		/** Only for Win32. 
		*	For Mac OS X's Carbon, it's always 0.
		*/
		uLong m_iWndExAttributes;

	// constructors & destructor
	protected:
		SWindowDelegate(uLong aBaseWndClassStyle, uLong aBaseWndAttributes, uLong aBaseWndExAttributes);
	public:
		SWindowDelegate(void);
		virtual ~SWindowDelegate(void){}

	// member functions
	private:
		/** Call for _setWindowStyle(uInt, uInt) to set the window's style.		--- outmoded ---
		*	@remarks
		*		Users could override it to customize window style.
		*/
		/*
		virtual std::string _setWindowStyle(void);
		*/

	protected:
		/**	Sets window style. Only Called by _setWindowStyle(void)			--- outmoded ---
		*/
		/* 
		std::string _setWindowStyle(uLong aWndClassStyle, uLong aWndAttributes, uLong aWndExAttributes);
		*/

	public:
		/** Call for system call to create new window and return it's handle.
		*/
		SWindowHandle _createWindow(const std::string &aTitle, SRect aRect, SWindowHandle aHParent, SWindow *aBindedWindow);

		// window operation
		/** Sets the specified window visible.
		*	@return true - the window was previously visible;
		false - the window was not previously visible.
		*/
		virtual bool showWindow(SWindowHandle aHandle);
		/** Redraw the specified window. Will send a paint event.
		*/
		virtual bool updateWindow(SWindowHandle aHandle);
		/** Sets the specified window invisible.
		*	@return true - the window was previously visible;
		*			false - the window was not previously visible.
		*/
		virtual bool hideWindow(SWindowHandle aHandle);

		/** Moves to a new position and scales this window's size. */
		virtual bool moveWindow(SWindowHandle aHandle, int aNewX, int aNewY, int aNewWidth, int aNewHeight);

		/** Destroys the specified window. */
		virtual bool destroyWindow(SWindowHandle aHandle);

		virtual bool validateRect(SWindowHandle aHandle, const SRect *aRect);
		virtual bool invalidateRect(SWindowHandle aHandle, const SRect *aRect, bool bErase);

		virtual bool isWindowVisible(SWindowHandle aHandle);

		virtual bool isWindow(SWindowHandle aHandle);
		
		/** Gets the specified window's content bounds. */
		virtual SRect getWindowBounds(SWindowHandle aHandle);

	}; //#### end class SWindowDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _GuiExport SMainFrameDelegate : public SWindowDelegate{
	// member variables

	// constructors & destructor
	public:
		/** Constructor.
		*	@remarks
		*		The default values for style and attributes are CS_HREDRAW | CS_VREDRAW, WS_OVERLAPPEDWINDOW, 0.
		*/
		SMainFrameDelegate(void);
		SMainFrameDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes);
		virtual ~SMainFrameDelegate(void){}

	// member functions
	private:
		/// Overrides from SWindowDelegate.		--- outmoded ---
		/*
		std::string _setWindowStyle(void);
		*/
		
	public:
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_APPLE
		/** Sets the specified window visible. Works on the true window(For Mac). Overrides from SWindowDelegate.
		 *	@return true - the window was previously visible;
		 false - the window was not previously visible.
		 */
		bool showWindow(SWindowHandle aHandle);
		/** Sets the specified window invisible. Works on the true window(For Mac). Overrides from SWindowDelegate.
		 *	@return true - the window was previously visible;
		 *			false - the window was not previously visible.
		 */
		bool hideWindow(SWindowHandle aHandle);
		
		/** Moves to a new position and scales this window's size. Works on the true window(For Mac). Overrides from SWindowDelegate.
		 */
		bool moveWindow(SWindowHandle aHandle, int aNewX, int aNewY, int aNewWidth, int aNewHeight);
		
		/** Destroys the specified window. Works on the true window(For Mac). Overrides from SWindowDelegate.
		 */
		bool destroyWindow(SWindowHandle aHandle);
		
		/** Checks if the specified window is visible. Works on the true window(For Mac).
		 *	Overrides from SWindowDelegate.
		 */
		bool isWindowVisible(SWindowHandle aHandle);
		
		/** Checks if the specified window is valid. Works on the true window(For Mac).
		 *	Overrides from SWindowDelegate.
		 */
		bool isWindow(SWindowHandle aHandle);
		
		/** Gets the specified window's content bounds. Works on the true window(For Mac).
		 *	Overrides from SWindowDelegate.
		 */
		virtual SRect getWindowBounds(SWindowHandle aHandle);
#endif

	}; //#### end class SMainFrameDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _GuiExport SWidgetDelegate : public SWindowDelegate{
	// member variables

	// constructors & destructor
	public:
		/** Constructor.
		*	@remarks
		*		The default values for style and attributes are CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, WS_CHILD | WS_VISIBLE, 0.
		*/
		SWidgetDelegate(void);
		SWidgetDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes);
		virtual ~SWidgetDelegate(void){}

	// member functions
	private:
		/// Overrides from SWindowDelegate.		--- outmoded ---
		/*
		std::string _setWindowStyle(void);
		*/

	}; //#### end class SWidgetDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _GuiExport SViewDelegate : public SWidgetDelegate{
	// member variables

	// constructors & destructor
	public:
		SViewDelegate(void);
		SViewDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes);
		virtual ~SViewDelegate(void){}

	// member functions

	}; //#### end class SViewDelegate


	/** class representation
	@remarks

	@note

	@par

	*/
	class _GuiExport SGraphicsViewDelegate : public SViewDelegate{
	// member variables

	// constructors & destructor
	public:
		SGraphicsViewDelegate(void);
		SGraphicsViewDelegate(uLong aAddWndClassStyle, uLong aAddWndAttributes, uLong aAddWndExAttributes);
		~SGraphicsViewDelegate(void){}

	// member functions
	public:

	}; //#### end class SGraphicsViewDelegate

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SWINDOWDELEGATE_H__