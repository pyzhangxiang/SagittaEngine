/////////////////////////////////////////////////////////
// file: SagittaDef @ 2007-12-7 by Zhang Xiang
// Defines for Sagitta
/////////////////////////////////////////////////////////
#ifndef __SAGITTADEF_H__
#define __SAGITTADEF_H__

// INCLUDES ////////////////////////////////////////////
#include "SagittaPlatform.h"
#include <iostream>
#include <string>

// DECLARES //////////////////////////////////////////
namespace Sagitta{

	// TYPE DEFINE
	typedef float Real;
	typedef unsigned char uByte;
	typedef unsigned short uShort;
	typedef unsigned int uInt;
	typedef unsigned long uLong;
	typedef long SStatus;
	typedef std::string StdString;

	// ENUMS
	enum{
		NOERR = 0,
		NONPROCESSEDERR = 1
	};

	// STRUCTS
	struct SPoint{
		float x;
		float y;
		SPoint(float _x, float _y) : x(_x), y(_y){}
	};

	struct SRect{
		short left;
		short top;
		short right;
		short bottom;
		SRect(short _left, short _top, short _right, short _bottom)
			: left(_left), top(_top), right(_right), bottom(_bottom){}
	};

	// GLOBAL VARIABLES

	// MACROS
	// Compare a and b to find whose value is bigger or smaller.
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

	// Swap two variables value
#define SWAP(a, b, t) {t = a; a = b; b = t;}

	// FUNCTIONS

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SAGITTADEF_H__