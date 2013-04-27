//  [11/7/2012 fabiozhang]

#include "sgUtil.h"
#include "math/sgMathHeader.h"

namespace Sagitta{

	UInt32 sgGetDataTypeSize( UInt32 type )
	{
		switch (type) {
		case RDT_F:
			return sizeof(Real);
		case RDT_FV2:
			return sizeof(Vector2);
		case RDT_FV3:
			return sizeof(Vector3);
		case RDT_FV4:
			return sizeof(Vector4);
		case RDT_I:
			return sizeof(int);
		case RDT_IV2:
			return sizeof(int) * 2;
		case RDT_IV3:
			return sizeof(int) * 3;
		case RDT_IV4:
			return sizeof(int) * 4;
		case RDT_UBYTE:
			return sizeof(uByte);
		case RDT_UBYTE2:
			return sizeof(uByte) * 2;
		case RDT_UBYTE3:
			return sizeof(uByte) * 3;
		case RDT_UBYTE4:
			return sizeof(uByte) * 4;
		case RDT_B:
			return sizeof(bool);
		case RDT_BV2:
			return sizeof(bool) * 2;
		case RDT_BV3:
			return sizeof(bool) * 3;
		case RDT_BV4:
			return sizeof(bool) * 4;
		case RDT_FM22:
			return sizeof(Real) * 4;
		case RDT_FM33:
			return sizeof(Matrix3);
		case RDT_FM44:
			return sizeof(Matrix4);
		default:
			return 0;
		}
	}

} // namespace Sagitta
