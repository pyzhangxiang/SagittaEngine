//  [10/13/2012  zhangxiang]

#ifndef __SGRENDERTARGET_H__
#define __SGRENDERTARGET_H__


#include "engine/common/sgMemObject.h"
#include "math/sgTypeDef.h"


namespace Sagitta{

	class _SG_KernelExport sgRenderTarget : public sgMemObject
	{
    private:
        UInt32 mRtId;
        UInt32 mRtTextureId;
        
        UInt32 mWidth;
        UInt32 mHeight;
        UInt32 mPixelComponents;
        
	public:
		sgRenderTarget(UInt32 rtId, UInt32 textureId
                       , UInt32 width, UInt32 height
                       , UInt32 components);
		virtual ~sgRenderTarget(void);

        UInt32 getRtId(void) const{ return mRtId; }
        UInt32 getRtTextureId(void) const{ return mRtTextureId; }
        
        bool isActive(void) const;
        
        UInt32 getWidth(void) const{ return mWidth; }
        UInt32 getHeight(void) const{ return mHeight; }
        UInt32 getPixelComponents(void) const{ return mPixelComponents; }

	}; 

} // namespace Sagitta


#endif // __SGRENDERTARGET_H__

