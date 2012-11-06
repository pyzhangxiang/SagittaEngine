//  [11/6/2012 fabiozhang]

#ifndef __SGSCENERENDEREFFECT_H__
#define __SGSCENERENDEREFFECT_H__

#include "sgRenderEffect.h"

namespace Sagitta
{
    
	class _SG_KernelExport sgSceneRenderEffect : public sgRenderEffect
	{
        SG_META_DECLARE_ABSTRACT(sgSceneRenderEffect)
        
    public:
        sgSceneRenderEffect(void);
        virtual ~sgSceneRenderEffect(void) = 0;
        
		virtual void render(sg_render::CurrentRenderParam *param, sgSceneObject *object);
		
	protected:
		void renderObject(sgSceneObject *object);
	};

} // namespace Sagitta


#endif // __SGSCENERENDEREFFECT_H__

