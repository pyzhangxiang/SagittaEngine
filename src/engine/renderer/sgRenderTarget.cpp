//  [10/13/2012  zhangxiang]

#include "sgRenderTarget.h"
#include "sgViewport.h"
#include "sgRenderer.h"

namespace Sagitta{

	sgRenderTarget::sgRenderTarget(UInt32 rtId, UInt32 width, UInt32 height)
    : sgMemObject(), mRtId(rtId)
    , mWidth(width), mHeight(height)
    {
		mViewport = new sgViewport(mWidth, mHeight, 0.0, 0.0, 1.0, 1.0, 0, 0);
	}

	sgRenderTarget::~sgRenderTarget(void)
    {
        delete mViewport;
	}

	void sgRenderTarget::setViewport( Real ratioLeft, Real ratioTop, Real ratioWidth, Real ratioHeight )
	{
		mViewport->setLeft(ratioLeft);
		mViewport->setTop(ratioTop);
		mViewport->setWidth(ratioWidth);
		mViewport->setHeight(ratioHeight);
	}

	void sgRenderTarget::resize( UInt32 width, UInt32 height )
	{
		mViewport->_targetResized(width, height);
	}

	void sgRenderTarget::setCamera( sgCameraComponent *camera )
	{
		mViewport->setCamera(camera);
	}

} // namespace Sagitta
