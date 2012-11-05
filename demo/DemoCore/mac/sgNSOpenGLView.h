//
//  sgNSOpenGLView.h
//  cocoaogl
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <OpenGL/glext.h>
#import <OpenGL/glu.h>

@interface sgNSOpenGLView : NSOpenGLView
{
    int lastMouseX;
    int lastMouseY;
}
@end
