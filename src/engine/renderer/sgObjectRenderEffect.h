//  [4/11/2012  zhangxiang]

#ifndef __SGOBJECTRENDEREFFECT_H__
#define __SGOBJECTRENDEREFFECT_H__

#include "sgRenderEffect.h"

namespace Sagitta{
    
    class sgSceneRenderEffect;
    
	class _SG_KernelExport sgObjectRenderEffect : public sgRenderEffect
	{
        SG_META_DECLARE(sgObjectRenderEffect)
        
        friend class sgSceneRenderEffect;
        
    public:
        sgObjectRenderEffect(void);
        virtual ~sgObjectRenderEffect(void);
        
    protected:
		/// render the object pass by pass
		void render(sg_render::CurrentRenderParam *param, sgSceneObject *object);

	};

} // namespace Sagitta


#endif // __SGOBJECTRENDEREFFECT_H__

