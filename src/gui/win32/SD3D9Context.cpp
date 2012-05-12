//////////////////////////////////////////////////////
// file: SD3D9Context.cpp @ 2008-12-13 by Zhang Xiang
// defines of the class SD3D9Context
// SD3D9Context is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "../SD3D9Context.h"
#include "../SWindow.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [12/14/2008 zhangxiang]
	SD3D9Context::SD3D9Context(SWindow *aWindow) : SRenderContext(aWindow){
		try{
			buildRenderEnv();
		}catch(sgException &e){
			::ReleaseDC(m_pWindow->handle(), m_ContextHandle);
			// log for future ...
			// SLog(e.what());
		}
	}

	//  [12/14/2008 zhangxiang]
	SD3D9Context::~SD3D9Context(void){
		disposeRenderEnv();
		// ReleaseDC will be called in the super class's destructor
	}

	//  [12/14/2008 zhangxiang]
	void SD3D9Context::buildRenderEnv(void){
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if(!m_pD3D){
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create D3D failed.", "SD3D9Context::buildRenderEnv(void)");
		}

		D3DDISPLAYMODE  d3ddm;
		ZeroMemory(&d3ddm, sizeof(d3ddm));
		if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Get D3D display mode failed.", "SD3D9Context::buildRenderEnv(void)");
		}

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_pWindow->handle(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
			delete m_pD3D;
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create D3D device failed.", "SD3D9Context::buildRenderEnv(void)");
		}
	}

	//  [12/14/2008 zhangxiang]
	void SD3D9Context::disposeRenderEnv(void){
		if(m_pD3D){
			delete m_pD3D;
			m_pD3D = 0;
		}
		if(m_pDevice){
			delete m_pDevice;
			m_pDevice = 0;
		}
	}

} // namespace Sagitta