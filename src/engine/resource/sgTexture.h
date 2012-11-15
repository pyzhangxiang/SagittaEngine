//  [10/11/2012  zhangxiang]

#ifndef __SGTEXTURE_H__
#define __SGTEXTURE_H__


#include "sgResource.h"


namespace Sagitta{
    
    class sgImageLoader;
	class sgFrameBuffer;

	class _SG_KernelExport sgTexture : public sgResource
	{
		SG_META_DECLARE(sgTexture)
        
    private:
        sgImageLoader *mImage;
        Int32 mTextureId;

        UInt32 mWidth;
        UInt32 mHeight;
        UInt32 mPixelComponents;
        
	public:
		sgTexture(void);
		virtual ~sgTexture(void);

        sgFrameBuffer *getBuffer(void) const;
        
        Int32 getTextureId(void) const{ return mTextureId; }
        
        bool isActive(void) const;
        bool hasData(void) const;
        
        UInt32 getWidth(void) const{ return mWidth; }
        UInt32 getHeight(void) const{ return mHeight; }
        UInt32 getPixelComponents(void) const{ return mPixelComponents; }

	protected:
		virtual void onSetFilename(void);

	}; 

} // namespace Sagitta


#endif // __SGTEXTURE_H__

