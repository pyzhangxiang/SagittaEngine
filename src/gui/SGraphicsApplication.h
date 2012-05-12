////////////////////////////////////////////////////////////////
// file: SGraphicsApplication.h @ 2007-2007-12-3 by Zhang Xiang
// declaration of the class SGraphicsApplication
// SGraphicsApplication is a class ...
////////////////////////////////////////////////////////////////
#ifndef __SGRAPHICSAPPLICATION_H__
#define __SGRAPHICSAPPLICATION_H__

// INCLUSIONS //////////////////////////////////////////////////
#include "SApplication.h"

// DECLARATIONS ////////////////////////////////////////////////
namespace Sagitta{

	class SWindow;
	class sgSceneManager;

	class _SG_GuiExport SGraphicsApplication : public SApplication{
	// constructors & destructors
	public:
		SGraphicsApplication(void);
		virtual ~SGraphicsApplication(void);

	// member variables
	protected:
		HDC hDC;
		HGLRC hRC; 
		static sgSceneManager *ms_pUniqueSM;

	// member functions
	public:
		virtual bool initialize(const HINSTANCE vhInstance = NULL, const int &viCmdShow = SW_SHOWNORMAL, const std::wstring &vsAppName = L"New Application");
		virtual bool initialzie(SWindow *vMainWindow, const std::wstring &vsAppName = L"New Application");
		virtual int run();

		bool enableGL(void);
		void disableGL(void);

		virtual LRESULT winProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static sgSceneManager* currentSceneManager(void);
	};
};

#endif //__SGRAPHICSAPPLICATION_H__