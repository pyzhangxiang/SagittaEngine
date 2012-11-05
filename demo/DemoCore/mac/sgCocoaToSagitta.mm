//
//  sgCocoaToSagitta.m
//  Sagitta
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "sgCocoaToSagitta.h"
#import "../sgKeyDefine.h"

@implementation sgCocoaToSagitta

+(unsigned int)getMofifiers:(NSEvent*)event
{
    unsigned int res = Sagitta::MKS_NON;
    
    unsigned int modifiers = [event modifierFlags];
    if(modifiers & NSShiftKeyMask) 
    {
        res |= Sagitta::MKS_Shift;
    }
    if(modifiers & NSCommandKeyMask) 
    {
        res |= Sagitta::MKS_Ctrl;
    }
    if(modifiers & NSAlternateKeyMask) 
    {
        res |= Sagitta::MKS_Alt;
    }
    // only convert 3 modifiers, no responding to others
    
    return res;

}

+(int)getKey:(NSEvent*)event
{
    int res = Sagitta::Key_Unknown;
    
    NSString *characters = [event charactersIgnoringModifiers];
    
    if([characters length] < 1)
        return res;
    
    unichar keyChar = [characters characterAtIndex:0];
    if(keyChar >= 32    // space
       && keyChar <= 126)   // ~
    {
        res = keyChar;
    }
    else if(keyChar >= NSF1FunctionKey
            && keyChar <= NSF12FunctionKey)
    {
        res = Sagitta::Key_F1 + keyChar - NSF1FunctionKey;
    }
    else
    {
        switch(keyChar)
        {
            case NSUpArrowFunctionKey:
                res = Sagitta::Key_Up;
                break;
            case NSDownArrowFunctionKey:
                res = Sagitta::Key_Down;
                break;
            case NSLeftArrowFunctionKey:
                res = Sagitta::Key_Left;
                break;
            case NSRightArrowFunctionKey:
                res = Sagitta::Key_Right;
                break;
            case 27:    // esc
                res = Sagitta::Key_Esc;
                break;
            case 13:    // return 
                res = Sagitta::Key_Enter;
                break;
        }
    }
    
    return res;
    
}

+(unsigned int)getMouseButtons:(NSEvent*)event
{
    unsigned int res = 0;
    unsigned int buttons = [NSEvent pressedMouseButtons];
    if(buttons & 1<<0)
    {
        res |= Sagitta::MBT_Left;
    }
    if(buttons & 1<<1)
    {
        res |= Sagitta::MBT_Right;
    }
    if(buttons & 1<<2)
    {
        res |= Sagitta::MBT_Middle;
    }
    
    return res;
}

@end
