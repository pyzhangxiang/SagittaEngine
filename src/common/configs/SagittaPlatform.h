//////////////////////////////////////////////////////
// file: SagittaPlatform.h @ 2008-1-19 by Zhang Xiang
// declaration of the platform of Sagitta
//////////////////////////////////////////////////////
#ifndef __SAGITTAPLATFORM_H__
#define __SAGITTAPLATFORM_H__

// INCLUSIONS ////////////////////////////////////////

// DECLARATIONS //////////////////////////////////////
//namespace Sagitta{

#define SAGITTA_PLATFORM_WIN32 1
#define SAGITTA_PLATFORM_APPLE 2
#define SAGITTA_PLATFORM_LINUX 3

#define SAGITTA_COMPILER_MSVC 1
#define SAGITTA_COMPILER_GNUC 2
#define SAGITTA_COMPILER_BORL 3

#define SAGITTA_ENDIAN_LITTLE 1
#define SAGITTA_ENDIAN_BIG 2

#define SAGITTA_ARCHITECTURE_32 1
#define SAGITTA_ARCHITECTURE_64 2

/** current platform */
#if defined(__WIN32__) || defined(_WIN32)
#	define SAGITTA_PLATFORM SAGITTA_PLATFORM_WIN32
#elif defined(__APPLE_CC__) || defined(_APPLE_CC)
#	define SAGITTA_PLATFORM SAGITTA_PLATFORM_APPLE
#else
#	define SAGITTA_PLATFORM SAGITTA_PLATFORM_LINUX
#endif

/** arch type */
#if defined(__x86_64__) || defined(_M_X64)
#	define SAGITTA_ARCH_TYPE SAGITTA_ARCHITECTURE_64
#else
#	define SAGITTA_ARCH_TYPE SAGITTA_ARCHITECTURE_32
#endif

/** Windows Settings */
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32
#	if defined(SAGITTA_STATIC_LIB)
#		define _SG_CommonExport
#		define _SG_MathExport
#		define _SG_KernelExport
#		define _SG_GuiExport
#	else
#		define _SG_CommonExport

#		if defined(SAGITTA_MATH)
#			define _SG_MathExport __declspec(dllexport)
#		else
#			define _SG_MathExport __declspec(dllimport)
#		endif

#		if defined(SAGITTA_KERNEL)
#			define _SG_KernelExport __declspec(dllexport)
#		else
#			define _SG_KernelExport __declspec(dllimport)
#		endif

#		if defined(SAGITTA_GUI)
#			define _SG_GuiExport __declspec(dllexport)
#		else
#			define _SG_GuiExport __declspec(dllimport)
#		endif
#	endif

#	define SAGITTA_UNICODE_SUPPORT 1
#endif

/** Apple/Linux Settings */
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_APPLE || SAGITTA_PLATFORM == SAGITTA_PLATFORM_LINUX
#	define _SG_CommonExport
#	define _SG_MathExport
#	define _SG_KernelExport
#	define _SG_GuiExport

	// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

	// Always enable unicode support for the moment
	// Perhaps disable in old versions of gcc if necessary
#	define OGRE_UNICODE_SUPPORT 1
#endif

/** disable UNICODE */
#ifdef UNICODE
#undef UNICODE
#endif
//}

// DEFINITIONS ///////////////////////////////////////

#endif // __SAGITTAPLATFORM_H__