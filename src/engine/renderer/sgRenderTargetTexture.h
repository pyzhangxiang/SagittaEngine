//  [11/19/2012 fabiozhang]

#ifndef __SGRENDERTARGETTEXTURE_H__
#define __SGRENDERTARGETTEXTURE_H__

#include "sgRenderTarget.h"

namespace Sagitta{

	class sgViewport;

	class _SG_KernelExport sgRenderTargetTexture : public sgRenderTarget
	{
    private:
        UInt32 mRtTextureId;
        
        UInt32 mPixelComponents;
        
	public:
		sgRenderTargetTexture(UInt32 rtId, UInt32 textureId
                       , UInt32 width, UInt32 height
                       , UInt32 components);
		virtual ~sgRenderTargetTexture(void);

        virtual bool isActive(void) const;
        
        UInt32 getRtTextureId(void) const{ return mRtTextureId; }
        UInt32 getPixelComponents(void) const{ return mPixelComponents; }

	}; 

} // namespace Sagitta


#endif // __SGRENDERTARGETTEXTURE_H__

