//
//  sgInputEvent.h
//  Sagitta
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Sagitta_sgInputEvent_h
#define Sagitta_sgInputEvent_h

#include <math/SagittaPlatform.h>

struct _SG_DemoExport sgInputEvent
{
    sgInputEvent(void){}
    virtual ~sgInputEvent(void){};
};

struct _SG_DemoExport sgMouseEvent : public sgInputEvent
{
    unsigned int buttons;
	unsigned int modifiers;
    int x;
    int y;
	int deltaX;
	int deltaY;
};

struct _SG_DemoExport sgKeyEvent : public sgInputEvent
{
    unsigned int modifiers;
    int key;
    bool repeat;
};

struct _SG_DemoExport sgWheelEvent : public sgInputEvent
{
    unsigned int modifiers;
    float delta;
    int x;
    int y;
    int deltaX;
	int deltaY;
};



#endif
