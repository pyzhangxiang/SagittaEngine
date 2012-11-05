//
//  sgNSOpenGLView.m
//  cocoaogl
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "sgNSOpenGLView.h"
#import "sgCocoaToSagitta.h"
#import "../sgDemo.h"
#import <iostream>

static NSTimer *gTimer = nil;

@implementation sgNSOpenGLView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        NSOpenGLPixelFormatAttribute attrs[] =
        {
            NSOpenGLPFAAllRenderers,
            NSOpenGLPFADepthSize, 32,
            0
        };
        NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        self.pixelFormat = pixelFormat;
        [pixelFormat release];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(_surfaceNeedsUpdate:)
                                                     name:NSViewGlobalFrameDidChangeNotification
                                                   object:self];
        
        
        NSPoint screenPt = [NSEvent mouseLocation];
        NSRect rect = self.frame;
        lastMouseX = screenPt.x - rect.origin.x;
        lastMouseY = screenPt.y - rect.origin.y;
        
    }
    
    return self;
}

- (void) _surfaceNeedsUpdate:(NSNotification*)notification
{
    [self update];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
    //glClear(GL_COLOR_BUFFER_BIT);

    // no need to check zero app pointer
    // let it crash
    if(!GetApp()->isDestroying())
    {
        static CFAbsoluteTime last = CFAbsoluteTimeGetCurrent();//clock();
        static CFAbsoluteTime curr = last;
        curr = CFAbsoluteTimeGetCurrent();//clock();
        //std::cout << curr << " - " << last  << " = " << curr - last << "\n";
        float dt = (float)(curr - last);// / 7000;//(float)CLOCKS_PER_SEC;
        //if(dt > GetApp()->getFrameRate())
        {
            
            last = curr;
            curr = 0;
            GetApp()->update(dt);
            GetApp()->render();
            glFlush();
        }
        
    }
    
}

-(void)timerEvent:(NSTimer*)timer
{
    [self setNeedsDisplay: YES];
    [self drawRect:[self bounds]];
}


-(void)prepareOpenGL
{
//pTimer = [NSTimer timerWithTimeInterval:(1.0/60.0) target:self selector:@selector(idle:) userInfo:nil repeats:YES];
//[[NSRunLoop currentRunLoop]addTimer:pTimer forMode: NSDefaultRunLoopMode];
    
    //glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    
    // no need to check zero app pointer
    // let it crash
    GetApp()->initialize();
    
    gTimer = [NSTimer timerWithTimeInterval:GetApp()->getFrameRate()
                                    target:self
                                  selector:@selector(timerEvent:)
                                  userInfo:nil
                                   repeats:YES];
    
    [[NSRunLoop mainRunLoop] addTimer:gTimer forMode:NSDefaultRunLoopMode];
}

-(void)clearGLContext
{
    // Do any OpenGL Cleanup 
}

-(void)reshape 
{
    NSRect rect = [self bounds];
    //glViewport(0, 0, rect.size.width, rect.size.height); 

    // no need to check zero app pointer
    // let it crash
    GetApp()->resize(rect.size.width, rect.size.height);
}

-(void)keyDown:(NSEvent *)theEvent
{
    sgKeyEvent e;
    e.repeat = [theEvent isARepeat] == YES;
    e.modifiers = [sgCocoaToSagitta getMofifiers:theEvent];
    e.key = [sgCocoaToSagitta getKey:theEvent];

    GetApp()->keyPressEvent(e);
}

-(void)keyUp:(NSEvent *)theEvent
{
    sgKeyEvent e;
    e.repeat = [theEvent isARepeat] == YES;
    e.modifiers = [sgCocoaToSagitta getMofifiers:theEvent];
    e.key = [sgCocoaToSagitta getKey:theEvent];
    
    GetApp()->keyReleaseEvent(e);
}

-(void)mouseDown:(NSEvent *)theEvent
{
    sgMouseEvent e;
    e.buttons = [sgCocoaToSagitta getMouseButtons:theEvent];
    e.modifiers = [sgCocoaToSagitta getMofifiers:theEvent];
    NSPoint screenPt = [NSEvent mouseLocation];
//    NSRect rect = [self bounds];
    NSRect rect = [self.window convertRectToScreen:self.frame];
    e.x = screenPt.x - rect.origin.x;
    e.y = screenPt.y - rect.origin.y;
    e.deltaX = e.x - lastMouseX;
    e.deltaY = e.y - lastMouseY;
    lastMouseX = e.x;
    lastMouseY = e.y;
    GetApp()->mousePressEvent(e);
}

- (void)mouseUp:(NSEvent *)theEvent
{
    sgMouseEvent e;
    e.buttons = [sgCocoaToSagitta getMouseButtons:theEvent];
    e.modifiers = [sgCocoaToSagitta getMofifiers:theEvent];
    NSPoint screenPt = [NSEvent mouseLocation];
//    NSRect rect = [self bounds];
    NSRect rect = [self.window convertRectToScreen:self.frame];
    e.x = screenPt.x - rect.origin.x;
    e.y = screenPt.y - rect.origin.y;
    e.deltaX = e.x - lastMouseX;
    e.deltaY = e.y - lastMouseY;
    lastMouseX = e.x;
    lastMouseY = e.y;
    if([theEvent clickCount] >= 2)
    {
        GetApp()->mouseDoubleClickEvent(e);
    }
    else
    {
        GetApp()->mouseReleaseEvent(e);
    }
}

-(void)mouseMoved:(NSEvent *)theEvent
{
    sgMouseEvent e;
    e.buttons = 0;//[sgCocoaToSagitta getMouseButtons:theEvent];
    e.modifiers = [sgCocoaToSagitta getMofifiers:theEvent];
    NSPoint screenPt = [NSEvent mouseLocation];
//    NSRect rect = [self bounds];
    NSRect rect = [self.window convertRectToScreen:self.frame];
    e.x = screenPt.x - rect.origin.x;
    e.y = screenPt.y - rect.origin.y;
    e.deltaX = e.x - lastMouseX;
    e.deltaY = e.y - lastMouseY;
    lastMouseX = e.x;
    lastMouseY = e.y;
    GetApp()->mouseMoveEvent(e);
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    sgMouseEvent e;
    e.buttons = [sgCocoaToSagitta getMouseButtons:theEvent];
    e.modifiers = [sgCocoaToSagitta getMofifiers:theEvent];
    NSPoint screenPt = [NSEvent mouseLocation];
    //    NSRect rect = [self bounds];
    NSRect rect = [self.window convertRectToScreen:self.frame];
    e.x = screenPt.x - rect.origin.x;
    e.y = screenPt.y - rect.origin.y;
    e.deltaX = e.x - lastMouseX;
    e.deltaY = e.y - lastMouseY;
    lastMouseX = e.x;
    lastMouseY = e.y;
    GetApp()->mouseMoveEvent(e);
}

-(void)scrollWheel:(NSEvent *)theEvent
{
    sgWheelEvent e;
    e.delta = -[theEvent deltaY];
    e.modifiers = [sgCocoaToSagitta getMofifiers:theEvent];
    NSPoint screenPt = [NSEvent mouseLocation];
    //NSRect rect = [self bounds];
    NSRect rect = [self.window convertRectToScreen:self.frame];
    e.x = screenPt.x - rect.origin.x;
    e.y = screenPt.y - rect.origin.y;
    e.delta = e.x - lastMouseX;
    e.deltaY = e.y - lastMouseY;
    lastMouseX = e.x;
    lastMouseY = e.y;
    GetApp()->wheelEvent(e);
}



@end
