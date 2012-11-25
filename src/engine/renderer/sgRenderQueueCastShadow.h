//  [4/11/2012  zhangxiang]

#ifndef __SGRENDERQUEUECASTSHADOW_H__
#define __SGRENDERQUEUECASTSHADOW_H__

#include "sgRenderQueue.h"

namespace Sagitta{


	class _SG_KernelExport sgRenderQueueCastShadow : public sgRenderQueue
	{
        SG_META_DECLARE(sgRenderQueueCastShadow)
        
	public:
        sgRenderQueueCastShadow(void);
		virtual ~sgRenderQueueCastShadow(void);

        virtual void sort(void);

	};

} // namespace Sagitta


#endif // __SGRENDERQUEUECASTSHADOW_H__

