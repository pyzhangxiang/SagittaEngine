//////////////////////////////////////////////////////
// file: sgGLRenderer.h @ 2008-8-1 by Zhang Xiang
// declares of the class sgGLRenderer
// sgGLRenderer is a class ...
//////////////////////////////////////////////////////
#ifndef __SGGLRENDERER_H__
#define __SGGLRENDERER_H__

// INCLUDES //////////////////////////////////////////
#include "sgRenderer.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_KernelExport sgGLRenderer : public sgRenderer{
	// member variables

	// constructors & destructor
	public:
		sgGLRenderer(int aTWidth, int aTHeight);
		virtual ~sgGLRenderer(void);

	// member functions
	private:
		/** Sets viewport. Overrides from sgRenderer. */
		void setViewport(sgViewport *aViewport) const;

		/** clear frame buffers. Overrides from sgRenderer. */
		void clearFrameBuffers(uInt aFlags,
							const Color &aBkColor,
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

	}; //#### end class sgGLRenderer

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SGGLRENDERER_H__
