//  [4/11/2012  zhangxiang]

#ifndef __SGRENDERQUEUE_H__
#define __SGRENDERQUEUE_H__

#include "engine/common/sgObject.h"
#include "engine/common/sgStlAllocator.h"

namespace Sagitta{

	class sgSceneObject;

	class _SG_KernelExport sgRenderQueue : public sgObject
	{
        SG_META_DECLARE(sgRenderQueue)
        
    public:
        typedef sg_vector(sgSceneObject*) ObjectList;
    protected:
        ObjectList mObjectList;
        
	public:
        sgRenderQueue(void);
		virtual ~sgRenderQueue(void);
        
        void push_back(sgSceneObject *object);
        void clear(void);
        const ObjectList &getObjectList(void) const{ return mObjectList; }

        virtual void sort(void){}
	protected:
		virtual bool filter(sgSceneObject *object){ return true; }

	};

} // namespace Sagitta


#endif // __SGRENDERQUEUE_H__

