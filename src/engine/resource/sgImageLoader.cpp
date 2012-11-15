

#include "sgImageLoader.h"
#include "sgResourceCenter.h"
#include "engine/common/sgStringUtil.h"
#include "engine/common/sgException.h"
#include "engine/common/sgLogSystem.h"
#include "engine/buffer/sgFrameBuffer.h"
#include <stb_image/stb_image.h>

namespace Sagitta{

	SG_META_DEFINE_ABSTRACT(sgImageLoader, sgObject)

	sgImageLoader::sgImageLoader( void ) 
	: mImageData(NULL)
	, mWidth(0)
	, mHeight(0)
	, mPixelComponents(0)
	, mIsActive(false)
	{
		mImageData = new sgFrameBuffer();
	}

	sgImageLoader::~sgImageLoader( void )
	{
		release();
	}

	void sgImageLoader::release( void )
	{
		mWidth = mHeight = mPixelComponents = 0;
		mIsActive = false;
	}

	bool sgImageLoader::load( const std::string &filename )
	{
		release();
		mIsActive = loadImpl(filename);
		return mIsActive;
	}


	//////////////////////////////////////////////////////////////////////////
	SG_META_DEFINE(sgImageLoader_stb, sgImageLoader)

	sgImageLoader_stb::sgImageLoader_stb( void )
	{

	}

	sgImageLoader_stb::~sgImageLoader_stb( void )
	{
		
	}

	bool sgImageLoader_stb::loadImpl( const std::string &filename )
	{
		int w, h, comps;
		std::string fullpath = sgResourceCenter::instance()->getResourcePath(filename);

		uByte *tImageData = stbi_load(fullpath.c_str(), &w, &h, &comps, 0);
		if(tImageData == NULL)
		{
			sgLogSystem::instance()->error(stbi_failure_reason());
			return false;
		}

		mWidth = w;
		mHeight = h;
		mPixelComponents = comps;

		sgFrameBuffer *buffer = this->getImageData();
		buffer->resize(mWidth, mHeight, mPixelComponents * sizeof(uByte));
		memcpy(buffer->data(), tImageData, buffer->getSizeInBytes());

		stbi_image_free(tImageData);

		return true;
	}

	
	//////////////////////////////////////////////////////////////////////////
	SG_META_DEFINE(sgImageLoader_pvr, sgImageLoader)

	sgImageLoader_pvr::sgImageLoader_pvr( void )
	{

	}

	sgImageLoader_pvr::~sgImageLoader_pvr( void )
	{
	}

	bool sgImageLoader_pvr::loadImpl( const std::string &filename )
	{
		return true;
	}

	
} // namespace Sagitta

