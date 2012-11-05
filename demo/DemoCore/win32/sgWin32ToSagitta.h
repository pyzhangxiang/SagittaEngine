//
//  sgCocoaToSagitta.h
//  Sagitta
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
#ifndef __SGWIN32TOSAGITTA_H__
#define __SGWIN32TOSAGITTA_H__


struct sgWin32ToSagitta
{
	static unsigned int getMofifiers(void);
	static unsigned int getMofifiers(unsigned int aSysMK);
	static int getKey(int keyChar);
	static unsigned int getMouseButtons(unsigned int aSysMK);
};

#endif // __SGWIN32TOSAGITTA_H__
