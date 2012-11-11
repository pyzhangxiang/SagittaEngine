//  [10/11/2012  zhangxiang]

#include "sgTexture.h"
#include "engine/buffer/sgFrameBuffer.h"
#include "engine/renderer/sgRenderer.h"

namespace Sagitta{

	SG_META_DEFINE(sgTexture, sgResource)

	sgTexture::sgTexture()
    : sgResource(), mBuffer(NULL)
    , mTextureId(-1)
    {
        mBuffer = new sgFrameBuffer();
	}

	sgTexture::~sgTexture(void)
    {
        if(mTextureId >= 0)
        {
            sgGetRenderer()->deleteTexture(mTextureId);
            mTextureId = -1;
        }
        if(mBuffer)
        {
            delete mBuffer;
            mBuffer = NULL;
        }
	}
    
    void sgTexture::_setTexureId(UInt32 tid)
    {
        mTextureId = tid;
    }

    bool sgTexture::isActive(void) const
    {
        return mTextureId >= 0;
    }
    
    bool sgTexture::hasData(void) const
    {
        return mBuffer != NULL && mBuffer->data() != NULL;
    }
    
} // namespace Sagitta
