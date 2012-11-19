//  [10/13/2012  zhangxiang]

#include "sgRenderTargetWindow.h"
#include "engine/renderer/sgRenderer.h"

namespace Sagitta{

	sgRenderTargetWindow::sgRenderTargetWindow(UInt32 width, UInt32 height)
    : sgRenderTarget(0, width, height)
    {
        
	}

	sgRenderTargetWindow::~sgRenderTargetWindow(void)
    {
	}

    bool sgRenderTargetWindow::isActive(void) const
    {
        return true;
    }

} // namespace Sagitta
