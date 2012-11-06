//  [11/6/2012 fabiozhang]

#ifndef __SGSCENERENDEREFFECT_H__
#define __SGSCENERENDEREFFECT_H__

#include "sgRenderEffect.h"

namespace Sagitta
{
    class sgRenderer;
    
	class _SG_KernelExport sgSceneRenderEffect : public sgRenderEffect
	{
        SG_META_DECLARE_ABSTRACT(sgSceneRenderEffect)
        
        friend class sgRenderer;
        
    private:
        size_t mCurrentPass;
        
    public:
        sgSceneRenderEffect(void);
        virtual ~sgSceneRenderEffect(void) = 0;
		
	protected:
        void renderScene(sg_render::CurrentRenderParam *param);
		void renderObject(sgSceneObject *object);
        
        virtual void setUniformScene(void){}
        virtual void setUniformObject(sgSceneObject *object){}
	};

} // namespace Sagitta


#endif // __SGSCENERENDEREFFECT_H__

