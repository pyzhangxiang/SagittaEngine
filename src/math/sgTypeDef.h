//////////////////////////////////////////////////////
// file: sgMath.h @ 2008-1-19 by Zhang Xiang
//
// common declares and defines for math lib
// 
//////////////////////////////////////////////////////
#ifndef __SGTYPEDEF_H__
#define __SGTYPEDEF_H__

// INCLUDES ////////////////////////////////////////

#ifdef _MSC_VER
#include "pstdint.h"
#else
#include <stdint.h>
#endif

#include <math.h>
#include <string>

// DECLARES //////////////////////////////////////

// DEFINES ///////////////////////////////////////

namespace Sagitta{

	// TYPE DEFINE
	
	typedef unsigned char uByte;
	typedef unsigned short uShort;
	typedef unsigned int uInt;
	typedef unsigned long uLong;
	typedef long SStatus;
	typedef std::string StdString;
    
    typedef int8_t Int8;
    typedef uint8_t UInt8;
    typedef Int8 Byte;
    typedef UInt8 UByte;
    
    typedef int16_t Int16;
    typedef uint16_t UInt16;
    
    typedef int32_t Int32;
    typedef uint32_t UInt32;
    
    typedef int64_t Int64;
    typedef uint64_t UInt64;
    
    typedef float Float32;
    typedef double Float64;
    
    typedef Float32 Real;
    
    typedef UInt32 id_type;
#define ID_NULL 0
    
//    typedef std::string sg_string;

};

#endif // __SGTYPEDEF_H__
