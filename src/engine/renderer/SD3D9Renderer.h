//////////////////////////////////////////////////////
// file: SD3D9Renderer.h @ 2008-8-6 by Zhang Xiang
// declares of the class SD3D9Renderer
// SD3D9Renderer is a class ...
//////////////////////////////////////////////////////
#ifndef __SD3D9RENDERER_H__
#define __SD3D9RENDERER_H__

// INCLUDES //////////////////////////////////////////
#include "SRenderer.h"
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
	class _SagittaExport SD3D9Renderer : public SRenderer{
	// member variables
	protected:
		HWND m_HWnd;
		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pDevice;
		mutable LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;

	// constructors & destructor
	public:
		SD3D9Renderer(HWND aHWnd, int aTWidth, int aTHeight);
		virtual ~SD3D9Renderer(void);

	// member functions
	private:
		/** Sets viewport. Overrides from SRenderer. */
		void setViewport(SViewport *aViewport) const;

		/** clear frame buffers. Overrides from SRenderer. */
		void clearFrameBuffers(uInt aFlags, const Color &aBkColor,
							Real aBkDepth, int aBkStencil) const;

		/** Sets projection matrix. Overrides from SRenderer.
			@param
				aMatrix The raw matrix from camera.
		*/
		void setProjMatrix(const Matrix4 &aMatrix) const;

		/** Sets view matrix. Overrides from SRenderer.
			@param
				aMatrix The raw matrix from camera.
		*/
		void setViewMatrix(const Matrix4 &aMatrix) const;

		/** Internal hook. Calls 3D API to setup lights. Overrides from SRenderer. */
		int setupLightsImpl(const Color &aGlobalAmbiantColor) const;

		/** Renders a specific renderable object. Overrides from SRenderer. */
		void render(const SRenderOption &aGlobalRop, SRenderable *aRenderable) const;

		/** Draw frame buffer to target and do something after rendering. Overrides from SRenderer. */
		void postRenderImpl(void) const;

		/** Resets light state. Overrides from SRenderer. */
		void resetLights(int aLightNum) const;

	protected:
		/** Overridden from SRenderer. Does conversion between Sagitta element type to graphics api's element type (e.g. ET_POINTS to GL_POINTS). */
		int retMapping(int aRet) const;

	public:
		/** Overridden from SRenderer. Initialize the environment of graphics device. */
		void init(void);

		/** Overridden from SRenderer. Reset graphics attributes when the window's size changed. */
	//	void resize(int aWidth, int aHeight);

	}; //#### end class SD3D9Renderer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif

#endif // __SD3D9RENDERER_H__