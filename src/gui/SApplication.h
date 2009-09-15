/////////////////////////////////////////////////////////
// file: SApplicatioin.h @ 2007-11-17 by Zhang Xiang
// declaration of the class SApplication
// SApplication is a class ...
/////////////////////////////////////////////////////////
#ifndef __SAPPLICATION_H__
#define __SAPPLICATION_H__

// INCLUSION ////////////////////////////////////////////
#include "SListener.h"
#include "../common/utils/Singleton.h"
#include <string>
#include <map>

// DECLARATION //////////////////////////////////////////
namespace Sagitta{

	class SWindow;
	class SMainFrame;
	class SEvent;

	class _GuiExport SApplication : public SListener/*, public Singleton<SApplication>*/{
	// type define
	protected:
		typedef std::map<SWindowHandle, SWindow*> WindowMap;

	// static member variables
	private:
		static SApplication *single_app;

	// member variables
	private:
		std::string m_sAppName;

		/// This application hold the ownership of windows.
		WindowMap m_WindowMap;
		SMainFrame *m_pMainWindow;

	// constructors & destructors
	public:
		SApplication(void);
		virtual ~SApplication(void);

	// static member function
	public:
		static SApplication *instance_ptr(void);
		static SApplication &instance(void);

	// member functions
	private:
		/** Registers the specified window. */
		void registerWindow(SWindowHandle aHandle, SWindow *aWindow);

		/** Unregister the specified window. */
		void unregisterWindow(SWindow *aWindow);

	protected:
		/** Overrides from SListener. */
		SStatus handleEvent(const SEvent &aEvent);

		/** Processes window create event. */
		void onWindowCreate(SWindowHandle aHandle, SWindow *aWindow);

		/** Processes window destroy event. */
		void onWindowDispose(SWindow *aWindow);

	public:
		/** Application runs. */
		virtual SStatus run();

		/** Quit application. */
		void quit(int aQuitCode);

		/** Gets the main window. */
		SMainFrame *mainWindow() const;
		/** Sets the main window. */
		void setMainWindow(SMainFrame *aWindow);

		/** Finds a window by handle. */
		SWindow *findWindow(SWindowHandle aHandle) const;
		/** Check if the specified window is in this application. */
		bool findWindow(SWindow *aWindow) const;

		/** Resister a connection between two Controller. */
		void registerConnection(SController *_aSender, const std::string &_aSignal, SController *_aReceiver, const std::string &_aSlot);

		/** Asks a Responser to response a Sender's action. */
		void response(SController *_aSender, const std::string &_aSignal);

	};

} // namespace Sagitta

#endif // __SAPPLICATION_H__