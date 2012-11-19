//  [10/13/2012  zhangxiang]

#ifndef __SGRENDERTARGETWINDOW_H__
#define __SGRENDERTARGETWINDOW_H__


#include "sgRenderTarget.h"
#include "math/sgTypeDef.h"


namespace Sagitta{

	class _SG_KernelExport sgRenderTargetWindow : public sgRenderTarget
	{
    private:

	public:
		sgRenderTargetWindow(UInt32 width, UInt32 height);
		virtual ~sgRenderTargetWindow(void);

        virtual bool isActive(void) const;

	}; 

} // namespace Sagitta


#endif // __SGRENDERTARGETWINDOW_H__

