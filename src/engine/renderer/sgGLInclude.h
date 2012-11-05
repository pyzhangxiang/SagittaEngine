//  [28/10/2012 zhangxiang]

#ifndef __SGGLINCLUDE_H__
#define __SGGLINCLUDE_H__

#include "math/SagittaPlatform.h"

#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
#	include <gl/glew.h>
//#	include <gl/glut.h>
#elif SAGITTA_PLATFORM == SAGITTA_PLATFORM_APPLE
#   include <gl/glew.h>
#	include <OpenGL/glu.h>
#	include <OpenGL/glext.h>
#else
#endif

#endif // __SGGLINCLUDE_H__
