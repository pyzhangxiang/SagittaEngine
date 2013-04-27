
#ifndef __SGRENDERTECHNIQUESHADOWMAP_H__
#define __SGRENDERTECHNIQUESHADOWMAP_H__

#include "sgRenderTechnique.h"

namespace Sagitta
{
    class sgRenderTargetWindow;
	class sgRenderTargetTexture;

	class _SG_KernelExport sgRenderTechniqueShadowMap : public sgRenderTechnique
	{
        SG_META_DECLARE(sgRenderTechniqueShadowMap)

    private:
        sgRenderTargetWindow *mRTWindow;
        sgRenderTargetTexture *mDepthMap;
        
        sgStrHandle mDepthTextureName;
        sgStrHandle mDepthMVPName;
        Matrix4 mDepthVP;
        
    public:
        sgRenderTechniqueShadowMap(void);
        virtual ~sgRenderTechniqueShadowMap(void);
        
        virtual bool preRenderPass(void);
		virtual void _setUniformObjectExtra(sg_render::CurrentRenderParam *param, sgSceneObject *object);
	};


} // namespace Sagitta


#endif // __SGRENDERTECHNIQUESHADOWMAP_H__

