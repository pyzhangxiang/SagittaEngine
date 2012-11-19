//  [10/13/2012  zhangxiang]

#include "sgRenderTargetTexture.h"
#include "sgRenderer.h"

namespace Sagitta{

	sgRenderTargetTexture::sgRenderTargetTexture(UInt32 rtId, UInt32 textureId
                                   , UInt32 width, UInt32 height
                                   , UInt32 components)
    : sgRenderTarget(rtId, width, height)
	, mRtTextureId(textureId)
    , mPixelComponents(components)
    {
        
	}

	sgRenderTargetTexture::~sgRenderTargetTexture(void)
    {
        //sgGetRenderer()->deleteRenderTarget(this);
	}

    bool sgRenderTargetTexture::isActive(void) const
    {
        return getRtId() > 0 && mRtTextureId > 0;
    }

} // namespace Sagitta
