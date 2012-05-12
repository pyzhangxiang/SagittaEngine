//////////////////////////////////////////////////////
// file: sgD3D9Renderer.h @ 2008-8-6 by Zhang Xiang
// declares of the class sgD3D9Renderer
// sgD3D9Renderer is a class ...
//////////////////////////////////////////////////////
#ifndef __SGD3D9RENDERER_H__
#define __SGD3D9RENDERER_H__

// INCLUDES //////////////////////////////////////////
#include "sgRenderer.h"
#if SAGITTA_PLATFORM == SAGITTA_PLATFORM_WIN32

#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dxerr9.h>

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	// some D3D commonly used macros
	#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
	#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
	#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgD3D9Renderer : public sgRenderer{
	// member variables
	protected:
		HWND m_HWnd;
		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pDevice;
		mutable LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	// constructors & destructor
	public:
		sgD3D9Renderer(HWND aHWnd, int aTWidth, int aTHeight);
		virtual ~sgD3D9Renderer(void);

	// member functions
	private:
		/** Sets viewport. Overrides from sgRenderer. */
		void setViewport(sgViewport *aViewport) const;

		/** clear frame buffers. Overrides from sgRenderer. */
		void clearFrameBuffers(uInt aFlags, const Color &aBkColor,
							Real aBkDepth, int aBkStencil) const;

		/** Sets projection matrix. Overrides from sgRenderer.
			@param
				aMatrix The raw matrix from camera.
		*/
		void setProjMatrix(const Matrix4 &aMatrix) const;

		/** Sets view matrix. Overrides from sgRenderer.
			@param
				aMatrix The raw matrix from camera.
		*/
		void setViewMatrix(const Matrix4 &aMatrix) const;

		/** Internal hook. Calls 3D API to setup lights. Overrides from sgRenderer. */
		int setupLightsImpl(const Color &aGlobalAmbiantColor) const;

		/** Renders a specific renderable object. Overrides from sgRenderer. */
		void render(const sgRenderOption &aGlobalRop, sgRenderable *aRenderable) const;

		/** Draw frame buffer to target and do something after rendering. Overrides from sgRenderer. */
		void postRenderImpl(void) const;

		/** Resets light state. Overrides from sgRenderer. */
		void resetLights(int aLightNum) const;

	protected:
		/** Overridden from sgRenderer. Does conversion between Sagitta element type to graphics api's element type (e.g. ET_POINTS to GL_POINTS). */
		int retMapping(int aRet) const;

	public:
		/** Overridden from sgRenderer. Initialize the environment of graphics device. */
		void init(void);

		/** Overridden from sgRenderer. Reset graphics attributes when the window's size changed. */
	//	void resize(int aWidth, int aHeight);

	}; //#### end class sgD3D9Renderer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif

#endif // __SGD3D9RENDERER_H__

