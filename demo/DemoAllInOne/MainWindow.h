#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <gui/SGraphicsFrame.h>


class MainWindow : public Sagitta::SGraphicsFrame{
public:
	MainWindow();
	~MainWindow();

public:
	bool onKeyDown(Sagitta::uLong aKey, Sagitta::uLong aRepeatCount, Sagitta::uLong aModifiers);
};

#endif // __MAINWINDOW_H__