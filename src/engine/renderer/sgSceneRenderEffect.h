//  [11/6/2012 fabiozhang]

#ifndef __SGSCENERENDEREFFECT_H__
#define __SGSCENERENDEREFFECT_H__

#include "sgRenderEffect.h"

namespace Sagitta
{
    class sgRenderer;
    
	class _SG_KernelExport sgSceneRenderEffect : public sgRenderEffect
	{
        SG_META_DECLARE(sgSceneRenderEffect)
        
        friend class sgRenderer;

	protected:
		sg_render::CurrentRenderParam *mCurrentRenderParam;
        
    public:
        sgSceneRenderEffect(void);
        virtual ~sgSceneRenderEffect(void);
		
	protected:
		/// render scene pass by pass
        void render(sg_render::CurrentRenderParam *param);
        
        /// do something before and after the render pass
        virtual void preRenderPass(void){}
        virtual void postRenderPass(void){}
        
	};

} // namespace Sagitta


#endif // __SGSCENERENDEREFFECT_H__

