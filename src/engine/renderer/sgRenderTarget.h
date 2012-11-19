//  [10/13/2012  zhangxiang]

#ifndef __SGRENDERTARGET_H__
#define __SGRENDERTARGET_H__


#include "engine/common/sgMemObject.h"
#include "math/sgTypeDef.h"


namespace Sagitta{

	class sgViewport;
	class sgCameraComponent;

	class _SG_KernelExport sgRenderTarget : public sgMemObject
	{
    private:
        UInt32 mRtId;
        
        UInt32 mWidth;
        UInt32 mHeight;

       
		sgViewport *mViewport;

	public:
		sgRenderTarget(UInt32 rtId, UInt32 width, UInt32 height);
		virtual ~sgRenderTarget(void) = 0;

        UInt32 getRtId(void) const{ return mRtId; }
       
        virtual bool isActive(void) const = 0;
        
        UInt32 getWidth(void) const{ return mWidth; }
        UInt32 getHeight(void) const{ return mHeight; }

		sgViewport *getViewport(void) const{ return mViewport; }
		void setViewport(Real ratioLeft, Real ratioRight, Real ratioWidth, Real ratioHeight);
		void resize(UInt32 width, UInt32 height);
		void setCamera(sgCameraComponent *camera);
	}; 

} // namespace Sagitta


#endif // __SGRENDERTARGET_H__

