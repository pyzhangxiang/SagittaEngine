//  [10/11/2012  zhangxiang]

#include "sgTexture.h"
#include "sgImageLoader.h"
#include "sgResourceCenter.h"
#include "engine/buffer/sgFrameBuffer.h"
#include "engine/renderer/sgRenderer.h"
#include "engine/common/sgStringUtil.h"

namespace Sagitta{

	SG_META_DEFINE(sgTexture, sgResource)

	sgTexture::sgTexture()
    : sgResource(), mImage(NULL)
    , mTextureId(-1), mWidth(0)
    , mHeight(0), mPixelComponents(0)
    {
	}

	sgTexture::~sgTexture(void)
    {
        if(mTextureId >= 0)
        {
            sgGetRenderer()->deleteTexture(mTextureId);
            mTextureId = -1;
        }
        if(mImage != NULL)
        {
            sgObject::destroyObject(mImage);
			mImage = NULL;
        }
	}

    bool sgTexture::isActive(void) const
    {
        return mTextureId >= 0;
    }
    
    bool sgTexture::hasData(void) const
    {
        return mImage != NULL && mImage->isActive();
    }

	void sgTexture::onSetFilename( void )
	{
		std::string filename = getFilename().getStr();
		std::string fileExt = sgStringUtil::getFileExtention(filename);
		
		mImage = sgResourceCenter::instance()->getImageLoader(fileExt);
		if(mImage == NULL)
			return ;

		mImage->load(filename);
		
        mWidth = mImage->getWidth();
        mHeight = mImage->getHeight();
        mPixelComponents = mImage->getPixelComponents();
	
		mTextureId = sgGetRenderer()->createTexture(this);

	}

	sgFrameBuffer * sgTexture::getBuffer( void ) const
	{
		if( mImage == NULL || !(mImage->isActive()) )
			return NULL;

		return mImage->getImageData();

	}

} // namespace Sagitta
