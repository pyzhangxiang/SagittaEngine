//  [10/11/2012  zhangxiang]

#include "sgTexture.h"
#include "sgLoader.h"
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

	void sgTexture::onSetFilename( void )
	{
		int width, height, comps;
		uByte *imageData = sgLoader::load_image(getFilename().getStr(), width, height, comps);
		if(imageData == NULL)
			return ;
	
		sgFrameBuffer *buffer = this->getBuffer();
		buffer->resize(width, height, comps * sizeof(uByte));
		memcpy(buffer->data(), imageData, buffer->getSizeInBytes());

		mTextureId = sgGetRenderer()->createTexture(this);

	}

} // namespace Sagitta
