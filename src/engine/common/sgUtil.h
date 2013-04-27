//  [11/7/2012 fabiozhang]

#ifndef __SGUTIL_H__
#define __SGUTIL_H__

#include "math/SagittaPlatform.h"
#include "math/sgTypeDef.h"

namespace Sagitta{

	enum RenderDataType
	{
		RDT_NIL,
		RDT_F,
		RDT_FV2,
		RDT_FV3,
		RDT_FV4,
		RDT_I,
		RDT_IV2,
		RDT_IV3,
		RDT_IV4,
		RDT_UBYTE,
		RDT_UBYTE2,
		RDT_UBYTE3,
		RDT_UBYTE4,
		RDT_UI,
		RDT_UIV2,
		RDT_UIV3,
		RDT_UIV4,
		RDT_B,	// boolean
		RDT_BV2,
		RDT_BV3,
		RDT_BV4,
		RDT_FM22,
		RDT_FM33,
		RDT_FM44,
		RDT_TEXTURE,
		RDT_COUNT,
	};
    
	_SG_KernelExport UInt32 sgGetDataTypeSize(UInt32 type);
    
    enum PixelFormat
    {
        PIXEL_FORMAT_RED,
        PIXEL_FORMAT_GREEN,
        PIXEL_FORMAT_BLUE,
        PIXEL_FORMAT_ALPHA,
    //    PIXEL_FORMAT_RG,
        PIXEL_FORMAT_RGB,
        PIXEL_FORMAT_BGR,
        PIXEL_FORMAT_RGBA,
        PIXEL_FORMAT_BGRA,
    };
    
    template<typename T, typename U>
    T sgMin(const T &a, const U &b)
    {
        return a < b ? a : b;
    }
    template<typename T, typename U>
    T sgMax(const T &a, const U &b)
    {
        return a > b ? a : b;
    }

} // namespace Sagitta

#endif // __SGUTIL_H__

