#include <windows.h>
#include "../sgDemo.h"
#include "sgWin32GLWindow.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

static HWND gRenderHwnd = 0;

void sgMessageBox(const std::string &title, const std::string &content)
{
	MessageBox(NULL, content.c_str(), title.c_str(), MB_OK);
}

// console
void RedirectIOToConsole()

{

	int hConHandle;

	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	FILE *fp;

	// allocate a console for this app

	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),

		&coninfo);

	coninfo.dwSize.Y = 999;//MAX_CONSOLE_LINES;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),

		coninfo.dwSize);

	// redirect unbuffered STDOUT to the console

	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w" );

	*stdout = *fp;

	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console

	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "r" );

	*stdin = *fp;

	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console

	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w" );

	*stderr = *fp;

	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog

	// point to console as well

	std::ios::sync_with_stdio();

}

int sgDemo::run(int argc, char *argv[])
{
	RedirectIOToConsole();

    sgWin32GLWindow win;
	win.setRect(mRenderWindowX, mRenderWindowY, mRenderWindowW, mRenderWindowH);
	win.setTitle(mRenderWindowTitle);

	gRenderHwnd = win.create();
	if(!gRenderHwnd)
		return 1;

	bool done = false;
	MSG msg;
	while(!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if(!mDestroying)
			{
				static clock_t last = clock();
				static clock_t curr = last;

				curr = clock();
				float dt = (float)(curr - last) / (float)CLK_TCK;
				if(dt > mFrameRate)
				{
					last = curr;
					curr = 0;
					this->update(dt);
					this->render();
					//HDC mhdc = ::GetDC(gRenderHwnd);
					::SwapBuffers(win.getHdc());
					//::ReleaseDC(gRenderHwnd, mhdc);
				}
			}
			
		
		}
	}

    return 0; //msg.wParam;
}

int sgDemo::getRenderWindowWidth(void) const
{
	RECT rc;
	if(!::GetWindowRect(gRenderHwnd, &rc))
		return 0;
    return rc.right - rc.left;
}

int sgDemo::getRenderWindowHeight(void) const
{
	RECT rc;
	if(!::GetWindowRect(gRenderHwnd, &rc))
		return 0;
	return rc.bottom - rc.top;
}


// winmain for win32
extern "C" int main(int argc, char *argv[]);

extern "C" int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	int    argc;
	char** argv;

	char*  arg;
	int    index;

	// count the arguments

	argc = 1;
	arg  = lpCmdLine;

	while (arg[0] != 0) {

		while (arg[0] != 0 && arg[0] == ' ') {
			arg++;
		}

		if (arg[0] != 0) {

			argc++;

			while (arg[0] != 0 && arg[0] != ' ') {
				arg++;
			}

		}

	}    

	// tokenize the arguments

	argv = (char**)malloc(argc * sizeof(char*));

	arg = lpCmdLine;
	index = 1;

	while (arg[0] != 0) {

		while (arg[0] != 0 && arg[0] == ' ') {
			arg++;
		}

		if (arg[0] != 0) {

			argv[index] = arg;
			index++;

			while (arg[0] != 0 && arg[0] != ' ') {
				arg++;
			}

			if (arg[0] != 0) {
				arg[0] = 0;    
				arg++;
			}

		}

	}    

	// put the program name into argv[0]

	char filename[_MAX_PATH];

	GetModuleFileName(NULL, filename, _MAX_PATH);
	argv[0] = filename;

	int ret = main(argc, argv);

	free(argv);

	return ret;
}