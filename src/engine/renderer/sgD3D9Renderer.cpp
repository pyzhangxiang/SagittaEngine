//////////////////////////////////////////////////////
// file: sgD3D9Renderer.cpp @ 2008-8-6 by Zhang Xiang
// defines of the class sgD3D9Renderer
// sgD3D9Renderer is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgD3D9Renderer.h"
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32

#include "sgViewport.h"
#include "../buffer/sgVertexData.h"
#include "../buffer/sgVertexIndexBuffer.h"
#include "../buffer/sgVertexBufferElement.h"
#include "../scenegraph/sgSceneManager.h"
#include "../scenegraph/sgCamera.h"
#include "../scenegraph/sgRenderable.h"
#include "../scenegraph/sgRenderQueue.h"
#include "../../common/utils/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	//  [8/7/2008 zhangxiang]
	sgD3D9Renderer::sgD3D9Renderer(HWND aHWnd, int aTWidth, int aTHeight) :
	sgRenderer(aTWidth, aTHeight, true),
	m_pD3D(NULL), m_pDevice(NULL),
	m_pVertexDeclaration(NULL), m_HWnd(aHWnd){
		
	}

	//  [8/7/2008 zhangxiang]
	sgD3D9Renderer::~sgD3D9Renderer(void){
		// shutdown();
		SAFE_RELEASE(m_pD3D);
		SAFE_RELEASE(m_pDevice);
		SAFE_RELEASE(m_pVertexDeclaration);
	}

	//  [8/7/2008 zhangxiang]
	void sgD3D9Renderer::init(void){
		if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create D3D failed.", "SD3D9RenderSystem::SD3D9RenderSystem");

		D3DDISPLAYMODE  d3ddm;
		ZeroMemory(&d3ddm, sizeof(d3ddm));
		if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Get D3D display mode failed.", "SD3D9RenderSystem::SD3D9RenderSystem");

		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferFormat = d3ddm.Format;
		d3dpp.Windowed = true;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_HWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))){
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create D3D device failed.", "SD3D9RenderSystem::SD3D9RenderSystem");
		}

		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	}

	//  [1/15/2009 zhangxiang]
	void sgD3D9Renderer::setViewport(sgViewport *aViewport) const{

	}

	//  [1/15/2009 zhangxiang]
	void sgD3D9Renderer::clearFrameBuffers(uInt aFlags, const Color &aBkColor,
										Real aBkDepth, int aBkStencil) const{
		DWORD d3d9_flags = 0;
		if(aFlags & sgViewport::CB_COLOR){
			d3d9_flags |= D3DCLEAR_TARGET;
		}
		if(aFlags & sgViewport::CB_DEPTH){
			d3d9_flags |= D3DCLEAR_ZBUFFER;
		}
		if(aFlags & sgViewport::CB_STENCIL){
			d3d9_flags |= D3DCLEAR_STENCIL;
		}

		HRESULT hr;
		if(FAILED(hr = m_pDevice->Clear(
									0,
									NULL,
									D3DCLEAR_TARGET,
									aBkColor.toD3DColor(),
									aBkDepth,
									aBkStencil))){
		//	StdString msg = DXGetErrorDescription9(hr);
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR,
				"Error clearing frame buffer : "/*  + msg*/, "D3D9Renderer::clearFrameBuffers" );
		}
	}

	//  [1/15/2009 zhangxiang]
	void sgD3D9Renderer::setProjMatrix(const Matrix4 &aMatrix) const{
		// set projection matrix
		m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)(aMatrix.transpose().arr()));
	}

	//  [1/15/2009 zhangxiang]
	void sgD3D9Renderer::setViewMatrix(const Matrix4 &aMatrix) const{
		// set view matrix
		m_pDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)(aMatrix.transpose().arr()));
	}

	//  [1/10/2009 zhangxiang]
	int sgD3D9Renderer::setupLightsImpl(const Color &aGlobalAmbiantColor) const{
		// for future ...
		return 0;
	}

	//  [1/15/2009 zhangxiang]
	void sgD3D9Renderer::render(const sgRenderOption &aGlobalRop, sgRenderable *aRenderable) const{
		// setup material

		// get vertex buffer
		sgVertexData *pvb = new sgVertexData();
		sgVertexIndexBuffer *pvib = new sgVertexIndexBuffer(sgVertexBufferElement::ET_VERTEX);
		aRenderable->getVertexBuffer(pvb, pvib);

		// set vertex declaration and d3d vertex buffer
		LPDIRECT3DVERTEXBUFFER9 *pD3DVB = new LPDIRECT3DVERTEXBUFFER9[pvb->elementNum()];
		SAFE_RELEASE(m_pVertexDeclaration);

		size_t elemIndex = 0;
		size_t *vertexsize = new uInt[pvb->elementNum()];
		D3DVERTEXELEMENT9 *d3delments = new D3DVERTEXELEMENT9[pvb->elementNum() + 1];
		sgVertexData::ConstIterator elemIt = pvb->getConstIterator();
		for(; elemIt.hasMoreElements(); elemIt++, elemIndex++){
			sgVertexBufferElement *element = elemIt.value();

			d3delments[elemIndex].Method = D3DDECLMETHOD_DEFAULT;
			d3delments[elemIndex].Offset = element->offset();
			d3delments[elemIndex].Stream = element->source();
			d3delments[elemIndex].UsageIndex = 0;	// for future ...
			switch(element->type()){
					case sgVertexBufferElement::ET_VERTEX:
						{
							d3delments[elemIndex].Type = D3DDECLTYPE_FLOAT3;
							d3delments[elemIndex].Usage = D3DDECLUSAGE_POSITION;
							break;
						}

					case sgVertexBufferElement::ET_COLOR:
						{
							d3delments[elemIndex].Type = D3DDECLTYPE_D3DCOLOR;
							d3delments[elemIndex].Usage = D3DDECLUSAGE_COLOR;
							break;
						}

					case sgVertexBufferElement::ET_NORMAL:
						{
							d3delments[elemIndex].Type = D3DDECLTYPE_FLOAT3;
							d3delments[elemIndex].Usage = D3DDECLUSAGE_NORMAL;
							break;
						}

					case sgVertexBufferElement::ET_TEXTURE_COORD:
						{
							d3delments[elemIndex].Type = D3DDECLTYPE_FLOAT1;
							d3delments[elemIndex].Usage = D3DDECLUSAGE_TEXCOORD;
							break;
						}

					case sgVertexBufferElement::ET_FOG_COORDINATE:
						{
							d3delments[elemIndex].Type = D3DDECLTYPE_FLOAT2;
							d3delments[elemIndex].Usage = D3DDECLUSAGE_FOG;
							break;
						}

					default:
						{
							THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE, "Invalid render state.", "SD3D9RenderSystem::render");
							break;
						}
			}

			vertexsize[elemIndex] = element->vertexSize();

			if(FAILED(m_pDevice->CreateVertexBuffer(sizeof(element->data()), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &pD3DVB[elemIndex], NULL)))
				THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create D3D vertex buffer failed", "SD3D9RenderSystem::render");
			void *data = NULL;
			pD3DVB[elemIndex]->Lock(0, sizeof(element->data()), (void**)&data, 0);
			memcpy(data, element->data(), sizeof(element->data()));
			pD3DVB[elemIndex]->Unlock();
		}
		// Add terminator
		d3delments[elemIndex].Stream = 0xff;
		d3delments[elemIndex].Offset = 0;
		d3delments[elemIndex].Type = D3DDECLTYPE_UNUSED;
		d3delments[elemIndex].Method = 0;
		d3delments[elemIndex].Usage = 0;
		d3delments[elemIndex].UsageIndex = 0;

		if(FAILED(m_pDevice->CreateVertexDeclaration(d3delments, &m_pVertexDeclaration)))
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create declaration failed.", "SD3D9RenderSystem::render");
		if(FAILED(m_pDevice->SetVertexDeclaration(m_pVertexDeclaration)))
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Set declaration failed.", "SD3D9RenderSystem::render");

		// setup d3d index buffer
		LPDIRECT3DINDEXBUFFER9 pD3DVIB = NULL;
		if(FAILED(m_pDevice->CreateIndexBuffer(sizeof(pvib->data()), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pD3DVIB, NULL)))
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create D3D index buffer failed.", "SD3D9RenderSystem::render");
		void *pIndex = NULL;
		pD3DVIB->Lock(0, sizeof(pvib->data()), (void**)&pIndex, 0);
		memcpy(pIndex, pvib->data(), sizeof(pvib->data()));
		pD3DVIB->Unlock();


		ID3DXMatrixStack *pStack = NULL;
		if(FAILED(D3DXCreateMatrixStack(0, &pStack)))
			THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Create D3D matrix stack failed.", "SD3D9RenderSystem::render");

		// set model matrix
		m_pDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX*)(Matrix4::IDENTITY.arr()));

		m_pDevice->BeginScene();
		{
			// model transform
			/*	pStack->Push();
			pStack->MultMatrix((D3DXMATRIX*)(renderable->getModelMatrix().arr()));*/
			// set stream source
			for(uInt i=0; i<elemIndex; i++){
				if(FAILED(m_pDevice->SetStreamSource(d3delments[i].Stream, pD3DVB[i], d3delments[i].Offset, vertexsize[i])))
					THROW_SAGI_EXCEPT(sgException::ERR_RENDERINGAPI_ERROR, "Set D3D stream source failed.", "SD3D9RenderSystem::render");
			}
			// set indeices
			m_pDevice->SetIndices(pD3DVIB);
			m_pDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)retMapping(aRenderable->renderOption().renderElementType()), 0, 0, pvb->vertexNum(), 0, pvib->polyNum());
			//pStack->Pop();
		}
		m_pDevice->EndScene();


		// render //////////////////////////////////////////////////////////////////////////


		// release resources
		for(uInt i=0; i<elemIndex; i++){
			SAFE_RELEASE(pD3DVB[i]);
		}
		delete []pD3DVB;
		delete []d3delments;
		delete []vertexsize;
		SAFE_RELEASE(pD3DVIB);


		delete pvb;
		delete pvib;
	}

	//  [1/15/2009 zhangxiang]
	void sgD3D9Renderer::postRenderImpl(void) const{
		// render over
		m_pDevice->Present(0, 0, 0, 0);
	}

	//  [1/10/2009 zhangxiang]
	void sgD3D9Renderer::resetLights(int aLightNum) const{
		// for future ...
	}

	//  [8/7/2008 zhangxiang]
	int sgD3D9Renderer::retMapping(int aRet) const{
		switch(aRet){
			case sgRenderOption::RET_POINTS:
				return D3DPT_POINTLIST;

			case sgRenderOption::RET_LINES:
				return D3DPT_LINELIST;

			case sgRenderOption::RET_LINE_STRIP:
			case sgRenderOption::RET_LINE_LOOP:
				return D3DPT_LINESTRIP;

			case sgRenderOption::RET_TRIANGLES:
				return D3DPT_TRIANGLELIST;

			case sgRenderOption::RET_TRIANGLE_STRIP:
				return D3DPT_TRIANGLESTRIP;

			case sgRenderOption::RET_TRIANGLE_FAN:
				return D3DPT_TRIANGLEFAN;

			default:
				THROW_SAGI_EXCEPT(sgException::ERR_INVALID_STATE,
								"Unknown render element type.",
								"sgD3D9Renderer::retMapping");
		}
	}

} // namespace Sagitta

#endif