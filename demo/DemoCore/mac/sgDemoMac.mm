#import "../sgDemo.h"
#import "sgNSOpenGLView.h"
#import "sgAppDelegate.h"
#import <Cocoa/Cocoa.h>

void sgMessageBox(const std::string &title, const std::string &content)
{
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    NSString *str = [[NSString alloc] initWithCString:content.c_str() encoding:NSASCIIStringEncoding];
    [alert setMessageText:str];
    [str release];
    [alert runModal];
    /*[alert beginSheetModalForWindow:nil
                  modalDelegate:nil
                 didEndSelector:nil
                    contextInfo:@"aa"];
*/
}

int sgDemo::run(int argc, char *argv[])
{
    //return NSApplicationMain(argc, (const char **)argv);
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    // The first time sharedApplication is called, it creates an instance of    NSApplication
    //
    
    sgAppDelegate *appDelegate = [[sgAppDelegate alloc] init];
    
    NSApp = [NSApplication sharedApplication];
    [NSApp setDelegate:appDelegate];
    
    // setup window
    NSScreen *mainScreen = [NSScreen mainScreen];
    NSRect screenRect = [mainScreen visibleFrame];
    
    
    if(mRenderWindowX == -1)
    {
        mRenderWindowX = (screenRect.size.width - mRenderWindowW)/2;
    }
    if(mRenderWindowY == -1)
    {
        mRenderWindowY = (screenRect.size.height - mRenderWindowH)/2;
    }
    NSRect rect = NSMakeRect(mRenderWindowX, mRenderWindowY, mRenderWindowW, mRenderWindowH);

    NSWindow *window = [[NSWindow alloc] initWithContentRect:rect 
                                              styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask
                                                backing:NSBackingStoreBuffered defer:NO];
    [window setTitle:@"Sagitta Demo"];
    [window setAcceptsMouseMovedEvents:YES];
    
    GetApp()->OnCreate();
    /*
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        0
    };
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];*/
    NSView *view = [[[sgNSOpenGLView alloc] initWithFrame:rect/* pixelFormat:pixelFormat*/] autorelease];
//    [pixelFormat release];
    [window setContentView:view];
    [window makeKeyAndOrderFront:nil];
    
    [NSApp run];
    
    
    [pool release];
    return EXIT_SUCCESS;
}

int sgDemo::getRenderWindowWidth(void) const
{
    NSView *view = [NSApplication sharedApplication].mainWindow.contentView;
    return view.bounds.size.width;
}

int sgDemo::getRenderWindowHeight(void) const
{
    NSView *view = [NSApplication sharedApplication].mainWindow.contentView;
    return view.bounds.size.height;
}