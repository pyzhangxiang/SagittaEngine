//  [10/11/2012  zhangxiang]

#include "sgTexture.h"
#include "engine/buffer/sgFrameBuffer.h"

namespace Sagitta{

	SG_META_DEFINE(sgTexture, sgResource)

	sgTexture::sgTexture()
    : sgResource(), mBuffer(NULL)
    , mTextureId(0)
    {
        mBuffer = new sgFrameBuffer();
	}

	sgTexture::~sgTexture(void)
    {
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
        return mBuffer != NULL && mBuffer->data() != NULL;
    }
    
} // namespace Sagitta
