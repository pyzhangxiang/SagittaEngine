
#ifndef __SGIMAGELOADER_H__
#define __SGIMAGELOADER_H__

#include "engine/common/sgObject.h"
#include <string>

namespace Sagitta
{   
	class sgFrameBuffer;

	class _SG_KernelExport sgImageLoader : public sgObject
	{
		SG_META_DECLARE_ABSTRACT(sgImageLoader)
	protected:
		sgFrameBuffer *mImageData;

		UInt32 mWidth;
		UInt32 mHeight;
		UInt32 mPixelComponents;

		bool mIsActive;

	public:
		sgImageLoader(void);
		virtual ~sgImageLoader(void) = 0;

		bool isActive(void) const{ return mIsActive; }
		sgFrameBuffer *getImageData(void){ return mImageData; }
		UInt32 getWidth(void) const{ return mWidth; }
		UInt32 getHeight(void) const{ return mHeight; }
		UInt32 getPixelComponents(void) const{ return mPixelComponents; }

		void release(void);

		bool load(const std::string &filename);
	protected:
		virtual bool loadImpl(const std::string &filename) = 0;
	};

	/// bmp, png, tga
	class _SG_KernelExport sgImageLoader_stb : public sgImageLoader
	{
		SG_META_DECLARE(sgImageLoader_stb)

	public:
		sgImageLoader_stb(void);
		virtual ~sgImageLoader_stb(void);

	protected:
		virtual bool loadImpl(const std::string &filename);	
	};

	/// pvr PowerVR
	class _SG_KernelExport sgImageLoader_pvr : public sgImageLoader
	{
		SG_META_DECLARE(sgImageLoader_pvr)

	public:
		sgImageLoader_pvr(void);
		virtual ~sgImageLoader_pvr(void);

	protected:
		virtual bool loadImpl(const std::string &filename);		
	};

	
} // namespace Sagitta



#endif // __SGIMAGELOADER_H__
