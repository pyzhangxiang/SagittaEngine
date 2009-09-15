//////////////////////////////////////////////////////
// file: SVector3.cpp @ 2008-1-21 by Zhang Xiang
// defines of the class Vector3
// Vector3 is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "SVector3.h"

// DEFINES ///////////////////////////////////////////
namespace Sagitta{
	const Vector3 Vector3::ZERO(0, 0, 0);
	const Vector3 Vector3::UNIT_X(1, 0, 0);
	const Vector3 Vector3::UNIT_Y(0, 1, 0);
	const Vector3 Vector3::UNIT_Z(0, 0, 1);
	const Vector3 Vector3::NEGATIVE_UNIT_X(-1,  0,  0);
	const Vector3 Vector3::NEGATIVE_UNIT_Y( 0, -1,  0);
	const Vector3 Vector3::NEGATIVE_UNIT_Z( 0,  0, -1);
	const Vector3 Vector3::UNIT_SCALE(1, 1, 1);
}
