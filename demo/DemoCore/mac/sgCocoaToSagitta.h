//
//  sgCocoaToSagitta.h
//  Sagitta
//
//  Created by Xiang Zhang on 12-8-25.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface sgCocoaToSagitta : NSObject

+(unsigned int)getMofifiers:(NSEvent*)event;
+(int)getKey:(NSEvent*)event;
+(unsigned int)getMouseButtons:(NSEvent*)event;

@end
