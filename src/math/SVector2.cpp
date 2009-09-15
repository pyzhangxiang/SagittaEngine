//////////////////////////////////////////////////////
// file: SVector2.cpp @ 2008-1-20 by Zhang Xiang
// defines of the class Vector2
// Vector2 is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SVector2.h"

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	const Vector2 Vector2::ZERO( 0, 0);
	const Vector2 Vector2::UNIT_X( 1, 0);
	const Vector2 Vector2::UNIT_Y( 0, 1);
	const Vector2 Vector2::NEGATIVE_UNIT_X( -1,  0);
	const Vector2 Vector2::NEGATIVE_UNIT_Y(  0, -1);
	const Vector2 Vector2::UNIT_SCALE(1, 1);
}
