//  [10/13/2012  zhangxiang]

#include "sgRenderTarget.h"
#include "engine/renderer/sgRenderer.h"

namespace Sagitta{

	sgRenderTarget::sgRenderTarget(UInt32 rtId, UInt32 textureId
                                   , UInt32 width, UInt32 height
                                   , UInt32 components)
    : sgMemObject(), mRtId(rtId), mRtTextureId(textureId)
    , mWidth(width), mHeight(height), mPixelComponents(components)
    {
        
	}

	sgRenderTarget::~sgRenderTarget(void)
    {
        sgGetRenderer()->deleteRenderTarget(this);
	}

    bool sgRenderTarget::isActive(void) const
    {
        return mRtId > 0 && mRtTextureId > 0;
    }

} // namespace Sagitta
